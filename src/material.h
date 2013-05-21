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
  Color emmission;
  Color reflectance;
  Color color;
};