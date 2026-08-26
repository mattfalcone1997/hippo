#pragma once

#include "InputParameters.h"
#include "MooseObject.h"
#include "fvMesh.H"

class FoamProblem;
class FoamMesh;

/*
HippoInterface is a base class containing convenient access to common
Hippo and Foam objects such as Foam::Time, Foam::fvMesh and
FoamProblem.
*/
class HippoInterface
{
public:
  explicit HippoInterface(const MooseObject * moose_object);

protected:
  // Retrieves FoamProblem
  FoamProblem & getFoamProblem() const;

  // Retrieves FoamMesh
  FoamMesh & getFoamMesh() const;

  // Retrieves Underlying OpenFOAM mesh object
  Foam::fvMesh & getFvMesh() const;

  // Retrieves OpenFOAM time object
  Foam::Time & getFoamTime() const;

private:
  FoamProblem & _foam_problem;
  static FoamProblem & extractFoamProblem(const MooseObject *);
};
