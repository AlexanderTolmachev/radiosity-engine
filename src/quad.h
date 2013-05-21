#pragma once

#include "shape.h"

class Quad;

typedef QSharedPointer<Quad> QuadPointer;

class Quad : public Shape {
  public:
    Quad(Vector vertex0, Vector vertex1, Vector vertex2, Vector vertex3, MaterialPointer material);
    virtual ~Quad();

  private:
    Vector mVertex0;
    Vector mVertex1;
    Vector mVertex2;
    Vector mVertex3;
};