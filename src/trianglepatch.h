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
    virtual Vector getCenter() const;
    virtual Vector getNormal() const;
    virtual Vector getRandomPoint() const;
    virtual Hemisphere getHemisphere() const;

    virtual PatchCollectionPointer split() const;    
    virtual RayIntersection intersectWithRay(const Ray &ray) const;    

  private:
    // Triangle vertices
    Vector mVertex0;
    Vector mVertex1;
    Vector mVertex2;
    // Precalculated normal
    Vector mNormal;
    // Precalculated area
    float mArea;
    // Precalculated center
    Vector mCenter;
};