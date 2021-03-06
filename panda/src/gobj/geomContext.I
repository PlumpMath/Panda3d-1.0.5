// Filename: geomContext.I
// Created by:  drose (19Mar04)
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
//     Function: GeomContext::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE GeomContext::
GeomContext(Geom *geom) :
  _geom(geom)
{
  _dirty_flags = 0;
}

////////////////////////////////////////////////////////////////////
//     Function: GeomContext::mark_dirty
//       Access: Public
//  Description: Marks the context "dirty", i.e. its properties are
//               different from the last time the GSG has seen them.
//               Presumably, the GSG will respond by updating the
//               properties and clearing the dirty bits the next time
//               it renders the geom.
//
//               The value is the union of all the bits that are to be
//               set dirty; bits that are not set in this parameter
//               are left unchanged.  See Geom::DirtyFlags for a
//               list of available bits.
//
//               Usually this function is not called directly, but
//               rather is called by Geom::mark_dirty() as a result
//               of changing properties directly on the geom.
////////////////////////////////////////////////////////////////////
INLINE void GeomContext::
mark_dirty(int flags_to_set) {
  _dirty_flags |= flags_to_set;
}

////////////////////////////////////////////////////////////////////
//     Function: GeomContext::clear_dirty_flags
//       Access: Public
//  Description: Removes the indicated flags from the "dirty" bits.
//               See mark_dirty().
//
//               The value is the union of all the bits that are to be
//               cleared; if a bit is set in the parameter, it will be
//               removed from the dirty set.  Bits that are not set in
//               this parameter are left unchanged.
//
//               This function is intended to be called by the GSG
//               after it has updated the geom parameters.
////////////////////////////////////////////////////////////////////
INLINE void GeomContext::
clear_dirty_flags(int flags_to_clear) {
  _dirty_flags &= ~flags_to_clear;
  _geom->_all_dirty_flags &= ~flags_to_clear;
}

////////////////////////////////////////////////////////////////////
//     Function: GeomContext::get_dirty_flags
//       Access: Public
//  Description: Returns the current state of the dirty flags.  If
//               this is non-zero, it represents the union of all
//               properties that have been changed since the last call
//               to clear_dirty_flags().
//
//               This function is intended to be called by the GSG to
//               determine what properties need to be updated.  See
//               Geom::DirtyFlags for a list of possible bits.
////////////////////////////////////////////////////////////////////
INLINE int GeomContext::
get_dirty_flags() const {
  return _dirty_flags;
}

