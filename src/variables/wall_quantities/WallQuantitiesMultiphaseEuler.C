#include "InputParameters.h"
#include "MooseObject.h"
#include "MooseTypes.h"
#include "WallQuantitiesMultiphaseEuler.h"
#include <scalarField.H>
#include "fvCFD_moose.h"
#include <wallBoiling.H>

WallQuantitiesMultiphaseEuler::WallQuantitiesMultiphaseEuler(const MooseObject * moose_object)
  : WallQuantitiesBase(moose_object), _phase_system()
{
  if (!getFvMesh().foundObject<Foam::phaseSystem>(Foam::phaseSystem::propertiesName))
    mooseError("No phaseSystem found");

  _phase_system = getFvMesh().lookupObject<Foam::phaseSystem>(Foam::phaseSystem::propertiesName);
}

Foam::scalarField
WallQuantitiesMultiphaseEuler::wallTemperature(const SubdomainName & boundary)
{
  auto & patch = getFoamPatch(boundary);
  Foam::Field<Foam::scalar> Tw(patch.size(), 0.);
  for (const auto & model : _phase_system->get().phases())
  {
    const auto & Tbf =
        patch.lookupPatchField<Foam::volScalarField, double>(model.thermo().T().name());

    const auto & alpha = model.boundaryField()[patch.index()];
    Tw += alpha * Tbf;
  }

  return Tw;
}

Foam::scalarField
WallQuantitiesMultiphaseEuler::wallHeatFlux(const SubdomainName & boundary)
{
  auto & patch = getFoamPatch(boundary);
  Foam::scalarField q_w(patch.size(), 0.);
  for (const auto & model : _phase_system->get().phases())
  {
    const auto & kappaEffbf = model.kappaEff(patch.index());
    const auto & Tbf =
        patch.lookupPatchField<Foam::volScalarField, double>(model.thermo().T().name());

    const auto & alpha = model.boundaryField()[patch.index()];
    q_w += alpha * kappaEffbf * Tbf.snGrad();
  }

  auto boiling_models = _phase_system->get().fvModels().lookupType<Foam::fv::wallBoiling>();
  for (const auto & model : boiling_models)
  {
    if (model.isPatchActive(patch.index()))
    {
      const auto & boiling_patch = model.mDotPf(patch.index());
      q_w += boiling_patch.property("qQuenching");
      q_w += boiling_patch.property("qEvaporative");
    }
  }

  return q_w;
}
