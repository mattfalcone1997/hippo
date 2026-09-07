#include "FoamWallTemperature.h"

registerMooseObject("hippoApp", FoamWallTemperature);

InputParameters
FoamWallTemperature::validParams()
{
  return FoamWallVariableBase::validParams();
}

FoamWallTemperature::FoamWallTemperature(const InputParameters & params)
  : FoamWallVariableBase(params)
{
}

const Foam::scalarField
FoamWallTemperature::getFoamField()
{
  return _wall_quantities->wallTemperature();
}
