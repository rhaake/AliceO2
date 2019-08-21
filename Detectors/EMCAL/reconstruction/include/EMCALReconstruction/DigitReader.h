// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file DigitReader.h
/// \brief Definition of EMCAL digit reader

#ifndef ALICEO2_EMCAL_DIGITREADER_H
#define ALICEO2_EMCAL_DIGITREADER_H

#include "DataFormatsEMCAL/Digit.h"

namespace o2
{
namespace emcal
{
/// \class DigitReader
/// \brief DigitReader class for EMCAL
///
class DigitReader
{
 public:
  DataReader() = default;
  ~DataReader() = default;

  //
 protected:
  //
};

//_______________________________________________________________________

/// \class DigitDataReader
/// \brief DigitDataReader class for TOF. Feeds the MC digits to the Cluster Finder
///
class DigitDataReader : public DataReader
{
 public:
  DigitDataReader() = default;
  void setDigitArray(const std::vector<o2::tof::Digit>* a)
  {
    mDigitArray = a;
    mIdx = 0;
  }

  void init() override
  {
    mIdx = 0;
    mLastDigit = nullptr;
  }

  Bool_t getNextStripData(StripData& stripData) override;

 private:
  const std::vector<o2::tof::Digit>* mDigitArray = nullptr;
  const Digit* mLastDigit = nullptr;
  Int_t mIdx = 0;
};

//_______________________________________________________________________

/// \class RawDataReader
/// \brief RawDataReader class for TOF. Feeds raw data to the Cluster Finder
///
class RawDataReader : public DataReader
{
 public:
  Bool_t getNextStripData(StripData& stripData) override;
};

} // namespace tof
} // namespace o2

#endif /* ALICEO2_EMCAL_DIGITREADER_H */
