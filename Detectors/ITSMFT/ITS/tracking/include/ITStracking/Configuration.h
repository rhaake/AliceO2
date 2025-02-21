// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
/// \file Configuration.h
/// \brief
///

#ifndef TRACKINGITSU_INCLUDE_CONFIGURATION_H_
#define TRACKINGITSU_INCLUDE_CONFIGURATION_H_

#include <array>
#include <climits>
#include <vector>
#include <cmath>

#include "ITStracking/Constants.h"

namespace o2
{
namespace its
{

template <typename Param>
class Configuration : public Param
{
 public:
  static Configuration<Param>& getInstance()
  {
    static Configuration<Param> instance;
    return instance;
  }
  Configuration(const Configuration<Param>&) = delete;
  const Configuration<Param>& operator=(const Configuration<Param>&) = delete;

 private:
  Configuration() = default;
};

struct TrackingParameters {
  TrackingParameters& operator=(const TrackingParameters& t);

  int CellMinimumLevel();

  /// General parameters
  int ClusterSharing = 0;
  int MinTrackLength = 7;
  /// Trackleting cuts
  float TrackletMaxDeltaPhi = 0.3f;
  float TrackletMaxDeltaZ[constants::its::TrackletsPerRoad] = {0.1f, 0.1f, 0.3f, 0.3f, 0.3f, 0.3f};
  /// Cell finding cuts
  float CellMaxDeltaTanLambda = 0.025f;
  float CellMaxDCA[constants::its::CellsPerRoad] = {0.05f, 0.04f, 0.05f, 0.2f, 0.4f};
  float CellMaxDeltaPhi = 0.14f;
  float CellMaxDeltaZ[constants::its::CellsPerRoad] = {0.2f, 0.4f, 0.5f, 0.6f, 3.0f};
  /// Neighbour finding cuts
  float NeighbourMaxDeltaCurvature[constants::its::CellsPerRoad - 1] = {0.008f, 0.0025f, 0.003f, 0.0035f};
  float NeighbourMaxDeltaN[constants::its::CellsPerRoad - 1] = {0.002f, 0.0090f, 0.002f, 0.005f};
};

struct MemoryParameters {
  /// Memory coefficients
  MemoryParameters& operator=(const MemoryParameters& t);
  int MemoryOffset = 256;
  float CellsMemoryCoefficients[constants::its::CellsPerRoad] = {2.3208e-08f, 2.104e-08f, 1.6432e-08f, 1.2412e-08f, 1.3543e-08f};
  float TrackletsMemoryCoefficients[constants::its::TrackletsPerRoad] = {0.0016353f, 0.0013627f, 0.000984f, 0.00078135f, 0.00057934f, 0.00052217f};
};

struct IndexTableParameters {
  IndexTableParameters();
  void ComputeInverseBinSizes();
  int ZBins = 20;
  int PhiBins = 20;
  float InversePhiBinSize = 20 / constants::math::TwoPi;
  std::array<float, constants::its::LayersNumber> InverseZBinSize;
};

inline int TrackingParameters::CellMinimumLevel()
{
  return MinTrackLength - constants::its::ClustersPerCell + 1;
}

inline IndexTableParameters::IndexTableParameters()
{
  ComputeInverseBinSizes();
}

inline void IndexTableParameters::ComputeInverseBinSizes()
{
  InversePhiBinSize = PhiBins / constants::math::TwoPi;
  for (int iL = 0; iL < constants::its::LayersNumber; ++iL) {
    InverseZBinSize[iL] = 0.5f * ZBins / constants::its::LayersZCoordinate()[iL];
  }
}

inline TrackingParameters& TrackingParameters::operator=(const TrackingParameters& t)
{
  this->ClusterSharing = t.ClusterSharing;
  this->MinTrackLength = t.MinTrackLength;
  /// Trackleting cuts
  this->TrackletMaxDeltaPhi = t.TrackletMaxDeltaPhi;
  for (int iT = 0; iT < constants::its::TrackletsPerRoad; ++iT)
    this->TrackletMaxDeltaZ[iT] = t.TrackletMaxDeltaZ[iT];
  /// Cell finding cuts
  this->CellMaxDeltaTanLambda = t.CellMaxDeltaTanLambda;
  this->CellMaxDeltaPhi = t.CellMaxDeltaPhi;
  for (int iC = 0; iC < constants::its::CellsPerRoad; ++iC) {
    this->CellMaxDCA[iC] = t.CellMaxDCA[iC];
    this->CellMaxDeltaZ[iC] = t.CellMaxDeltaZ[iC];
  }
  /// Neighbour finding cuts
  for (int iC = 0; iC < constants::its::CellsPerRoad - 1; ++iC) {
    this->NeighbourMaxDeltaCurvature[iC] = t.NeighbourMaxDeltaCurvature[iC];
    this->NeighbourMaxDeltaN[iC] = t.NeighbourMaxDeltaN[iC];
  }
  return *this;
}

inline MemoryParameters& MemoryParameters::operator=(const MemoryParameters& t)
{
  this->MemoryOffset = t.MemoryOffset;
  for (int iC = 0; iC < constants::its::CellsPerRoad; ++iC)
    this->CellsMemoryCoefficients[iC] = t.CellsMemoryCoefficients[iC];
  for (int iT = 0; iT < constants::its::TrackletsPerRoad; ++iT)
    this->TrackletsMemoryCoefficients[iT] = t.TrackletsMemoryCoefficients[iT];
  return *this;
}

struct VertexingParameters {
  float zCut = 0.002f;   //0.002f
  float phiCut = 0.005f; //0.005f
  float pairCut = 0.04f;
  float clusterCut = 0.8f;
  float tanLambdaCut = 0.025f;
  int clusterContributorsCut = 16;
  int phiSpan = -1;
  int zSpan = -1;
};

} // namespace its
} // namespace o2

#endif /* TRACKINGITSU_INCLUDE_CONFIGURATION_H_ */
