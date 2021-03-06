// Filename: dcClassParameter.h
// Created by:  drose (18Jun04)
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

#ifndef DCCLASSPARAMETER_H
#define DCCLASSPARAMETER_H

#include "dcbase.h"
#include "dcParameter.h"

class DCClass;

////////////////////////////////////////////////////////////////////
//       Class : DCClassParameter
// Description : This represents a class (or struct) object used as a
//               parameter itself.  This means that all the fields of
//               the class get packed into the message.
////////////////////////////////////////////////////////////////////
class EXPCL_DIRECT DCClassParameter : public DCParameter {
public:
  DCClassParameter(const DCClass *dclass);
  DCClassParameter(const DCClassParameter &copy);

PUBLISHED:
  virtual DCClassParameter *as_class_parameter();
  virtual const DCClassParameter *as_class_parameter() const;
  virtual DCParameter *make_copy() const;
  virtual bool is_valid() const;

  const DCClass *get_class() const;

public:
  virtual DCPackerInterface *get_nested_field(int n) const;

  virtual void output_instance(ostream &out, bool brief, const string &prename, 
                               const string &name, const string &postname) const;
  virtual void generate_hash(HashGenerator &hashgen) const;

protected:
  virtual bool do_check_match(const DCPackerInterface *other) const;
  virtual bool do_check_match_class_parameter(const DCClassParameter *other) const;
  virtual bool do_check_match_array_parameter(const DCArrayParameter *other) const;

private:
  typedef pvector<DCPackerInterface *> Fields;
  Fields _nested_fields;

  const DCClass *_dclass;
};

#endif
