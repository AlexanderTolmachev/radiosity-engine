#include "trianglepatch.h"
#include "rayintersection.h"
#include "numbergenerator.h"

TrianglePatch::TrianglePatch(Vector vertex0, Vector vertex1, Vector vertex2, MaterialPointer material)
  : Patch(material),
    mVertex0(vertex0),
    mVertex1(vertex1),
    mVertex2(vertex2),
    mArea(0.0f) {
  // Precalculate triangle normal
  Vector e1 = vertex1 - vertex0;
  Vector e2 = vertex2 - vertex0;
  mNormal = e1.crossProduct(e2);
  mNormal.normalize();
  // Precalculate triangle area
  mArea = 0.5f * (mVertex1 - mVertex0).crossProduct(mVertex2 - mVertex0).length();
  // Precalculate triangle center
  mCenter = (mVertex0 + mVertex1 + mVertex2) * 0.333333333f;
}

TrianglePatch::~TrianglePatch() {
}

float TrianglePatch::getSize() const {
  float edgeLength1 = (mVertex1 - mVertex0).length();
  float edgeLength2 = (mVertex2 - mVertex0).length();
  float edgeLength3 = (mVertex2 - mVertex1).length();  
  
  return std::max(edgeLength1, std::max(edgeLength2, edgeLength3));
  // return (edgeLength1 + edgeLength2 + edgeLength3) * 0.333333333f;
}

float TrianglePatch::getArea() const {
  return mArea;
}

Vector TrianglePatch::getCenter() const {
  return mCenter;
}

Vector TrianglePatch::getNormal() const {
  return mNormal;
}

// TODO: Not used, remove
Vector TrianglePatch::getRandomPoint() const {
  // According to http://www.cs.princeton.edu/~funk/tog02.pdf, section 4.2
  // P = (1 - sqrt(r1)) * A + (sqrt(r1) * (1 - r2)) * B + (sqrt(r1) * r2) * C, where r1, r2 ~U[0, 1]
  float r1 = NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne();
  float r2 = NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne();
  float r1sqrt = sqrt(r1);

  return mVertex0 * (1 - r1sqrt) + mVertex1 * (r1sqrt * (1 - r2)) + mVertex2 * (r1sqrt * r2);
}

// Returns a hemisphere with a such readuis, that hemisphere base circle has the same area as patch
Hemisphere TrianglePatch::getHemisphere() const {
  float radius = sqrt(mArea / M_PI);
  Vector xAxisDirection = mVertex0 - mCenter;
  return Hemisphere(mCenter, radius, mNormal, xAxisDirection);
}

PatchCollectionPointer TrianglePatch::split() const {
  Vector edgeCenter0 = (mVertex0 + mVertex1) * 0.5f;
  Vector edgeCenter1 = (mVertex1 + mVertex2) * 0.5f;
  Vector edgeCenter2 = (mVertex0 + mVertex2) * 0.5f;

  PatchCollectionPointer newPatches = PatchCollectionPointer(new PatchCollection());
  newPatches->push_back(TrianglePatchPointer(new TrianglePatch(mVertex0, edgeCenter0, edgeCenter2, getMaterial())));
  newPatches->push_back(TrianglePatchPointer(new TrianglePatch(mVertex1, edgeCenter1, edgeCenter0, getMaterial())));
  newPatches->push_back(TrianglePatchPointer(new TrianglePatch(mVertex2, edgeCenter2, edgeCenter1, getMaterial())));
  newPatches->push_back(TrianglePatchPointer(new TrianglePatch(edgeCenter0, edgeCenter1, edgeCenter2, getMaterial())));

  return newPatches;
}

RayIntersection TrianglePatch::intersectWithRay(const Ray &ray) const {
  Vector rayOrigin	= ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  Vector e1 = mVertex1 - mVertex0;
  Vector e2 = mVertex2 - mVertex0;

  Vector pvector = rayDirection.crossProduct(e2);
  float	determinant = e1.dotProduct(pvector);

  if (fabs(determinant) < FLOAT_ZERO) {
    return RayIntersection();
  }

  const float invertedDeterminant = 1.0f / determinant;

  Vector tvec	= rayOrigin - mVertex0;
  float	lambda = tvec.dotProduct(pvector);

  lambda *= invertedDeterminant;

  if (lambda < 0.0f || lambda > 1.0f) {
    return RayIntersection();
  }

  Vector qvec = tvec.crossProduct(e1);
  float	mue	= rayDirection.dotProduct(qvec);

  mue *= invertedDeterminant;

  if (mue < 0.0f || mue + lambda > 1.0f) {
    return RayIntersection();
  }

  float f = e2.dotProduct(qvec);

  f = f * invertedDeterminant - FLOAT_ZERO;
  if (f < FLOAT_ZERO) {
    return RayIntersection();
  }
  
  TrianglePatchPointer pointer = TrianglePatchPointer(new TrianglePatch(*this));
  return RayIntersection(true, pointer, f /*, getNormal(ray, f) */);
}
