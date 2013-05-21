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
    Shape(MaterialPointer material) :mMaterial(material) {} 
    virtual ~Shape() {}
    
    virtual PatchCollectionPointer splitIntoPatches(float patchSize) const = 0;

    MaterialPointer getMaterial() const { return mMaterial; }

  private:
    MaterialPointer mMaterial;
};