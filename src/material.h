/*!
 *\file material.h
 *\brief Contains Material structure declaration
 */

#pragma once

#include <QSharedPointer>

#include "types.h"

struct Material;

typedef QSharedPointer<Material> MaterialPointer;

struct Material { 
  Color emmissionColor;
  Color reflectanceColor;
  float emissionFactor;
  float reflectanceFactor;
};