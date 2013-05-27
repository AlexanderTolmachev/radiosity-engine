#include "box.h"
#include "patchcollection.h"

Box::Box(Vector center, float width, float height, float depth, 
         float rotationXAngle, float rotationYAngle, float rotationZAngle, 
         MaterialPointer material) 
  : Shape(material),
    mCenter(center), 
    mWidth(width), 
    mHeight(height), 
    mDepth(depth),
    mRotationXAngle(rotationXAngle), 
    mRotationYAngle(rotationYAngle), 
    mRotationZAngle(rotationZAngle) {
}

Box::~Box() {
}

PatchCollectionPointer Box::splitIntoPatches() const {
  PatchCollectionPointer patches = PatchCollectionPointer(new PatchCollection());

  for each (auto face in getFaces()) {
    PatchCollectionPointer facePatches = face->splitIntoPatches();
    patches->addPatches(facePatches);
  }

  return patches;
}

PatchCollectionPointer Box::splitIntoPatches(float patchSize) const {
  PatchCollectionPointer patches = PatchCollectionPointer(new PatchCollection());

  for each (auto face in getFaces()) {
    PatchCollectionPointer facePatches = face->splitIntoPatches(patchSize);
    patches->addPatches(facePatches);
  }

  return patches;
}

std::vector<ShapePointer> Box::getFaces() const {
  Vector xDirection = Vector(1.0f, 0.0f, 0.0f);
  Vector yDirection = Vector(0.0f, 1.0f, 0.0f);
  Vector zDirection = Vector(0.0f, 0.0f, 1.0f);

  float halfWidth = mWidth / 2.0f;
  float halfHeight = mHeight / 2.0f;
  float halfDepth = mDepth / 2.0f;

  Matrix rotationMatrix = Matrix::createRotationAroundAxis(mRotationXAngle, mRotationYAngle, mRotationZAngle);

  Vector vertex0 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * (-halfHeight) + zDirection * halfDepth);
  Vector vertex1 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * (-halfHeight) + zDirection * halfDepth);  
  Vector vertex2 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * halfHeight + zDirection * halfDepth);  
  Vector vertex3 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * halfHeight + zDirection * halfDepth);  
  Quad frontSurface = Quad(vertex0, vertex1, vertex2, vertex3, getMaterial());

  vertex0 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * (-halfHeight) + zDirection * halfDepth);
  vertex1 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * (-halfHeight) + zDirection * (-halfDepth));  
  vertex2 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * halfHeight + zDirection * (-halfDepth));  
  vertex3 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * halfHeight + zDirection * halfDepth);  
  Quad rightSurface = Quad(vertex0, vertex1, vertex2, vertex3, getMaterial());

  vertex0 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * (-halfHeight) + zDirection * (-halfDepth));
  vertex1 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * (-halfHeight) + zDirection * (-halfDepth));  
  vertex2 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * halfHeight + zDirection * (-halfDepth));  
  vertex3 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * halfHeight + zDirection * (-halfDepth));  
  Quad backSurface = Quad(vertex0, vertex1, vertex2, vertex3, getMaterial());

  vertex0 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * (-halfHeight) + zDirection * (-halfDepth));
  vertex1 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * (-halfHeight) + zDirection * halfDepth);  
  vertex2 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * halfHeight + zDirection * halfDepth);  
  vertex3 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * halfHeight + zDirection * (-halfDepth));  
  Quad leftSurface = Quad(vertex0, vertex1, vertex2, vertex3, getMaterial());

  vertex0 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * halfHeight + zDirection * halfDepth);
  vertex1 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * halfHeight + zDirection * (-halfDepth));  
  vertex2 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * halfHeight + zDirection * (-halfDepth));  
  vertex3 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * halfHeight + zDirection * halfDepth);  
  Quad topSurface = Quad(vertex0, vertex1, vertex2, vertex3, getMaterial());

  vertex0 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * (-halfHeight) + zDirection * halfDepth);
  vertex1 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * (-halfHeight) + zDirection * halfDepth);  
  vertex2 = mCenter + rotationMatrix * (xDirection * (-halfWidth) + yDirection * (-halfHeight) + zDirection * (-halfDepth));  
  vertex3 = mCenter + rotationMatrix * (xDirection * halfWidth + yDirection * (-halfHeight) + zDirection * (-halfDepth));  
  Quad bottomSurface = Quad(vertex0, vertex1, vertex2, vertex3, getMaterial());

  std::vector<ShapePointer> faces;
  faces.push_back(QuadPointer(new Quad(frontSurface)));
  faces.push_back(QuadPointer(new Quad(rightSurface)));
  faces.push_back(QuadPointer(new Quad(backSurface)));
  faces.push_back(QuadPointer(new Quad(leftSurface)));
  faces.push_back(QuadPointer(new Quad(bottomSurface)));
  faces.push_back(QuadPointer(new Quad(topSurface)));
  return faces;
}
