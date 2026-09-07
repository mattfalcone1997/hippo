#pragma once

#include "HippoObject.h"
#include "InputParameters.h"
#include "MooseTypes.h"
#include <functional>
#include <fvPatch.H>
#include <fvPatchFieldsFwd.H>
#include <scalarField.H>
#include "UserObject.h"

class WallQuantitiesBase : protected HippoObject
{
public:
  static InputParameters validParams();
  explicit WallQuantitiesBase(const InputParameters & params);
  virtual Foam::scalarField wallTemperature() = 0;
  virtual Foam::scalarField wallHeatFlux() = 0;
  virtual Foam::scalarField heatTransferCoefficient(UserObject & t_bulk_uo);

protected:
  const Foam::fvPatch & _patch;
};
