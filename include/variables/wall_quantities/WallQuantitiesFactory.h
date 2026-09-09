#pragma once

#include "MooseEnum.h"
#include "MooseObject.h"
#include "WallQuantitiesBase.h"
#include <memory>

namespace Hippo
{
namespace internal
{
inline MooseEnum
getWallQuantitiesEnum()
{
  return MooseEnum("FLUID MULTIPHASE_EULER", "FLUID");
}

std::unique_ptr<WallQuantitiesBase> createWallQuantities(const MooseObject & moose_object,
                                                         const MooseEnum & wall_quantity);
}
}
