#pragma once

#include "InputParameters.h"
#include "WallQuantitiesBase.h"
#include <fvPatchFieldsFwd.H>

class WallQuantitiesFluid : public WallQuantitiesBase
{
public:
  static InputParameters validParams();
  explicit WallQuantitiesFluid(const InputParameters & params);

  virtual Foam::scalarField wallTemperature() override;
  virtual Foam::scalarField wallHeatFlux() override;
  virtual Foam::scalarField heatTransferCoefficient() override;

protected:
  const Foam::fvPatchScalarField & _boundary_temp;
};
