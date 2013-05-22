#pragma once

#include <QString>
#include <QImage>

#include "scene.h"
#include "raytracer.h"

class RadiosityEngine {
public:
  RadiosityEngine();
  virtual ~RadiosityEngine();

  void setScene(ScenePointer scene);
  void setImageResolution(int width, int height);
  void calculateIllumination(int interationsNumber, float patchSize);
  void renderScene();
  void saveRenderedImageToFile(const QString &filePath);

private:
  void initialize();

private:
  ScenePointer mScene;
  RayTracer mRayTracer;

  PatchCollectionPointer mScenePatches;
  float mTotalPatchesArea;
  Color mAvarageReflectanceProgressionSum;
  Color mAmbientIlluminationValue;
};