// Filename: clockObject.I
// Created by:  drose (17Feb00)
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
//     Function: ClockObject::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ClockObject::
~ClockObject() {
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::set_mode
//       Access: Published
//  Description: Changes the mode of the clock, e.g. from normal
//               (real-time) to non-real-time.  In non-real-time mode,
//               tick() will add the value of dt to the value returned
//               by get_frame_time(), regardless of how much time has
//               actually passed.  In normal, real-time mode, tick()
//               will set the value returned by get_frame_time() to
//               the current real time.
////////////////////////////////////////////////////////////////////
INLINE void ClockObject::
set_mode(ClockObject::Mode mode) {
  _mode = mode;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_mode
//       Access: Published
//  Description: Returns the current mode of the clock.  See
//               set_mode().
////////////////////////////////////////////////////////////////////
INLINE ClockObject::Mode ClockObject::
get_mode() const {
  return _mode;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_frame_time
//       Access: Published
//  Description: Returns the time in seconds as of the last time
//               tick() was called (typically, this will be as of the
//               start of the current frame).
//
//               This is generally the kind of time you want to ask
//               for in most rendering and animation contexts, since
//               it's important that all of the animation for a given
//               frame remains in sync with each other.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_frame_time() const {
  return _reported_frame_time;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_real_time
//       Access: Published
//  Description: Returns the actual number of seconds elapsed since
//               the ClockObject was created, or since it was last
//               reset.  This is useful for doing real timing
//               measurements, e.g. for performance statistics.
//
//               This returns the most precise timer we have for short
//               time intervals, but it may tend to drift over the
//               long haul.  If more accurate timekeeping is needed
//               over a long period of time, use get_long_time()
//               instead.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_real_time() const {
  return (_true_clock->get_short_time() - _start_short_time);
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_long_time
//       Access: Published
//  Description: Returns the actual number of seconds elapsed since
//               the ClockObject was created, or since it was last
//               reset.
//
//               This is similar to get_real_time(), except that it
//               uses the most accurate counter we have over a long
//               period of time, and so it is less likely to drift.
//               However, it may not be very precise for measuring
//               short intervals.  On Windows, for instace, this is
//               only accurate to within about 55 milliseconds.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_long_time() const {
  return (_true_clock->get_long_time() - _start_long_time);
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::reset
//       Access: Published
//  Description: Simultaneously resets both the time and the frame
//               count to zero.
////////////////////////////////////////////////////////////////////
INLINE void ClockObject::
reset() {
  set_real_time(0.0);
  set_frame_time(0.0);
  set_frame_count(0);
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_frame_count
//       Access: Published
//  Description: Returns the number of times tick() has been called
//               since the ClockObject was created, or since it was
//               last reset.  This is generally the number of frames
//               that have been rendered.
////////////////////////////////////////////////////////////////////
INLINE int ClockObject::
get_frame_count() const {
  return _frame_count;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_net_frame_rate
//       Access: Published
//  Description: Returns the average frame rate since the last reset.
//               This is simply the total number of frames divided by
//               the total elapsed time.  This reports the virtual
//               frame rate if the clock is in (or has been in)
//               M_non_real_time mode.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_net_frame_rate() const {
  return (double)get_frame_count() / get_frame_time();
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_dt
//       Access: Published
//  Description: Returns the elapsed time for the previous frame: the
//               number of seconds elapsed between the last two calls
//               to tick().
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_dt() const {
  if (_max_dt > 0.0) {
    return min(_max_dt, _dt);
  }
  return _dt;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::set_dt
//       Access: Published
//  Description: In non-real-time mode, sets the number of seconds
//               that should appear to elapse between frames.  In
//               forced mode, sets our target dt.  In normal mode,
//               this has no effect.
////////////////////////////////////////////////////////////////////
INLINE void ClockObject::
set_dt(double dt) {
  _dt = dt;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_max_dt
//       Access: Published
//  Description: Returns the current maximum allowable time elapsed
//               between any two frames.  See set_max_dt().
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_max_dt() const {
  return _max_dt;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::set_max_dt
//       Access: Published
//  Description: Sets a limit on the value returned by get_dt().  If
//               this value is less than zero, no limit is imposed;
//               otherwise, this is the maximum value that will ever
//               be returned by get_dt(), regardless of how much time
//               has actually elapsed between frames.
//
//               This limit is only imposed in real-time mode; in
//               non-real-time mode, the dt is fixed anyway and max_dt
//               is ignored.
//
//               This is generally used to guarantee reasonable
//               behavior even in the presence of a very slow or
//               chuggy frame rame.
////////////////////////////////////////////////////////////////////
INLINE void ClockObject::
set_max_dt(double max_dt) {
  _max_dt = max_dt;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_degrade_factor
//       Access: Published
//  Description: In degrade mode, returns the ratio by which the
//               performance is degraded.  A value of 2.0 causes the
//               clock to be slowed down by a factor of two (reducing
//               performance to 1/2 what would be otherwise).
//
//               This has no effect if mode is not M_degrade.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_degrade_factor() const {
  return _degrade_factor;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::set_degrade_factor
//       Access: Published
//  Description: In degrade mode, sets the ratio by which the
//               performance is degraded.  A value of 2.0 causes the
//               clock to be slowed down by a factor of two (reducing
//               performance to 1/2 what would be otherwise).
//
//               This has no effect if mode is not M_degrade.
////////////////////////////////////////////////////////////////////
INLINE void ClockObject::
set_degrade_factor(double degrade_factor) {
  _degrade_factor = degrade_factor;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::set_average_frame_rate_interval
//       Access: Published
//  Description: Specifies the interval of time (in seconds) over
//               which get_average_frame_rate() averages the number of
//               frames per second to compute the frame rate.
//               Changing this does not necessarily immediately change
//               the result of get_average_frame_rate(), until this
//               interval of time has elapsed again.
//
//               Setting this to zero disables the computation of
//               get_average_frame_rate().
////////////////////////////////////////////////////////////////////
INLINE void ClockObject::
set_average_frame_rate_interval(double time) {
  _average_frame_rate_interval = time;
  if (_average_frame_rate_interval == 0.0) {
    _ticks.clear();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_average_frame_rate_interval
//       Access: Published
//  Description: Returns the interval of time (in seconds) over
//               which get_average_frame_rate() averages the number of frames
//               per second to compute the frame rate.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_average_frame_rate_interval() const {
  return _average_frame_rate_interval;
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_average_frame_rate
//       Access: Published
//  Description: Returns the average frame rate in number of frames
//               per second over the last
//               get_average_frame_rate_interval() seconds.  This
//               measures the virtual frame rate if the clock is in
//               M_non_real_time mode.
////////////////////////////////////////////////////////////////////
INLINE double ClockObject::
get_average_frame_rate() const {
  if (_ticks.empty()) {
    return 0.0;
  } else {
    return _ticks.size() / (_reported_frame_time - _ticks.front());
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ClockObject::get_global_clock
//       Access: Published
//  Description: Returns a pointer to the global ClockObject.  This is
//               the ClockObject that most code should use for
//               handling scene graph rendering and animation.
////////////////////////////////////////////////////////////////////
INLINE ClockObject *ClockObject::
get_global_clock() {
  if (_global_clock == (ClockObject *)NULL) {
    make_global_clock();
  }
  return _global_clock;
}

////////////////////////////////////////////////////////////////////
//     Function: TimeVal::contructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE TimeVal::
TimeVal() {
}

////////////////////////////////////////////////////////////////////
//     Function: TimeVal::get_sec
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ulong TimeVal::
get_sec() const {
  return tv[0];
}

////////////////////////////////////////////////////////////////////
//     Function: TimeVal::get_usec
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ulong TimeVal::
get_usec() const {
  return tv[1];
}
