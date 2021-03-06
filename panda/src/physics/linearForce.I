// Filename: linearForce.I
// Created by:  charles (13Jun00)
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

////////////////////////////////////////////////////////////////////
//    Function : set_amplitude
//      Access : Public
////////////////////////////////////////////////////////////////////
INLINE void LinearForce::
set_amplitude(float a) {
  _amplitude = a;
}

////////////////////////////////////////////////////////////////////
//    Function : get_amplitude
//      Access : Public
////////////////////////////////////////////////////////////////////
INLINE float LinearForce::
get_amplitude() const {
  return _amplitude;
}

////////////////////////////////////////////////////////////////////
//    Function : get_mass_dependent
//      Access : Public
////////////////////////////////////////////////////////////////////
INLINE bool LinearForce::
get_mass_dependent() const {
  return _mass_dependent;
}

////////////////////////////////////////////////////////////////////
//    Function : set_mass_Dependent
//      Access : Public
////////////////////////////////////////////////////////////////////
INLINE void LinearForce::
set_mass_dependent(bool m) {
  _mass_dependent = m;
}

////////////////////////////////////////////////////////////////////
//    Function : set_vector_masks
//      Access : Public
////////////////////////////////////////////////////////////////////
INLINE void LinearForce::
set_vector_masks(bool x, bool y, bool z) {
  _x_mask = x;
  _y_mask = y;
  _z_mask = z;
}
