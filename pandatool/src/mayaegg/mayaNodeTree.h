// Filename: mayaNodeTree.h
// Created by:  drose (06Jun03)
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

#ifndef MAYANODETREE_H
#define MAYANODETREE_H

#include "pandatoolbase.h"

#include "mayaNodeDesc.h"
#include "mayaBlendDesc.h"
#include "globPattern.h"
#include "indirectCompareNames.h"
#include "ordered_vector.h"
#include "pset.h"

class MayaToEggConverter;
class EggData;
class EggGroupNode;
class EggTable;
class EggXfmSAnim;
class EggSAnimData;

////////////////////////////////////////////////////////////////////
//       Class : MayaNodeTree
// Description : Describes a complete tree of maya nodes for
//               conversion.
////////////////////////////////////////////////////////////////////
class MayaNodeTree {
public:
  MayaNodeTree(MayaToEggConverter *converter);
  MayaNodeDesc *build_node(const MDagPath &dag_path);
  bool build_hierarchy();

  void tag_all();
  bool tag_selected();
  bool tag_named(const GlobPattern &glob);

  int get_num_nodes() const;
  MayaNodeDesc *get_node(int n) const;

  void clear();
  void clear_egg(EggData *egg_data, EggGroupNode *egg_root, 
                 EggGroupNode *skeleton_node, EggGroupNode *morph_node);
  EggGroup *get_egg_group(MayaNodeDesc *node_desc);
  EggTable *get_egg_table(MayaNodeDesc *node_desc);
  EggXfmSAnim *get_egg_anim(MayaNodeDesc *node_desc);
  EggSAnimData *get_egg_slider(MayaBlendDesc *blend_desc);

  bool ignore_slider(const string &name) const;
  void report_ignored_slider(const string &name);

  MayaBlendDesc *add_blend_desc(MayaBlendDesc *blend_desc);
  int get_num_blend_descs() const;
  MayaBlendDesc *get_blend_desc(int n) const;

  void reset_sliders();

public:
  PT(MayaNodeDesc) _root;
  float _fps;

private:
  MayaNodeDesc *r_build_node(const string &path);

  MayaToEggConverter *_converter;

  EggData *_egg_data;
  EggGroupNode *_egg_root;
  EggGroupNode *_skeleton_node;
  EggGroupNode *_morph_node;

  typedef pmap<string, MayaNodeDesc *> NodesByPath;
  NodesByPath _nodes_by_path;

  typedef pvector<MayaNodeDesc *> Nodes;
  Nodes _nodes;

  typedef ov_set<PT(MayaBlendDesc), IndirectCompareNames<MayaBlendDesc> > BlendDescs;
  BlendDescs _blend_descs;

  typedef pset<string> Strings;
  Strings _ignored_slider_names;
};

#endif
