#include "InputParameters.h"
#include "MooseTypes.h"
#include "WallQuantitiesBase.h"
#include "WallQuantitiesMultiphaseEuler.h"
#include <scalarField.H>

InputParameters
WallQuantitiesMultiphaseEuler::validParams()
{
  return WallQuantitiesBase::validParams();
}

WallQuantitiesMultiphaseEuler::WallQuantitiesMultiphaseEuler(const InputParameters & params)
  : WallQuantitiesBase(params), _phase_system()
{

  if (!getFvMesh().foundObject<Foam::phaseSystem>(Foam::phaseSystem::propertiesName))
    mooseError("No phaseSystem found");

  _phase_system = getFvMesh().lookupObject<Foam::phaseSystem>(Foam::phaseSystem::propertiesName);
}

Foam::scalarField
WallQuantitiesMultiphaseEuler::wallTemperature()
{
  Foam::Field<Foam::scalar> Tw(_patch.size(), 0.);
  for (const auto & model : _phase_system->get().phases())
  {
    const auto & Tbf =
        _patch.lookupPatchField<Foam::volScalarField, double>(model.thermo().T().name());

    const auto & alpha = model.boundaryField()[_patch.index()];
    Tw += alpha * Tbf;
  }

  return Tw;
}

Foam::scalarField
WallQuantitiesMultiphaseEuler::wallHeatFlux()
{
  Foam::Field<Foam::scalar> q_w(_patch.size(), 0.);
  for (const auto & model : _phase_system->get().phases())
  {
    const auto & kappaEffbf = model.kappaEff(_patch.index());
    const auto & Tbf =
        _patch.lookupPatchField<Foam::volScalarField, double>(model.thermo().T().name());

    const auto & alpha = model.boundaryField()[_patch.index()];
    q_w += alpha * kappaEffbf * Tbf.snGrad();
  }

  auto boiling_models = _phase_system->get().fvModels().lookupType<Foam::fv::wallBoiling>();
  for (const auto & model : boiling_models)
  {
    const auto & boiling_patch = model.mDotPf(_patch.index());
    q_w += boiling_patch.property("qQuenching");
    q_w += boiling_patch.property("qEvaporative");
  }

  return q_w;
}
