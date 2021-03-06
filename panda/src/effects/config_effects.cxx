// Filename: config_effects.cxx
// Created by:  jason (18Jul00)
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

#include "config_effects.h"
#include "cgShader.h"
#include "cgShaderAttrib.h"
#include "cgShaderContext.h"
#include "lensFlareNode.h"
#include "pandaSystem.h"


#include "dconfig.h"

Configure(config_effects);
NotifyCategoryDef(effects, "");

ConfigureFn(config_effects) {
  init_libeffects();
}

////////////////////////////////////////////////////////////////////
//     Function: init_libeffects
//  Description: Initializes the library.  This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libeffects() {
#ifdef HAVE_CG
  CgShader::init_type();
  CgShaderAttrib::init_type();
  CgShaderContext::init_type();

  PandaSystem *ps = PandaSystem::get_global_ptr();
  ps->add_system("Cg");
#endif
#if 0  // temporarily disabled until we can port to new scene graph.
  LensFlareNode::init_type();
  LensFlareNode::register_with_read_factory();
#endif  // temporarily disabled until we can port to new scene graph.
}
