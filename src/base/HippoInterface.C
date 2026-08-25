#include "FoamProblem.h"
#include "HippoInterface.h"
#include "InputParameters.h"
#include "MooseObject.h"
#include "OutputInterface.h"

HippoInterface::HippoInterface(const MooseObject * moose_object)
  : _foam_problem(extractFoamProblem(moose_object))
{
}

FoamProblem &
HippoInterface::getFoamProblem() const
{
  return _foam_problem;
}

FoamMesh &
HippoInterface::getFoamMesh() const
{
  return _foam_problem.mesh();
}

Foam::fvMesh &
HippoInterface::getFvMesh() const
{
  return getFoamMesh().fvMesh();
}

Foam::Time &
HippoInterface::getFoamTime() const
{
  return const_cast<Foam::Time &>(getFvMesh().time());
}

FoamProblem &
HippoInterface::extractFoamProblem(const MooseObject * moose_object)
{
  const InputParameters & params = moose_object->parameters();
  auto * problem = params.getCheckedPointerParam<FEProblemBase *>("_fe_problem_base");
  auto * foam_problem = dynamic_cast<FoamProblem *>(problem);
  if (!foam_problem)
    mooseError(moose_object->type(), " can only be used with FoamProblem");
  return *foam_problem;
}
