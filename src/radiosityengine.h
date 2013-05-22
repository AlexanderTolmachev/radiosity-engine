#pragma once

#include <QString>
#include <QImage>
#include <QHash>

#include "scene.h"
#include "raytracer.h"

typedef std::vector<std::pair<PatchPointer, float>> PatchesAndFactorsCollection;
typedef QSharedPointer<PatchesAndFactorsCollection> PatchesAndFactorsCollectionPointer;

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
  PatchesAndFactorsCollectionPointer getVisiblePatchesWithFormFactors(PatchPointer patch);
  PatchesAndFactorsCollectionPointer calculateVisiblePatchesWithFormFactors(PatchPointer patch);

private:
  ScenePointer mScene;
  RayTracer mRayTracer;

  PatchCollectionPointer mScenePatches;
  float mTotalPatchesArea;
  Color mAvarageReflectanceProgressionSum;
  Color mAmbientIlluminationValue;
  QHash<PatchPointer, PatchesAndFactorsCollectionPointer> mPatchToVisiblePatchesAndFormFactorsHash;
};

uint qHash (const PatchPointer &patch);