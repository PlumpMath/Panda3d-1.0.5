// Filename: softEggGroupUserData.h
// Created by:  masad (25Sep03)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2003, Disney Enterprises, Inc.  All rights reserved
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

#ifndef SOFTEGGGROUPUSERDATA_H
#define SOFTEGGGROUPUSERDATA_H

#include "pandatoolbase.h"
#include "eggUserData.h"

////////////////////////////////////////////////////////////////////
//       Class : SoftEggGroupUserData
// Description : This class contains extra user data which is
//               piggybacked onto EggGroup objects for the purpose of
//               the softimage converter.
////////////////////////////////////////////////////////////////////
class SoftEggGroupUserData : public EggUserData {
public:
  INLINE SoftEggGroupUserData();
  INLINE SoftEggGroupUserData(const SoftEggGroupUserData &copy);
  INLINE void operator = (const SoftEggGroupUserData &copy);

  bool _vertex_color;
  bool _double_sided;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    EggUserData::init_type();
    register_type(_type_handle, "SoftEggGroupUserData",
                  EggUserData::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "softEggGroupUserData.I"

#endif
