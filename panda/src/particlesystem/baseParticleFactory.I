// Filename: baseParticleFactory.I
// Created by:  charles (05Jul00)
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
//    Function : set_lifespan_base
// Description : public
////////////////////////////////////////////////////////////////////
INLINE void BaseParticleFactory::
set_lifespan_base(float lb) {
  _lifespan_base = lb;
}

////////////////////////////////////////////////////////////////////
//    Function : set_lifespan_spread
// Description : public
////////////////////////////////////////////////////////////////////
INLINE void BaseParticleFactory::
set_lifespan_spread(float ld) {
  _lifespan_spread = ld;
}

////////////////////////////////////////////////////////////////////
//    Function : set_mass_base
// Description : public
////////////////////////////////////////////////////////////////////
INLINE void BaseParticleFactory::
set_mass_base(float mb) {
  _mass_base = mb;
}

////////////////////////////////////////////////////////////////////
//    Function : set_mass_spread
// Description : public
////////////////////////////////////////////////////////////////////
INLINE void BaseParticleFactory::
set_mass_spread(float md) {
  _mass_spread = md;
}

////////////////////////////////////////////////////////////////////
//    Function : set_terminal_velocity_base
// Description : public
////////////////////////////////////////////////////////////////////
INLINE void BaseParticleFactory::
set_terminal_velocity_base(float tvb) {
  _terminal_velocity_base = tvb;
}

////////////////////////////////////////////////////////////////////
//    Function : set_terminal_velocity_spread
// Description : public
////////////////////////////////////////////////////////////////////
INLINE void BaseParticleFactory::
set_terminal_velocity_spread(float tvd) {
  _terminal_velocity_spread = tvd;
}

////////////////////////////////////////////////////////////////////
//    Function : get_lifespan_base
// Description : public
////////////////////////////////////////////////////////////////////
INLINE float BaseParticleFactory::
get_lifespan_base() const {
  return _lifespan_base;
}

////////////////////////////////////////////////////////////////////
//    Function : get_lifespan_spread
// Description : public
////////////////////////////////////////////////////////////////////
INLINE float BaseParticleFactory::
get_lifespan_spread() const {
  return _lifespan_spread;
}

////////////////////////////////////////////////////////////////////
//    Function : get_mass_base
// Description : public
////////////////////////////////////////////////////////////////////
INLINE float BaseParticleFactory::
get_mass_base() const {
  return _mass_base;
}

////////////////////////////////////////////////////////////////////
//    Function : get_mass_spread
// Description : public
////////////////////////////////////////////////////////////////////
INLINE float BaseParticleFactory::
get_mass_spread() const {
  return _mass_spread;
}

////////////////////////////////////////////////////////////////////
//    Function : get_terminal_velocity_base
// Description : public
////////////////////////////////////////////////////////////////////
INLINE float BaseParticleFactory::
get_terminal_velocity_base() const {
  return _terminal_velocity_base;
}

////////////////////////////////////////////////////////////////////
//    Function : get_terminal_velocity_spread
// Description : public
////////////////////////////////////////////////////////////////////
INLINE float BaseParticleFactory::
get_terminal_velocity_spread() const {
  return _terminal_velocity_spread;
}

