#include "WallQuantitiesBase.h"
#include "FoamProblem.h"

InputParameters
WallQuantitiesBase::validParams()
{
  InputParameters params = HippoObject::validParams();
  params.addRequiredParam<SubdomainName>("boundary", "Boundary this object applies to");
  params.addRequiredParam<UserObjectName>("T_bulk_uo", "Name of bulk temperature user object");
  return params;
}

WallQuantitiesBase::WallQuantitiesBase(const InputParameters & params)
  : HippoObject(params),
    _patch(getFvMesh().boundary()[getParam<SubdomainName>("boundary")]),
    _t_bulk_uo(getFoamProblem().getUserObject<UserObject>(getParam<UserObjectName>("T_bulk_uo")))
{
}
