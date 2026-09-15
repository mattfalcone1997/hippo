#pragma once

#include "FoamWallPostprocessor.h"
#include "InputParameters.h"
#include <scalarField.H>

class FoamWallAverageTemperature : public FoamWallPostprocessor
{
public:
  static InputParameters validParams();

  explicit FoamWallAverageTemperature(const InputParameters & params);

  void compute() override;

  Foam::scalarField wallField(const std::string & boundary) override
  {
    return _wall_quantities->wallTemperature(boundary);
  };
};
