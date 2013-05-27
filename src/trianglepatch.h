/*!
 *\file triangle.h
 *\brief Contains Triangle class declaration
 */

#pragma once

#include "patch.h"
#include "types.h"
#include "vertex.h"

class TrianglePatch;

typedef QSharedPointer<TrianglePatch> TrianglePatchPointer;

class TrianglePatch : public Patch {
  public:
    TrianglePatch(const VertexPointer &vertex0, const VertexPointer &vertex1, const VertexPointer &vertex2, const MaterialPointer &material);
    virtual ~TrianglePatch();
    
    virtual float getSize() const;
    virtual float getArea() const;
    virtual Vector getCenter() const;
    virtual Vector getNormal() const;
    virtual Vector getRandomPoint() const;
    virtual Hemisphere getHemisphere() const;
    virtual std::vector<VertexPointer> getVertices() const;
    virtual Color getColor(const RayIntersection &rayIntersection) const;

    virtual QSharedPointer<PatchCollection> split() const;    
    virtual RayIntersection intersectWithRay(const Ray &ray) const;    

  private:
    // Triangle vertices
    VertexPointer mVertex0;
    VertexPointer mVertex1;
    VertexPointer mVertex2;
    // Precalculated normal
    Vector mNormal;
    // Precalculated area
    float mArea;
    // Precalculated center
    Vector mCenter;
};