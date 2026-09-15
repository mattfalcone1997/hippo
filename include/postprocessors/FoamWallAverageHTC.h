#pragma once

#include "FoamProblem.h"
#include "FoamWallPostprocessor.h"
#include "InputParameters.h"
#include "MooseTypes.h"
#include "UserObject.h"
#include <map>
#include <scalarField.H>

class FoamWallAverageHTC : public FoamWallPostprocessor
{
public:
  static InputParameters validParams();

  FoamWallAverageHTC(const InputParameters & params);

  void compute() override;

protected:
  Foam::scalarField wallField(const std::string & boundary) override
  {
    return _wall_quantities->heatTransferCoefficient(
        boundary, getFoamProblem().getUserObject<UserObject>(_t_bulk_uo_names.at(boundary)));
  }
  std::map<SubdomainName, UserObjectName> _t_bulk_uo_names;
};
