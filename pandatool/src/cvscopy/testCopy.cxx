// Filename: testCopy.cxx
// Created by:  drose (31Oct00)
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

#include "testCopy.h"
#include "cvsSourceDirectory.h"

////////////////////////////////////////////////////////////////////
//     Function: TestCopy::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
TestCopy::
TestCopy() {
  set_program_description
    ("This program copies one or more files into a CVS source hierarchy.  "
     "Rather than copying the named files immediately into the current "
     "directory, it first scans the entire source hierarchy, identifying all "
     "the already-existing files.  If the named file to copy matches the "
     "name of an already-existing file in the current directory or elsewhere "
     "in the hierarchy, that file is overwritten.\n\n"

     "This is primarily useful as a test program for libcvscopy.");
}

////////////////////////////////////////////////////////////////////
//     Function: TestCopy::run
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void TestCopy::
run() {
  SourceFiles::iterator fi;
  for (fi = _source_files.begin(); fi != _source_files.end(); ++fi) {
    CVSSourceDirectory *dest = import(*fi, 0, _model_dir);
    if (dest == (CVSSourceDirectory *)NULL) {
      exit(1);
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TestCopy::copy_file
//       Access: Protected, Virtual
//  Description: Called by import() if the timestamps indicate that a
//               file needs to be copied.  This does the actual copy
//               of a file from source to destination.  If new_file is
//               true, then dest does not already exist.
////////////////////////////////////////////////////////////////////
bool TestCopy::
copy_file(const Filename &source, const Filename &dest,
          CVSSourceDirectory *, void *, bool) {
  return copy_binary_file(source, dest);
}


int main(int argc, char *argv[]) {
  TestCopy prog;
  prog.parse_command_line(argc, argv);
  prog.run();
  return 0;
}
