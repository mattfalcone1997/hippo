#include "AdjacentCellBulkTemperature.h"
#include "HippoBase.h"
#include "FoamProblem.h"
#include "HippoInterface.h"
#include "MooseTypes.h"
#include "petsclog.h"
#include <ListOps.H>
#include <Pstream/mpi/PstreamGlobals.H>
#include <PstreamReduceOps.H>
#include <UList.H>
#include <VectorSpace.H>
#include <fvMesh.H>
#include <ops.H>
#include <scalar.H>
#include <scalarField.H>
#include "Pstream.H"

registerMooseObject("hippoApp", AdjacentCellBulkTemperature);

InputParameters
AdjacentCellBulkTemperature::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params.addRequiredParam<SubdomainName>("boundary", "Boundary this object applies to.");
  return params;
}

AdjacentCellBulkTemperature::AdjacentCellBulkTemperature(const InputParameters & params)
  : GeneralUserObject(params),
    HippoInterface(this),
    _foam_patch{getFoamPatch(getParam<SubdomainName>("boundary"))}
{
}

const Foam::fvPatch &
AdjacentCellBulkTemperature::getFoamPatch(const std::string & boundary)
{
  const Foam::label & boundaryId{getFvMesh().boundary().findIndex(boundary)};
  return getFvMesh().boundary()[boundaryId];
}

Real
AdjacentCellBulkTemperature::spatialValue(const Point & point) const
{
  // Boundary face centres
  const Foam::vectorField & face_centres = _foam_patch.Cf();
  Foam::point p_moose{point(0), point(1), point(2)};
  Foam::scalar minDist = Foam::rootVGreat;
  Foam::label idx;

  for (int i = 0; i < face_centres.size(); ++i)
  {
    Foam::point p = face_centres[i];
    Foam::scalar dist{Foam::magSqr(p_moose - p)};
    if (dist < minDist)
    {
      idx = _foam_patch.faceCells()[i];
      minDist = dist;
    }
  }

  return getFvMesh().lookupObject<Foam::volScalarField>("T")[idx];
}
