#pragma once

#include "FoamWallPostprocessor.h"
#include "InputParameters.h"
#include <scalarField.H>

class FoamWallHeatFluxIntegral : public FoamWallPostprocessor
{
public:
  static InputParameters validParams();

  FoamWallHeatFluxIntegral(const InputParameters & params);

  void compute() override;

protected:
  Foam::scalarField wallField(const std::string & boundary) override
  {
    return _wall_quantities->wallHeatFlux(boundary);
  }
};
