// Filename: datagramIterator.h
// Created by:  jns (07Feb00)
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

#ifndef DATAGRAMITERATOR_H
#define DATAGRAMITERATOR_H

#include "pandabase.h"

#include "datagram.h"
#include "numeric_types.h"

////////////////////////////////////////////////////////////////////
//       Class : DatagramIterator
// Description : A class to retrieve the individual data elements
//               previously stored in a Datagram.  Elements may be
//               retrieved one at a time; it is up to the caller to
//               know the correct type and order of each element.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEXPRESS DatagramIterator {
PUBLISHED:
  INLINE DatagramIterator();
  INLINE DatagramIterator(const Datagram &datagram, size_t offset = 0);
  INLINE DatagramIterator(const DatagramIterator &copy);
  INLINE void operator = (const DatagramIterator &copy);
  INLINE ~DatagramIterator();

  INLINE bool get_bool();
  INLINE PN_int8 get_int8();
  INLINE PN_uint8 get_uint8();

  INLINE PN_int16 get_int16();
  INLINE PN_int32 get_int32();
  INLINE PN_int64 get_int64();
  INLINE PN_uint16 get_uint16();
  INLINE PN_uint32 get_uint32();
  INLINE PN_uint64 get_uint64();
  INLINE float get_float32();
  INLINE PN_float64 get_float64();

  INLINE PN_int16 get_be_int16();
  INLINE PN_int32 get_be_int32();
  INLINE PN_int64 get_be_int64();
  INLINE PN_uint16 get_be_uint16();
  INLINE PN_uint32 get_be_uint32();
  INLINE PN_uint64 get_be_uint64();
  INLINE float get_be_float32();
  INLINE PN_float64 get_be_float64();

  string get_string();
  string get_string32();
  string get_z_string();
  string get_fixed_string(size_t size);

  INLINE void skip_bytes(size_t size);
  string extract_bytes(size_t size);

  INLINE string get_remaining_bytes() const;
  INLINE int get_remaining_size() const;

  INLINE const Datagram &get_datagram() const;
  INLINE size_t get_current_index() const;

  void output(ostream &out) const;
  void write(ostream &out, unsigned int indent=0) const;

private:
  const Datagram *_datagram;
  size_t _current_index;
};

// These generic functions are primarily for reading a value from a
// datagram from within a template in which the actual type of the
// value is not known.  If you do know the type, it's preferable to
// use the explicit get_*() method from above instead.

INLINE void
generic_read_datagram(bool &result, DatagramIterator &source);
INLINE void
generic_read_datagram(int &result, DatagramIterator &source);
INLINE void
generic_read_datagram(float &result, DatagramIterator &source);
INLINE void
generic_read_datagram(double &result, DatagramIterator &source);
INLINE void
generic_read_datagram(string &result, DatagramIterator &source);

#include "datagramIterator.I"

#endif
