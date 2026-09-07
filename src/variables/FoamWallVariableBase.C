#include "FoamFieldBase.h"
#include "FoamWallVariableBase.h"
#include "InputParameters.h"
#include "MooseTypes.h"
#include "WallQuantitiesFluid.h"
#include "WallQuantitiesMultiphaseEuler.h"
#include "FoamMesh.h"
#include "hippoUtils.h"
#include "FoamProblem.h"

InputParameters
FoamWallVariableBase::validParams()
{
  auto params = FoamFieldBase::validParams();
  MooseEnum wall_quantity("FLUID MULTIPHASE_EULER", "FLUID");
  params.addParam<MooseEnum>(
      "wall_quantity", wall_quantity, "Wall quantity based on fluid solver type");
  params.addRequiredParam<SubdomainName>("boundary", "Boundary this object mirrors.");
  return params;
}

FoamWallVariableBase::FoamWallVariableBase(const InputParameters & params)
  : FoamFieldBase(params), _wall_quantities(createWallQuantities())
{
}

std::unique_ptr<WallQuantitiesBase>
FoamWallVariableBase::createWallQuantities()
{
  const MooseEnum & wall_quantity(getParam<MooseEnum>("wall_quantity"));

  if (wall_quantity == "FLUID")
  {
    auto params = WallQuantitiesFluid::validParams();
    params.transferParam<SubdomainName>(parameters(), "boundary");
    return std::make_unique<WallQuantitiesFluid>(params);
  }
  else if (wall_quantity == "MULTIPHASE_EULER")
  {
    auto params = WallQuantitiesMultiphaseEuler::validParams();
    params.transferParam<SubdomainName>(parameters(), "boundary");
    return std::make_unique<WallQuantitiesMultiphaseEuler>(params);
  }

  mooseError("WallQuantity '", wall_quantity, "' not found.");
}

void
FoamWallVariableBase::transferVariable()
{
  THREAD_ID tid = getParam<THREAD_ID>("_tid");
  auto & moose_var = getFoamProblem().getVariable(tid, _name);

  const Foam::scalarField T_wall = _wall_quantities->wallTemperature();

  Hippo::internal::copyFieldFoamToMoose(
      getFoamMesh(),
      getFoamField(),
      moose_var,
      getFoamMesh().getSubdomainID(getParam<SubdomainName>("boundary")));

  moose_var.sys().solution().close();
}
