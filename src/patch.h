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
    const Color& getAccumulatedColor() const;
    const Color& getResidualColor() const;
    
    Color getEmissionEnergy() const;
    float getEmissionEnergyValue() const;

    void setAccumulatedColor(const Color &incidentLight);
    void setResidualColor(const Color &excidentLight);

  private:
    // Patch material
    MaterialPointer mMaterial;
    /* Values used to calculate result color, names according to http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter39.html */
    // Already accumulated color
    Color mAccumulatedColor;
    // Color to be shot from patch
    Color mResidualColor;
};

bool comparePatchesByEmissionEmergy(const PatchPointer &patch1, const PatchPointer &patch2);