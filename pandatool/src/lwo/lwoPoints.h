// Filename: lwoPoints.h
// Created by:  drose (24Apr01)
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

#ifndef LWOPOINTS_H
#define LWOPOINTS_H

#include "pandatoolbase.h"

#include "lwoChunk.h"

#include "luse.h"

////////////////////////////////////////////////////////////////////
//       Class : LwoPoints
// Description : An array of points that will be referenced by later
//               chunks.
////////////////////////////////////////////////////////////////////
class LwoPoints : public LwoChunk {
public:
  int get_num_points() const;
  const LPoint3f &get_point(int n) const;

public:
  virtual bool read_iff(IffInputFile *in, size_t stop_at);
  virtual void write(ostream &out, int indent_level = 0) const;

private:
  typedef pvector<LPoint3f> Points;
  Points _points;

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    LwoChunk::init_type();
    register_type(_type_handle, "LwoPoints",
                  LwoChunk::get_class_type());
  }

private:
  static TypeHandle _type_handle;
};

#endif


