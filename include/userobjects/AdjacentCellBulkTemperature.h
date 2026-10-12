#pragma once

#include "fvCFD_moose.h"
#include "HippoInterface.h"
#include "WallQuantitiesBase.h"

#include "GeneralUserObject.h"
#include "InputParameters.h"
#include "KDTree.h"

#include <fvPatch.H>
#include <scalarField.H>

#include <memory>

/// UserObject that provides the bulk fluid temperature for HTC calculations based on the
/// temperature of the adjacent cell to the wall.
class AdjacentCellBulkTemperature : public GeneralUserObject, public HippoInterface
{
public:
  static InputParameters validParams();
  AdjacentCellBulkTemperature(const InputParameters & params);

  void execute() override;
  void initialize() override {};
  void finalize() override {};

  Real spatialValue(const Point & point) const override;

private:
  const Foam::fvPatch & getFoamPatch(const std::string & boundary);
  void buildKDTree();

  /// Number of points on each rank for the MPI_Allgatherv call
  std::vector<int> _mpi_sizes;
  /// Rank offsets for the MPI_Allgatherv call.
  std::vector<int> _mpi_gather_offsets;
  /// Reference to the OpenFOAM patch
  const Foam::fvPatch & _foam_patch;
  /// Gathered face centres of the boundary patch across all ranks
  std::vector<Point> _kd_centres;
  /// Gather adjacent cell temperature to the boundary patch across all ranks
  Foam::scalarField _gl_t_adjacent;
  /// The KDTree used to search for the closest point
  std::unique_ptr<KDTree> _kd_tree;
  /// WallQuantities object used to calculate the temperature
  std::unique_ptr<WallQuantitiesBase> _wall_quantities;
};
