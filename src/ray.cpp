/*!
 *\file ray.cpp
 *\brief Contains Ray class definition
 */

#include "ray.h"

Ray::Ray(const Vector &originPosition, const Vector &direction) 
  : mOriginPosition(originPosition), 
    mDirection(direction) {
  mDirection.normalize();
}

Ray::~Ray() {
}

Vector Ray::getPointAt(float distance) const {
  return mOriginPosition + mDirection * distance;
}

Vector Ray::getOriginPosition() const {
  return mOriginPosition;
}

Vector Ray::getDirection() const {
  return mDirection;
}
