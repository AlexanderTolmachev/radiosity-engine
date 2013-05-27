#include "quad.h"
#include "trianglepatch.h"

Quad::Quad(Vector vertex0, Vector vertex1, Vector vertex2, Vector vertex3, MaterialPointer material) 
  : Shape(material),
    mVertex0(vertex0),
    mVertex1(vertex1),
    mVertex2(vertex2),
    mVertex3(vertex3) {
}

Quad::~Quad() {
}

PatchCollectionPointer Quad::splitIntoPatches() const {
  VertexPointer vertex0 = VertexPointer(new Vertex(mVertex0));
  VertexPointer vertex1 = VertexPointer(new Vertex(mVertex1));
  VertexPointer vertex2 = VertexPointer(new Vertex(mVertex2));
  VertexPointer vertex3 = VertexPointer(new Vertex(mVertex3));

  PatchCollectionPointer patches = PatchCollectionPointer(new PatchCollection());
  patches->addPatch(TrianglePatchPointer(new TrianglePatch(vertex0, vertex1, vertex2, getMaterial())));
  patches->addPatch(TrianglePatchPointer(new TrianglePatch(vertex0, vertex2, vertex3, getMaterial())));
  return patches;
}