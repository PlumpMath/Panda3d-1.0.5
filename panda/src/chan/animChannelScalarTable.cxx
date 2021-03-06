// Filename: animChannelScalarTable.cxx
// Created by:  drose (22Feb99)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////


#include "animChannelScalarTable.h"
#include "animBundle.h"
#include "config_chan.h"

#include "indent.h"
#include "datagram.h"
#include "datagramIterator.h"
#include "bamReader.h"
#include "bamWriter.h"
#include "fftCompressor.h"

TypeHandle AnimChannelScalarTable::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
AnimChannelScalarTable::
AnimChannelScalarTable(AnimGroup *parent, const string &name)
  : AnimChannelScalar(parent, name) {
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
AnimChannelScalarTable::
AnimChannelScalarTable(void){
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::has_changed
//       Access: Public, Virtual
//  Description: Returns true if the value has changed since the last
//               call to has_changed().  last_frame is the frame
//               number of the last call; this_frame is the current
//               frame number.
////////////////////////////////////////////////////////////////////
bool AnimChannelScalarTable::
has_changed(int last_frame, int this_frame) {
  if (last_frame != this_frame && _table.size() > 1) {
    if (_table[last_frame % _table.size()] !=
        _table[this_frame % _table.size()]) {
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::get_value
//       Access: Public, Virtual
//  Description: Gets the value of the channel at the indicated frame.
////////////////////////////////////////////////////////////////////
void AnimChannelScalarTable::
get_value(int frame, float &value) {
  if (_table.empty()) {
    value = 0.0f;
  } else {
    value = _table[frame % _table.size()];
  }
}


////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::set_table
//       Access: Public
//  Description: Assigns the data table.
////////////////////////////////////////////////////////////////////
void AnimChannelScalarTable::
set_table(const CPTA_float &table) {
  int num_frames = _root->get_num_frames();

  if (table.size() > 1 && (int)table.size() < num_frames) {
    // The new table has an invalid number of frames--it doesn't match
    // the bundle's requirement.
    return;
  }

  _table = table;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::write
//       Access: Public, Virtual
//  Description: Writes a brief description of the table and all of
//               its descendants.
////////////////////////////////////////////////////////////////////
void AnimChannelScalarTable::
write(ostream &out, int indent_level) const {
  indent(out, indent_level)
    << get_type() << " " << get_name() << " " << _table.size();

  if (!_children.empty()) {
    out << " {\n";
    write_descendants(out, indent_level + 2);
    indent(out, indent_level) << "}";
  }

  out << "\n";
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::write_datagram
//       Access: Public
//  Description: Function to write the important information in
//               the particular object to a Datagram
////////////////////////////////////////////////////////////////////
void AnimChannelScalarTable::
write_datagram(BamWriter *manager, Datagram &me)
{
  AnimChannelScalar::write_datagram(manager, me);

  if (compress_channels && !FFTCompressor::is_compression_available()) {
    chan_cat.error()
      << "Compression is not available; writing uncompressed channels.\n";
    compress_channels = false;
  }

  me.add_bool(compress_channels);
  if (!compress_channels) {
    // Write out everything the old way, as floats.
    me.add_uint16(_table.size());
    for(int i = 0; i < (int)_table.size(); i++) {
      me.add_float32(_table[i]);
    }

  } else {
    // Some channels, particularly blink channels, may involve only a
    // small number of discrete values.  If we come across one of
    // those, write it out losslessly, since the lossy compression
    // could damage it significantly (and we can achieve better
    // compression directly anyway).  We consider the channel value
    // only to the nearest 1000th for this purpose, because floats
    // aren't very good at being precisely equal to each other.
    static const int max_values = 16;
    static const float scale = 1000.0f;

    pmap<int, int> index;
    int i;
    for (i = 0;
         i < (int)_table.size() && (int)index.size() <= max_values;
         i++) {
      int value = (int)cfloor(_table[i] * scale + 0.5f);
      index.insert(pmap<int, int>::value_type(value, index.size()));
    }
    int index_length = index.size();
    if (index_length <= max_values) {
      // All right, here's a blink channel.  Now we write out the
      // index table, and then a table of all the index values, two
      // per byte.
      me.add_uint8(index_length);

      if (index_length > 0) {
        // We need to write the index in order by its index number; for
        // this, we need to invert the index.
        vector_float reverse_index(index_length);
        pmap<int, int>::iterator mi;
        for (mi = index.begin(); mi != index.end(); ++mi) {
          float f = (float)(*mi).first / scale;
          int i = (*mi).second;
          nassertv(i >= 0 && i < (int)reverse_index.size());
          reverse_index[i] = f;
        }

        for (i = 0; i < index_length; i++) {
          me.add_float32(reverse_index[i]);
        }

        // Now write out the actual channels.  We write these two at a
        // time, in the high and low nibbles of each byte.
        int table_length = _table.size();
        me.add_uint16(table_length);

        if (index_length == 1) {
          // In fact, we don't even need to write the channels at all,
          // if there weren't at least two different values.

        } else {
          for (i = 0; i < table_length - 1; i+= 2) {
            int value1 = (int)cfloor(_table[i] * scale + 0.5f);
            int value2 = (int)cfloor(_table[i + 1] * scale + 0.5f);
            int i1 = index[value1];
            int i2 = index[value2];

            me.add_uint8((i1 << 4) | i2);
          }

          // There might be one odd value.
          if (i < table_length) {
            int value1 = (int)cfloor(_table[i] * scale + 0.5f);
            int i1 = index[value1];

            me.add_uint8(i1 << 4);
          }
        }
      }

    } else {
      // No, we have continuous channels.  Write them out using lossy
      // compression.
      me.add_uint8(0xff);

      FFTCompressor compressor;
      compressor.set_quality(compress_chan_quality);
      compressor.set_use_error_threshold(true);
      compressor.write_header(me);

      compressor.write_reals(me, _table, _table.size());
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::fillin
//       Access: Protected
//  Description: Function that reads out of the datagram (or asks
//               manager to read) all of the data that is needed to
//               re-create this object and stores it in the appropiate
//               place
////////////////////////////////////////////////////////////////////
void AnimChannelScalarTable::
fillin(DatagramIterator& scan, BamReader* manager)
{
  AnimChannelScalar::fillin(scan, manager);

  bool wrote_compressed = scan.get_bool();

  PTA_float temp_table=PTA_float::empty_array(0);

  if (!wrote_compressed) {
    // Regular floats.
    int size = scan.get_uint16();
    for(int i = 0; i < size; i++) {
      temp_table.push_back(scan.get_float32());
    }

  } else {
    // Compressed channels.
    // Did we write them as discrete or continuous channel values?
    int index_length = scan.get_uint8();

    if (index_length < 0xff) {
      // Discrete.  Read in the index.
      if (index_length > 0) {
        float *index = (float *)alloca(index_length * sizeof(float));

        int i;
        for (i = 0; i < index_length; i++) {
          index[i] = scan.get_float32();
        }

        // Now read in the channel values.
        int table_length = scan.get_uint16();
        if (index_length == 1) {
          // With only one index value, we can infer the table.
          for (i = 0; i < table_length; i++) {
            temp_table.push_back(index[0]);
          }
        } else {
          // Otherwise, we must read it.
          for (i = 0; i < table_length - 1; i+= 2) {
            int num = scan.get_uint8();
            int i1 = (num >> 4) & 0xf;
            int i2 = num & 0xf;
            temp_table.push_back(index[i1]);
            temp_table.push_back(index[i2]);
          }
          // There might be one odd value.
          if (i < table_length) {
            int num = scan.get_uint8();
            int i1 = (num >> 4) & 0xf;
            temp_table.push_back(index[i1]);
          }
        }
      }
    } else {
      // Continuous channels.
      FFTCompressor compressor;
      compressor.read_header(scan, manager->get_file_minor_ver());
      compressor.read_reals(scan, temp_table.v());
    }
  }

  _table = temp_table;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::make_AnimChannelScalarTable
//       Access: Protected
//  Description: Factory method to generate a AnimChannelScalarTable object
////////////////////////////////////////////////////////////////////
TypedWritable* AnimChannelScalarTable::
make_AnimChannelScalarTable(const FactoryParams &params)
{
  AnimChannelScalarTable *me = new AnimChannelScalarTable;
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  me->fillin(scan, manager);
  return me;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimChannelScalarTable::register_with_factory
//       Access: Public, Static
//  Description: Factory method to generate a AnimChannelScalarTable object
////////////////////////////////////////////////////////////////////
void AnimChannelScalarTable::
register_with_read_factory(void)
{
  BamReader::get_factory()->register_factory(get_class_type(), make_AnimChannelScalarTable);
}




