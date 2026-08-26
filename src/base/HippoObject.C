#include "HippoObject.h"

InputParameters
HippoObject::validParams()
{
  return MooseObject::validParams();
}

HippoObject::HippoObject(const InputParameters & params) : MooseObject(params), HippoInterface(this)
{
}
