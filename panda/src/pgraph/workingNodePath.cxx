// Filename: workingNodePath.cxx
// Created by:  drose (16Mar02)
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

#include "workingNodePath.h"


////////////////////////////////////////////////////////////////////
//     Function: WorkingNodePath::is_valid
//       Access: Public
//  Description: Returns true if the WorkingNodePath object appears to
//               be a valid NodePath reference, false otherwise.
////////////////////////////////////////////////////////////////////
bool WorkingNodePath::
is_valid() const {
  if (_node == (PandaNode *)NULL) {
    return false;
  }
  if (_next == (WorkingNodePath *)NULL) {
    return (_start != (NodePathComponent *)NULL);
  }

  return _next->is_valid();
}

////////////////////////////////////////////////////////////////////
//     Function: WorkingNodePath::get_num_nodes
//       Access: Public
//  Description: Returns the number of nodes in the path from the root
//               to the current node.
//
//               Since a WorkingNodePath always consists of, at
//               minimum, a nonempty parent NodePath and one child
//               node, this method will always return at least 2.
////////////////////////////////////////////////////////////////////
int WorkingNodePath::
get_num_nodes() const {
  if (_next == (WorkingNodePath *)NULL) {
    return _start->get_length();
  }

  return _next->get_num_nodes() + 1;
}

////////////////////////////////////////////////////////////////////
//     Function: WorkingNodePath::get_node
//       Access: Public
//  Description: Returns the nth node of the path, where 0 is the
//               referenced (bottom) node and get_num_nodes() - 1 is
//               the top node.  This requires iterating through the
//               path.
////////////////////////////////////////////////////////////////////
PandaNode *WorkingNodePath::
get_node(int index) const {
  nassertr(index >= 0, NULL);
  if (index == 0) {
    return _node;
  }

  if (_next == (WorkingNodePath *)NULL) {
    return get_node_path().get_node(index - 1);
  }

  return _next->get_node(index - 1);
}

////////////////////////////////////////////////////////////////////
//     Function: WorkingNodePath::output
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void WorkingNodePath::
output(ostream &out) const {
  // Cheesy and slow, but when you're outputting the thing, presumably
  // you're not in a hurry.
  get_node_path().output(out);
}

////////////////////////////////////////////////////////////////////
//     Function: WorkingNodePath::r_get_node_path
//       Access: Private
//  Description: The private, recursive implementation of
//               get_node_path(), this returns the NodePathComponent
//               representing the NodePath.
////////////////////////////////////////////////////////////////////
PT(NodePathComponent) WorkingNodePath::
r_get_node_path() const {
  if (_next == (WorkingNodePath *)NULL) {
    nassertr(_start != (NodePathComponent *)NULL, NULL);
    return _start;
  }

  nassertr(_start == (NodePathComponent *)NULL, NULL);
  nassertr(_node != (PandaNode *)NULL, NULL);

  PT(NodePathComponent) comp = _next->r_get_node_path();
  nassertr(comp != (NodePathComponent *)NULL, NULL);

  PT(NodePathComponent) result = PandaNode::get_component(comp, _node);
  if (result == (NodePathComponent *)NULL) {
    // This means we found a disconnected chain in the
    // WorkingNodePath's ancestry: the node above this node isn't
    // connected.  In this case, don't attempt to go higher; just
    // truncate the NodePath at the bottom of the disconnect.
    return PandaNode::get_top_component(_node, true);
  }

  return result;
}
