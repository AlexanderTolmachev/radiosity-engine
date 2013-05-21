/*!
 *\file shape.h
 *\brief Contains abstract Shape class definition
 */

#pragma once

#include <QSharedPointer>

#include "material.h"
#include "patch.h"

class Shape;

typedef QSharedPointer<Shape> ShapePointer;

class Shape {
  public:
    Shape(MaterialPointer material);
    virtual ~Shape();
    
    MaterialPointer getMaterial() const;

    PatchCollectionPointer splitIntoPatches(float patchSize) const;
    virtual PatchCollectionPointer splitIntoPatches() const = 0;

  private:
    MaterialPointer mMaterial;
};