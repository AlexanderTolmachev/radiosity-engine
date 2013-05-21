#pragma once

#include "patch.h"

class QuadPatch;

typedef QSharedPointer<QuadPatch> QuadPatchPointer;

class QuadPatch : public Patch {
public:
  QuadPatch(Vector vertex0, Vector vertex1, Vector vertex2, Vector vertex3, MaterialPointer material);
  virtual ~QuadPatch();

  RayIntersection intersectWithRay(const Ray &ray) const;

private:
  Vector mVertex0;
  Vector mVertex1;
  Vector mVertex2;
  Vector mVertex3;
};