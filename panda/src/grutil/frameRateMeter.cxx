// Filename: frameRateMeter.cxx
// Created by:  drose (23Dec03)
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

#include "frameRateMeter.h"
#include "camera.h"
#include "displayRegion.h"
#include "orthographicLens.h"
#include "clockObject.h"
#include "config_grutil.h"
#include "depthTestAttrib.h"
#include "depthWriteAttrib.h"
#include "pStatTimer.h"
#include <stdio.h>  // For sprintf/snprintf

PStatCollector FrameRateMeter::_show_fps_pcollector("Cull:Show fps");

TypeHandle FrameRateMeter::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: FrameRateMeter::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
FrameRateMeter::
FrameRateMeter(const string &name) : TextNode(name) {
  _update_interval = frame_rate_meter_update_interval;
  _text_pattern = frame_rate_meter_text_pattern;
  _clock_object = ClockObject::get_global_clock();

  set_align(A_right);
  set_transform(LMatrix4f::scale_mat(frame_rate_meter_scale) * 
                LMatrix4f::translate_mat(LVector3f::rfu(1.0f - frame_rate_meter_side_margins * frame_rate_meter_scale, 0.0f, 1.0f - frame_rate_meter_scale)));
  set_card_color(0.0f, 0.0f, 0.0f, 0.4f);
  set_card_as_margin(frame_rate_meter_side_margins, frame_rate_meter_side_margins, 0.1f, 0.0f);

  do_update();
}

////////////////////////////////////////////////////////////////////
//     Function: FrameRateMeter::Destructor
//       Access: Published, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
FrameRateMeter::
~FrameRateMeter() {
  clear_window();
}

////////////////////////////////////////////////////////////////////
//     Function: FrameRateMeter::setup_window
//       Access: Published
//  Description: Sets up the frame rate meter to create a
//               DisplayRegion to render itself into the indicated
//               window.
////////////////////////////////////////////////////////////////////
void FrameRateMeter::
setup_window(GraphicsOutput *window) {
  clear_window();

  _window = window;

  _root = NodePath("frame_rate_root");
  _root.attach_new_node(this);

  CPT(RenderAttrib) dt = DepthTestAttrib::make(DepthTestAttrib::M_none);
  CPT(RenderAttrib) dw = DepthWriteAttrib::make(DepthWriteAttrib::M_off);
  _root.node()->set_attrib(dt, 1);
  _root.node()->set_attrib(dw, 1);
  _root.set_material_off(1);
  _root.set_two_sided(1, 1);
    
  // Create a display region that covers the entire window.
  _display_region = _window->make_display_region();
  _display_region->set_sort(frame_rate_meter_layer_sort);
    
  // Finally, we need a camera to associate with the display region.
  PT(Camera) camera = new Camera("frame_rate_camera");
  NodePath camera_np = _root.attach_new_node(camera);
    
  PT(Lens) lens = new OrthographicLens;
  
  static const float left = -1.0f;
  static const float right = 1.0f;
  static const float bottom = -1.0f;
  static const float top = 1.0f;
  lens->set_film_size(right - left, top - bottom);
  lens->set_film_offset((right + left) * 0.5, (top + bottom) * 0.5);
  lens->set_near_far(-1000, 1000);
  
  camera->set_lens(lens);
  camera->set_scene(_root);
  _display_region->set_camera(camera_np);
}

////////////////////////////////////////////////////////////////////
//     Function: FrameRateMeter::clear_window
//       Access: Published
//  Description: Undoes the effect of a previous call to
//               setup_window().
////////////////////////////////////////////////////////////////////
void FrameRateMeter::
clear_window() {
  if (_window != (GraphicsOutput *)NULL) {
    _window->remove_display_region(_display_region);
    _window = (GraphicsOutput *)NULL;
    _display_region = (DisplayRegion *)NULL;
  }
  _root = NodePath();
}

////////////////////////////////////////////////////////////////////
//     Function: FrameRateMeter::cull_callback
//       Access: Protected, Virtual
//  Description: If has_cull_callback() returns true, this function
//               will be called during the cull traversal to perform
//               any additional operations that should be performed at
//               cull time.  This may include additional manipulation
//               of render state or additional visible/invisible
//               decisions, or any other arbitrary operation.
//
//               By the time this function is called, the node has
//               already passed the bounding-volume test for the
//               viewing frustum, and the node's transform and state
//               have already been applied to the indicated
//               CullTraverserData object.
//
//               The return value is true if this node should be
//               visible, or false if it should be culled.
////////////////////////////////////////////////////////////////////
bool FrameRateMeter::
cull_callback(CullTraverser *trav, CullTraverserData &data) {
  // Statistics
  PStatTimer timer(_show_fps_pcollector);
  
  // Check to see if it's time to update.
  double now = _clock_object->get_frame_time();
  double elapsed = now - _last_update;
  if (elapsed < 0.0 || elapsed >= _update_interval) {
    do_update();
  }

  return TextNode::cull_callback(trav, data);
}

////////////////////////////////////////////////////////////////////
//     Function: FrameRateMeter::do_update
//       Access: Private
//  Description: Resets the text according to the current frame rate.
////////////////////////////////////////////////////////////////////
void FrameRateMeter::
do_update() {
  _last_update = _clock_object->get_frame_time();

  double frame_rate = _clock_object->get_average_frame_rate();

  static const size_t buffer_size = 1024;
  char buffer[buffer_size];
#ifdef WIN32_VC
  // Windows doesn't define snprintf().  Hope we don't overflow.
  sprintf(buffer, _text_pattern.c_str(), frame_rate);
#else
  snprintf(buffer, buffer_size, _text_pattern.c_str(), frame_rate);
#endif
  nassertv(strlen(buffer) < buffer_size);

  if (get_text() == buffer) {
    // Never mind; the frame rate hasn't changed.
    return;
  }

  set_text(buffer);
}
