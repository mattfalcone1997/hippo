#pragma once

#include "FoamFieldBase.h"
#include "InputParameters.h"

#include "MooseTypes.h"
#include "fvCFD_moose.h"
#include <basicThermo.H>
#include <functional>
#include <fvPatchFieldsFwd.H>
#include <scalarField.H>

class FoamMultiphaseHeatTransferCoeff : public FoamFieldBase
{
public:
  static InputParameters validParams();

  explicit FoamMultiphaseHeatTransferCoeff(const InputParameters & params);

  virtual void transferVariable();

  std::string foamVariable() const { return ""; };

protected:
  Foam::scalarField calculateHTC();
  const Foam::scalarField calculate_qw();
  const Foam::scalarField calculate_temp_w();
  // Pointer to the FoamMesh object
  const SubdomainID _subdomain;
  const UserObjectName & _t_bulk_uo_name;
  std::optional<std::reference_wrapper<const Foam::phaseSystem>> _phase_system;
};
