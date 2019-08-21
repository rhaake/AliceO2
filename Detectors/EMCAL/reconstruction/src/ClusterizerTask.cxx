// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file  ClusterizerTask.cxx
/// \brief Implementation of the EMCAL cluster finder task

#include "FairLogger.h"      // for LOG
#include "FairRootManager.h" // for FairRootManager
#include "EMCALReconstruction/ClusterizerTask.h"

ClassImp(o2::emcal::ClusterizerTask);

using namespace o2::emcal;

//_____________________________________________________________________
ClusterizerTask::ClusterizerTask(ClusterizerParameters* parameters) : FairTask("EMCALClusterizerTask"), mClusterizer(parameters->getTimeCut(), parameters->getTimeMin(), parameters->getTimeMax(), parameters->getGradientCut(), parameters->getDoEnergyGradientCut(), parameters->getThresholdSeedEnergy(), parameters->getThresholdCellEnergy())
{
}

//_____________________________________________________________________
/// \brief Init function
/// Inititializes the Clusterizer and connects input and output container
InitStatus ClusterizerTask::Init()
{
  FairRootManager* mgr = FairRootManager::Instance();
  if (!mgr) {
    LOG(ERROR) << "Could not instantiate FairRootManager. Exiting ...";
    return kERROR;
  }

  // Register input container for EMCAL digits
  mDigitArray = mgr->InitObjectAs<const std::vector<o2::emcal::Digit>*>("EMCALDigit");
  if (!mDigitArray) {
    LOG(ERROR) << "EMCAL digits not registered in the FairRootManager. Exiting ...";
    return kERROR;
  }

  // Register output containers
  mgr->RegisterAny("EMCALCluster", mClustersArray, kTRUE);
  mgr->RegisterAny("EMCALClusterDigitIndices", mClustersDigitIndices, kTRUE);

  return kSUCCESS;
}

//_____________________________________________________________________
void ClusterizerTask::Exec(Option_t* option)
{
  LOG(DEBUG) << "Running clusterization on new event";

  // Get default geometry object if not yet set
  if (!mGeometry) {
    mGeometry = Geometry::GetInstanceFromRunNumber(223409); // NOTE: Hardcoded for run II run
  }
  if (!mGeometry)
    LOG(ERROR) << "Failure accessing geometry";

  // Set geometry object in clusterizer
  if (!mClusterizer.getGeometry()) {
    mClusterizer.setGeometry(mGeometry);
  }
  if (!mClusterizer.getGeometry())
    LOG(ERROR) << "Could not set geometry in clusterizer";

  // Find clusters on digits given in mDigitArray (pass by ref)
  mClusterizer.findClusters(*mDigitArray);

  // Get found clusters + digit indices for output
  mClustersArray = mClusterizer.getFoundClusters();
  mClustersDigitIndices = mClusterizer.getFoundClustersDigitIndices();
}
