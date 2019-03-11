// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_PRIMITIVES_BLOCK_H
#define BITCOIN_PRIMITIVES_BLOCK_H

#include <primitives/transaction.h>
#include <serialize.h>
#include <uint256.h>

/** Nodes collect new transactions into a block, hash them into a hash tree,
 * and scan through nonce values to make the block's hash satisfy proof-of-work
 * requirements.  When they solve the proof-of-work, they broadcast the block
 * to everyone and the block is added to the block chain.  The first transaction
 * in the block is a special one that creates a new coin owned by the creator
 * of the block.
 */
class CBlockHeader
{
public:
    // header
    int32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;

    CBlockHeader()
    {
        SetNull();
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(this->nVersion);
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);
    }

    void SetNull()
    {
        nVersion = 0;
        hashPrevBlock.SetNull();
        hashMerkleRoot.SetNull();
        nTime = 0;
        nBits = 0;
        nNonce = 0;
    }

    bool IsNull() const
    {
        return (nBits == 0);
    }

    uint256 GetHash() const;

    uint256 GetPoWHash() const;

    int64_t GetBlockTime() const
    {
        return (int64_t)nTime;
    }

    uint64_t GetStakeEntropyBit() const;

    const char* begin() const
    {
        return ((char*)&(nVersion));
    }
};


class CBlock : public CBlockHeader
{
public:
    // network and disk
    std::vector<CTransactionRef> vtx;

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L853
    // [PINK] block signature - signed by coin base txout[0]'s owner
    std::vector<unsigned char> vchBlockSig;

    // memory only
    mutable bool fChecked;

    CBlock()
    {
        SetNull();
    }

    CBlock(const CBlockHeader &header)
    {
        SetNull();
        *(static_cast<CBlockHeader*>(this)) = header;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITEAS(CBlockHeader, *this);
        READWRITE(vtx);
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L881
        READWRITE(vchBlockSig);
    }

    void SetNull()
    {
        CBlockHeader::SetNull();
        vtx.clear();
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L899
        vchBlockSig.clear();
        fChecked = false;
    }

    CBlockHeader GetBlockHeader() const
    {
        CBlockHeader block;
        block.nVersion       = nVersion;
        block.hashPrevBlock  = hashPrevBlock;
        block.hashMerkleRoot = hashMerkleRoot;
        block.nTime          = nTime;
        block.nBits          = nBits;
        block.nNonce         = nNonce;
        return block;
    }

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L938
    bool IsProofOfStake() const
    {
        return (vtx.size() > 1 && vtx[1]->IsCoinStake());
    }

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L943
    bool IsProofOfWork() const
    {
        return !IsProofOfStake();
    }

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.h#L948
    std::pair<COutPoint, uint32_t> GetProofOfStake() const
    {
        return IsProofOfStake() ? std::make_pair(vtx[1]->vin[0].prevout, vtx[1]->nTime) : std::make_pair(COutPoint(), (uint32_t)0);
    }

    // [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/main.h#L954
    // [PINK] TODO: Check if it requires int64_t or we can use uint32_t
    int64_t GetMaxTransactionTime() const
    {
        int64_t maxTransactionTime = 0;
        for (const CTransactionRef& tx : vtx)
            maxTransactionTime = std::max(maxTransactionTime, (int64_t)tx->nTime);
        return maxTransactionTime;
    }

    std::string ToString() const;
};

/** Describes a place in the block chain to another node such that if the
 * other node doesn't have the same branch, it can find a recent common trunk.
 * The further back it is, the further before the fork it may be.
 */
struct CBlockLocator
{
    std::vector<uint256> vHave;

    CBlockLocator() {}

    explicit CBlockLocator(const std::vector<uint256>& vHaveIn) : vHave(vHaveIn) {}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        int nVersion = s.GetVersion();
        if (!(s.GetType() & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(vHave);
    }

    void SetNull()
    {
        vHave.clear();
    }

    bool IsNull() const
    {
        return vHave.empty();
    }
};

#endif // BITCOIN_PRIMITIVES_BLOCK_H
