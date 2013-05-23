#pragma once

#include "types.h"

struct Hemisphere {
  Vector center;
  float radius;
  // Hemisphere internal 
  Vector xAxis;
  Vector yAxis;
  Vector zAxis; // View direction
};