// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file DigitReader.cxx
/// \brief Implementation of EMCAL digit reader

#include "EMCALReconstruction/DigitReader.h"
#include "FairLogger.h" // for LOG

using namespace o2::Emcal;
using o2::emcal::Digit;


//______________________________________________________________________________
void DigitPixelReader::openInput(const std::string inpName)
{
  // open input file, load digits, MC labels
  assert(det.getID() == o2::detectors::DetID::ITS || det.getID() == o2::detectors::DetID::MFT);

  clear();
  std::string detName = det.getName();

  if (!(mInputTree = o2::utils::RootChain::load("o2sim", inpName))) {
    LOG(FATAL) << "Failed to load Digits tree from " << inpName << FairLogger::endl;
  }
  mInputTree->SetBranchAddress((detName + "Digit").c_str(), &mDigitsSelf);
  if (!mDigitsSelf) {
    LOG(FATAL) << "Failed to find " << (detName + "Digit").c_str() << " branch in the " << mInputTree->GetName()
               << " from file " << inpName << FairLogger::endl;
  }
  setDigits(mDigitsSelf);

  if (!(mInputTreeROF = o2::utils::RootChain::load((detName + "DigitROF").c_str(), inpName))) {
    LOG(FATAL) << "Failed to load ROF records tree from " << inpName << FairLogger::endl;
  }
  mInputTreeROF->SetBranchAddress((detName + "DigitROF").c_str(), &mROFRecVecSelf);
  if (!mROFRecVecSelf) {
    LOG(FATAL) << "Failed to find " << (detName + "DigitROF").c_str() << " branch in the " << mInputTree->GetName()
               << " from file " << inpName << FairLogger::endl;
  }
  setROFRecords(mROFRecVecSelf);

  if (!(mInputTreeMC2ROF = o2::utils::RootChain::load((detName + "DigitMC2ROF").c_str(), inpName))) {
    LOG(FATAL) << "Failed to load MC2ROF records tree from " << inpName << FairLogger::endl;
  }
  mInputTreeMC2ROF->SetBranchAddress((detName + "DigitMC2ROF").c_str(), &mMC2ROFRecVecSelf);
  if (!mMC2ROFRecVecSelf) {
    LOG(FATAL) << "Failed to find " << (detName + "DigitMC2ROF").c_str() << " branch in the " << mInputTree->GetName()
               << " from file " << inpName << FairLogger::endl;
  }
  setMC2ROFRecords(mMC2ROFRecVecSelf);

  mInputTree->SetBranchAddress((detName + "DigitMCTruth").data(), &mDigitsMCTruthSelf);
  setDigitsMCTruth(mDigitsMCTruthSelf);
}

//______________________________________________________________________________
bool DigitPixelReader::readNextEntry()
{
  // load next entry from the self-managed input
  auto nev = mInputTree->GetEntries();
  if (mInputTreeROF->GetEntries() != nev || nev != 1) {
    LOG(FATAL) << "In the self-managed mode the Digits and ROFRecords trees must have 1 entry only";
  }
  auto evID = mInputTree->GetReadEntry();
  if (evID < -1)
    evID = -1;
  if (++evID < nev) {
    init();
    mInputTree->GetEntry(evID);
    mInputTreeROF->GetEntry(evID);
    if (evID == 0) {
      mInputTreeMC2ROF->GetEntry(0); // onle one entry is expected
    }
    return true;
  } else {
    return false;
  }
}

//______________________________________________________________________________
void DigitPixelReader::clear()
{
  // clear data structures
  mInputTree.reset();
  delete mDigitsSelf;
  delete mDigitsMCTruthSelf;
  mDigitsMCTruthSelf = nullptr;
  mDigitsSelf = nullptr;
}

