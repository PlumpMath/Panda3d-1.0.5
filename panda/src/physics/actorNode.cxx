// Filename: actorNode.cxx
// Created by:  charles (07Aug00)
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

#include "actorNode.h"
#include "config_physics.h"
#include "physicsObject.h"

#include "transformState.h"

TypeHandle ActorNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function : ActorNode
//       Access : public
//  Description : Constructor
////////////////////////////////////////////////////////////////////
ActorNode::
ActorNode(const string &name) :
    PhysicalNode(name) {
  _contact_vector = LVector3f::zero();
  add_physical(new Physical(1, true));
  _mass_center = get_physical(0)->get_phys_body();
  _mass_center->set_active(true);
  #ifndef NDEBUG
    _mass_center->set_name(name);
  #endif
  _ok_to_callback = true;
}

////////////////////////////////////////////////////////////////////
//     Function : ActorNode
//       Access : public
//  Description : Copy Constructor.
////////////////////////////////////////////////////////////////////
ActorNode::
ActorNode(const ActorNode &copy) :
  PhysicalNode(copy) {
  _contact_vector = LVector3f::zero();
  _ok_to_callback = true;
  _mass_center = get_physical(0)->get_phys_body();
}

////////////////////////////////////////////////////////////////////
//     Function : ~ActorNode
//       Access : public
//  Description : destructor
////////////////////////////////////////////////////////////////////
ActorNode::
~ActorNode() {
}

////////////////////////////////////////////////////////////////////
//     Function : update_transform
//       Access : public
//  Description : this sets the transform generated by the contained
//                Physical, moving the node and subsequent geometry.
//                i.e. copy from PhysicsObject to PandaNode
////////////////////////////////////////////////////////////////////
void ActorNode::
update_transform() {
  LMatrix4f lcs = _mass_center->get_lcs();

  // lock the callback so that this doesn't call transform_changed.
  _ok_to_callback = false;
  set_transform(TransformState::make_mat(lcs));
  _ok_to_callback = true;
}

////////////////////////////////////////////////////////////////////
//     Function : transform_changed
//       Access : private, virtual
//  Description : node hook.  This function handles outside
//                (non-physics) actions on the actor
//                and updates the internal representation of the node.
//                i.e. copy from PandaNode to PhysicsObject
////////////////////////////////////////////////////////////////////
void ActorNode::
transform_changed() {
  // this callback could be triggered by update_transform, BAD.
  if (_ok_to_callback == false) {
    return;
  }

  // get the transform
  CPT(TransformState) transform = get_transform();

  // extract the orientation
  if (_mass_center->get_oriented() == true) {
    _mass_center->set_orientation(transform->get_quat());
  }

  // apply
  _mass_center->set_position(transform->get_pos());
}

////////////////////////////////////////////////////////////////////
//     Function : write
//       Access : Public
//  Description : Write a string representation of this instance to
//                <out>.
////////////////////////////////////////////////////////////////////
void ActorNode::
write(ostream &out, unsigned int indent) const {
  #ifndef NDEBUG //[
  out.width(indent); out<<""; out<<"ActorNode:\n";
  out.width(indent+2); out<<""; out<<"_ok_to_callback "<<_ok_to_callback<<"\n";
  out.width(indent+2); out<<""; out<<"_mass_center\n";
  _mass_center->write(out, indent+4);
  PhysicalNode::write(out, indent+2);
  #endif //] NDEBUG
}
