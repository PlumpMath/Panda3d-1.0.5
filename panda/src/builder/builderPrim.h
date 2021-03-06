// Filename: builderPrim.h
// Created by:  drose (09Sep97)
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

#ifndef BUILDERPRIM_H
#define BUILDERPRIM_H

///////////////////////////////////////////////////////////////////
//
// BuilderPrim, BuilderPrimI
//
// The basic class for passing primitives (polygons, etc.) to the
// builder.  See the comments at the beginning of builder.h and
// builderVertex.h.
//
// A BuilderPrim has a few attributes of its own--color and
// normal--which are defined in builderAttrib.h (which it inherits
// from).  It also has a collection of vertices, represented as
// BuilderVertex objects, each of which can have its own attributes as
// well.  Any additional attributes, such as texture, lighting, etc.,
// are considered to be external to the primitive, and are defined in
// the BuilderBucket object.
//
// BuilderPrimI is exactly like BuilderPrim, except that it represents
// an indexed primitive.  A BuilderPrimI keeps its collection of
// vertices as BuilderVertexI's, which store their values as index
// numbers into an array rather than as actual coordinate values. The
// arrays themselves are stored in the BuilderBucket.
//
// In fact, BuilderPrim and BuilderPrimI are both instantiations of
// the same template object, BuilderPrimTempl, with different vertex
// types (BuilderVertex and BuilderVertexI, respectively).
//
// It is this templating that drives most of the code in this package.
// A lot of stuff in the builder tool, and everything in the mesher
// tool, is templated on the BuilderPrim type, so the same code is
// used to support both indexed and nonindexed primitives.
//
//
// In addition to storing the primitives--individual polygons,
// generally--as passed in by user code, BuilderPrim objects can store
// the compound primitives that might have been generated by the
// mesher, like triangle strips.  In this case, in addition to an
// array of vertices, it has an array of component attributes, which
// store the attributes specific to each individual component
// (e.g. the normal of each triangle in a triangle strip).
//
///////////////////////////////////////////////////////////////////

#include "pandabase.h"
#include "builderPrimTempl.h"
#include "builderBucket.h"
#include "builderTypes.h"
#include "pta_ushort.h"
#include "geom.h"

EXPORT_TEMPLATE_CLASS(EXPCL_PANDAEGG, EXPTP_PANDAEGG, BuilderPrimTempl<BuilderVertex>);
EXPORT_TEMPLATE_CLASS(EXPCL_PANDAEGG, EXPTP_PANDAEGG, BuilderPrimTempl<BuilderVertexI>);

/////////////////////////////////////////////////////////////////////
//       Class : BuilderPrim
// Description : The basic class for passing nonindexed primitives
//               to the builder.  See the comments at the the head of
//               this file, and in builder.h.
//
//               Look in builderPrimTempl.h and builderAttribTempl.h
//               for most of the interface to BuilderPrim.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEGG BuilderPrim : public BuilderPrimTempl<BuilderVertex> {
public:
  INLINE BuilderPrim();
  INLINE BuilderPrim(const BuilderPrim &copy);
  INLINE BuilderPrim &operator = (const BuilderPrim &copy);

  BuilderPrim &nonindexed_copy(const BuilderPrimTempl<BuilderVertexI> &copy,
                               const BuilderBucket &bucket);

  void flatten_vertex_properties();

  static void fill_geom(Geom *geom, const PTA_BuilderV &v_array,
                        GeomBindType n_attr, const PTA_BuilderN &n_array,
                        GeomBindType c_attr, const PTA_BuilderC &c_array,
                        const TexCoordFill &texcoords,
                        const BuilderBucket &bucket,
                        int num_prims, int num_components, int num_verts);
};


///////////////////////////////////////////////////////////////////
//       Class : BuilderPrimI
// Description : The basic class for passing indexed primitives
//               to the builder.
//
//               Look in builderPrimTempl.h and builderAttribTempl.h
//               for most of the interface to BuilderPrimI.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEGG BuilderPrimI : public BuilderPrimTempl<BuilderVertexI> {
public:
  INLINE BuilderPrimI();
  INLINE BuilderPrimI(const BuilderPrimI &copy);
  INLINE BuilderPrimI &operator = (const BuilderPrimI &copy);

  void flatten_vertex_properties();

  static void fill_geom(Geom *geom, const PTA_ushort &v_array,
                        GeomBindType n_attr, PTA_ushort n_array,
                        GeomBindType c_attr, PTA_ushort c_array,
                        const TexCoordFill &texcoords,
                        const BuilderBucket &bucket,
                        int num_prims, int num_components, int num_verts);
};

#include "builderPrim.I"


// Tell GCC that we'll take care of the instantiation explicitly here.
#ifdef __GNUC__
#pragma interface
#endif

#endif
