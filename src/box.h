#pragma once

#include "shape.h"
#include "quad.h"

class Box;

typedef QSharedPointer<Box> BoxPointer;

class Box : public Shape {
  public:
    Box(Vector center, float width, float height, float depth, 
        float rotationXAngle, float rotationYAngle, float rotationZAngle, 
        MaterialPointer material);
    virtual ~Box();

    virtual PatchCollectionPointer splitIntoPatches() const;
    virtual PatchCollectionPointer splitIntoPatches(float patchSize) const;
  
  private:
    std::vector<ShapePointer> getFaces() const;

  private:
    Vector mCenter;
    float mWidth;
    float mHeight;
    float mDepth;
    float mRotationXAngle;
    float mRotationYAngle;
    float mRotationZAngle;
};