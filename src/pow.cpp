// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>


// [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.cpp#L1114
// [PINK] TODO: change it to support initial headers check?
uint32_t GetNextTargetRequired(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, bool fProofOfStake)
{
    if (pindexLast->nHeight < 817990) {
        return GetNextTargetRequiredV1(pindexLast, pblock, params, fProofOfStake);
    }
    // Fork fixing nActualSpacing calculations for PoS/FPoS blocks
    // (makes PoS and FPoS next target calculations completelly independent).
    return GetNextTargetRequiredV2(pindexLast, pblock, params, fProofOfStake);
}

uint32_t GetNextTargetRequiredV1(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, bool fProofOfStake)
{
    assert(pindexLast != nullptr);

    arith_uint256 bnStakeTarget = UintToArith256(params.posLimit);

    if (params.IsFlashStake(pblock->nTime))
        bnStakeTarget = UintToArith256(params.fposLimit);


    arith_uint256 bnTargetLimit = fProofOfStake ? bnStakeTarget :  UintToArith256(params.powLimit);

    const CBlockIndex* pindexPrev = GetLastBlockIndex(pindexLast, fProofOfStake);
    if (pindexPrev->pprev == nullptr)
        return bnTargetLimit.GetCompact(); // first block
    const CBlockIndex* pindexPrevPrev = GetLastBlockIndex(pindexPrev->pprev, fProofOfStake);
    if (pindexPrevPrev->pprev == nullptr)
        return bnTargetLimit.GetCompact(); // second block


    bool fFlashStake = false;
    bool fFlashFlip = false;

    int nTS;

    if (fProofOfStake)
    {
        if (params.IsFlashStake(pblock->nTime))
        {
            fFlashStake = true;
            nTS = params.nFposTargetTimespan;
            if (!params.IsFlashStake(pindexPrev->nTime))
                fFlashFlip = true;
        } else {
            nTS = params.nPosTargetTimespan;
            if (params.IsFlashStake(pindexPrev->nTime))
                fFlashFlip = true;
        }
    }
    else {
        nTS = params.nPowTargetSpacing;
    }


    // [PINK] Target change every block
    // [PINK] Retarget with exponential moving toward target spacing
    arith_uint256 bnNew;
    int64_t nInterval;
    int64_t nActualSpacing;

    if (fFlashFlip)
    {
        // Replaces old version of nActualSpacing calculation for flip
        // blocks as it was discovered to always resolve to 0.
        const CBlockIndex* pPrev = GetLastBlockIndex2(pindexPrev, fFlashStake);
        if (pPrev == nullptr)
            return bnTargetLimit.GetCompact();
        nActualSpacing = 0;
        bnNew.SetCompact(pPrev->nBits);
    }
    else {
        bnNew.SetCompact(pindexPrev->nBits);
        nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();
    }

    if (pindexPrev->nHeight < params.V2104Height)
    {
        if (nActualSpacing < 0)
            nActualSpacing = nTS;
    }


    if (fFlashStake)
        nInterval = params.nFposTargetTimespan / nTS;
    else if (fProofOfStake)
        nInterval = params.nPosTargetTimespan / nTS;
    else
        nInterval = params.nPowTargetTimespan / nTS;

    bnNew *= ((nInterval - 1) * nTS + nActualSpacing + nActualSpacing);
    bnNew /= ((nInterval + 1) * nTS);

    if (bnNew <= 0 || bnNew > bnTargetLimit)
        bnNew = bnTargetLimit;

    return bnNew.GetCompact();
}

uint32_t GetNextTargetRequiredV2(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params, bool fProofOfStake)
{
    arith_uint256 bnTargetLimit;
    int64_t nActualSpacing;
    int nTS;
    arith_uint256 bnNew;

    bool fFlashStake = false;

    // Gets first previous block (PoW/PoS).
    const CBlockIndex* pindexPrev = GetLastBlockIndex(pindexLast, fProofOfStake);

    if (fProofOfStake) // PoS or FPoS block
    {
        if (params.IsFlashStake(pblock->nTime)) {
            fFlashStake = true;
            bnTargetLimit = UintToArith256(params.fposLimit);
            nTS = params.nFposTargetSpacing;
        }
        else {
            bnTargetLimit = UintToArith256(params.posLimit);
            nTS = params.nPosTargetSpacing;
        }

        // Gets two previous same algorithm (PoS or FPoS) blocks.
        const CBlockIndex* pPrevSameAlgo = GetLastBlockIndex2(pindexPrev, fFlashStake);
        const CBlockIndex* pPrevPrevSameAlgo = GetLastBlockIndex2(pPrevSameAlgo->pprev, fFlashStake);

        nActualSpacing = pPrevSameAlgo->GetBlockTime() - pPrevPrevSameAlgo->GetBlockTime();
        // Makes sure that time spacing between consecutive
        // PoS/FPoS periods is removed from nActualSpacing final value.
        // Additionally slows down target adjustment in case of very large
        // block spacing (higher than 1h) for all PoS blocks.
        nActualSpacing %= 3600;
        bnNew.SetCompact(pPrevSameAlgo->nBits);
    }
    else // PoW block
    {
        bnTargetLimit = UintToArith256(params.powLimit);
        nTS = params.nPowTargetSpacing;

        // Gets second PoW block.
        const CBlockIndex* pindexPrevPrev = GetLastBlockIndex(pindexPrev->pprev, false);

        nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();
        bnNew.SetCompact(pindexPrev->nBits);
    }


    // ppcoin: target change every block
    // ppcoin: retarget with exponential moving toward target spacing
    int64_t nInterval;

    if (fFlashStake)
        nInterval = params.nFposTargetTimespan / nTS;
    else if (fProofOfStake)
        nInterval = params.nPosTargetTimespan / nTS;
    else
        nInterval = params.nPowTargetTimespan / nTS;

    bnNew *= ((nInterval - 1) * nTS + nActualSpacing + nActualSpacing);
    bnNew /= ((nInterval + 1) * nTS);

    if (bnNew <= 0 || bnNew > bnTargetLimit)
        bnNew = bnTargetLimit;

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

// [PINK] TODO: replace it entairly
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
