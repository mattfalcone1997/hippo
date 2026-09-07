#include "UserObject.h"
#include "WallQuantitiesBase.h"
#include <fvPatch.H>
#include <scalarField.H>

InputParameters
WallQuantitiesBase::validParams()
{
  InputParameters params = HippoObject::validParams();
  return params;
}

WallQuantitiesBase::WallQuantitiesBase(const InputParameters & params) : HippoObject(params) {}

Foam::scalarField
WallQuantitiesBase::heatTransferCoefficient(const SubdomainName & boundary, UserObject & t_bulk_uo)
{
  const Foam::scalarField T_wall{wallTemperature(boundary)};
  const Foam::scalarField q{wallHeatFlux(boundary)};
  const Foam::fvPatch & patch = getFoamPatch(boundary);

  const Foam::vectorField & cellCenters{patch.Cf()};
  const Foam::scalar eps = Foam::ROOTVSMALL;

  t_bulk_uo.execute();
  Foam::scalarField htc{patch.size(), 0};
  for (int i = 0; i < htc.size(); ++i)
  {
    const Point p{cellCenters[i].x(), cellCenters[i].y(), cellCenters[i].z()};
    const Foam::scalar T_ref = t_bulk_uo.spatialValue(p);
    htc[i] = q[i] / (T_wall[i] - T_ref + eps);
  }

  return htc;
}

const Foam::fvPatch &
WallQuantitiesBase::getFoamPatch(const SubdomainName & boundary)
{
  return getFvMesh().boundary()[boundary];
}
