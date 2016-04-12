// Filename: dataNode.h
// Created by:  drose (11Mar02)
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

#ifndef DATANODE_H
#define DATANODE_H

////////////////////////////////////////////////////////////////////
//
// The Data Graph.
//
// The data graph is intended to hook up devices and their inputs
// and/or outputs in a clean interface.  It uses the same graph
// relationship that is used to construct the scene graph, with the
// same sort of nodes and NodePaths.
//
// In a data graph, each node may potentially produce and/or consume
// data, and the arcs transmit data downward, from the root of the
// graph to its leaves.  Thus, an input device such as a mouse might
// be added to the graph near the root, and a tformer-style object
// that interprets the mouse data as a trackball motion and outputs a
// matrix might be the immediate child of the mouse, followed by an
// object that accepts a matrix and sets it on some particular arc in
// the scene graph.
//
// Each different kind of DataNode defines its own set of input values
// and output values, identified by name.  When a DataNode is attached
// to another DataNode, the inputs of the child are automatically
// connected up to the corresponding outputs of the parent, and an
// error message is issued if there are no matching connections.
//
////////////////////////////////////////////////////////////////////

#include "pandabase.h"

#include "pandaNode.h"
#include "pointerTo.h"

class DataNodeTransmit;

////////////////////////////////////////////////////////////////////
//       Class : DataNode
// Description : The fundamental type of node for the data graph.  The
//               DataNode class is itself primarily intended as an
//               abstract class; it defines no inputs and no outputs.
//               Most kinds of data nodes will derive from this to
//               specify the inputs and outputs in the constructor.
//
//               DataNode does not attempt to cycle its data with a
//               PipelineCycler.  The data graph is intended to be
//               used only within a single thread.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA DataNode : public PandaNode {
PUBLISHED:
  INLINE DataNode(const string &name);

protected:
  INLINE DataNode(const DataNode &copy);
public:
  virtual PandaNode *make_copy() const;

  void transmit_data(const DataNodeTransmit inputs[],
                     DataNodeTransmit &output);

  INLINE int get_num_inputs() const;
  INLINE int get_num_outputs() const;

PUBLISHED:
  void write_inputs(ostream &out) const;
  void write_outputs(ostream &out) const;
  void write_connections(ostream &out) const;

protected:
  int define_input(const string &name, TypeHandle data_type);
  int define_output(const string &name, TypeHandle data_type);

protected:
  // Inherited from PandaNode
  virtual void parents_changed();

  // Local to DataNode
  virtual void do_transmit_data(const DataNodeTransmit &input,
                                DataNodeTransmit &output);

private:
  void reconnect();

  class WireDef {
  public:
    TypeHandle _data_type;
    int _index;
  };

  typedef pmap<string, WireDef> Wires;

  Wires _input_wires;
  Wires _output_wires;

  class DataConnection {
  public:
    int _parent_index;
    int _output_index;
    int _input_index;
  };
  typedef pvector<DataConnection> DataConnections;
  DataConnections _data_connections;

public:
  virtual void write_datagram(BamWriter *manager, Datagram &dg);

protected:
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PandaNode::init_type();
    register_type(_type_handle, "DataNode",
                  PandaNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "dataNode.I"

#endif