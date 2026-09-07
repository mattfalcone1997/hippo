#include "FoamHeatTransferCoeff.h"
#include "FoamProblem.h"
#include "FoamWallVariableBase.h"
#include "InputParameters.h"
#include "MooseTypes.h"
#include "Registry.h"
#include "UserObject.h"
#include <scalar.H>
#include <scalarAndError.H>
#include <scalarField.H>
#include <string>
#include <volFieldsFwd.H>

registerMooseObject("hippoApp", FoamHeatTransferCoeff);

InputParameters
FoamHeatTransferCoeff::validParams()
{
  InputParameters params = FoamWallVariableBase::validParams();
  params.addRequiredParam<UserObjectName>("bulk_temperature_uo",
                                          "UserObject describing the bulk temperature");
  return params;
}

FoamHeatTransferCoeff::FoamHeatTransferCoeff(const InputParameters & params)
  : FoamWallVariableBase(params), _t_bulk_uo_name(getParam<UserObjectName>("bulk_temperature_uo"))
{
  if (getParam<std::vector<SubdomainName>>("boundary").size() > 1)
    mooseError("For FoamHeatTransferCoeff there can only be one boundary.");
}

const Foam::scalarField
FoamHeatTransferCoeff::getFoamField(const SubdomainName & boundary)
{
  return _wall_quantities->heatTransferCoefficient(
      boundary, getFoamProblem().getUserObject<UserObject>(_t_bulk_uo_name));
}
