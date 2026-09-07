#include "MooseTypes.h"
#include "WallQuantitiesBase.h"
#include "WallQuantitiesFluid.h"
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
  : WallQuantitiesBase(params),
    _boundary_temp(
        _patch.lookupPatchField<Foam::volScalarField, double>(getParam<std::string>("T_name")))
{
}

Foam::scalarField
WallQuantitiesFluid::wallTemperature()
{
  return _boundary_temp.primitiveField();
}

Foam::scalarField
WallQuantitiesFluid::wallHeatFlux()
{
  Foam::Field<Foam::scalar> q_w(_patch.size(), 0.);
  const Foam::thermophysicalTransportModel & ttm =
      getFvMesh().lookupType<Foam::thermophysicalTransportModel>();

  // use kappaEff as this would also account for turbulence modelling while being the same as
  // molecular in other cases
  const auto & kappaEffbf = ttm.kappaEff(_patch.index());
  q_w = kappaEffbf * _boundary_temp.snGrad();

  return q_w;
}

Foam::scalarField
WallQuantitiesFluid::heatTransferCoefficient()
{
  Foam::scalarField htc{_patch.size(), 0};
  const Foam::vectorField & cellCenters{_patch.Cf()};
  const Foam::scalar eps = Foam::ROOTVSMALL;

  auto q = wallHeatFlux();
  for (int i = 0; i < htc.size(); ++i)
  {
    const Point p{cellCenters[i].x(), cellCenters[i].y(), cellCenters[i].z()};
    const Foam::scalar T_ref = _t_bulk_uo.spatialValue(p);
    htc[i] = q[i] / (_boundary_temp[i] - T_ref + eps);
  }

  return htc;
}
