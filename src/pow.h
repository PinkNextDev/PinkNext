// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POW_H
#define BITCOIN_POW_H

#include <consensus/params.h>

#include <stdint.h>

class CBlockHeader;
class CBlockIndex;
class uint256;

// [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L111
uint32_t GetNextTargetRequired(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, bool fProofOfStake);
uint32_t GetNextTargetRequiredV1(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, bool fProofOfStake);
uint32_t GetNextTargetRequiredV2(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, bool fProofOfStake);

// [PINK] Replace it and remove
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&);
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&);

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params&);

#endif // BITCOIN_POW_H
