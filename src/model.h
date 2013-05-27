#pragma once

#include "shape.h"

class MeshModel;

typedef QSharedPointer<MeshModel> MeshModelPointer;

class MeshModel : public Shape {
  public:
    MeshModel(const PatchCollectionPointer &patches, const MaterialPointer &material);
    virtual ~MeshModel();

    virtual PatchCollectionPointer splitIntoPatches() const;

  private:
    PatchCollectionPointer mPatches;
};