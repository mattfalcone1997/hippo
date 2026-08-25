#pragma once

#include "InputParameters.h"
#include "MooseObject.h"
#include "HippoInterface.h"

/*
HippoObject is a base class for MooseObjects accessing OpenFOAM
objects
*/
class HippoObject : public MooseObject, protected HippoInterface
{
public:
  static InputParameters validParams();
  HippoObject(const InputParameters & params);
};
