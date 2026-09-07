#include "FoamWallHeatFlux.h"
#include "Registry.h"
#include <memory>

registerMooseObject("hippoApp", FoamWallHeatFlux);

InputParameters
FoamWallHeatFlux::validParams()
{
  return FoamWallVariableBase::validParams();
}

FoamWallHeatFlux::FoamWallHeatFlux(const InputParameters & params) : FoamWallVariableBase(params) {}

const Foam::scalarField
FoamWallHeatFlux::getFoamField(const SubdomainName & boundary)
{
  return _wall_quantities->wallHeatFlux(boundary);
}
