// Filename: cullableObject.cxx
// Created by:  drose (04Mar02)
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

#include "cullableObject.h"


CullableObject *CullableObject::_deleted_chain = (CullableObject *)NULL;
int CullableObject::_num_ever_allocated = 0;
TypeHandle CullableObject::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: CullableObject::Destructor
//       Access: Public
//  Description: Automatically deletes the whole chain of these things.
////////////////////////////////////////////////////////////////////
CullableObject::
~CullableObject() {
  if (_next != (CullableObject *)NULL) {
    delete _next;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CullableObject::output
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void CullableObject::
output(ostream &out) const {
  if (_geom != (Geom *)NULL) {
    out << *_geom;
  } else {
    out << "(null)";
  }
}
