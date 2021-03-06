// Filename: sparkleParticleRenderer.I
// Created by:  charles (27Jun00)
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
//    Function : set_center_color
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE void SparkleParticleRenderer::
set_center_color(const Colorf& c) {
  _center_color = c;
}

////////////////////////////////////////////////////////////////////
//    Function : set_edge_color
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE void SparkleParticleRenderer::
set_edge_color(const Colorf& c) {
  _edge_color = c;
}

////////////////////////////////////////////////////////////////////
//    Function : set_life_scale
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE void SparkleParticleRenderer::
set_life_scale(SparkleParticleRenderer::SparkleParticleLifeScale ls) {
  _life_scale = ls;
}

////////////////////////////////////////////////////////////////////
//    Function : set_birth_radius
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE void SparkleParticleRenderer::
set_birth_radius(float radius) {
  _birth_radius = radius;
}

////////////////////////////////////////////////////////////////////
//    Function : set_death_radius
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE void SparkleParticleRenderer::
set_death_radius(float radius) {
  _death_radius = radius;
}

////////////////////////////////////////////////////////////////////
//    Function : get_center_color
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE const Colorf& SparkleParticleRenderer::
get_center_color() const {
  return _center_color;
}

////////////////////////////////////////////////////////////////////
//    Function : get_edge_color
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE const Colorf& SparkleParticleRenderer::
get_edge_color() const {
  return _edge_color;
}

////////////////////////////////////////////////////////////////////
//    Function : get_life_scale
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE SparkleParticleRenderer::SparkleParticleLifeScale SparkleParticleRenderer::
get_life_scale() const {
  return _life_scale;
}

////////////////////////////////////////////////////////////////////
//    Function : get_birth_radius
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE float SparkleParticleRenderer::
get_birth_radius() const {
  return _birth_radius;
}

////////////////////////////////////////////////////////////////////
//    Function : get_death_radius
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE float SparkleParticleRenderer::
get_death_radius() const {
  return _death_radius;
}

////////////////////////////////////////////////////////////////////
//    Function : get_radius
//      Access : public
////////////////////////////////////////////////////////////////////
INLINE float SparkleParticleRenderer::
get_radius(BaseParticle *bp) {
  if (_life_scale == SP_NO_SCALE)
    return _birth_radius;
  else {
    float s_x = CUBIC_T(bp->get_parameterized_age());
    return LERP(s_x, _birth_radius, _death_radius);
  }
}
