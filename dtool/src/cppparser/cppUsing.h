// Filename: cppUsing.h
// Created by:  drose (16Nov99)
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

#ifndef CPPUSING_H
#define CPPUSING_H

#include "dtoolbase.h"

#include "cppDeclaration.h"

class CPPIdentifier;
class CPPScope;

///////////////////////////////////////////////////////////////////
//       Class : CPPUsing
// Description :
////////////////////////////////////////////////////////////////////
class CPPUsing : public CPPDeclaration {
public:
  CPPUsing(CPPIdentifier *ident, bool full_namespace, const CPPFile &file);

  virtual void output(ostream &out, int indent_level, CPPScope *scope,
                      bool complete) const;
  virtual SubType get_subtype() const;

  virtual CPPUsing *as_using();

  CPPIdentifier *_ident;
  bool _full_namespace;
};

#endif
