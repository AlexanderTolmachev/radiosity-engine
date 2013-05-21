/*!
 *\file ray.h
 *\brief Contains Ray class declaration
 */

#pragma once

#include "types.h"

class Ray {
  public:
    Ray(const Vector &originPosition, const Vector &direction);
    virtual ~Ray();

    Vector getPointAt(float distance) const;

    Vector getOriginPosition() const;
    Vector getDirection() const;

  private:
    Vector mOriginPosition;
    Vector mDirection;
};