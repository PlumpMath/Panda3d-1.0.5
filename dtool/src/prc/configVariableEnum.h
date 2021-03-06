// Filename: configVariableEnum.h
// Created by:  drose (21Oct04)
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

#ifndef CONFIGVARIABLEENUM_H
#define CONFIGVARIABLEENUM_H

#include "dtoolbase.h"
#include "configVariable.h"

////////////////////////////////////////////////////////////////////
//       Class : ConfigVariableEnum
// Description : This class specializes ConfigVariable as an
//               enumerated type.  It is a template class, so it
//               cannot be easily published; it's not really necessary
//               outside of C++ anyway.
//
//               This variable assumes that the enumerated type in
//               question has input and output stream operators
//               defined that do the right thing (outputting a
//               sensible string for the type, and converting a string
//               to the correct value).
////////////////////////////////////////////////////////////////////
template<class EnumType>
class ConfigVariableEnum : public ConfigVariable {
public:
  INLINE ConfigVariableEnum(const string &name, EnumType default_value, 
                            const string &description = string(), 
                            int flags = 0);
  INLINE ConfigVariableEnum(const string &name, const string &default_value, 
                            const string &description = string(), 
                            int flags = 0);
  INLINE ~ConfigVariableEnum();

  INLINE void operator = (EnumType value);
  INLINE operator EnumType () const;

  INLINE int size() const;
  INLINE EnumType operator [] (int n) const;

  INLINE void set_value(EnumType value);
  INLINE EnumType get_value() const;
  INLINE EnumType get_default_value() const;

  INLINE EnumType get_word(int n) const;
  INLINE void set_word(int n, EnumType value);

private:
  INLINE EnumType parse_string(const string &value) const;
  INLINE string format_enum(EnumType value) const;

  int _value_seq;
  EnumType _value;

  bool _got_default_value;
  EnumType _default_value;
};

#include "configVariableEnum.I"

#endif

