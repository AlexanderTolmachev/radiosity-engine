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
    void calculateIllumination(int interationsNumber, float patchSize, int raysNumberPerPatch);
    void renderScene();
    void saveRenderedImageToFile(const QString &filePath);

  private:
    void initialize();
    void processIteration();
    void shootRadiosity(PatchPointer sourcePatch);
    void postProcess();
  
    PatchCollectionPointer getSourcePatches() const;
    PatchesAndFactorsCollectionPointer getVisiblePatchesWithFormFactors(const PatchPointer &patch);
    PatchesAndFactorsCollectionPointer calculateVisiblePatchesWithFormFactors(const PatchPointer &patch);

  private:
    ScenePointer mScene;
    RayTracer mRayTracer;
    PatchCollectionPointer mScenePatches;

    int mRaysNumberPerPatch;
    float mTotalPatchesArea;
    Color mTotalReflectance;

    QHash<unsigned int, PatchPointer> mScenePatchesHash;
    QHash<unsigned int, PatchesAndFactorsCollectionPointer> mPatchToVisiblePatchesAndFormFactorsHash;
};
