// Filename: eggXfmAnimData.cxx
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

#include "eggXfmAnimData.h"
#include "eggXfmSAnim.h"
#include "eggSAnimData.h"
#include "eggMiscFuncs.h"
#include "config_egg.h"

#include "indent.h"
#include "luse.h"
#include "lmatrix.h"
#include "compose_matrix.h"

TypeHandle EggXfmAnimData::_type_handle;


////////////////////////////////////////////////////////////////////
//     Function: EggXfmAnimData::Conversion constructor
//       Access: Public
//  Description: Converts the newer-style XfmSAnim table to the
//               older-style XfmAnim table.
////////////////////////////////////////////////////////////////////
EggXfmAnimData::
EggXfmAnimData(const EggXfmSAnim &convert_from)
  : EggAnimData(convert_from.get_name())
{
  if (convert_from.has_order()) {
    set_order(convert_from.get_order());
  }
  if (convert_from.has_fps()) {
    set_fps(convert_from.get_fps());
  }
  _coordsys = convert_from.get_coordinate_system();

  // First, get the table names so we know how to build up our
  // contents string.  Also store up the SAnim tables themselves in a
  // temporary vector for convenience.

  pvector<EggSAnimData *> subtables;

  EggXfmSAnim::const_iterator ci;
  for (ci = convert_from.begin(); ci != convert_from.end(); ++ci) {
    if ((*ci)->is_of_type(EggSAnimData::get_class_type())) {
      EggSAnimData *sanim = DCAST(EggSAnimData, *ci);
      nassertv(sanim->get_name().length() == 1);

      if (sanim->get_num_rows() > 0) {
        subtables.push_back(sanim);
        _contents += sanim->get_name()[0];
      }
    }
  }

  // Now, go through and extract out all the data.
  int num_rows = convert_from.get_num_rows();
  for (int row = 0; row < num_rows; row++) {
    for (int col = 0; col < (int)subtables.size(); col++) {
      EggSAnimData *sanim = subtables[col];
      if (sanim->get_num_rows() == 1) {
        add_data(sanim->get_value(0));
      } else {
        nassertv(row < sanim->get_num_rows());
        add_data(sanim->get_value(row));
      }
    }
  }
}


////////////////////////////////////////////////////////////////////
//     Function: EggXfmAnimData::get_value
//       Access: Public
//  Description: Returns the value of the aggregate row of the table
//               as a matrix.  This is a convenience function that
//               treats the 2-d table as if it were a single table of
//               matrices.
////////////////////////////////////////////////////////////////////
void EggXfmAnimData::
get_value(int row, LMatrix4d &mat) const {
  LVector3d scale(1.0, 1.0, 1.0);
  LVector3d shear(0.0, 0.0, 0.0);
  LVector3d hpr(0.0, 0.0, 0.0);
  LVector3d translate(0.0, 0.0, 0.0);

  for (int col = 0; col < get_num_cols(); col++) {
    double value = get_value(row, col);

    switch (_contents[col]) {
    case 'i':
      scale[0] = value;
      break;

    case 'j':
      scale[1] = value;
      break;

    case 'k':
      scale[2] = value;
      break;

    case 'a':
      shear[0] = value;
      break;
      
    case 'b':
      shear[1] = value;
      break;
      
    case 'c':
      shear[2] = value;
      break;

    case 'h':
      hpr[0] = value;
      break;

    case 'p':
      hpr[1] = value;
      break;

    case 'r':
      hpr[2] = value;
      break;

    case 'x':
      translate[0] = value;
      break;

    case 'y':
      translate[1] = value;
      break;

    case 'z':
      translate[2] = value;
      break;

    default:
      // The contents string contained an invalid letter.
      nassertv(false);
    }
  }

  // So now we've got the twelve components; build a matrix.
  EggXfmSAnim::compose_with_order(mat, scale, shear, hpr, translate, get_order(),
                                  _coordsys);
}

////////////////////////////////////////////////////////////////////
//     Function: EggXfmAnimData::is_anim_matrix
//       Access: Public, Virtual
//  Description: Returns true if this node represents a table of
//               animation transformation data, false otherwise.
////////////////////////////////////////////////////////////////////
bool EggXfmAnimData::
is_anim_matrix() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: EggXfmAnimData::write
//       Access: Public, Virtual
//  Description: Writes the data to the indicated output stream in Egg
//               format.
////////////////////////////////////////////////////////////////////
void EggXfmAnimData::
write(ostream &out, int indent_level) const {
  write_header(out, indent_level, "<Xfm$Anim>");

  if (has_fps()) {
    indent(out, indent_level + 2)
      << "<Scalar> fps { " << get_fps() << " }\n";
  }

  if (has_order()) {
    indent(out, indent_level + 2)
      << "<Char*> order { " << get_order() << " }\n";
  }

  if (has_contents()) {
    indent(out, indent_level + 2)
      << "<Char*> contents { " << get_contents() << " }\n";
  }

  indent(out, indent_level + 2) << "<V> {\n";
  write_long_list(out, indent_level + 4, _data.begin(), _data.end(),
        "", "", 72);
  indent(out, indent_level + 2) << "}\n";
  indent(out, indent_level) << "}\n";
}

////////////////////////////////////////////////////////////////////
//     Function: EggXfmAnimData::r_transform
//       Access: Protected, Virtual
//  Description: Applies the indicated transform to all the rows of
//               the table.  This actually forces the generation of a
//               totally new set of rows.
////////////////////////////////////////////////////////////////////
void EggXfmAnimData::
r_transform(const LMatrix4d &mat, const LMatrix4d &inv,
            CoordinateSystem to_cs) {
  // We need to build an inverse matrix that doesn't reflect the
  // translation component.
  LMatrix4d inv1 = inv;
  inv1.set_row(3, LVector3d(0.0, 0.0, 0.0));

  // Now we build a temporary copy of the table as an EggXfmSAnim.  We
  // do this because this kind of table is easier to build and
  // optimize.

  if (to_cs == CS_default) {
    to_cs = _coordsys;
  }

  EggXfmSAnim new_table(get_name(), to_cs);
  if (has_fps()) {
    new_table.set_fps(get_fps());
  }

  // We insist on the standard order now.
  new_table.set_order(get_standard_order());

  // Now build up the data into the new table.
  LMatrix4d orig_mat;
  for (int r = 0; r < get_num_rows(); r++) {
    get_value(r, orig_mat);
    bool result = new_table.add_data(inv1 * orig_mat * mat);

    if (!result) {
      egg_cat.error()
        << "Transform from " << _coordsys << " to " << to_cs
        << " failed!\n";
      LVector3d scale, shear, hpr, trans;
      bool d = decompose_matrix(orig_mat, scale, shear, hpr, trans, _coordsys);
      egg_cat.error(false)
        << "orig:\n" << orig_mat
        << "d = " << d
        << "\n  scale: " << scale
        << "\n  shear: " << shear
        << "\n  hpr: " << hpr
        << "\n  trans: " << trans << "\n";

      LMatrix4d new_mat = inv1 * orig_mat * mat;
      d = decompose_matrix(new_mat, scale, shear, hpr, trans, to_cs);
      egg_cat.error(false)
        << "new:\n" << new_mat
        << "d = " << d
        << "\n  scale: " << scale
        << "\n  shear: " << shear
        << "\n  hpr: " << hpr
        << "\n  trans: " << trans << "\n";
    }

    // If this assertion fails, we attempted to transform by non-affine
    // matrix or some such thing that cannot be represented in an anim
    // file.
    nassertv(result);
  }

  // Now clean out the redundant columns we created.
  new_table.optimize();

  // And copy that back into EggXfmAnimData table form.
  EggXfmAnimData copy_table(new_table);
  (*this) = copy_table;
}

////////////////////////////////////////////////////////////////////
//     Function: EggXfmAnimData::r_mark_coordsys
//       Access: Protected, Virtual
//  Description: This is only called immediately after loading an egg
//               file from disk, to propagate the value found in the
//               CoordinateSystem entry (or the default Y-up
//               coordinate system) to all nodes that care about what
//               the coordinate system is.
////////////////////////////////////////////////////////////////////
void EggXfmAnimData::
r_mark_coordsys(CoordinateSystem cs) {
  _coordsys = cs;
}
