#include "FoamWallAverageHTC.h"
#include "FoamWallPostprocessor.h"
#include "InputParameters.h"
#include "MooseError.h"
#include "MooseTypes.h"
#include "UserObject.h"

registerMooseObject("hippoApp", FoamWallAverageHTC);

InputParameters
FoamWallAverageHTC::validParams()
{
  InputParameters params = FoamWallPostprocessor::validParams();
  params.addRequiredParam<std::vector<UserObjectName>>(
      "bulk_temperature_uo", "Bulk temperature user objects for each boundary");
  return params;
}

FoamWallAverageHTC::FoamWallAverageHTC(const InputParameters & params)
  : FoamWallPostprocessor(params), _t_bulk_uo_names()
{
  const auto & boundaries = getParam<std::vector<SubdomainName>>("boundary");
  const auto & t_bulks = getParam<std::vector<UserObjectName>>("bulk_temperature_uo");

  if (t_bulks.size() != boundaries.size())
    paramError("bulk_temperature_uo", "Provide one bulk temperature user object per boundary.");

  for (auto i = 0lu; i < _boundary.size(); ++i)
  {
    _t_bulk_uo_names[boundaries[i]] = t_bulks[i];
  }
}

void
FoamWallAverageHTC::compute()
{
  _value = integrateField() / getArea();
}
