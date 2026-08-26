#pragma once

#include "HippoObject.h"

class FoamVariableField : public HippoObject
{
public:
  static InputParameters validParams();

  explicit FoamVariableField(const InputParameters & params);

  // transfer variable from OpenFOAM field to MOOSE variable
  virtual void transferVariable();

  // returns the name of the foam variable this object shadows
  std::string foamVariable() const { return _foam_variable; };

protected:
  // variable name or functionObject to be shadowed
  std::string _foam_variable;
};
