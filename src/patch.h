#pragma once

#include <vector>

#include "material.h"
#include "ray.h"
#include "hemisphere.h"

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
    virtual Vector getCenter() const = 0;
    virtual Vector getNormal() const = 0;
    virtual Vector getRandomPoint() const = 0;
    virtual Hemisphere getHemisphere() const = 0;
    
    virtual PatchCollectionPointer split() const = 0;
    virtual RayIntersection intersectWithRay(const Ray &ray) const = 0;

    unsigned int getId() const;
    const MaterialPointer& getMaterial() const; 
    const Color& getAccumulatedColor() const;
    const Color& getResidualColor() const;
    
    Color getEmissionEnergy() const;
    float getEmissionEnergyValue() const;

    void updateAccumulatedColor(const Color &colorDelta);
    void updateResidualColor(const Color &colorDelta);
    void resetResidualColor();

  private:
    // Patch id
    unsigned int mId;
    // Patch material
    MaterialPointer mMaterial;
    
    /* Values used to calculate result color, names according to http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter39.html */
    // Already accumulated color
    Color mAccumulatedColor;
    // Color to be shot from patch
    Color mResidualColor;
};

bool comparePatchesByEmissionEmergy(const PatchPointer &patch1, const PatchPointer &patch2);
