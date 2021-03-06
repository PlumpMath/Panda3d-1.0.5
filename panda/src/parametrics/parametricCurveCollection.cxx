// Filename: parametricCurveCollection.cxx
// Created by:  drose (04Mar01)
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

#include "parametricCurveCollection.h"
#include "config_parametrics.h"
#include "curveFitter.h"
#include "parametricCurveDrawer.h"
#include "nurbsCurve.h"

#include "indent.h"
#include "compose_matrix.h"
#include "string_utils.h"
#include "look_at.h"

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
ParametricCurveCollection::
ParametricCurveCollection() {
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::add_curve
//       Access: Published
//  Description: Adds a new ParametricCurve to the collection.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
add_curve(ParametricCurve *curve) {
  prepare_add_curve(curve);
  _curves.push_back(curve);
  redraw();
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::add_curve
//       Access: Published
//  Description: Adds a new ParametricCurve to the collection at the
//               indicated index.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
add_curve(ParametricCurve *curve, int index) {
  prepare_add_curve(curve);
  index = max(min(index, (int)_curves.size()), 0);
  _curves.insert(_curves.begin() + index, curve);
  redraw();
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::add_curves
//       Access: Published
//  Description: Adds all the curves found in the scene graph rooted
//               at the given node.  Returns the number of curves
//               found.
////////////////////////////////////////////////////////////////////
int ParametricCurveCollection::
add_curves(PandaNode *node) {
  int num_curves = r_add_curves(node);

  if (num_curves > 0) {
    redraw();
  }

  return num_curves;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::remove_curve
//       Access: Published
//  Description: Removes the indicated ParametricCurve from the
//               collection.  Returns true if the curve was removed,
//               false if it was not a member of the collection.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
remove_curve(ParametricCurve *curve) {
  int curve_index = -1;
  for (int i = 0; curve_index == -1 && i < (int)_curves.size(); i++) {
    if (_curves[i] == curve) {
      curve_index = i;
    }
  }

  if (curve_index == -1) {
    // The indicated curve was not a member of the collection.
    return false;
  }

  remove_curve(curve_index);

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::remove_curve
//       Access: Published
//  Description: Removes the indicated ParametricCurve from the
//               collection, by its index number.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
remove_curve(int index) {
  nassertv(index >= 0 && index < (int)_curves.size());
  PT(ParametricCurve) curve = _curves[index];
  prepare_remove_curve(curve);
  _curves.erase(_curves.begin() + index);
  redraw();
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::has_curve
//       Access: Published
//  Description: Returns true if the indicated ParametricCurve appears in
//               this collection, false otherwise.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
has_curve(ParametricCurve *curve) const {
  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    if (curve == (*ci)) {
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::clear
//       Access: Published
//  Description: Removes all ParametricCurves from the collection.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
clear() {
  ParametricCurves::iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    prepare_remove_curve(curve);
  }
  _curves.clear();

  redraw();
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::clear_timewarps
//       Access: Published
//  Description: Removes all the timewarp curves from the collection.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
clear_timewarps() {
  PT(ParametricCurve) xyz_curve = (ParametricCurve *)NULL;
  PT(ParametricCurve) hpr_curve = (ParametricCurve *)NULL;

  ParametricCurves::iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);

    switch (curve->get_curve_type()) {
    case PCT_XYZ:
      if (xyz_curve == (ParametricCurve *)NULL) {
        xyz_curve = curve;
      } else {
        prepare_remove_curve(curve);
      }
      break;

    case PCT_HPR:
      if (hpr_curve == (ParametricCurve *)NULL) {
        hpr_curve = curve;
      } else {
        prepare_remove_curve(curve);
      }
      break;

    default:
      prepare_remove_curve(curve);
    }
  }

  _curves.clear();
  _curves.push_back(xyz_curve);

  if (hpr_curve != (ParametricCurve *)NULL) {
    _curves.push_back(hpr_curve);
  }

  redraw();
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::get_xyz_curve
//       Access: Published
//  Description: Returns the first XYZ curve in the collection, if
//               any, or NULL if there are none.
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveCollection::
get_xyz_curve() const {
  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (curve->get_curve_type() == PCT_XYZ) {
      return curve;
    }
  }
  return (ParametricCurve *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::get_hpr_curve
//       Access: Published
//  Description: Returns the first HPR curve in the collection, if
//               any, or NULL if there are none.
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveCollection::
get_hpr_curve() const {
  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (curve->get_curve_type() == PCT_HPR) {
      return curve;
    }
  }
  return (ParametricCurve *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::get_default_curve
//       Access: Published
//  Description: If there is an XYZ curve in the collection, returns
//               it; otherwise, returns the first curve whose type is
//               unspecified.  Returns NULL if no curve meets the
//               criteria.
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveCollection::
get_default_curve() const {
  ParametricCurve *xyz_curve = get_xyz_curve();
  if (xyz_curve != (ParametricCurve *)NULL) {
    return xyz_curve;
  }

  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (curve->get_curve_type() == PCT_NONE) {
      return curve;
    }
  }
  return (ParametricCurve *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::get_num_timewarps
//       Access: Published
//  Description: Returns the number of timewarp curves in the
//               collection.
////////////////////////////////////////////////////////////////////
int ParametricCurveCollection::
get_num_timewarps() const {
  int count = 0;

  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (curve->get_curve_type() == PCT_T) {
      count++;
    }
  }

  return count;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::get_timewarp_curve
//       Access: Published
//  Description: Returns the nth timewarp curve in the collection.
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveCollection::
get_timewarp_curve(int n) const {
  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (curve->get_curve_type() == PCT_T) {
      if (n == 0) {
        return curve;
      }
      n--;
    }
  }
  nassertr(false, (ParametricCurve *)NULL);
  return (ParametricCurve *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::make_even
//       Access: Published
//  Description: Discards all existing timewarp curves and recomputes
//               a new timewarp curve that maps distance along the
//               curve to parametric time, so that the distance
//               between any two points in parametric time is
//               proportional to the approximate distance of those
//               same two points along the XYZ curve.
//
//               segments_per_unit represents the number of segments to
//               take per each unit of parametric time of the original
//               XYZ curve.
//
//               The new timewarp curve (and thus, the apparent range
//               of the collection) will range from 0 to max_t.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
make_even(float max_t, float segments_per_unit) {
  ParametricCurve *xyz_curve = get_xyz_curve();
  if (xyz_curve == (ParametricCurve *)NULL) {
    parametrics_cat.error()
      << "No XYZ curve for make_even().\n";
    return;
  }

  clear_timewarps();

  // Now divvy up the XYZ curve into num_segments sections, each
  // approximately the same length as all the others.
  CurveFitter fitter;

  int num_segments = max(1, (int)cfloor(segments_per_unit * xyz_curve->get_max_t() + 0.5f));

  if (parametrics_cat.is_debug()) {
    parametrics_cat.debug()
      << "Calculating length of curve.\n";
  }

  float net_length = xyz_curve->calc_length();
  float segment_length = net_length / (float)num_segments;

  if (parametrics_cat.is_debug()) {
    parametrics_cat.debug()
      << "Curve has total length " << net_length << "; dividing into "
      << num_segments << " segments of " << segment_length << " units each.\n";
  }

  float last_t = 0.0f;
  fitter.add_xyz(0.0f, LVecBase3f(last_t, 0.0f, 0.0f));
  float val_inc= max_t/num_segments;
  float val=val_inc;

  for (int i = 0; i < num_segments; i++,val+=val_inc) {
    float next_t = xyz_curve->find_length(last_t, segment_length);
    fitter.add_xyz(/*(float)(i + 1)/num_segments * max_t,*/
                   val, LVecBase3f(next_t, 0.0f, 0.0f));

    if (parametrics_cat.is_spam()) {
      parametrics_cat.spam()
        << "Point " << i << " is at " << next_t << "\n";
    }

    last_t = next_t;
  }

  if (parametrics_cat.is_debug()) {
    parametrics_cat.debug()
      << "Done computing segments.\n";
  }

  fitter.compute_tangents(1);
  PT(ParametricCurveCollection) fit = fitter.make_nurbs();
  ParametricCurve *t_curve = fit->get_xyz_curve();
  nassertv(t_curve != (ParametricCurve *)NULL);
  t_curve->set_curve_type(PCT_T);
  add_curve(t_curve);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::face_forward
//       Access: Published
//  Description: Discards the existing HPR curve and generates a new
//               one that looks in the direction of travel along the
//               XYZ curve, based on the XYZ curve's tangent at each
//               point.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
face_forward(float segments_per_unit) {
  ParametricCurve *xyz_curve = get_xyz_curve();
  if (xyz_curve == (ParametricCurve *)NULL) {
    parametrics_cat.error()
      << "No XYZ curve for face_forward().\n";
    return;
  }

  // Eliminate all the old hpr curves, and also take note of the index
  // number of the first XYZ curve.
  int xyz_index = -1;
  ParametricCurves::const_iterator ci;
  ParametricCurves new_curves;

  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (curve->get_curve_type() == PCT_HPR) {
      prepare_remove_curve(curve);
    } else {
      if (curve->get_curve_type() == PCT_XYZ && xyz_index == -1) {
        xyz_index = (ci - _curves.begin());
      }
      new_curves.push_back(curve);
    }
  }
  _curves.swap(new_curves);

  // Now divvy up the XYZ curve into num_segments sections, of equal
  // length in parametric time (based on the timewarp curves).
  CurveFitter fitter;

  float max_t = get_max_t();
  int num_segments = (int)cfloor(segments_per_unit * max_t + 0.5);

  LVecBase3f hpr(0.0f, 0.0f, 0.0f);

  // We compute the first HPR point a little point into the beginning
  // of the curve, instead of at 0.0f, because the tangent at 0.0f is
  // likely to be zero.
  determine_hpr(0.001, xyz_curve, hpr);
  fitter.add_hpr(0.0f, hpr);

  for (int i = 0; i < num_segments; i++) {
    float t = (float)(i + 1) / num_segments * max_t;
    determine_hpr(t, xyz_curve, hpr);
    fitter.add_hpr(t, hpr);
  }

  fitter.wrap_hpr();
  fitter.compute_tangents(1);
  PT(ParametricCurveCollection) fit = fitter.make_nurbs();
  ParametricCurve *hpr_curve = fit->get_hpr_curve();
  nassertv(hpr_curve != (ParametricCurve *)NULL);
  add_curve(hpr_curve, xyz_index + 1);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::reset_max_t
//       Access: Published
//  Description: Adjusts the apparent length of the curve by applying
//               a new timewarp that maps the range [0..max_t] to the
//               range [0..get_max_t()].  After this call, the curve
//               collection will contain one more timewarp curve, and
//               get_max_t() will return the given max_t value.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
reset_max_t(float max_t) {
  // Define a linear NURBS curve.
  PT(NurbsCurve) nurbs = new NurbsCurve;
  nurbs->set_curve_type(PCT_T);
  nurbs->set_order(2);
  nurbs->append_cv(LVecBase3f(0.0f, 0.0f, 0.0f));
  nurbs->append_cv(LVecBase3f(get_max_t(), 0.0f, 0.0f));
  nurbs->set_knot(0, 0.0f);
  nurbs->set_knot(1, 0.0f);
  nurbs->set_knot(2, max_t);
  nurbs->set_knot(3, max_t);
  nurbs->recompute();
  add_curve(nurbs);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::evaluate
//       Access: Published
//  Description: Computes the position and rotation represented by the
//               first XYZ and HPR curves in the collection at the
//               given point t, after t has been modified by all the
//               timewarp curves in the collection applied in
//               sequence, from back to front.
//
//               Returns true if the point is valid (i.e. t is within
//               the bounds indicated by all the timewarp curves and
//               within the bounds of the curves themselves), or false
//               otherwise.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
evaluate(float t, LVecBase3f &xyz, LVecBase3f &hpr) const {
  // First, apply all the timewarps in sequence, from back to front.
  // Also take note of the XYZ and HPR curves.
  ParametricCurve *xyz_curve = (ParametricCurve *)NULL;
  ParametricCurve *hpr_curve = (ParametricCurve *)NULL;
  ParametricCurve *default_curve = (ParametricCurve *)NULL;

  float t0 = t;
  LVecBase3f point;

  ParametricCurves::const_reverse_iterator ci;
  for (ci = _curves.rbegin(); ci != _curves.rend(); ++ci) {
    ParametricCurve *curve = (*ci);

    switch (curve->get_curve_type()) {
    case PCT_XYZ:
      xyz_curve = curve;
      break;

    case PCT_HPR:
      hpr_curve = curve;
      break;

    case PCT_NONE:
      default_curve = curve;
      break;

    case PCT_T:
      if (!curve->get_point(t0, point)) {
        return false;
      }
      t0 = point[0];
    }
  }

  if (xyz_curve == (ParametricCurve *)NULL) {
    xyz_curve = default_curve;
  }

  // Now compute the position and orientation.
  if (xyz_curve != (ParametricCurve *)NULL) {
    if (!xyz_curve->get_point(t0, xyz)) {
      return false;
    }
  }

  if (hpr_curve != (ParametricCurve *)NULL) {
    if (!hpr_curve->get_point(t0, hpr)) {
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::evaluate
//       Access: Published
//  Description: Computes the transform matrix representing
//               translation to the position indicated by the first
//               XYZ curve in the collection and the rotation
//               indicated by the first HPR curve in the collection,
//               after t has been modified by all the timewarp curves
//               in the collection applied in sequence, from back to
//               front.
//
//               Returns true if the point is valid (i.e. t is within
//               the bounds indicated by all the timewarp curves and
//               within the bounds of the curves themselves), or false
//               otherwise.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
evaluate(float t, LMatrix4f &result, CoordinateSystem cs) const {
  LVecBase3f xyz(0.0f, 0.0f, 0.0f);
  LVecBase3f hpr(0.0f, 0.0f, 0.0f);

  if (!evaluate(t, xyz, hpr)) {
    return false;
  }

  compose_matrix(result, 
                 LVecBase3f(1.0f, 1.0f, 1.0f), 
                 LVecBase3f(0.0f, 0.0f, 0.0f),
                 hpr, xyz, cs);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::evaluate_t
//       Access: Published
//  Description: Determines the value of t that should be passed to
//               the XYZ and HPR curves, after applying the given
//               value of t to all the timewarps.  Return -1.0f if the
//               value of t exceeds one of the timewarps' ranges.
////////////////////////////////////////////////////////////////////
float ParametricCurveCollection::
evaluate_t(float t) const {
  float t0 = t;
  LVecBase3f point;

  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);

    if (curve->get_curve_type() == PCT_T) {
      if (!curve->get_point(t0, point)) {
        return -1.0f;
      }
      t0 = point[0];
    }
  }

  return t0;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::adjust_xyz
//       Access: Published
//  Description: Adjust the XYZ curve at the indicated time to the new
//               value.  The curve shape will change correspondingly.
//               Returns true if successful, false if unable to make
//               the adjustment for some reason.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
adjust_xyz(float t, const LVecBase3f &xyz) {
  ParametricCurve *xyz_curve = get_xyz_curve();
  if (xyz_curve == (ParametricCurve *)NULL) {
    return false;
  }

  float t0 = evaluate_t(t);
  if (t0 >= 0.0f && t < xyz_curve->get_max_t()) {
    return xyz_curve->adjust_point(t, xyz[0], xyz[1], xyz[2]);
  }
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::adjust_hpr
//       Access: Published
//  Description: Adjust the HPR curve at the indicated time to the new
//               value.  The curve shape will change correspondingly.
//               Returns true if successful, false if unable to make
//               the adjustment for some reason.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
adjust_hpr(float t, const LVecBase3f &hpr) {
  ParametricCurve *hpr_curve = get_hpr_curve();
  if (hpr_curve == (ParametricCurve *)NULL) {
    return false;
  }

  float t0 = evaluate_t(t);
  if (t0 >= 0.0f && t < hpr_curve->get_max_t()) {
    return hpr_curve->adjust_point(t, hpr[0], hpr[1], hpr[2]);
  }
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::recompute
//       Access: Published
//  Description: Ensures all the curves are freshly computed and
//               up-to-date.  Returns true if everything is valid,
//               false if at least one curve is incorrect.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
recompute() {
  bool all_ok = true;

  ParametricCurves::iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    if (!curve->recompute()) {
      all_ok = false;
    }
  }

  return all_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::stitch
//       Access: Published
//  Description: Regenerates this curve as one long curve: the first
//               curve connected end-to-end with the second one.
//               Either a or b may be the same as 'this'.  This will
//               lose any timewarps on the input curves.
//
//               Returns true if successful, false on failure.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
stitch(const ParametricCurveCollection *a,
       const ParametricCurveCollection *b) {
  PT(ParametricCurve) a_xyz = a->get_xyz_curve();
  PT(ParametricCurve) b_xyz = b->get_xyz_curve();

  PT(ParametricCurve) a_hpr = a->get_hpr_curve();
  PT(ParametricCurve) b_hpr = b->get_hpr_curve();

  clear();

  if (a_xyz != (ParametricCurve *)NULL && b_xyz != (ParametricCurve *)NULL) {
    PT(NurbsCurve) new_xyz = new NurbsCurve;
    if (!new_xyz->stitch(a_xyz, b_xyz)) {
      return false;
    }
    new_xyz->set_curve_type(PCT_XYZ);
    add_curve(new_xyz);
  }

  if (a_hpr != (ParametricCurve *)NULL && b_hpr != (ParametricCurve *)NULL) {
    PT(NurbsCurve) new_hpr = new NurbsCurve;
    if (!new_hpr->stitch(a_hpr, b_hpr)) {
      return false;
    }
    new_hpr->set_curve_type(PCT_HPR);
    add_curve(new_hpr);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::output
//       Access: Published
//  Description: Writes a brief one-line description of the
//               ParametricCurveCollection to the indicated output stream.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
output(ostream &out) const {
  if (get_num_curves() == 1) {
    out << "1 ParametricCurve";
  } else {
    out << get_num_curves() << " ParametricCurves";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::write
//       Access: Published
//  Description: Writes a complete multi-line description of the
//               ParametricCurveCollection to the indicated output stream.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
write(ostream &out, int indent_level) const {
  ParametricCurves::const_iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    indent(out, indent_level) << *curve << "\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::write_egg
//       Access: Published
//  Description: Writes an egg description of all the nurbs curves in
//               the collection to the specified output file.  Returns
//               true if the file is successfully written.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
write_egg(Filename filename, CoordinateSystem cs) {
  ofstream out;
  filename.set_text();

  if (!filename.open_write(out)) {
    parametrics_cat.error()
      << "Unable to write to " << filename << "\n";
    return false;
  }
  return write_egg(out, filename, cs);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::write_egg
//       Access: Published
//  Description: Writes an egg description of all the nurbs curves in
//               the collection to the specified output stream.  Returns
//               true if the file is successfully written.
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
write_egg(ostream &out, const Filename &filename, CoordinateSystem cs) {
  if (cs == CS_default) {
    cs = get_default_coordinate_system();
  }

  if (cs != CS_invalid) {
    out << "<CoordinateSystem> { ";
    switch (cs) {
    case CS_zup_right:
      out << "Z-Up";
      break;

    case CS_yup_right:
      out << "Y-Up";
      break;

    case CS_zup_left:
      out << "Z-Up-Left";
      break;

    case CS_yup_left:
      out << "Y-Up-Left";
      break;

    default:
      break;
    }
    out << " }\n\n";
  }

  int xyz_count = 0;
  int hpr_count = 0;
  int t_count = 0;

  ParametricCurves::iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);

    if (!curve->has_name()) {
      // If we don't have a name, come up with one.
      string name = filename.get_basename_wo_extension();

      switch (curve->get_curve_type()) {
      case PCT_XYZ:
        name += "_xyz";
        if (xyz_count > 0) {
          name += format_string(xyz_count);
        }
        xyz_count++;
        break;

      case PCT_HPR:
        name += "_hpr";
        if (hpr_count > 0) {
          name += format_string(hpr_count);
        }
        hpr_count++;
        break;

      case PCT_T:
        name += "_t";
        if (t_count > 0) {
          name += format_string(t_count);
        }
        t_count++;
        break;
      }

      curve->set_name(name);
    }

    if (!curve->write_egg(out, filename, CS_invalid)) {
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::r_add_curves
//       Access: Private
//  Description: The recursive implementation of add_curves().
////////////////////////////////////////////////////////////////////
int ParametricCurveCollection::
r_add_curves(PandaNode *node) {
  int num_curves = 0;

  if (node->is_of_type(ParametricCurve::get_class_type())) {
    ParametricCurve *curve = DCAST(ParametricCurve, node);
    prepare_add_curve(curve);
    _curves.push_back(curve);
    num_curves++;
  }

  int num_children = node->get_num_children();
  for (int i = 0; i < num_children; i++) {
    PandaNode *child = node->get_child(i);
    num_curves += r_add_curves(child);
  }

  return num_curves;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::register_drawer
//       Access: Public
//  Description: Registers a Drawer with this curve collection that
//               will automatically be updated whenever the collection
//               is modified, so that the visible representation of
//               the curve is kept up to date.  This is called
//               automatically by the ParametricCurveDrawer.
//
//               Any number of Drawers may be registered with a
//               particular curve collection.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
register_drawer(ParametricCurveDrawer *drawer) {
  _drawers.push_back(drawer);

  ParametricCurves::iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    curve->register_drawer(drawer);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::unregister_drawer
//       Access: Public
//  Description: Removes a previously registered drawer from the list
//               of automatically-refreshed drawers.  This is called
//               automatically by the ParametricCurveDrawer.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
unregister_drawer(ParametricCurveDrawer *drawer) {
  _drawers.remove(drawer);

  ParametricCurves::iterator ci;
  for (ci = _curves.begin(); ci != _curves.end(); ++ci) {
    ParametricCurve *curve = (*ci);
    curve->unregister_drawer(drawer);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::determine_hpr
//       Access: Private
//  Description: Computes the orientation at the given point in time,
//               based on the tangent of the XYZ curve.  Returns true
//               if the orientation can be determined, or false if it
//               cannot (in which case hpr is left unchanged).
////////////////////////////////////////////////////////////////////
bool ParametricCurveCollection::
determine_hpr(float t, ParametricCurve *xyz_curve, LVecBase3f &hpr) const {
  float t0 = evaluate_t(t);

  LVector3f tangent;
  if (!xyz_curve->get_tangent(t0, tangent)) {
    return false;
  }

  if (tangent.length_squared() == 0.0f) {
    return false;
  }

  LMatrix3f mat;
  look_at(mat, tangent);

  LVecBase3f scale, shear;
  return decompose_matrix(mat, scale, shear, hpr);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::prepare_add_curve
//       Access: Private
//  Description: Registers the curve with the list of drawers that
//               share this collection, in preparation for adding it
//               to the _curves list.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
prepare_add_curve(ParametricCurve *curve) {
  DrawerList::iterator di;
  for (di = _drawers.begin(); di != _drawers.end(); ++di) {
    ParametricCurveDrawer *drawer = (*di);
    curve->register_drawer(drawer);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::prepare_remove_curve
//       Access: Private
//  Description: Unregisters the curve with the list of drawers that
//               share this collection, in preparation for removing it
//               from the _curves list.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
prepare_remove_curve(ParametricCurve *curve) {
  DrawerList::iterator di;
  for (di = _drawers.begin(); di != _drawers.end(); ++di) {
    ParametricCurveDrawer *drawer = (*di);
    curve->unregister_drawer(drawer);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveCollection::redraw
//       Access: Private
//  Description: Calls redraw() on all drawers that share this
//               collection.
////////////////////////////////////////////////////////////////////
void ParametricCurveCollection::
redraw() {
  DrawerList::iterator di;
  for (di = _drawers.begin(); di != _drawers.end(); ++di) {
    ParametricCurveDrawer *drawer = (*di);
    drawer->redraw();
  }
}
