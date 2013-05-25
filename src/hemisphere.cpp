#include <cmath>

#include <QtDebug>

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

// Returns projection of random point on hemisphere base circle to hemisphere surface
// Needed for formfactors calculation method, described at
// Michael F. Cohen and John R. Wallace, "Radiosity and Realistic Image Synthesis", section 4.9.5.
Vector Hemisphere::getRandomCirclePointProjectedToSurface() const {
  Vector pointOnBase = getRandomPointOnBaseCircle();
  float distanceToCenter = pointOnBase.length();
  float offsetValue = sqrt(mRadius * mRadius - distanceToCenter * distanceToCenter);

  return pointOnBase + mZAxis * offsetValue + mCenter;
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
  
  float radius = mRadius - FLOAT_ZERO; // Decrease radius to avoid exceeding the bounds of circle
  float xCoord = r * cos(t) * radius;
  float yCoord = r * sin(t) * radius;

  return mXAxis * xCoord + mYAxis * yCoord;
}