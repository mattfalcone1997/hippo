#pragma once

#include "FoamWallPostprocessor.h"
#include "InputParameters.h"
#include <scalarField.H>

class FoamWallAverageHeatFlux : public FoamWallPostprocessor
{
public:
  static InputParameters validParams();

  FoamWallAverageHeatFlux(const InputParameters & params);

  void compute() override;

protected:
  Foam::scalarField wallField(const std::string & boundary) override
  {
    return _wall_quantities->wallHeatFlux(boundary);
  }
};
