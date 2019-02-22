// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_PARAMS_H
#define BITCOIN_CONSENSUS_PARAMS_H

#include <uint256.h>
#include <limits>
#include <map>
#include <string>

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    DEPLOYMENT_CSV, // Deployment of BIP68, BIP112, and BIP113.
    DEPLOYMENT_SEGWIT, // Deployment of BIP141, BIP143, and BIP147.
    // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
    MAX_VERSION_BITS_DEPLOYMENTS
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;

    /** Constant for nTimeout very far in the future. */
    static constexpr int64_t NO_TIMEOUT = std::numeric_limits<int64_t>::max();

    /** Special value for nStartTime indicating that the deployment is always active.
     *  This is useful for testing, as it means tests don't need to deal with the activation
     *  process (which takes at least 3 BIP9 intervals). Only tests that specifically test the
     *  behaviour during activation cannot use this. */
    static constexpr int64_t ALWAYS_ACTIVE = -1;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    // [PINK] nSubsidyHalvingInterval = nHalvingPoint*YEARLY_BLOCKCOUNT
    // [PINK] YEARLY_BLOCKCOUNT = 423400 - number of blocks in a year
    int nSubsidyHalvingInterval;
    /* Block hash that is excepted from BIP16 enforcement */
    uint256 BIP16Exception;
    /** Block height and hash at which BIP34 becomes active */
    int BIP34Height;
    uint256 BIP34Hash;
    /** Block height at which BIP65 becomes active */
    int BIP65Height;
    /** Block height at which BIP66 becomes active */
    int BIP66Height;

    // [PINK] Dec 2017 bug fix
    int V2104Height;
    // [PINK] Dec 2017 bug fix: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2149
    uint32_t V2104Time;
    // [PINK] FPoS 2.0: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2241
    uint32_t V220Time;
    // [PINK] 51% attack prev change: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L94
    uint32_t V221Time;
    // [PINK] PoS <-> FPoS flip period fix: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1098
    uint32_t V222Height;

    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks in a retargeting period,
     * (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];
    /** Proof of work and proof of stake parameters */
    uint256 powLimit;
    // [PINK] Added PoS nad FPoS params
    uint256 posLimit;
    uint256 fposLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan;
    // [PINK] Added PoS and FPoS params
    int64_t nPosTargetSpacing;
    int64_t nPosTargetTimespan;
    int64_t nFposTargetSpacing;
    int64_t nFposTargetTimespan;
    int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }
    uint256 nMinimumChainWork;
    uint256 defaultAssumeValid;

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L54
    uint32_t nHour1;
    uint32_t nHour2;
    uint32_t nHour3;
    uint32_t nHour4;

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1221
    bool IsFlashStake(uint32_t nTime) const
    {
        time_t rawtime;
        struct tm *ptm;

        bool bIsFlash = false;

        rawtime = nTime;
        ptm = gmtime(&rawtime);
        int nHour = ptm->tm_hour;
        if (nHour == nHour1 || nHour == nHour2 || nHour == nHour3 || nHour == nHour4) {
            bIsFlash = true;
        }

        return bIsFlash;
    }

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.h#L47
    inline int64_t PastDrift(int64_t nTime) const { return nTime - 10 * 60; }   // up to 10 minutes from the past
    inline int64_t FutureDrift(int64_t nTime) const { return nTime + 10 * 60; } // up to 10 minutes from the future
};
} // namespace Consensus

#endif // BITCOIN_CONSENSUS_PARAMS_H
