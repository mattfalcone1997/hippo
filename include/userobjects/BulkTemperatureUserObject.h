#pragma once
#include "GeneralUserObject.h"

/// Base class for bulk temperature calculations for HTC.
class BulkTemperatureUserObject : public GeneralUserObject
{
public:
  static InputParameters validParams();
  BulkTemperatureUserObject(const InputParameters & params);

  void initialize() override {};
  void finalize() override {};
};
