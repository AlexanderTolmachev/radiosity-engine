#include <queue>

#include "shape.h"

Shape::Shape(MaterialPointer material) 
  :mMaterial(material) {
} 

Shape::~Shape() {
}

MaterialPointer Shape::getMaterial() const { 
  return mMaterial; 
}

PatchCollectionPointer Shape::splitIntoPatches(float patchSize) const {
  std::queue<PatchPointer> patchQueue;
  PatchCollectionPointer splittedShapePatches = PatchCollectionPointer(new PatchCollection());
  PatchCollectionPointer shapePatches = splitIntoPatches();

  for each (auto patch in shapePatches->getPatches()) {
    if (patch->getSize() <= patchSize) {
      splittedShapePatches->addPatch(patch);
    } else {
      patchQueue.push(patch);
    }
  }

  while (!patchQueue.empty()) {
    PatchPointer patch = patchQueue.front();
    patchQueue.pop();

    PatchCollectionPointer splittedPatches = patch->split();
    for each (auto splittedPatch in splittedPatches->getPatches()) {
      if (splittedPatch->getSize() <= patchSize) {
        splittedShapePatches->addPatch(splittedPatch);
      } else {
        patchQueue.push(splittedPatch);
      }
    }
  }

  return splittedShapePatches;
}
