#include "FoamWallAverageHeatFlux.h"

InputParameters
FoamWallAverageHeatFlux::validParams()
{
  InputParameters params = FoamWallPostprocessor::validParams();
  return params;
}

FoamWallAverageHeatFlux::FoamWallAverageHeatFlux(const InputParameters & params)
  : FoamWallPostprocessor(params)
{
}

void
FoamWallAverageHeatFlux::compute()
{
  _value = integrateField() / getArea();
}
