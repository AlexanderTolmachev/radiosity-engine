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

    /**
    * Split shape into patches of any size;
    */
    virtual PatchCollectionPointer splitIntoPatches() const = 0;
    /**
    * Split shape into patches such as each patch size is not grater than patchSize.
    */
    PatchCollectionPointer splitIntoPatches(float patchSize) const;

  private:
    MaterialPointer mMaterial;
};