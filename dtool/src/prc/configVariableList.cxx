// Filename: configVariableList.cxx
// Created by:  drose (20Oct04)
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

#include "configVariableList.h"

////////////////////////////////////////////////////////////////////
//     Function: ConfigVariableList::output
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void ConfigVariableList::
output(ostream &out) const {
  out << get_num_values() << " values.";
}

////////////////////////////////////////////////////////////////////
//     Function: ConfigVariableList::write
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void ConfigVariableList::
write(ostream &out) const {
  int num_values = get_num_values();
  for (int i = 0; i < num_values; ++i) {
    out << get_string_value(i) << "\n";
  }
}
