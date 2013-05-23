#include <cmath>

#include "hemisphere.h"
#include "numbergenerator.h"

#define _USE_MATH_DEFINES

/**
* public:
*/
Hemisphere::Hemisphere(const Vector &cener, float radius, const Vector &viewDirection, const Vector &xAxisDirection)
  : mCenter(cener), 
    mRadius(radius), 
    mZAxis(viewDirection),
    mXAxis(xAxisDirection) {
  mXAxis.normalize();
  mZAxis.normalize();
  mYAxis = mZAxis.crossProduct(mXAxis);
  mYAxis.normalize();
}

Hemisphere::~Hemisphere() {
}

Vector Hemisphere::getCenter() const {
  return mCenter;
}

float Hemisphere::getRadius() const {
  return mRadius;
}

Vector Hemisphere::getViewDirection() const {
  return mZAxis;
}

Vector Hemisphere::getRandomCirclePointProjectedToSurface() const {
  Vector pointOnBase = getRandomPointOnBaseCircle();
  float distanceToCenter = pointOnBase.length();
  float offsetValue = sqrt(mRadius * mRadius - distanceToCenter * distanceToCenter);
  Vector pointAtSurface = pointOnBase + mZAxis * offsetValue;

  assert ( abs(pointAtSurface.length() - mRadius) <= FLOAT_ZERO );

  return pointAtSurface + mCenter;
}

/**
* private:
*/
// Generate random point on hemisphere base circle; coordinates are relative to center
Vector Hemisphere::getRandomPointOnBaseCircle() const {
  // See http://stackoverflow.com/a/5838055 for details
  float t = 2.0f * (float) M_PI * NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne();
  float u = NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne() + NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne();
  float r = (u > 1.0f) ? (2.0f - u) : u;
  
  float xCoord = r * cos(t) * mRadius;
  float yCoord = r * sin(t) * mRadius;

  return mXAxis * xCoord + mYAxis * yCoord;
}