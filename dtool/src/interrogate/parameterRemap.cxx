// Filename: parameterRemap.cxx
// Created by:  drose (01Aug00)
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

#include "parameterRemap.h"


////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
ParameterRemap::
~ParameterRemap() {
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::pass_parameter
//       Access: Public, Virtual
//  Description: Outputs an expression that converts the indicated
//               variable from the original type to the new type, for
//               passing into the actual C++ function.
////////////////////////////////////////////////////////////////////
void ParameterRemap::
pass_parameter(ostream &out, const string &variable_name) {
  out << variable_name;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::prepare_return_expr
//       Access: Public, Virtual
//  Description: This will be called immediately before
//               get_return_expr().  It outputs whatever lines the
//               remapper needs to the function to set up its return
//               value, e.g. to declare a temporary variable or
//               something.  It should return the modified expression.
////////////////////////////////////////////////////////////////////
string ParameterRemap::
prepare_return_expr(ostream &, int, const string &expression) {
  return expression;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::get_return_expr
//       Access: Public, Virtual
//  Description: Returns an expression that evalutes to the
//               appropriate value type for returning from the
//               function, given an expression of the original type.
////////////////////////////////////////////////////////////////////
string ParameterRemap::
get_return_expr(const string &expression) {
  return expression;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::temporary_to_return
//       Access: Public, Virtual
//  Description: Returns the string that converts the expression
//               stored in the indicated temporary variable to the
//               appropriate return value type.  This is normally a
//               pass-through, but in cases when the temporary
//               variable type must be different than the return type
//               (i.e. get_temporary_type() != get_new_type()), this
//               might perform some operation.
////////////////////////////////////////////////////////////////////
string ParameterRemap::
temporary_to_return(const string &temporary) {
  return temporary;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::return_value_needs_management
//       Access: Public, Virtual
//  Description: Returns true if the return value represents a value
//               that was newly allocated, and hence must be
//               explicitly deallocated later by the caller.
////////////////////////////////////////////////////////////////////
bool ParameterRemap::
return_value_needs_management() {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::get_return_value_destructor
//       Access: Public, Virtual
//  Description: If return_value_needs_management() returns true, this
//               should return the index of the function that should
//               be called when it is time to destruct the return
//               value.  It will generally be the same as the
//               destructor for the class we just returned a pointer
//               to.
////////////////////////////////////////////////////////////////////
FunctionIndex ParameterRemap::
get_return_value_destructor() {
  return 0;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::return_value_should_be_simple
//       Access: Public, Virtual
//  Description: This is a hack around a problem VC++ has with
//               overly-complex expressions, particularly in
//               conjunction with the 'new' operator.  If this
//               parameter type is one that will probably give VC++ a
//               headache, this should be set true to indicate that
//               the code generator should save the return value
//               expression into a temporary variable first, and pass
//               the temporary variable name in instead.
////////////////////////////////////////////////////////////////////
bool ParameterRemap::
return_value_should_be_simple() {
  return false;
}


////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::new_type_is_atomic_string
//       Access: Public, Virtual
//  Description: Returns true if the type represented by the
//               conversion is now the atomic string type.  We have to
//               have this crazy method for representing atomic
//               string, because there's no such type in C (and hence
//               no corresponding CPPType *).
////////////////////////////////////////////////////////////////////
bool ParameterRemap::
new_type_is_atomic_string() {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemap::is_this
//       Access: Public, Virtual
//  Description: Returns true if this is the "this" parameter.
////////////////////////////////////////////////////////////////////
bool ParameterRemap::
is_this() {
  return false;
}

