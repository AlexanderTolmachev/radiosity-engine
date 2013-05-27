#include "patchcollection.h"

PatchCollection::PatchCollection() {
}

PatchCollection::~PatchCollection() {
}

void PatchCollection::addPatch(const PatchPointer &patch) {
  if (!patchesHash.contains(patch->getId())) {
    patchesHash.insert(patch->getId(), patch);
    for each (auto vertex in patch->getVertices()) {
      addVertex(vertex);
      addAdjacentPatchToVertex(vertex, patch);
    }
  }
}

void PatchCollection::addPatches(const PatchCollectionPointer &patches) {
  for each (auto patch in patches->getPatches()) {
    addPatch(patch);
  }
  // Vertices will be added automatically
}

void PatchCollection::addVertex(const VertexPointer &vertex) {
  if (!verticesHash.contains(vertex->getId())) {
    verticesHash.insert(vertex->getId(), vertex);
  }
}

void PatchCollection::addAdjacentPatchToVertex(const VertexPointer &vertex, const PatchPointer &patch) {
  if (!verticesToAdjacentPatchIdsHash.contains(vertex->getId())) {
    verticesToAdjacentPatchIdsHash.insert(vertex->getId(), QSet<unsigned int>());
  }
  verticesToAdjacentPatchIdsHash[vertex->getId()].insert(patch->getId());
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

QList<PatchPointer> PatchCollection::getPatchesAdjacentToVertex(const VertexPointer &vertex) const {
  QList<PatchPointer> adjacentPatches;

  for each (auto patchId in verticesToAdjacentPatchIdsHash.value(vertex->getId())) {
    adjacentPatches.append(patchesHash.value(patchId));
  }

  return adjacentPatches;
}

void PatchCollection::insertPatch(const PatchPointer &patch) {
  PatchPointer patchToInsert = patch;
  std::vector<VertexPointer> patchVertices = patch->getVertices();
  VertexPointer vertex0 = patchVertices[0];
  VertexPointer vertex1 = patchVertices[1];
  VertexPointer vertex2 = patchVertices[2];
}
