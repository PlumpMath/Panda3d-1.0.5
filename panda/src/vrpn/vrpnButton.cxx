// Filename: vrpnButton.cxx
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

#include "vrpnButton.h"
#include "vrpnButtonDevice.h"
#include "vrpnClient.h"
#include "config_vrpn.h"

#include "indent.h"

#include <algorithm>

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
VrpnButton::
VrpnButton(const string &button_name, vrpn_Connection *connection) :
  _button_name(button_name)
{
  _button = new vrpn_Button_Remote(_button_name.c_str(), connection);

  _button->register_change_handler((void*)this, &vrpn_button_callback);
}

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
VrpnButton::
~VrpnButton() {
  delete _button;
}

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::mark
//       Access: Public
//  Description: Adds the indicated VrpnButtonDevice to the list of
//               devices that are sharing this VrpnButton.
////////////////////////////////////////////////////////////////////
void VrpnButton::
mark(VrpnButtonDevice *device) {
  if (vrpn_cat.is_debug()) {
    vrpn_cat.debug() << *this << " marking " << *device << "\n";
  }
  _devices.push_back(device);
}

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::unmark
//       Access: Public
//  Description: Removes the indicated VrpnButtonDevice from the list
//               of devices that are sharing this VrpnButton.
////////////////////////////////////////////////////////////////////
void VrpnButton::
unmark(VrpnButtonDevice *device) {
  if (vrpn_cat.is_debug()) {
    vrpn_cat.debug() << *this << " unmarking " << *device << "\n";
  }

  Devices::iterator di =
    find(_devices.begin(), _devices.end(), device);

  if (di != _devices.end()) {
    _devices.erase(di);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::output
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void VrpnButton::
output(ostream &out) const {
  out << _button_name;
}

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::write
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void VrpnButton::
write(ostream &out, int indent_level) const {
  indent(out, indent_level)
    << get_button_name() << " ("
    << _devices.size() << " devices)\n";
}

////////////////////////////////////////////////////////////////////
//     Function: VrpnButton::vrpn_button_callback
//       Access: Private, Static
//  Description: Receives the button event data from the VRPN
//               code and sends it to any interested
//               VrpnButtonDevices.
////////////////////////////////////////////////////////////////////
void VrpnButton::
vrpn_button_callback(void *userdata, const vrpn_BUTTONCB info) {
  VrpnButton *self = (VrpnButton *)userdata;
  if (vrpn_cat.is_debug()) {
    vrpn_cat.debug()
      << *self << " got button " << info.button << " = " << info.state << "\n";
  }

  Devices::iterator di;
  for (di = self->_devices.begin(); di != self->_devices.end(); ++di) {
    VrpnButtonDevice *device = (*di);
    device->lock();
    device->set_button_state(info.button, info.state != 0);
    device->unlock();
  }
}
