// Filename: physicsManager.h
// Created by:  charles (14Jun00)
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

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "pandabase.h"
#include "pointerTo.h"

#include "physical.h"
#include "linearForce.h"
#include "angularForce.h"
#include "linearIntegrator.h"
#include "angularIntegrator.h"
#include "physicalNode.h"

#include "plist.h"
#include "pvector.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysicsManager
// Description : Physics don't get much higher-level than this.
//               Attach as many Physicals (particle systems, etc..)
//               as you want, pick an integrator and go.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSICS PhysicsManager {
public:
  // NOTE that the physicals container is NOT reference counted.
  // this does indeed mean that you are NOT supposed to use this
  // as a primary storage container for the physicals.  This is so
  // because physicals, on their death, ask to be removed from their
  // current physicsmanager, if one exists, relieving the client from
  // the task and also allowing for dynamically created and destroyed
  // physicals.
  typedef pvector<Physical *> PhysicalsVector;
  typedef pvector<PT(LinearForce)> LinearForceVector;
  typedef pvector<PT(AngularForce)> AngularForceVector;

PUBLISHED:
  PhysicsManager();
  virtual ~PhysicsManager();

  INLINE void attach_linear_integrator(LinearIntegrator *i);
  INLINE void attach_angular_integrator(AngularIntegrator *i);
  INLINE void attach_physical(Physical *p);
  INLINE void attach_physicalnode(PhysicalNode *p);
  INLINE void attach_physical_node(PhysicalNode *p);
  INLINE void add_linear_force(LinearForce *f);
  INLINE void add_angular_force(AngularForce *f);
  INLINE void clear_linear_forces();
  INLINE void clear_angular_forces();
  INLINE void clear_physicals();

  INLINE void set_viscosity(float viscosity);
  INLINE float get_viscosity() const;
  
  void remove_physical(Physical *p);
  void remove_physical_node(PhysicalNode *p);
  void remove_linear_force(LinearForce *f);
  void remove_angular_force(AngularForce *f);
  void do_physics(float dt);
  
  virtual void output(ostream &out) const;
  virtual void write_physicals(ostream &out, unsigned int indent=0) const;
  virtual void write_linear_forces(ostream &out, unsigned int indent=0) const;
  virtual void write_angular_forces(ostream &out, unsigned int indent=0) const;
  virtual void write(ostream &out, unsigned int indent=0) const;

  virtual void debug_output(ostream &out, unsigned int indent=0) const;

public:
  friend class Physical;

private:
  float _viscosity;
  PhysicalsVector _physicals;
  LinearForceVector _linear_forces;
  AngularForceVector _angular_forces;

  PT(LinearIntegrator) _linear_integrator;
  PT(AngularIntegrator) _angular_integrator;
};

#include "physicsManager.I"

#endif // PHYSICSMANAGER_H
