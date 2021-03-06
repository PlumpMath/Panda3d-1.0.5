// Filename: graphicsPipeSelection.cxx
// Created by:  drose (15Aug02)
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

#include "graphicsPipeSelection.h"
#include "mutexHolder.h"
#include "string_utils.h"
#include "filename.h"
#include "load_dso.h"
#include "config_display.h"
#include "pset.h"

GraphicsPipeSelection *GraphicsPipeSelection::_global_ptr = NULL;

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
GraphicsPipeSelection::
GraphicsPipeSelection() {
  // We declare these variables here instead of in config_display, in
  // case this constructor is running at static init time.
  ConfigVariableString load_display
    ("load-display", "*",
     PRC_DESC("Specify the name of the default graphics display library or "
              "GraphicsPipe to load.  It is the name of a shared library (or * for "
              "all libraries named in aux-display), optionally followed by the "
              "name of the particular GraphicsPipe class to create."));
  
  ConfigVariableList aux_display
    ("aux-display",
     PRC_DESC("Names each of the graphics display libraries that are available on "
              "a particular platform.  This variable may be repeated several "
              "times.  These libraries will be tried one at a time if the library "
              "specified by load_display cannot be loaded."));

  _default_display_module = load_display.get_word(0);
  _default_pipe_name = load_display.get_word(1);

  if (_default_display_module == "*") {
    // '*' or empty string is the key for all display modules.
    _default_display_module = string();

  } else if (!_default_display_module.empty()) {
    _display_modules.push_back(_default_display_module);
  }

  // Also get the set of modules named in the various aux-display
  // Config variables.  We'll want to know this when we call
  // load_modules() later.
  int num_aux = aux_display.get_num_unique_values();
  for (int i = 0; i < num_aux; i++) {
    string name = aux_display.get_unique_value(i);
    if (name != _default_display_module) {
      _display_modules.push_back(name);
    }
  }

  _default_module_loaded = false;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::Destructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
GraphicsPipeSelection::
~GraphicsPipeSelection() {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::get_num_pipe_types
//       Access: Published
//  Description: Returns the number of different types of
//               GraphicsPipes that are available to create through
//               this interface.
////////////////////////////////////////////////////////////////////
int GraphicsPipeSelection::
get_num_pipe_types() const {
  load_default_module();

  int result;
  {
    MutexHolder holder(_lock);
    result = _pipe_types.size();
  }
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::get_pipe_type
//       Access: Published
//  Description: Returns the nth type of GraphicsPipe available
//               through this interface.
////////////////////////////////////////////////////////////////////
TypeHandle GraphicsPipeSelection::
get_pipe_type(int n) const {
  load_default_module();

  TypeHandle result;
  {
    MutexHolder holder(_lock);
    if (n >= 0 && n < (int)_pipe_types.size()) {
      result = _pipe_types[n]._type;
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::print_pipe_types
//       Access: Published
//  Description: Writes a list of the currently known GraphicsPipe
//               types to nout, for the user's information.
////////////////////////////////////////////////////////////////////
void GraphicsPipeSelection::
print_pipe_types() const {
  load_default_module();

  MutexHolder holder(_lock);
  nout << "Known pipe types:" << endl;
  PipeTypes::const_iterator pi;
  for (pi = _pipe_types.begin(); pi != _pipe_types.end(); ++pi) {
    const PipeType &pipe_type = (*pi);
    nout << "  " << pipe_type._type << "\n";
  }
  if (_display_modules.empty()) {
    nout << "(all display modules loaded.)\n";
  } else {
    nout << "(" << _display_modules.size() 
         << " aux display modules not yet loaded.)\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::make_pipe
//       Access: Published
//  Description: Creates a new GraphicsPipe of the indicated type (or
//               a type more specific than the indicated type, if
//               necessary) and returns it.  Returns NULL if the type
//               cannot be matched.
////////////////////////////////////////////////////////////////////
PT(GraphicsPipe) GraphicsPipeSelection::
make_pipe(TypeHandle type) {
  load_default_module();

  MutexHolder holder(_lock);
  PipeTypes::const_iterator ti;

  // First, look for an exact match of the requested type.
  for (ti = _pipe_types.begin(); ti != _pipe_types.end(); ++ti) {
    const PipeType &ptype = (*ti);
    if (ptype._type == type) {
      // Here's an exact match.
      PT(GraphicsPipe) pipe = (*ptype._constructor)();
      if (pipe != (GraphicsPipe *)NULL) {
        return pipe;
      }
    }
  }

  // Now look for a more-specific type.
  for (ti = _pipe_types.begin(); ti != _pipe_types.end(); ++ti) {
    const PipeType &ptype = (*ti);
    if (ptype._type.is_derived_from(type)) {
      // Here's an approximate match.
      PT(GraphicsPipe) pipe = (*ptype._constructor)();
      if (pipe != (GraphicsPipe *)NULL) {
        return pipe;
      }
    }
  }

  // Couldn't find a matching pipe type.
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::make_default_pipe
//       Access: Published
//  Description: Creates a new GraphicsPipe of some arbitrary type.
//               The user may specify a preference using the Configrc
//               file; otherwise, one will be chosen arbitrarily.
////////////////////////////////////////////////////////////////////
PT(GraphicsPipe) GraphicsPipeSelection::
make_default_pipe() {
  load_default_module();

  MutexHolder holder(_lock);
  PipeTypes::const_iterator ti;

  if (!_default_pipe_name.empty()) {
    // First, look for an exact match of the default type name from
    // the Configrc file (excepting case and hyphen/underscore).
    for (ti = _pipe_types.begin(); ti != _pipe_types.end(); ++ti) {
      const PipeType &ptype = (*ti);
      if (cmp_nocase_uh(ptype._type.get_name(), _default_pipe_name) == 0) {
        // Here's an exact match.
        PT(GraphicsPipe) pipe = (*ptype._constructor)();
        if (pipe != (GraphicsPipe *)NULL) {
          return pipe;
        }
      }
    }
    
    // No match; look for a substring match.
    string preferred_name = downcase(_default_pipe_name);
    for (ti = _pipe_types.begin(); ti != _pipe_types.end(); ++ti) {
      const PipeType &ptype = (*ti);
      string ptype_name = downcase(ptype._type.get_name());
      if (ptype_name.find(preferred_name) != string::npos) {
        // Here's a substring match.
        PT(GraphicsPipe) pipe = (*ptype._constructor)();
        if (pipe != (GraphicsPipe *)NULL) {
          return pipe;
        }
      }
    }
  }
    
  // Couldn't find a matching pipe type; choose the first one on the
  // list.
  for (ti = _pipe_types.begin(); ti != _pipe_types.end(); ++ti) {
    const PipeType &ptype = (*ti);
    PT(GraphicsPipe) pipe = (*ptype._constructor)();
    if (pipe != (GraphicsPipe *)NULL) {
      return pipe;
    }
  }

  // Nothing.  Probably the list was empty.
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::load_aux_modules
//       Access: Published
//  Description: Loads all the modules named in the aux-display
//               Configrc variable, making as many graphics pipes as
//               possible available.
////////////////////////////////////////////////////////////////////
void GraphicsPipeSelection::
load_aux_modules() {
  DisplayModules::iterator di;
  for (di = _display_modules.begin(); di != _display_modules.end(); ++di) {
    load_named_module(*di);
  }

  _display_modules.clear();
  _default_module_loaded = true;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::add_pipe_type
//       Access: Public
//  Description: Adds a new kind of GraphicsPipe to the list of
//               available pipes for creation.  Normally, this is
//               called at static init type by the various shared
//               libraries as they are linked in.  Returns true on
//               success, false on failure.
////////////////////////////////////////////////////////////////////
bool GraphicsPipeSelection::
add_pipe_type(TypeHandle type, PipeConstructorFunc *func) {
  if (!type.is_derived_from(GraphicsPipe::get_class_type())) {
    display_cat.warning()
      << "Attempt to register " << type << " as a GraphicsPipe type.\n";
    return false;
  }
  
  // First, make sure we don't already have a GraphicsPipe of this
  // type.
  MutexHolder holder(_lock);
  PipeTypes::const_iterator ti;
  for (ti = _pipe_types.begin(); ti != _pipe_types.end(); ++ti) {
    const PipeType &ptype = (*ti);
    if (ptype._type == type) {
      display_cat.warning()
        << "Attempt to register GraphicsPipe type " << type
        << " more than once.\n";
      return false;
    }
  }

  // Ok, now add a new entry.
  _pipe_types.push_back(PipeType(type, func));

  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::do_load_default_module
//       Access: Private
//  Description: Loads the particular display module listed in the
//               load-display Configrc variable, which should default
//               the default pipe time.  If this string is empty or
//               "*", loads all modules named in aux-display.
////////////////////////////////////////////////////////////////////
void GraphicsPipeSelection::
do_load_default_module() {
  if (_default_display_module.empty()) {
    load_aux_modules();
    return;
  }

  load_named_module(_default_display_module);

  DisplayModules::iterator di =
    find(_display_modules.begin(), _display_modules.end(), 
         _default_display_module);
  if (di != _display_modules.end()) {
    _display_modules.erase(di);
  }

  _default_module_loaded = true;

  if (_pipe_types.empty()) {
    // If we still don't have any pipes after loading the default
    // module, automatically load the aux modules.
    load_aux_modules();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsPipeSelection::load_named_module
//       Access: Private
//  Description: Loads the indicated display module by looking for a
//               matching .dll or .so file.
////////////////////////////////////////////////////////////////////
void GraphicsPipeSelection::
load_named_module(const string &name) {
  Filename dlname = Filename::dso_filename("lib" + name + ".so");
  display_cat.info()
    << "loading display module: " << dlname.to_os_specific() << endl;
  void *tmp = load_dso(dlname);
  if (tmp == (void *)NULL) {
    display_cat.info()
      << "Unable to load: " << load_dso_error() << endl;
  }
}
