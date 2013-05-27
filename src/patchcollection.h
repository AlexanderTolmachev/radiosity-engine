#pragma once

#include <QHash>
#include <QList>
#include <QSet>

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
    QList<PatchPointer> getPatchesAdjacentToVertex(const VertexPointer &vertex) const;
  
  private:
    void addVertex(const VertexPointer &vertex);
    void addAdjacentPatchToVertex(const VertexPointer &vertex, const PatchPointer &patch);
    void insertPatch(const PatchPointer &patch);

  private:
    QHash<unsigned int, VertexPointer> verticesHash;
    QHash<unsigned int, PatchPointer> patchesHash;
    QHash<unsigned int, QSet<unsigned int>> verticesToAdjacentPatchIdsHash;
};