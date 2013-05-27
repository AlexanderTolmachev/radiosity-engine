#pragma once

#include "types.h"
#include "model.h"

/*
struct Vertex {
  Vector position;
  Vector normal;
};
*/

class ObjFileReader {
  public:
    MeshModelPointer readMeshFromObjFile(const QString &fileName, const Vector &translation, const Vector &scale, MaterialPointer material) const;
  private:
    Vector readVector(QString line, const QString& prefix) const;
    void readFaceIndices(QString line, const QString& prefix, int &index0, int &index1, int &index2) const;
};