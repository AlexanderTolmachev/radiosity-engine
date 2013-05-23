#pragma once

#include "shape.h"

class Box;

typedef QSharedPointer<Box> BoxPointer;

class Box : public Shape {
  public:
    Box(Vector center, float width, float height, float depth, MaterialPointer material);
    virtual ~Box();

    virtual PatchCollectionPointer splitIntoPatches() const;

  private:
    Vector mCenter;
    float mWidth;
    float mHeight;
    float mDepth;
};