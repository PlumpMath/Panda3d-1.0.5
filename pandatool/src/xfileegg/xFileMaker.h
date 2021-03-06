// Filename: xFileMaker.h
// Created by:  drose (19Jun01)
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

#ifndef XFILEMAKER_H
#define XFILEMAKER_H

#include "pandatoolbase.h"

#include "filename.h"
#include "pmap.h"
#include "luse.h"
#include "xFile.h"

class EggNode;
class EggGroupNode;
class EggGroup;
class EggBin;
class EggData;
class EggVertexPool;
class Datagram;
class XFileMesh;

////////////////////////////////////////////////////////////////////
//       Class : XFileMaker
// Description : This class converts a Panda scene graph into a .X
//               file and writes it out.
////////////////////////////////////////////////////////////////////
class XFileMaker {
public:
  XFileMaker();
  ~XFileMaker();

  bool write(const Filename &filename);

  bool add_tree(EggData &egg_data);

private:
  bool add_node(EggNode *egg_node, XFileNode *x_parent);
  bool add_group(EggGroup *egg_group, XFileNode *x_parent);
  bool add_bin(EggBin *egg_bin, XFileNode *x_parent);
  bool add_polyset(EggBin *egg_bin, XFileNode *x_parent);

  bool recurse_nodes(EggGroupNode *egg_node, XFileNode *x_parent);

  XFileMesh *get_mesh(XFileNode *x_parent);
  bool finalize_mesh(XFileNode *x_parent, XFileMesh *mesh);

  PT(XFile) _x_file;

  int _mesh_index;

  typedef pmap<XFileNode *, XFileMesh *> Meshes;
  Meshes _meshes;
};

#endif

