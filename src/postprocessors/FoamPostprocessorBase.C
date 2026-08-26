#include "FoamPostprocessorBase.h"

InputParameters
FoamPostprocessorBase::validParams()
{
  return GeneralPostprocessor::validParams();
}

FoamPostprocessorBase::FoamPostprocessorBase(const InputParameters & params)
  : GeneralPostprocessor(params), HippoInterface(this)
{
}
