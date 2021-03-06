// Filename: filenameUnifier.h
// Created by:  drose (05Dec00)
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

#ifndef FILENAMEUNIFIER_H
#define FILENAMEUNIFIER_H

#include "pandatoolbase.h"

#include "filename.h"

#include "pmap.h"

////////////////////////////////////////////////////////////////////
//       Class : FilenameUnifier
// Description : This static class does the job of converting
//               filenames from relative to absolute to canonical or
//               whatever is appropriate.  Its main purpose is to
//               allow us to write relative pathnames to the bam file
//               and turn them back into absolute pathnames on read,
//               so that a given bam file does not get tied to
//               absolute pathnames.
////////////////////////////////////////////////////////////////////
class FilenameUnifier {
public:
  static void set_txa_filename(const Filename &txa_filename);
  static void set_rel_dirname(const Filename &rel_dirname);

  static Filename make_bam_filename(Filename filename);
  static Filename get_bam_filename(Filename filename);
  static Filename make_egg_filename(Filename filename);
  static Filename make_user_filename(Filename filename);
  static void make_canonical(Filename &filename);

private:

  static Filename _txa_filename;
  static Filename _txa_dir;
  static Filename _rel_dirname;

  typedef pmap<string, string> CanonicalFilenames;
  static CanonicalFilenames _canonical_filenames;
};

#endif

