#include "FoamWallAverageTemperature.h"
#include "FoamWallPostprocessor.h"
#include "InputParameters.h"

InputParameters
FoamWallAverageTemperature::validParams()
{
  InputParameters params = FoamWallPostprocessor::validParams();
  return params;
}

FoamWallAverageTemperature::FoamWallAverageTemperature(const InputParameters & params)
  : FoamWallPostprocessor(params)
{
}

void
FoamWallAverageTemperature::compute()
{
  _value = integrateField() / getArea();
}
