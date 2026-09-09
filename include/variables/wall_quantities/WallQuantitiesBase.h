#pragma once

#include "HippoInterface.h"
#include "InputParameters.h"
#include "MooseObject.h"
#include "MooseTypes.h"
#include <UList.H>
#include <fvPatch.H>
#include <fvPatchFieldsFwd.H>
#include <scalarField.H>
#include "UserObject.h"

class WallQuantitiesBase : protected HippoInterface
{
public:
  explicit WallQuantitiesBase(const MooseObject * moose_object);
  virtual Foam::scalarField wallTemperature(const SubdomainName & boundary) = 0;
  virtual Foam::scalarField wallHeatFlux(const SubdomainName & boundary) = 0;
  virtual Foam::scalarField internalTemperature(const Foam::labelUList & cells) = 0;
  virtual Foam::scalarField heatTransferCoefficient(const SubdomainName & boundary,
                                                    UserObject & t_bulk_uo);

protected:
  const Foam::fvPatch & getFoamPatch(const SubdomainName & boundary);
};
