#pragma once

#include "WallQuantitiesBase.h"
#include <functional>
#include <fvPatchFieldsFwd.H>
#include <scalarField.H>
#include <phaseSystem.H>

class WallQuantitiesMultiphaseEuler : public WallQuantitiesBase
{
public:
  static InputParameters validParams();
  explicit WallQuantitiesMultiphaseEuler(const InputParameters & params);

  virtual Foam::scalarField wallTemperature() override;
  virtual Foam::scalarField wallHeatFlux() override;

protected:
  std::optional<std::reference_wrapper<const Foam::phaseSystem>> _phase_system;
};
