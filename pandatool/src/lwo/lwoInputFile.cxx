// Filename: lwoInputFile.cxx
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

#include "lwoInputFile.h"
#include "lwoBoundingBox.h"
#include "lwoClip.h"
#include "lwoDiscontinuousVertexMap.h"
#include "lwoHeader.h"
#include "lwoLayer.h"
#include "lwoPoints.h"
#include "lwoPolygons.h"
#include "lwoPolygonTags.h"
#include "lwoTags.h"
#include "lwoSurface.h"
#include "lwoVertexMap.h"

TypeHandle LwoInputFile::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: LwoInputFile::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
LwoInputFile::
LwoInputFile() {
}

////////////////////////////////////////////////////////////////////
//     Function: LwoInputFile::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
LwoInputFile::
~LwoInputFile() {
}

////////////////////////////////////////////////////////////////////
//     Function: LwoInputFile::get_vx
//       Access: Public
//  Description: Reads a Lightwave variable-length index.  This is
//               either a 2-byte or 4-byte integer.
////////////////////////////////////////////////////////////////////
int LwoInputFile::
get_vx() {
  PN_uint16 top = get_be_uint16();
  if ((top & 0xff00) == 0xff00) {
    // The first byte is 0xff, which indicates we have a 4-byte
    // integer.
    PN_uint16 bottom = get_be_uint16();
    return ((int)(top & 0xff) << 16) | bottom;
  }

  // The first byte is not 0xff, which indicates we have a 2-byte
  // integer.
  return top;
}

////////////////////////////////////////////////////////////////////
//     Function: LwoInputFile::get_vec3
//       Access: Public
//  Description: Reads a three-component vector of floats.
////////////////////////////////////////////////////////////////////
LVecBase3f LwoInputFile::
get_vec3() {
  LVecBase3f result;
  result[0] = get_be_float32();
  result[1] = get_be_float32();
  result[2] = get_be_float32();
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: LwoInputFile::get_filename
//       Access: Public
//  Description: Reads a Lightwave platform-neutral filename and
//               converts it to a Panda platform-neutral filename.
////////////////////////////////////////////////////////////////////
Filename LwoInputFile::
get_filename() {
  string name = get_string();
  size_t colon = name.find(':');
  if (colon == string::npos) {
    // No colon; it's just a relative path.
    return Filename(name);
  }

  // The colon separates the device and the path.
  string device = name.substr(0, colon);
  string path = name.substr(colon + 1);

  nout << "Ignoring filename device " << device << "\n";
  return Filename("/", path);
}

////////////////////////////////////////////////////////////////////
//     Function: LwoInputFile::make_new_chunk
//       Access: Protected, Virtual
//  Description: Allocates and returns a new chunk of the appropriate
//               type based on the given ID.
////////////////////////////////////////////////////////////////////
IffChunk *LwoInputFile::
make_new_chunk(IffId id) {
  if (id == IffId("FORM")) {
    return new LwoHeader;

  } else if (id == IffId("LAYR")) {
    return new LwoLayer;

  } else if (id == IffId("PNTS")) {
    return new LwoPoints;

  } else if (id == IffId("VMAP")) {
    return new LwoVertexMap;

  } else if (id == IffId("VMAD")) {
    return new LwoDiscontinuousVertexMap;

  } else if (id == IffId("POLS")) {
    return new LwoPolygons;

  } else if (id == IffId("TAGS") ||
             id == IffId("SRFS")) {
    return new LwoTags;

  } else if (id == IffId("PTAG")) {
    return new LwoPolygonTags;

  } else if (id == IffId("CLIP")) {
    return new LwoClip;

  } else if (id == IffId("SURF")) {
    return new LwoSurface;

  } else if (id == IffId("BBOX")) {
    return new LwoBoundingBox;

  } else {
    return IffInputFile::make_new_chunk(id);
  }
}
