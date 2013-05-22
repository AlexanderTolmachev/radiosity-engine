#pragma once

#include <vector>

#include "material.h"
#include "ray.h"


class Patch;
struct RayIntersection;

typedef QSharedPointer<Patch> PatchPointer;
typedef std::vector<PatchPointer> PatchCollection;
typedef QSharedPointer<PatchCollection> PatchCollectionPointer;

class Patch {
  public:
    Patch(MaterialPointer material);
    virtual ~Patch();

    virtual float getSize() const = 0;
    virtual float getArea() const = 0;
    virtual PatchCollectionPointer split() const = 0;
    virtual RayIntersection intersectWithRay(const Ray &ray) const = 0;

    const MaterialPointer& getMaterial() const; 
    const Color& getIncidentLight() const;
    const Color& getExcidentLight() const;

    void setIncidentLight(const Color &incidentLight);
    void setExcidentLight(const Color &excidentLight);

  private:
    MaterialPointer mMaterial;
    Color mIncidentLight;
    Color mExcidentLight;
};