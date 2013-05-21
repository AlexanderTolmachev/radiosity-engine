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

void Scene::setCamera(CameraPointer camera) {
  mCamera = camera;
}

void Scene::addShape(ShapePointer shape) {
  mShapes.push_back(shape);
}

CameraPointer Scene::getCamera() const {
  return mCamera;
}
