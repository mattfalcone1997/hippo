#pragma once

#include "FoamFieldBase.h"
#include "MooseTypes.h"
#include "WallQuantitiesBase.h"

class FoamWallVariableBase : public FoamFieldBase
{
public:
  static InputParameters validParams();

  FoamWallVariableBase(const InputParameters & params);

  void transferVariable() override;

  std::string foamVariable() const override { return ""; };

protected:
  std::unique_ptr<WallQuantitiesBase> _wall_quantities;
  std::unique_ptr<WallQuantitiesBase> createWallQuantities();
  virtual const Foam::scalarField getFoamField() = 0;
};
