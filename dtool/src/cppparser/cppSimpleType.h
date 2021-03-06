// Filename: cppSimpleType.h
// Created by:  drose (19Oct99)
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

#ifndef CPPSIMPLETYPE_H
#define CPPSIMPLETYPE_H

#include "dtoolbase.h"

#include "cppType.h"

///////////////////////////////////////////////////////////////////
//       Class : CPPSimpleType
// Description :
////////////////////////////////////////////////////////////////////
class CPPSimpleType : public CPPType {
public:
  enum Type {
    T_bool,
    T_char,
    T_int,
    T_float,
    T_double,
    T_void,
    T_unknown,

    // T_parameter is a special type which is assigned to expressions
    // that are discovered where a formal parameter was expected.
    // This is a special case for handling cases like this:
    //
    //   int foo(0);
    //
    // which really means the same thing as:
    //
    //   int foo = 0;
    //
    // but it initially looks like a function prototype.
    //
    T_parameter,
  };

  enum Flags {
    F_long      = 0x001,
    F_longlong  = 0x002,
    F_short     = 0x004,
    F_unsigned  = 0x008,
    F_signed    = 0x010,
  };

  CPPSimpleType(Type type, int flags = 0);

  Type _type;
  int _flags;

  virtual bool is_tbd() const;
  virtual bool is_parameter_expr() const;

  virtual string get_preferred_name() const;

  virtual void output(ostream &out, int indent_level, CPPScope *scope,
                      bool complete) const;
  virtual SubType get_subtype() const;

  virtual CPPSimpleType *as_simple_type();

protected:
  virtual bool is_equal(const CPPDeclaration *other) const;
  virtual bool is_less(const CPPDeclaration *other) const;
};

#endif
