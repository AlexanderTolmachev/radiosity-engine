#pragma once

#include "types.h"

class Hemisphere {
public:
  Hemisphere(const Vector &cener, float radius, const Vector &viewDirection, const Vector &xAxisDirecion);
  virtual ~Hemisphere();

  Vector getCenter() const;
  float getRadius() const;
  Vector getViewDirection() const;

  Vector getRandomCirclePointProjectedToSurface() const;

private:
  Vector getRandomPointOnBaseCircle() const;

private:
  Vector mCenter;
  float mRadius;
  // Hemisphere internal 
  Vector mXAxis;
  Vector mYAxis;
  Vector mZAxis; // View direction
};