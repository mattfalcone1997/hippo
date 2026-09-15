#include "FoamSidePostprocessor.h"
#include "InputParameters.h"
#include "MooseTypes.h"

InputParameters
FoamSidePostprocessor::validParams()
{
  auto params = FoamPostprocessorBase::validParams();
  params.addRequiredParam<std::vector<SubdomainName>>(
      "boundary", "List of boundaries where postprocessor applies.");
  return params;
}

FoamSidePostprocessor::FoamSidePostprocessor(const InputParameters & params)
  : FoamPostprocessorBase(params), _boundary(params.get<std::vector<SubdomainName>>("boundary"))
{
  for (auto & boundary : _boundary)
  {
    if (getFvMesh().boundary().findIndex(boundary) == -1)
      mooseError("Boundary '", boundary, "' not found in FoamMesh.");
  }
}

Real
FoamSidePostprocessor::getArea()
{
  Real area = 0.;
  // loop over boundary ids
  for (auto & boundary : _boundary)
  {
    auto & areas = getFvMesh().boundary()[boundary].magSf();
    for (int i = 0; i < areas.size(); ++i)
    {
      area += areas[i];
    }
  }
  // sum over ranks
  gatherSum(area);
  return area;
}
