#pragma once

#include <vector>

#include "material.h"

class Patch;

typedef QSharedPointer<Patch> PatchPointer;
typedef std::vector<PatchPointer> PatchCollection;
typedef QSharedPointer<PatchCollection> PatchCollectionPointer;

class Patch {
  public:
    Patch(MaterialPointer material);
    virtual ~Patch();

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