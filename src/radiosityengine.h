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
  void calculateIllumination(int interationsNumber, float patchSize, int samplePointsNumberPerPatch);
  void renderScene();
  void saveRenderedImageToFile(const QString &filePath);

private:
  void initialize();
  void processIteration();
  void shootRadiosity(PatchPointer sourcePatch);
  
  PatchCollectionPointer getSourcePatches() const;
  PatchesAndFactorsCollectionPointer getVisiblePatchesWithFormFactors(const PatchPointer &patch);
  PatchesAndFactorsCollectionPointer calculateVisiblePatchesWithFormFactors(const PatchPointer &patch);

  //bool isPatchVisibleFromSourcePatch(const PatchPointer &sourcePatch, const PatchPointer &patch) const;
  //float calculateFormFactor(const PatchPointer &sourcePatch, const PatchPointer &visiblePatch) const;

private:
  ScenePointer mScene;
  RayTracer mRayTracer;

  PatchCollectionPointer mScenePatches;
  QHash<unsigned int, PatchPointer> mScenePatchesHash;
  float mTotalPatchesArea;
  Color mAvarageReflectanceProgressionSum;
  Color mAmbientIlluminationValue;
  QHash<unsigned int, PatchesAndFactorsCollectionPointer> mPatchToVisiblePatchesAndFormFactorsHash;
  int mSamplePointsNumberPerPatch;
};
