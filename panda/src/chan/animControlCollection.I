// Filename: animControlCollection.I
// Created by:  drose (22Feb00)
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


////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::set_stop_event
//       Access: Public
//  Description: Sets the event that will be thrown when the next
//               animation that is played eventually comes to a stop.
//               Setting this does not affect any stop event that is
//               pending from a previously-started animation.
////////////////////////////////////////////////////////////////////
INLINE void AnimControlCollection::
set_stop_event(const CPT_Event &stop_event) {
  _stop_event = stop_event;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::clear_stop_event
//       Access: Public
//  Description: Indicates that the next-started animation will not
//               throw a stop event when it comes to a stop.  This
//               does not affect any already-started animations.
////////////////////////////////////////////////////////////////////
INLINE void AnimControlCollection::
clear_stop_event() {
  _stop_event = (const Event *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::has_stop_event
//       Access: Public
//  Description: Returns true if a stop event has been established via
//               set_stop_event().  If true, this means that
//               animations that are to be started in the future will
//               throw this event when they stop.  However, it does
//               not necessarily mean that the currently-playing
//               animation will throw this event.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
has_stop_event() const {
  return (_stop_event != (const Event *)NULL);
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::get_stop_event
//       Access: Public
//  Description: Returns the event that has been established via
//               set_stop_event().  This is the event that will be
//               thrown by animations that are started in the future
//               when they stop.  However, this may or may not be
//               associated with any currently-playing animations.
////////////////////////////////////////////////////////////////////
INLINE CPT_Event AnimControlCollection::
get_stop_event() const {
  return _stop_event;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::play
//       Access: Public
//  Description: Starts the named animation playing.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
play(const string &anim_name) {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  _last_started_control = control;
  control->play(_stop_event);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::play
//       Access: Public
//  Description: Starts the named animation playing.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
play(const string &anim_name, int from, int to) {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  _last_started_control = control;
  control->play(from, to, _stop_event);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::loop
//       Access: Public
//  Description: Starts the named animation looping.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
loop(const string &anim_name, bool restart) {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  _last_started_control = control;
  control->loop(restart);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::loop
//       Access: Public
//  Description: Starts the named animation looping.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
loop(const string &anim_name, bool restart, int from, int to) {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  _last_started_control = control;
  control->loop(restart, from, to);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::stop
//       Access: Public
//  Description: Stops the named animation.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
stop(const string &anim_name) {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  control->stop();
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::pose
//       Access: Public
//  Description: Sets to a particular frame in the named animation.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
pose(const string &anim_name, int frame) {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  _last_started_control = control;
  control->pose(frame);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::get_frame
//       Access: Public
//  Description: Returns the current frame in the named animation, or
//               0 if the animation is not found.
////////////////////////////////////////////////////////////////////
INLINE int AnimControlCollection::
get_frame(const string &anim_name) const {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return 0;
  }
  return control->get_frame();
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::get_frame
//       Access: Public
//  Description: Returns the current frame in the last-started
//               animation.
////////////////////////////////////////////////////////////////////
INLINE int AnimControlCollection::
get_frame() const {
  if (_last_started_control == (AnimControl *)NULL) {
    return 0;
  }
  return _last_started_control->get_frame();
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::is_playing
//       Access: Public
//  Description: Returns true if the named animation is currently
//               playing, false otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
is_playing(const string &anim_name) const {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return false;
  }
  return control->is_playing();
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::is_playing
//       Access: Public
//  Description: Returns true if the last-started animation is
//               currently playing, false otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool AnimControlCollection::
is_playing() const {
  if (_last_started_control == (AnimControl *)NULL) {
    return false;
  }
  return _last_started_control->is_playing();
}

////////////////////////////////////////////////////////////////////
//     Function: AnimControlCollection::get_num_frames
//       Access: Public
//  Description: Returns the total number of frames in the named
//               animation, or 0 if the animation is not found.
////////////////////////////////////////////////////////////////////
INLINE int AnimControlCollection::
get_num_frames(const string &anim_name) const {
  AnimControl *control = find_anim(anim_name);
  if (control == (AnimControl *)NULL) {
    return 0;
  }
  return control->get_num_frames();
}

INLINE ostream &
operator << (ostream &out, const AnimControlCollection &collection) {
  collection.output(out);
  return out;
}

