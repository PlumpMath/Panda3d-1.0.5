// Filename: pgButton.h
// Created by:  drose (13Mar02)
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

#ifndef PGBUTTON_H
#define PGBUTTON_H

#include "pandabase.h"

#include "pgItem.h"
#include "nodePath.h"
#include "pset.h"

////////////////////////////////////////////////////////////////////
//       Class : PGButton
// Description : This is a particular kind of PGItem that is
//               specialized to behave like a normal button object.
//               It keeps track of its own state, and handles mouse
//               events sensibly.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA PGButton : public PGItem {
PUBLISHED:
  PGButton(const string &name);
  virtual ~PGButton();

public:
  virtual PandaNode *make_copy() const;

  virtual void enter(const MouseWatcherParameter &param);
  virtual void exit(const MouseWatcherParameter &param);
  virtual void press(const MouseWatcherParameter &param, bool background);
  virtual void release(const MouseWatcherParameter &param, bool background);

  virtual void click(const MouseWatcherParameter &param);
  
  virtual void move(const MouseWatcherParameter &param);

  PGButton(const PGButton &copy);

PUBLISHED:
  enum State {
    S_ready = 0,
    S_depressed,
    S_rollover,
    S_inactive
  };

  void setup(const string &label);
  INLINE void setup(const NodePath &ready);
  INLINE void setup(const NodePath &ready, const NodePath &depressed);
  INLINE void setup(const NodePath &ready, const NodePath &depressed, 
                    const NodePath &rollover);
  void setup(const NodePath &ready, const NodePath &depressed,
             const NodePath &rollover, const NodePath &inactive);

  virtual void set_active(bool active);

  bool add_click_button(const ButtonHandle &button);
  bool remove_click_button(const ButtonHandle &button);
  bool has_click_button(const ButtonHandle &button);

  INLINE bool is_button_down();

  INLINE static string get_click_prefix();
  INLINE string get_click_event(const ButtonHandle &button) const;

private:
  typedef pset<ButtonHandle> Buttons;
  Buttons _click_buttons;

  bool _button_down;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PGItem::init_type();
    register_type(_type_handle, "PGButton",
                  PGItem::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "pgButton.I"

#endif
