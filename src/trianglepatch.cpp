#include "trianglepatch.h"
#include "rayintersection.h"

TrianglePatch::TrianglePatch(Vector vertex0, Vector vertex1, Vector vertex2, MaterialPointer material)
  : Patch(material),
    mVertex0(vertex0),
    mVertex1(vertex1),
    mVertex2(vertex2) {
  Vector e1 = vertex1 - vertex0;
  Vector e2 = vertex2 - vertex0;
  mNormal = e1.crossProduct(e2);
  mNormal.normalize();
}

TrianglePatch::~TrianglePatch() {
}

float TrianglePatch::getSize() const {
  // TODO: Maybe area is better?
  float edgeLength1 = (mVertex1 - mVertex0).length();
  float edgeLength2 = (mVertex2 - mVertex0).length();
  float edgeLength3 = (mVertex2 - mVertex1).length();  
  return (edgeLength1 + edgeLength2 + edgeLength3) / 3.0;
}

PatchCollectionPointer TrianglePatch::split() const {
  Vector edgeCenter0 = (mVertex0 + mVertex1) * 0.5;
  Vector edgeCenter1 = (mVertex1 + mVertex2) * 0.5;
  Vector edgeCenter2 = (mVertex0 + mVertex2) * 0.5;

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

  const float invertedDeterminant = 1.0 / determinant;

  Vector tvec	= rayOrigin - mVertex0;
  float	lambda = tvec.dotProduct(pvector);

  lambda *= invertedDeterminant;

  if (lambda < 0.0 || lambda > 1.0) {
    return RayIntersection();
  }

  Vector qvec = tvec.crossProduct(e1);
  float	mue	= rayDirection.dotProduct(qvec);

  mue *= invertedDeterminant;

  if (mue < 0.f || mue + lambda > 1.f) {
    return RayIntersection();
  }

  float f = e2.dotProduct(qvec);

  //f = f * invertedDeterminant - FLOAT_ZERO;
  //if (f < FLOAT_ZERO) {
  //  return RayIntersection();
  //}
  
  TrianglePatchPointer pointer = TrianglePatchPointer(new TrianglePatch(*this));
  return RayIntersection(true, pointer, f /*, getNormal(ray, f) */);
}

//Vector Triangle::getNormal(const Ray &ray, float distance) const {
//  return mNormal;
//}