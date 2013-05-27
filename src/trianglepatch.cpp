#include "trianglepatch.h"
#include "rayintersection.h"
#include "numbergenerator.h"
#include "patchcollection.h"

TrianglePatch::TrianglePatch(const VertexPointer &vertex0, const VertexPointer &vertex1, const VertexPointer &vertex2, const MaterialPointer &material)
  : Patch(material),
    mVertex0(vertex0),
    mVertex1(vertex1),
    mVertex2(vertex2),
    mArea(0.0f) {
  // Precalculate triangle normal
  Vector e1 = vertex1->getCoordinates() - vertex0->getCoordinates();
  Vector e2 = vertex2->getCoordinates() - vertex0->getCoordinates();
  mNormal = e1.crossProduct(e2);
  mNormal.normalize();
  // Precalculate triangle area
  mArea = 0.5f * (mVertex1->getCoordinates() - mVertex0->getCoordinates()).crossProduct(mVertex2->getCoordinates() - mVertex0->getCoordinates()).length();
  // Precalculate triangle center
  mCenter = (mVertex0->getCoordinates() + mVertex1->getCoordinates() + mVertex2->getCoordinates()) * 0.333333333f;
}

TrianglePatch::~TrianglePatch() {
}

float TrianglePatch::getSize() const {
  float edgeLength1 = (mVertex1->getCoordinates() - mVertex0->getCoordinates()).length();
  float edgeLength2 = (mVertex2->getCoordinates() - mVertex0->getCoordinates()).length();
  float edgeLength3 = (mVertex2->getCoordinates() - mVertex1->getCoordinates()).length();  
  
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
/**
* Generates random point inside triangle.
* Used method described at http://www.cs.princeton.edu/~funk/tog02.pdf, section 4.2
*/
Vector TrianglePatch::getRandomPoint() const {
  // P = (1 - sqrt(r1)) * A + (sqrt(r1) * (1 - r2)) * B + (sqrt(r1) * r2) * C, where r1, r2 ~U[0, 1]
  float r1 = NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne();
  float r2 = NumberGenerator::getInstance().generateRamdomNumberBetweenZeroAndOne();
  float r1sqrt = sqrt(r1);

  return mVertex0->getCoordinates() * (1 - r1sqrt) + mVertex1->getCoordinates() * (r1sqrt * (1 - r2)) + mVertex2->getCoordinates() * (r1sqrt * r2);
}

/**
* Returns a hemisphere with such a radius that hemisphere base circle has the same area as the patch.
*/
Hemisphere TrianglePatch::getHemisphere() const {
  float radius = sqrt(mArea / M_PI);
  Vector xAxisDirection = mVertex0->getCoordinates() - mCenter;
  return Hemisphere(mCenter, radius, mNormal, xAxisDirection);
}

std::vector<VertexPointer> TrianglePatch::getVertices() const {
  std::vector<VertexPointer> vertices;
  vertices.push_back(mVertex0);
  vertices.push_back(mVertex1);
  vertices.push_back(mVertex2);
  return vertices;
}

/**
* Split patch into 4 smaller patches.
*/
PatchCollectionPointer TrianglePatch::split() const {
  Vector edgeCenter0 = (mVertex0->getCoordinates() + mVertex1->getCoordinates()) * 0.5f;
  Vector edgeCenter1 = (mVertex1->getCoordinates() + mVertex2->getCoordinates()) * 0.5f;
  Vector edgeCenter2 = (mVertex0->getCoordinates() + mVertex2->getCoordinates()) * 0.5f;

  VertexPointer edgeCenter0Vertex = VertexPointer(new Vertex(edgeCenter0));
  VertexPointer edgeCenter1Vertex = VertexPointer(new Vertex(edgeCenter1));
  VertexPointer edgeCenter2Vertex = VertexPointer(new Vertex(edgeCenter2));

  PatchCollectionPointer newPatches = PatchCollectionPointer(new PatchCollection());
  newPatches->addPatch(TrianglePatchPointer(new TrianglePatch(mVertex0, edgeCenter0Vertex, edgeCenter2Vertex, getMaterial())));
  newPatches->addPatch(TrianglePatchPointer(new TrianglePatch(mVertex1, edgeCenter1Vertex, edgeCenter0Vertex, getMaterial())));
  newPatches->addPatch(TrianglePatchPointer(new TrianglePatch(mVertex2, edgeCenter2Vertex, edgeCenter1Vertex, getMaterial())));
  newPatches->addPatch(TrianglePatchPointer(new TrianglePatch(edgeCenter0Vertex, edgeCenter1Vertex, edgeCenter2Vertex, getMaterial())));

  return newPatches;
}

RayIntersection TrianglePatch::intersectWithRay(const Ray &ray) const {
  Vector rayOrigin	= ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  Vector e1 = mVertex1->getCoordinates() - mVertex0->getCoordinates();
  Vector e2 = mVertex2->getCoordinates() - mVertex0->getCoordinates();

  Vector pvector = rayDirection.crossProduct(e2);
  float	determinant = e1.dotProduct(pvector);

  if (fabs(determinant) < FLOAT_ZERO) {
    return RayIntersection();
  }

  const float invertedDeterminant = 1.0f / determinant;

  Vector tvec	= rayOrigin - mVertex0->getCoordinates();
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
  return RayIntersection(true, pointer, f, lambda, mue);
}

Color TrianglePatch::getColor(const RayIntersection &rayIntersection) const {
  float u = rayIntersection.u;
  float v = rayIntersection.v;
  return mVertex1->getColor() * u + mVertex2->getColor() * v + mVertex0->getColor() * (1 - u - v);
}
