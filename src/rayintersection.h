/*!
 *\file rayintersection.h
 *\brief Contains RayIntersection struct declaration
 */

#pragma once

#include <vector>

#include "types.h"
#include "patch.h"

struct RayIntersection {
  RayIntersection() 
    : rayIntersectsWithPatch(false), 
      patch(NULL), 
      distanceFromRayOrigin(MAX_DISTANCE_TO_INTERSECTON) {}
  RayIntersection(bool intersectsWithPatch, PatchPointer intersectsWith, float distance, float barycentricU, float barycentricV)
    : rayIntersectsWithPatch(intersectsWithPatch), 
      patch(intersectsWith), 
      distanceFromRayOrigin(distance),
      u(barycentricU),
      v(barycentricV) {}
  // Does intersection exist
  bool rayIntersectsWithPatch;
  // Shape the ray intersects with
  PatchPointer patch;
  // Distance from ray origin to intersection point
  float distanceFromRayOrigin;
  // Barycentric coordinates for triangle intersection
  float	u;
  float	v;
};