// Filename: parameterRemapPTToPointer.cxx
// Created by:  drose (10Aug00)
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

#include "parameterRemapPTToPointer.h"
#include "interrogate.h"
#include "interrogateBuilder.h"
#include "typeManager.h"

#include "cppType.h"
#include "cppStructType.h"
#include "cppDeclaration.h"
#include "notify.h"

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapPTToPointer::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ParameterRemapPTToPointer::
ParameterRemapPTToPointer(CPPType *orig_type) :
  ParameterRemap(orig_type)
{
  CPPStructType *pt_type = TypeManager::unwrap(_orig_type)->as_struct_type();
  assert(pt_type != (CPPStructType *)NULL);

  // A horrible hack around a CPPParser bug.  We don't trust the
  // CPPStructType pointer we were given; instead, we ask CPPParser to
  // parse a new type of the same name.  This has a better chance of
  // fully resolving templates.
  string name = pt_type->get_local_name(&parser);
  CPPType *new_type = parser.parse_type(name);
  if (new_type == (CPPType *)NULL) {
    nout << "Type " << name << " is unknown to parser.\n";
  } else {
    new_type = new_type->resolve_type(&parser, &parser);
    pt_type = new_type->as_struct_type();
    assert(pt_type != (CPPStructType *)NULL);
  }

  _pointer_type = TypeManager::get_pointer_type(pt_type);
  if (_pointer_type == (CPPType *)NULL) {
    // If we couldn't figure out the pointer type, forget it.
    nout << "Couldn't figure out pointer type for " << *pt_type << "\n";
    _is_valid = false;
    return;
  }

  _new_type = _pointer_type;

  // We must use an actual PointerTo to hold any temporary values,
  // until we can safely ref it.
  _temporary_type = pt_type;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapPTToPointer::pass_parameter
//       Access: Public, Virtual
//  Description: Outputs an expression that converts the indicated
//               variable from the new type to the original type, for
//               passing into the actual C++ function.
////////////////////////////////////////////////////////////////////
void ParameterRemapPTToPointer::
pass_parameter(ostream &out, const string &variable_name) {
  out << variable_name;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapPTToPointer::get_return_expr
//       Access: Public, Virtual
//  Description: Returns an expression that evalutes to the
//               appropriate value type for returning from the
//               function, given an expression of the original type.
////////////////////////////////////////////////////////////////////
string ParameterRemapPTToPointer::
get_return_expr(const string &expression) {
  return expression;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapPTToPointer::temporary_to_return
//       Access: Public, Virtual
//  Description: Returns the string that converts the expression
//               stored in the indicated temporary variable to the
//               appropriate return value type.  This is normally a
//               pass-through, but in cases when the temporary
//               variable type must be different than the return type
//               (i.e. get_temporary_type() != get_new_type()), this
//               might perform some operation.
////////////////////////////////////////////////////////////////////
string ParameterRemapPTToPointer::
temporary_to_return(const string &temporary) {
  return temporary + ".p()";
}

