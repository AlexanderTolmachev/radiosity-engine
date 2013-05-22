/*!
 *\file triangle.h
 *\brief Contains Triangle class declaration
 */

#pragma once

#include "patch.h"
#include "types.h"


class TrianglePatch;

typedef QSharedPointer<TrianglePatch> TrianglePatchPointer;

class TrianglePatch : public Patch {
  public:
    TrianglePatch(Vector vertex0, Vector vertex1, Vector vertex2, MaterialPointer material);
    virtual ~TrianglePatch();
    
    virtual float getSize() const;
    virtual float getArea() const;
    virtual PatchCollectionPointer split() const;
    
    virtual RayIntersection intersectWithRay(const Ray &ray) const;    
    // virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    Vector mVertex0;
    Vector mVertex1;
    Vector mVertex2;
    Vector mNormal;
};