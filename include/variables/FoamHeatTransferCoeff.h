#pragma once

#include "FoamWallVariableBase.h"
#include "InputParameters.h"

#include "MooseTypes.h"
#include <fvPatchFieldsFwd.H>
#include <scalarField.H>

class FoamHeatTransferCoeff : public FoamWallVariableBase
{
public:
  static InputParameters validParams();

  FoamHeatTransferCoeff(const InputParameters & params);

protected:
  virtual const Foam::scalarField getFoamField(const SubdomainName & boundary) override;
  const UserObjectName & _t_bulk_uo_name;
};
