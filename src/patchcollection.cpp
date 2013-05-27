#include "patchcollection.h"

PatchCollection::PatchCollection() {
}

PatchCollection::~PatchCollection() {
}

void PatchCollection::addPatch(const PatchPointer &patch) {
  if (!patchesHash.contains(patch->getId())) {
    patchesHash.insert(patch->getId(), patch);
  }

  for each (auto vertex in patch->getVertices()) {
    addVertex(vertex);
  }
}

void PatchCollection::addVertex(const VertexPointer &vertex) {
  if (!verticesHash.contains(vertex->getId())) {
    verticesHash.insert(vertex->getId(), vertex);
  }
}

void PatchCollection::addPatches(const PatchCollectionPointer &patches) {
  for each (auto patch in patches->getPatches()) {
    addPatch(patch);
  }
  // Vertices will be added automatically
}

QList<VertexPointer> PatchCollection::getVertices() const {
  return verticesHash.values();
}

QList<PatchPointer> PatchCollection::getPatches() const {
  return patchesHash.values();
}

VertexPointer PatchCollection::getVertexById(unsigned int vertexId) const {
  return verticesHash.value(vertexId, VertexPointer(NULL));
}

PatchPointer PatchCollection::getPatchById(unsigned int patchId) const {
  return patchesHash.value(patchId, PatchPointer(NULL));
}
