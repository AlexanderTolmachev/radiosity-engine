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
  PatchCollectionPointer patches = PatchCollectionPointer(new PatchCollection());
  patches->push_back(TrianglePatchPointer(new TrianglePatch(mVertex0, mVertex1, mVertex2, getMaterial())));
  patches->push_back(TrianglePatchPointer(new TrianglePatch(mVertex0, mVertex2, mVertex3, getMaterial())));
  return patches;
}