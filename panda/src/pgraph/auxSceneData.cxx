// Filename: auxSceneData.cxx
// Created by:  drose (27Sep04)
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

#include "auxSceneData.h"
#include "indent.h"

TypeHandle AuxSceneData::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: AuxSceneData::output
//       Access: Published, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void AuxSceneData::
output(ostream &out) const {
  out << get_type() << " expires " << get_expiration_time();
}

////////////////////////////////////////////////////////////////////
//     Function: AuxSceneData::write
//       Access: Published, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void AuxSceneData::
write(ostream &out, int indent_level) const {
  indent(out, indent_level) << *this << "\n";
}
