#pragma once

#include "Coupleable.h"
#include "InputParameters.h"
#include "MooseObject.h"
#include "MooseTypes.h"
#include "FoamMesh.h"
#include "VariadicTable.h"

typedef VariadicTable<std::string, std::string, std::string, std::string, std::string> BCInfoTable;

template <typename StrType>
inline std::string
listFromVector(std::vector<StrType> vec, StrType sep = ", ")
{
  if (vec.size() == 0)
    return std::string();
  else if (vec.size() == 1)
    return vec.at(0);

  std::string str;
  auto binary_op = [&](const std::string & acc, const std::string & it) { return acc + sep + it; };
  std::accumulate(vec.begin(), vec.end(), str, binary_op);
  return str;
}

class FoamBCBase : public MooseObject, public Coupleable
{
public:
  static InputParameters validParams();

  explicit FoamBCBase(const InputParameters & params);

  // returns foam variable BC applies to
  std::string foamVariable() const { return _foam_variable; };

  virtual void imposeBoundaryCondition() = 0;

  // returns foam variable BC applies to
  std::vector<SubdomainName> boundary() const { return _boundary; };

  virtual void initialSetup() = 0;

  virtual void addInfoRow(BCInfoTable table) = 0;

protected:
  // OpenFOAM variable which this BC is to be imposed on
  std::string _foam_variable;

  // Pointer to the FoamMesh object
  FoamMesh * _mesh;

  // Boundaries that this object applies to
  // TODO: Replace with inherited from BoundaryRestricted once FoamMesh is updated
  std::vector<SubdomainName> _boundary;
};
