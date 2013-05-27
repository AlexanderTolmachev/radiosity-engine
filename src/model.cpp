#include "model.h"

MeshModel::MeshModel(const PatchCollectionPointer &patches, const MaterialPointer &material) 
  : Shape(material), 
    mPatches(patches) {
}

MeshModel::~MeshModel() {
}

PatchCollectionPointer MeshModel::splitIntoPatches() const {
  return mPatches;
}