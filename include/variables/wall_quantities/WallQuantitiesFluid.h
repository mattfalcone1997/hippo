#pragma once

#include "InputParameters.h"
#include "MooseTypes.h"
#include "WallQuantitiesBase.h"
#include <fvPatchFieldsFwd.H>

class WallQuantitiesFluid : public WallQuantitiesBase
{
public:
  static InputParameters validParams();
  explicit WallQuantitiesFluid(const InputParameters & params);

  virtual Foam::scalarField wallTemperature(const SubdomainName & boundary) override;
  virtual Foam::scalarField wallHeatFlux(const SubdomainName & boundary) override;

protected:
  const std::string & getTFieldName();
};
