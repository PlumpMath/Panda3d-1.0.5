// Filename: pStatThreadData.h
// Created by:  drose (08Jul00)
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

#ifndef PSTATTHREADDATA_H
#define PSTATTHREADDATA_H

#include "pandatoolbase.h"

#include "referenceCount.h"

#include "pdeque.h"

class PStatCollectorDef;
class PStatFrameData;
class PStatClientData;

////////////////////////////////////////////////////////////////////
//       Class : PStatThreadData
// Description : A collection of FrameData structures for
//               recently-received frames within a particular thread.
//               This holds the raw data as reported by the client,
//               and it automatically handles frames received
//               out-of-order or skipped.  You can ask for a
//               particular frame by frame number or time and receive
//               the data for the nearest frame.
////////////////////////////////////////////////////////////////////
class PStatThreadData : public ReferenceCount {
public:
  PStatThreadData(const PStatClientData *client_data);
  ~PStatThreadData();

  INLINE const PStatClientData *get_client_data() const;

  bool is_empty() const;

  int get_latest_frame_number() const;
  int get_oldest_frame_number() const;
  bool has_frame(int frame_number) const;
  const PStatFrameData &get_frame(int frame_number) const;

  float get_latest_time() const;
  float get_oldest_time() const;
  const PStatFrameData &get_frame_at_time(float time) const;
  int get_frame_number_at_time(float time, int hint = -1) const;

  const PStatFrameData &get_latest_frame() const;

  bool get_elapsed_frames(int &then_i, int &now_i) const;
  float get_frame_rate() const;


  void set_history(float time);
  float get_history() const;

  void record_new_frame(int frame_number, PStatFrameData *frame_data);

private:
  void compute_elapsed_frames();
  const PStatClientData *_client_data;

  typedef pdeque<PStatFrameData *> Frames;
  Frames _frames;
  int _first_frame_number;
  float _history;

  bool _computed_elapsed_frames;
  bool _got_elapsed_frames;
  int _then_i;
  int _now_i;

  static PStatFrameData _null_frame;
};

#include "pStatThreadData.I"

#endif

