// Filename: pStatServer.cxx
// Created by:  drose (09Jul00)
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

#include "pStatServer.h"
#include "pStatReader.h"

#include "config_pstats.h"

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
PStatServer::
PStatServer() {
  _listener = new PStatListener(this);
  _next_udp_port = 0;
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
PStatServer::
~PStatServer() {
  delete _listener;
}


////////////////////////////////////////////////////////////////////
//     Function: PStatServer::listen
//       Access: Public
//  Description: Establishes a port number that the manager will
//               listen on for TCP connections.  This may be called
//               more than once to listen simulataneously on multiple
//               connections, as if that were at all useful.
//
//               The default parameter, -1, indicates the use of
//               whatever port number has been indicated in the Config
//               file.
//
//               This function returns true if the port was
//               successfully opened, or false if it could not open
//               the port.
////////////////////////////////////////////////////////////////////
bool PStatServer::
listen(int port) {
  if (port < 0) {
    port = pstats_port;
  }

  // Now try to listen to the port.
  PT(Connection) rendezvous = open_TCP_server_rendezvous(port, 5);

  if (rendezvous.is_null()) {
    // Couldn't get it.
    return false;
  }

  // Tell the listener about the new port.
  _listener->add_connection(rendezvous);

  if (_next_udp_port == 0) {
    _next_udp_port = port + 1;
  }
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: PStatServer::poll
//       Access: Public
//  Description: Checks for any network activity and handles it, if
//               appropriate, and then returns.  This must be called
//               periodically unless is_thread_safe() is redefined to
//               return true on this class and also on all
//               PStatMonitors in use.
//
//               Alternatively, a program may call main_loop() and
//               yield control of the program entirely to the
//               PStatServer.
////////////////////////////////////////////////////////////////////
void PStatServer::
poll() {
  // Delete all the readers that we couldn't delete before.
  while (!_lost_readers.empty()) {
    PStatReader *reader = _lost_readers.back();
    _lost_readers.pop_back();

    reader->lost_connection();
    delete reader;
  }
  while (!_removed_readers.empty()) {
    PStatReader *reader = _removed_readers.back();
    _removed_readers.pop_back();
    delete reader;
  }

  _listener->poll();

  Readers::const_iterator ri = _readers.begin();
  while (ri != _readers.end()) {
    // Preincrement the iterator, in case we remove it as a result of
    // calling poll().
    Readers::const_iterator rnext = ri;
    ++rnext;
    PStatReader *reader = (*ri).second;

    reader->poll();
    reader->idle();
    
    ri = rnext;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::main_loop
//       Access: Public
//  Description: An alternative to repeatedly calling poll(), this
//               function yields control of the program to the
//               PStatServer.  It does not return until the program
//               is done.
//
//               If interrupt_flag is non-NULL, it is the address of a
//               bool variable that is initially false, and may be
//               asynchronously set true to indicate the loop should
//               terminate.
////////////////////////////////////////////////////////////////////
void PStatServer::
main_loop(bool *interrupt_flag) {
  while (interrupt_flag == (bool *)NULL || !*interrupt_flag) {
    poll();
    // Not great.  This will totally blow in a threaded environment.
    // We need a portable way to sleep or block.
    PRIntervalTime sleep_timeout = PR_MillisecondsToInterval(100);
    PR_Sleep(sleep_timeout);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::add_reader
//       Access: Public
//  Description: Adds the newly-created PStatReader to the list of
//               currently active readers.
////////////////////////////////////////////////////////////////////
void PStatServer::
add_reader(Connection *connection, PStatReader *reader) {
  _readers[connection] = reader;
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::remove_reader
//       Access: Public
//  Description: Removes the indicated reader.
////////////////////////////////////////////////////////////////////
void PStatServer::
remove_reader(Connection *connection, PStatReader *reader) {
  Readers::iterator ri;
  ri = _readers.find(connection);
  if (ri == _readers.end() || (*ri).second != reader) {
    nout << "Attempt to remove undefined reader.\n";
  } else {
    _readers.erase(ri);
    _removed_readers.push_back(reader);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::get_udp_port
//       Access: Public
//  Description: Returns a new port number that will probably be free
//               to use as a UDP port.  The caller should be prepared
//               to accept the possibility that it will be already in
//               use by another process, however.
////////////////////////////////////////////////////////////////////
int PStatServer::
get_udp_port() {
  if (_available_udp_ports.empty()) {
    return _next_udp_port++;
  }
  int udp_port = _available_udp_ports.front();
  _available_udp_ports.pop_front();
  return udp_port;
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::release_udp_port
//       Access: Public
//  Description: Indicates that the given UDP port is once again free
//               for use.
////////////////////////////////////////////////////////////////////
void PStatServer::
release_udp_port(int port) {
  _available_udp_ports.push_back(port);
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::get_num_user_guide_bars
//       Access: Public
//  Description: Returns the current number of user-defined guide
//               bars.
////////////////////////////////////////////////////////////////////
int PStatServer::
get_num_user_guide_bars() const {
  return _user_guide_bars.size();
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::get_user_guide_bar_height
//       Access: Public
//  Description: Returns the height of the nth user-defined guide bar.
////////////////////////////////////////////////////////////////////
float PStatServer::
get_user_guide_bar_height(int n) const {
  nassertr(n >= 0 && n < (int)_user_guide_bars.size(), 0.0f);
  return _user_guide_bars[n];
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::move_user_guide_bar
//       Access: Public
//  Description: Adjusts the height of the nth user-defined guide bar.
////////////////////////////////////////////////////////////////////
void PStatServer::
move_user_guide_bar(int n, float height) {
  nassertv(n >= 0 && n < (int)_user_guide_bars.size());
  _user_guide_bars[n] = height;
  user_guide_bars_changed();
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::add_user_guide_bar
//       Access: Public
//  Description: Creates a new user guide bar and returns its index
//               number.
////////////////////////////////////////////////////////////////////
int PStatServer::
add_user_guide_bar(float height) {
  int n = (int)_user_guide_bars.size();
  _user_guide_bars.push_back(height);
  user_guide_bars_changed();

  return n;
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::remove_user_guide_bar
//       Access: Public
//  Description: Removes the user guide bar with the indicated index
//               number.  All subsequent index numbers are adjusted
//               down one.
////////////////////////////////////////////////////////////////////
void PStatServer::
remove_user_guide_bar(int n) {
  nassertv(n >= 0 && n < (int)_user_guide_bars.size());
  _user_guide_bars.erase(_user_guide_bars.begin() + n);
  user_guide_bars_changed();
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::find_user_guide_bar
//       Access: Public
//  Description: Returns the index number of the first user guide bar
//               found whose height is within the indicated range, or
//               -1 if no user guide bars fall within the range.
////////////////////////////////////////////////////////////////////
int PStatServer::
find_user_guide_bar(float from_height, float to_height) const {
  GuideBars::const_iterator gbi;
  for (gbi = _user_guide_bars.begin();
       gbi != _user_guide_bars.end();
       ++gbi) {
    float height = (*gbi);
    if (height >= from_height && height <= to_height) {
      return (int)(gbi - _user_guide_bars.begin());
    }
  }

  return -1;
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::user_guide_bars_changed
//       Access: Priate
//  Description: Called when the user guide bars have been changed.
////////////////////////////////////////////////////////////////////
void PStatServer::
user_guide_bars_changed() {
  Readers::iterator ri;
  for (ri = _readers.begin(); ri != _readers.end(); ++ri) {
    (*ri).second->get_monitor()->user_guide_bars_changed();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::is_thread_safe
//       Access: Public
//  Description: This should be redefined to return true in derived
//               classes that want to deal with multithreaded readers
//               and such.  If this returns true, the manager will
//               create the listener in its own thread, and thus the
//               PStatReader constructors at least will run in a
//               different thread.
//
//               This is not related to the question of whether the
//               reader can handle multiple different
//               PStatThreadDatas; it's strictly a question of whether
//               the readers themselves can run in a separate thread.
////////////////////////////////////////////////////////////////////
bool PStatServer::
is_thread_safe() {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: PStatServer::connection_reset
//       Access: Protected, Virtual
//  Description: Called when a lost connection is detected by the net
//               code, this should pass the word on to the interested
//               parties and clean up gracefully.
////////////////////////////////////////////////////////////////////
void PStatServer::
connection_reset(const PT(Connection) &connection, PRErrorCode errcode) {
  // Was this a client connection?  Tell the reader about it if it
  // was.
  close_connection(connection);

  Readers::iterator ri;
  ri = _readers.find(connection);
  if (ri != _readers.end()) {
    PStatReader *reader = (*ri).second;
    _readers.erase(ri);

    // Unfortunately, we can't delete the reader right away, because
    // we might have been called from a method on the reader!  We'll
    // have to save the reader pointer and delete it some time later.
    _lost_readers.push_back(reader);
  }
}
