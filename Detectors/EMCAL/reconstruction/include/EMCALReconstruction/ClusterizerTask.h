// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file  ClusterizerTask.h
/// \brief Definition of the EMCAL clusterizer task

#ifndef ALICEO2_EMCAL_CLUSTERIZERTASK
#define ALICEO2_EMCAL_CLUSTERIZERTASK

#include "FairTask.h"
#include "DataFormatsEMCAL/Cluster.h"
#include "DataFormatsEMCAL/Digit.h"
#include "EMCALBase/Geometry.h"
#include "EMCALReconstruction/ClusterizerParameters.h"
#include "EMCALReconstruction/Clusterizer.h"

namespace o2
{

namespace emcal
{

class ClusterizerTask : public FairTask
{

 public:
  ClusterizerTask(ClusterizerParameters* parameters);
  ~ClusterizerTask() = default;

  InitStatus Init() override;
  void Exec(Option_t* option) override;
  void setGeometry(Geometry* geometry) { mGeometry = geometry; }
  Geometry* getGeometry() { return mGeometry; }
  
 private:
  Clusterizer                 mClusterizer;                           ///< Clusterizer
  Geometry*                   mGeometry = nullptr;                    ///< Pointer to geometry object
  const std::vector<Digit>*   mDigitArray = nullptr;                  ///< Array of input digits
  const std::vector<Cluster>*       mClustersArray = nullptr;               ///< Array of clusters
  const std::vector<Short_t>*       mClustersDigitIndices = nullptr;        ///< Array of digit indices
  ClassDefOverride(ClusterizerTask, 1)
};
} // namespace emcal
} // namespace o2

#endif /* ALICEO2_EMCAL_CLUSTERIZERTASK */
