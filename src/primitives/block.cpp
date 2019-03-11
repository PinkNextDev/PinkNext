// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <arith_uint256.h>
#include <crypto/common.h>
#include <crypto/scrypt.h>

uint256 CBlockHeader::GetHash() const
{
    // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L911
    return GetPoWHash();
}

uint256 CBlockHeader::GetPoWHash() const
{
    uint256 thash;
    scrypt_1024_1_1_256(begin(), (char*)thash.begin());
    return thash;
}

// [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L928
// Entropy bit for stake modifier if chosen by modifier
uint64_t CBlockHeader::GetStakeEntropyBit() const
{
    // Take last bit of block hash as entropy bit
    uint64_t nEntropyBit = ((UintToArith256(GetHash()).GetLow64()) & (uint64_t)1);
    // [PINK] TODO: Find out how to log debug stuff conditionally ;)
    // if (fDebug && GetBoolArg("-printstakemodifier"))
    //     printf("GetStakeEntropyBit: hashBlock=%s nEntropyBit=%u\n", GetHash().ToString().c_str(), nEntropyBit);
    return nEntropyBit;
}


std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
