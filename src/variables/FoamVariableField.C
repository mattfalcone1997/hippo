#include "FoamFieldBase.h"
#include "FoamVariableField.h"
#include "InputParameters.h"
#include "MooseTypes.h"
#include "FoamProblem.h"

registerMooseObject("hippoApp", FoamVariableField);

InputParameters
FoamVariableField::validParams()
{
  auto params = FoamFieldBase::validParams();
  params.addRequiredParam<std::string>("foam_variable",
                                       "OpenFOAM variable or functionObject to be shadowed");

  return params;
}

FoamVariableField::FoamVariableField(const InputParameters & params)
  : FoamFieldBase(params), _foam_variable(params.get<std::string>("foam_variable"))
{
}

void
FoamVariableField::transferVariable()
{
  THREAD_ID tid = parameters().get<THREAD_ID>("_tid");
  auto & moose_var = getFoamProblem().getVariable(tid, _name);

  // Loop through subdomains extracting foam_variable and setting on libMesh elements
  for (auto subdomain : getFoamMesh().getSubdomainList())
  {
    size_t patch_count = getFoamMesh().getPatchCount(subdomain);
    size_t patch_offset = getFoamMesh().getPatchOffset(subdomain);

    auto & var = getFvMesh().boundary()[subdomain].lookupPatchField<Foam::volScalarField, double>(
        _foam_variable);
    for (size_t j = 0; j < patch_count; ++j)
    {
      auto elem = patch_offset + j;
      auto elem_ptr = getFoamMesh().getElemPtr(elem + getFoamMesh().rank_element_offset);
      assert(elem_ptr);
      auto dof_t = elem_ptr->dof_number(moose_var.sys().number(), moose_var.number(), 0);
      moose_var.sys().solution().set(dof_t, var[j]);
    }
  }

  moose_var.sys().solution().close();
}
