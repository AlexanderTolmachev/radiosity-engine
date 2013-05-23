#include <cstdlib>
#include <ctime>

#include "numbergenerator.h"

NumberGenerator& NumberGenerator::getInstance() {
  static NumberGenerator generator;
  return generator;
}

NumberGenerator::NumberGenerator()
  : mCurrentId(0) {
  srand((unsigned)time(0));
}

unsigned int NumberGenerator::generateNextId() {
  return mCurrentId++;
}

float NumberGenerator::generateRamdomNumberBetweenZeroAndOne() const {
  return ((float)rand() / (float)RAND_MAX);
}