/*!
 *\file shape.h
 *\brief Contains abstract Shape class definition
 */

#pragma once

#include <QSharedPointer>

#include "types.h"
//#include "material.h"

class Shape;
struct RayIntersection; 

typedef QSharedPointer<Shape> ShapePointer;

class Shape {
  public:
//    Shape(MaterialPointer material) :mMaterial(material) {} 
//    virtual ~Shape() {}

//    MaterialPointer getMaterial() const { return mMaterial; }
    
  private:
//    MaterialPointer mMaterial;
};