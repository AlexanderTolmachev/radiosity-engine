#pragma once

#include <QHash>
#include <QList>

#include "patch.h"

class PatchCollection;

typedef QSharedPointer<PatchCollection> PatchCollectionPointer;

class PatchCollection {
  public:
    PatchCollection();
    virtual ~PatchCollection();

    void addPatch(const PatchPointer &patch);
    void addPatches(const PatchCollectionPointer &patches);

    QList<VertexPointer> getVertices() const;
    QList<PatchPointer> getPatches() const;
    VertexPointer getVertexById(unsigned int vertexId) const;
    PatchPointer getPatchById(unsigned int patchId) const;
  
  private:
    void addVertex(const VertexPointer &vertex);

  private:
    QHash<unsigned int, VertexPointer> verticesHash;
    QHash<unsigned int, PatchPointer> patchesHash;
};