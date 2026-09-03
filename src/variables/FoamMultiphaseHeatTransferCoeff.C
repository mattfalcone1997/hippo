#include "FoamFieldBase.h"
#include "FoamMultiphaseHeatTransferCoeff.h"
#include "FoamProblem.h"
#include "InputParameters.h"
#include "MooseError.h"
#include "MooseTypes.h"
#include "Registry.h"
#include "UserObject.h"
#include "hippoUtils.h"
#include <basicThermo.H>
#include <objectRegistry.H>
#include <scalar.H>
#include <scalarAndError.H>
#include <scalarField.H>
#include <string>
#include <thermophysicalTransportModel.H>
#include <vector>
#include <volFieldsFwd.H>
#include <phaseSystem.H>
#include <wallBoiling.H>
#include "fluidThermophysicalTransportModel.H"

registerMooseObject("hippoApp", FoamMultiphaseHeatTransferCoeff);

InputParameters
FoamMultiphaseHeatTransferCoeff::validParams()
{
  InputParameters params = FoamFieldBase::validParams();
  params.addRequiredParam<UserObjectName>("bulk_temperature_uo",
                                          "UserObject describing the bulk temperature");
  params.addRequiredParam<SubdomainName>("boundary", "Boundary used to calculate the HTC on.");
  params.addParam<std::string>("T_name", "T", "Temperature field name in OpenFOAM");
  params.addParam<std::string>("alpha_name", "alpha", "Volume fraction name in OpenFOAM");
  params.addParam<std::vector<std::string>>("phases", {}, "Names of phases in multiphase flow.");
  return params;
}

FoamMultiphaseHeatTransferCoeff::FoamMultiphaseHeatTransferCoeff(const InputParameters & params)
  : FoamFieldBase(params),
    _subdomain(getFoamMesh().getSubdomainID(getParam<SubdomainName>("boundary"))),
    _t_bulk_uo_name(getParam<UserObjectName>("bulk_temperature_uo")),
    _phase_system()
{
  if (!getFvMesh().foundObject<Foam::phaseSystem>(Foam::phaseSystem::propertiesName))
    mooseError("No phaseSystem found");

  _phase_system = getFvMesh().lookupObject<Foam::phaseSystem>(Foam::phaseSystem::propertiesName);
}

void
FoamMultiphaseHeatTransferCoeff::transferVariable()
{
  THREAD_ID tid = getParam<THREAD_ID>("_tid");
  auto & moose_var = getFoamProblem().getVariable(tid, _name);

  const Foam::scalarField htc = calculateHTC();
  Hippo::internal::copyFieldFoamToMoose(getFoamMesh(), htc, moose_var, _subdomain);
  moose_var.sys().solution().close();
}

Foam::scalarField
FoamMultiphaseHeatTransferCoeff::calculateHTC()
{
  const std::string & subdomain{getParam<SubdomainName>("boundary")};

  const Foam::scalarField q = calculate_qw();
  const Foam::scalarField Tw = calculate_temp_w();

  UserObject & t_bulk_uo = getFoamProblem().getUserObject<UserObject>(_t_bulk_uo_name);
  t_bulk_uo.execute();

  Foam::scalarField htc{q.size(), 0};
  const Foam::vectorField & cellCenters{getFvMesh().boundary()[subdomain].Cf()};
  const Foam::scalar eps = Foam::ROOTVSMALL;
  for (int i = 0; i < htc.size(); ++i)
  {
    const Point p{cellCenters[i].x(), cellCenters[i].y(), cellCenters[i].z()};
    const Foam::scalar T_ref = t_bulk_uo.spatialValue(p);
    htc[i] = q[i] / (Tw[i] - T_ref + eps);
  }

  return htc;
}

const Foam::scalarField
FoamMultiphaseHeatTransferCoeff::calculate_qw()
{
  const auto & patch = getFvMesh().boundary()[getParam<SubdomainName>("boundary")];

  Foam::Field<Foam::scalar> q_w(patch.size(), 0.);
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
    const auto & boiling_patch = model.mDotPf(patch.index());
    q_w += boiling_patch.property("qQuenching");
    q_w += boiling_patch.property("qEvaporative");
  }

  return q_w;
}

const Foam::scalarField
FoamMultiphaseHeatTransferCoeff::calculate_temp_w()
{
  const auto & patch = getFvMesh().boundary()[getParam<SubdomainName>("boundary")];

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
