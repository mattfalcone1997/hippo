#pragma once

#include "GeneralPostprocessor.h"
#include "HippoInterface.h"
#include "fvCFD_moose.h"

class FoamPostprocessorBase : public GeneralPostprocessor, protected HippoInterface
{
public:
  static InputParameters validParams();

  FoamPostprocessorBase(const InputParameters & params);

  // We still want the Foam Postprocessors to be reported with the other
  // postprocessors but we want to define them empty
  void initialize() final {};

  void execute() final {};

  void finalize() final {};

  // Compute postprocessor, to be called within FoamProblem
  virtual void compute() = 0;
};
