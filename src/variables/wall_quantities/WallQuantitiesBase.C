#include "UserObject.h"
#include "WallQuantitiesBase.h"
#include "FoamProblem.h"
#include <scalarField.H>

InputParameters
WallQuantitiesBase::validParams()
{
  InputParameters params = HippoObject::validParams();
  params.addRequiredParam<SubdomainName>("boundary", "Boundary this object applies to");
  return params;
}

WallQuantitiesBase::WallQuantitiesBase(const InputParameters & params)
  : HippoObject(params), _patch(getFvMesh().boundary()[getParam<SubdomainName>("boundary")])
{
}

Foam::scalarField
WallQuantitiesBase::heatTransferCoefficient(UserObject & t_bulk_uo)
{
  const Foam::scalarField T_wall{wallTemperature()};
  const Foam::scalarField q{wallHeatFlux()};

  const Foam::vectorField & cellCenters{_patch.Cf()};
  const Foam::scalar eps = Foam::ROOTVSMALL;

  t_bulk_uo.execute();
  Foam::scalarField htc{_patch.size(), 0};
  for (int i = 0; i < htc.size(); ++i)
  {
    const Point p{cellCenters[i].x(), cellCenters[i].y(), cellCenters[i].z()};
    const Foam::scalar T_ref = t_bulk_uo.spatialValue(p);
    htc[i] = q[i] / (T_wall[i] - T_ref + eps);
  }

  return htc;
}
