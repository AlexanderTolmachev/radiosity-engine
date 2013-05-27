/*!
 *\file scene.cpp
 *\brief Contains Scene class definition
 */

#include "scene.h"

Scene::Scene()
  : mCamera(NULL) {
}

Scene::~Scene() {
}

void Scene::setCamera(const CameraPointer &camera) {
  mCamera = camera;
}

void Scene::addShape(const ShapePointer &shape) {
  mShapes.push_back(shape);
}

const CameraPointer& Scene::getCamera() const {
  return mCamera;
}

PatchCollectionPointer Scene::splitIntoPatches(float patchSize) const {
  PatchCollectionPointer scenePatches = PatchCollectionPointer(new PatchCollection());

  for each (auto shape in mShapes) {
    PatchCollectionPointer shapePatches = shape->splitIntoPatches(patchSize);
    scenePatches->addPatches(shapePatches);
  }

  return scenePatches;
}