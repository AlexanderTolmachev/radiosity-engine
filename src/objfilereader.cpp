
#include <vector>
#include <QFile>
#include <QStringList>

#include "objfilereader.h"
#include "trianglepatch.h"

MeshModelPointer ObjFileReader::readMeshFromObjFile(const QString &fileName, const Vector &translation, const Vector &scale, MaterialPointer material) const {
  QFile meshFile(fileName);

  meshFile.open(QIODevice::ReadOnly);
  if (!meshFile.isOpen()) {
    std::cerr << "Scene parsing error: Unable to open file at path '" << fileName.toUtf8().constData() << "'" << std::endl;
    return MeshModelPointer(NULL);
  }
  
  PatchCollectionPointer patches = PatchCollectionPointer(new PatchCollection());
  std::vector<VertexPointer> vertices;

  // Read lines, loop will be terminated by break
  while (true) {
    QString line = meshFile.readLine();
    if (meshFile.atEnd()) {
      break;
    }

    if (line.contains("#")) {
      // Skip comment line
    } else if (line.startsWith("v ")) {
      // Vertex line
      Vector position = readVector(line, "v");
      // Apply scale and translation
      Vector transformedPosition = position * scale + translation;
      VertexPointer vertex = VertexPointer(new Vertex(transformedPosition));
      vertices.push_back(vertex);
    } else if (line.startsWith("f ")) {
      // Face line
      int index0 = 0;
      int index1 = 0;
      int index2 = 0;            
      readFaceIndices(line, "f", index0, index1, index2);
      patches->addPatch(TrianglePatchPointer(new TrianglePatch(vertices[index0 - 1], vertices[index1 - 1], vertices[index2 - 1], material)));
    }
  }
  
  return MeshModelPointer(new MeshModel(patches, material));
}

Vector ObjFileReader::readVector(QString line, const QString& prefix) const {
  QString	string = line.remove(prefix + " ").remove("\n").remove("\r");
  QStringList coordinates = string.split(" ", QString::SkipEmptyParts);
  coordinates.removeAll(" ");
  coordinates.removeAll("");
  coordinates.removeAll("\n");
  
  return Vector(coordinates[0].toFloat(), coordinates[1].toFloat(), coordinates[2].toFloat());
}

void ObjFileReader::readFaceIndices(QString line, const QString& prefix, int &index0, int &index1, int &index2) const {
  QString	string = line.remove(prefix + " ").remove("\n").remove("\r");
  QStringList indices = string.split(" ", QString::SkipEmptyParts);
  indices.removeAll(" ");
  indices.removeAll("");
  indices.removeAll("\n");

  index0 = indices[0].toInt();
  index1 = indices[1].toInt();
  index2 = indices[2].toInt();
}