#include <cstdlib>
#include <ctime>

#include "numbergenerator.h"

NumberGenerator& NumberGenerator::getInstance() {
  static NumberGenerator generator;
  return generator;
}

NumberGenerator::NumberGenerator()
  : mCurrentPatchId(0),
    mCurrentVertexId(0) {
  srand((unsigned)time(0));
}

unsigned int NumberGenerator::generateNextPatchId() {
  return mCurrentPatchId++;
}

unsigned int NumberGenerator::generateNextVertexId() {
  return mCurrentVertexId++;
}

float NumberGenerator::generateRamdomNumberBetweenZeroAndOne() const {
  return ((float)rand() / (float)RAND_MAX);
}