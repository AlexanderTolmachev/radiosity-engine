#include "quadpatch.h"

QuadPatch::QuadPatch(Vector vertex0, Vector vertex1, Vector vertex2, Vector vertex3, MaterialPointer material) 
  : Patch(material),
    mVertex0(vertex0),
    mVertex1(vertex1),
    mVertex2(vertex2),
    mVertex3(vertex3) {
}

QuadPatch::~QuadPatch() {
}

