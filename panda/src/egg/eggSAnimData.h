// Filename: eggSAnimData.h
// Created by:  drose (19Feb99)
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

#ifndef EGGSANIMDATA_H
#define EGGSANIMDATA_H

#include "pandabase.h"

#include "eggAnimData.h"

////////////////////////////////////////////////////////////////////
//       Class : EggSAnimData
// Description : Corresponding to an <S$Anim> entry, this stores a
//               single column of numbers, for instance for a morph
//               target, or as one column in an EggXfmSAnim.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEGG EggSAnimData : public EggAnimData {
PUBLISHED:
  INLINE EggSAnimData(const string &name = "");
  INLINE EggSAnimData(const EggSAnimData &copy);
  INLINE EggSAnimData &operator = (const EggSAnimData &copy);

  INLINE int get_num_rows() const;
  INLINE double get_value(int row) const;
  INLINE void set_value(int row, double value);

  void optimize();

  virtual void write(ostream &out, int indent_level) const;

public:

  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    EggAnimData::init_type();
    register_type(_type_handle, "EggSAnimData",
                  EggAnimData::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "eggSAnimData.I"

#endif

