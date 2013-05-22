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
  void processIteration();
  void shootRadiosity(PatchPointer sourcePatch);
  
  PatchCollectionPointer getSourcePatches() const;
  PatchCollectionPointer getVisiblePatches(PatchPointer patch) const;
  float getFormFactor(PatchPointer sourcePatch, PatchPointer visiblePatch) const;

private:
  ScenePointer mScene;
  RayTracer mRayTracer;

  PatchCollectionPointer mScenePatches;
  float mTotalPatchesArea;
  Color mAvarageReflectanceProgressionSum;
  Color mAmbientIlluminationValue;
};