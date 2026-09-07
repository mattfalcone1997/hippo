#include "MooseError.h"
#include "MooseTypes.h"
#include "WallQuantitiesBase.h"
#include "WallQuantitiesFluid.h"
#include <basicThermo.H>
#include <scalarField.H>
#include <volFieldsFwd.H>
#include <ThermophysicalTransportModel.H>

InputParameters
WallQuantitiesFluid::validParams()
{
  InputParameters params = WallQuantitiesBase::validParams();
  params.addRequiredParam<std::string>("T_name", "Name of sinlge phase temperature field");
  return params;
}

WallQuantitiesFluid::WallQuantitiesFluid(const InputParameters & params)
  : WallQuantitiesBase(params)
{
}

const std::string &
WallQuantitiesFluid::getTFieldName()
{
  if (getFvMesh().lookupClass<Foam::basicThermo>().size() != 1)
    mooseError("Simulation should have exactly one thermo object");

  return getFvMesh().lookupType<Foam::basicThermo>().T().name();
}

Foam::scalarField
WallQuantitiesFluid::wallTemperature(const SubdomainName & boundary)
{
  auto & boundary_temp =
      getFoamPatch(boundary).lookupPatchField<Foam::volScalarField, double>(getTFieldName());
  return boundary_temp.primitiveField();
}

Foam::scalarField
WallQuantitiesFluid::wallHeatFlux(const SubdomainName & boundary)
{
  auto & patch = getFoamPatch(boundary);
  auto & boundary_temp =
      getFoamPatch(boundary).lookupPatchField<Foam::volScalarField, double>(getTFieldName());

  Foam::Field<Foam::scalar> q_w(patch.size(), 0.);
  const Foam::thermophysicalTransportModel & ttm =
      getFvMesh().lookupType<Foam::thermophysicalTransportModel>();

  // use kappaEff as this would also account for turbulence modelling while being the same as
  // molecular in other cases
  const auto & kappaEffbf = ttm.kappaEff(patch.index());
  q_w = kappaEffbf * boundary_temp.snGrad();

  return q_w;
}
