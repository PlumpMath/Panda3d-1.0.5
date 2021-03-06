// Filename: clientAnalogDevice.h
// Created by:  drose (26Jan01)
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

#ifndef CLIENTANALOGDEVICE_H
#define CLIENTANALOGDEVICE_H

#include "pandabase.h"

#include "clientDevice.h"

////////////////////////////////////////////////////////////////////
//       Class : ClientAnalogDevice
// Description : A device, attached to the ClientBase by a
//               AnalogNode, that records the data from a single
//               named analog device.  The named device can contain
//               any number of analog controls, numbered in
//               sequence beginning at zero.
//
//               Each analog control returns a value ranging from -1
//               to 1, reflecting the current position of the control
//               within its total range of motion.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA ClientAnalogDevice : public ClientDevice {
protected:
  INLINE ClientAnalogDevice(ClientBase *client, const string &device_name);

public:
  INLINE int get_num_controls() const;

  INLINE void set_control_state(int index, double state);
  INLINE double get_control_state(int index) const;
  INLINE bool is_control_known(int index) const;

  virtual void write(ostream &out, int indent_level = 0) const;
  void write_controls(ostream &out, int indent_level) const;

private:
  void ensure_control_index(int index);

protected:
  class AnalogState {
  public:
    INLINE AnalogState();

    double _state;
    bool _known;
  };

  typedef pvector<AnalogState> Controls;
  Controls _controls;


public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ClientDevice::init_type();
    register_type(_type_handle, "ClientAnalogDevice",
                  ClientDevice::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "clientAnalogDevice.I"

#endif
