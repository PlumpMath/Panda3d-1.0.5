// Filename: pmap.h
// Created by:  drose (05Jun01)
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

#ifndef PMAP_H
#define PMAP_H

#include "dtoolbase.h"
#include "pallocator.h"
#include "stl_compares.h"

#include <map>
#ifdef HAVE_STL_HASH
#include <hash_map>
#endif

#ifdef NO_STYLE_ALLOCATOR
// If we're not using custom allocators, just use the standard class
// definition.
#define pmap map
#define pmultimap multimap

#ifdef HAVE_STL_HASH
#define phash_map hash_map
#define phash_multimap hash_multimap
#else  // HAVE_STL_HASH
#define phash_map map
#define phash_multimap multimap
#endif  // HAVE_STL_HASH

#else  // NO_STYLE_ALLOCATOR

////////////////////////////////////////////////////////////////////
//       Class : pmap
// Description : This is our own Panda specialization on the default
//               STL map.  Its main purpose is to call the hooks
//               for MemoryUsage to properly track STL-allocated
//               memory.
////////////////////////////////////////////////////////////////////
template<class Key, class Value, class Compare = less<Key> >
class pmap : public map<Key, Value, Compare, pallocator<Value> > {
public:
  pmap() : map<Key, Value, Compare, pallocator<Value> >() { }
  pmap(const pmap<Key, Value, Compare> &copy) : map<Key, Value, Compare, pallocator<Value> >(copy) { }
  pmap(const Compare &comp) : map<Key, Value, Compare, pallocator<Value> >(comp) { }
};

////////////////////////////////////////////////////////////////////
//       Class : pmultimap
// Description : This is our own Panda specialization on the default
//               STL multimap.  Its main purpose is to call the hooks
//               for MemoryUsage to properly track STL-allocated
//               memory.
////////////////////////////////////////////////////////////////////
template<class Key, class Value, class Compare = less<Key> >
class pmultimap : public multimap<Key, Value, Compare, pallocator<Value> > {
public:
  pmultimap() : multimap<Key, Value, Compare, pallocator<Value> >() { }
  pmultimap(const pmultimap<Key, Value, Compare> &copy) : multimap<Key, Value, Compare, pallocator<Value> >(copy) { }
  pmultimap(const Compare &comp) : multimap<Key, Value, Compare, pallocator<Value> >(comp) { }
};

#ifdef HAVE_STL_HASH
////////////////////////////////////////////////////////////////////
//       Class : phash_map
// Description : This is our own Panda specialization on the default
//               STL hash_map.  Its main purpose is to call the hooks
//               for MemoryUsage to properly track STL-allocated
//               memory.
////////////////////////////////////////////////////////////////////
template<class Key, class Value, class Compare = method_hash<Key, less<Key> > >
class phash_map : public hash_map<Key, Value, Compare, pallocator<Value> > {
public:
  phash_map() : hash_map<Key, Value, Compare, pallocator<Value> >() { }
  phash_map(const phash_map<Key, Value, Compare> &copy) : hash_map<Key, Value, Compare, pallocator<Value> >(copy) { }
  phash_map(const Compare &comp) : hash_map<Key, Value, Compare, pallocator<Value> >(comp) { }
};

////////////////////////////////////////////////////////////////////
//       Class : phash_multimap
// Description : This is our own Panda specialization on the default
//               STL hash_multimap.  Its main purpose is to call the hooks
//               for MemoryUsage to properly track STL-allocated
//               memory.
////////////////////////////////////////////////////////////////////
template<class Key, class Value, class Compare = method_hash<Key, less<Key> > >
class phash_multimap : public hash_multimap<Key, Value, Compare, pallocator<Value> > {
public:
  phash_multimap() : hash_multimap<Key, Value, Compare, pallocator<Value> >() { }
  phash_multimap(const phash_multimap<Key, Value, Compare> &copy) : hash_multimap<Key, Value, Compare, pallocator<Value> >(copy) { }
  phash_multimap(const Compare &comp) : hash_multimap<Key, Value, Compare, pallocator<Value> >(comp) { }
};

#else // HAVE_STL_HASH
#define phash_map pmap
#define phash_multimap pmultimap
#endif  // HAVE_STL_HASH

#endif  // NO_STYLE_ALLOCATOR
#endif
