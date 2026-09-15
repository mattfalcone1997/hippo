#pragma once

#include "FoamSidePostprocessor.h"
#include "InputParameters.h"
#include "WallQuantitiesBase.h"
#include <memory>
#include <scalarField.H>

class FoamWallPostprocessor : public FoamSidePostprocessor
{
public:
  static InputParameters validParams();

  explicit FoamWallPostprocessor(const InputParameters & params);

  virtual PostprocessorValue getValue() const override;

protected:
  Real integrateField();

  virtual Foam::scalarField wallField(const std::string & boundary) = 0;

  std::unique_ptr<WallQuantitiesBase> _wall_quantities;

  Real _value;
};
