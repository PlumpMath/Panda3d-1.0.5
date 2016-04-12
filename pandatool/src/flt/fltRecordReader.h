// Filename: fltRecordReader.h
// Created by:  drose (24Aug00)
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

#ifndef FLTRECORDREADER_H
#define FLTRECORDREADER_H

#include "pandatoolbase.h"

#include "fltOpcode.h"
#include "fltError.h"

#include "datagram.h"
#include "datagramIterator.h"

////////////////////////////////////////////////////////////////////
//       Class : FltRecordReader
// Description : This class turns an istream into a sequence of
//               FltRecords by reading a sequence of Datagrams and
//               extracting the opcode from each one.  It remembers
//               where it is in the file and what the current record
//               is.
////////////////////////////////////////////////////////////////////
class FltRecordReader {
public:
  FltRecordReader(istream &in);
  ~FltRecordReader();

  FltOpcode get_opcode() const;
  DatagramIterator &get_iterator();
  const Datagram &get_datagram();
  int get_record_length() const;

  FltError advance(bool ok_eof = false);

  bool eof() const;
  bool error() const;

private:
  void read_next_header();

  istream &_in;
  Datagram _datagram;
  FltOpcode _opcode;
  int _record_length;
  DatagramIterator *_iterator;

  FltError _next_error;
  FltOpcode _next_opcode;
  int _next_record_length;

  enum State {
    S_begin,
    S_normal,
    S_eof,
    S_error
  };
  State _state;
};

#endif

