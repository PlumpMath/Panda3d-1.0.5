// Filename: eggMatrixTablePointer.h
// Created by:  drose (26Feb01)
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

#ifndef EGGMATRIXTABLEPOINTER_H
#define EGGMATRIXTABLEPOINTER_H

#include "pandatoolbase.h"

#include "eggJointPointer.h"

#include "eggTable.h"
#include "eggXfmSAnim.h"
#include "pointerTo.h"

////////////////////////////////////////////////////////////////////
//       Class : EggMatrixTablePointer
// Description : This stores a pointer back to an EggXfmSAnim table
//               (i.e. an <Xfm$Anim_S$> entry in an egg file),
//               corresponding to the animation data from a single
//               bundle for this joint.
////////////////////////////////////////////////////////////////////
class EggMatrixTablePointer : public EggJointPointer {
public:
  EggMatrixTablePointer(EggObject *object);

  virtual int get_num_frames() const; 
  virtual void extend_to(int num_frames);
  virtual LMatrix4d get_frame(int n) const;
  virtual void set_frame(int n, const LMatrix4d &mat);
  virtual bool add_frame(const LMatrix4d &mat);

  virtual void do_finish_reparent(EggJointPointer *new_parent);

  virtual bool do_rebuild();

  virtual void optimize();
  virtual void zero_channels(const string &components);
  virtual void quantize_channels(const string &components, double quantum);

  virtual EggJointPointer *make_new_joint(const string &name);

private:
  PT(EggTable) _table;
  PT(EggXfmSAnim) _xform;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    EggJointPointer::init_type();
    register_type(_type_handle, "EggMatrixTablePointer",
                  EggJointPointer::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif


