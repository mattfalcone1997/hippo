#include "FoamWallHeatFluxIntegral.h"

registerMooseObject("hippoApp", FoamWallHeatFluxIntegral);

InputParameters
FoamWallHeatFluxIntegral::validParams()
{
  InputParameters params = FoamWallPostprocessor::validParams();
  return params;
}

FoamWallHeatFluxIntegral::FoamWallHeatFluxIntegral(const InputParameters & params)
  : FoamWallPostprocessor(params)
{
}

void
FoamWallHeatFluxIntegral::compute()
{
  _value = integrateField();
}
