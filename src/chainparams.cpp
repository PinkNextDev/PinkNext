// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


static CBlock CreateGenesisBlock(const char* pszTimestamp, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    // [PINK] Set tx time == block time.
    txNew.nTime = nTime;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2752
    txNew.vin[0].scriptSig = CScript() << 0 << CScriptNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2753
    txNew.vout[0].SetEmpty();

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2747
    const char* pszTimestamp = "A black hole is a stable energy construct that occupies greater than three dimensions of physical space.";
    return CreateGenesisBlock(pszTimestamp, nTime, nNonce, nBits, nVersion);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#1002
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.h#L33
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.h#L37
        // [PINK] nSubsidyHalvingInterval = nHalvingPoint*YEARLY_BLOCKCOUNT
        // [PINK] YEARLY_BLOCKCOUNT = 423400 - number of blocks in a year
        consensus.nSubsidyHalvingInterval = 846800;
        // [PINK] Set to null because P2SH was always enabled for Pinkcoin
        consensus.BIP16Exception = uint256();
        // [PINK] Not activated yet so set to very high number.
        consensus.BIP34Height = 10000000;
        // [PINK] Not yet activated for Pinkcoin (set to 0)
        consensus.BIP34Hash = uint256();
        // [PINK] Not activated yet so set to very high number.
        consensus.BIP65Height = 10000000;
        // [PINK] Not activated yet so set to very high number.
        consensus.BIP66Height = 10000000;

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1176
        consensus.V2104Height = 315065;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2149
        consensus.V2104Time = 1513942698;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2241
        // [PINK] Sep 30 2018 0:00:00 (block 615543)
        consensus.V220Time = 1538265600;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L94
        // [PINK] Oct 29 2018 0:00:00 (block 646272)
        consensus.V221Time = 1540771200;

        // // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L54
        consensus.nHour1 = 15;  // 7am UTC-8
        consensus.nHour2 = 20;  // 12pm UTC-8
        consensus.nHour3 = 1;   // 5pm UTC-8
        consensus.nHour4 = 6;   // 10pm UTC-8

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L39 (40,41)
        consensus.powLimit  = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit  = uint256S("003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fposLimit = uint256S("003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1036
        consensus.nPowTargetTimespan = 60 * 60; // 60 minutes
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L47
        consensus.nPowTargetSpacing = 2 * 60;   //  2 minutes

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1037
        consensus.nPosTargetTimespan = 2 * 60 * 60; // 2 hours
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L48
        consensus.nPosTargetSpacing = 6 * 60;       // 6 minutes

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1038
        consensus.nFposTargetTimespan = 10 * 60; // 10 minutes
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L49
        consensus.nFposTargetSpacing = 1 * 60;   // 1 minute

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // [PINK] Disabled for now (is it disabled now??)
        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0;

        // [PINK] Disabled for now (is it disabled now??)
        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0;

        // The best chain should have at least this much trust.
        // [PINK] Chaintrust for block 640000 (last checkpoint)
        // [PINK] TODO: Change work to trust
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000001d04184de4fd21a7");

        // By default assume that the signatures in ancestors of this block are valid.
        // [PINK] Last checkpoint block 640000: https://github.com/Pink2Dev/Pink2/blob/master/src/checkpoints.cpp#L43
        consensus.defaultAssumeValid = uint256S("0x0000000000256123ab4a8ac67758f0a779dd31fb853f54bfc97ca6b97c653320");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L3054
        pchMessageStart[0] = 0xf2;
        pchMessageStart[1] = 0xf4;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xfb;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/protocol.h#L19
        nDefaultPort = 9134;
        nPruneAfterHeight = 500000;
        // [PINK] TODO: Check it
        m_assumed_blockchain_size = 240;
        m_assumed_chain_state_size = 3;

        // nTime, nNonce, nBits, nVersion, genesisReward
        // [PINK] nTime: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2749
        // [PINK] nNonce: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2762
        // [PINK] nBits: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2761 and Litecoin
        // [PINK] nVersion: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2759
        genesis = CreateGenesisBlock(1486329989, 6777712, 0x1e0fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.h#L43
        assert(consensus.hashGenesisBlock == uint256S("0x00000f79b700e6444665c4d090c9b8833664c4e2597c7087a6ba6391b956cc89"));
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2786
        assert(genesis.hashMerkleRoot == uint256S("0x96f872319c330aadbdc18543e27a305c6ab046801cfc81e20a004f3b26fad891"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("pinkarmy.ml");
        vSeeds.emplace_back("frankfurt.pinkarmy.ml");
        vSeeds.emplace_back("paris.pinkarmy.ml");
        vSeeds.emplace_back("singapore.pinkarmy.ml");
        vSeeds.emplace_back("sydney.pinkarmy.ml");
        vSeeds.emplace_back("tokyo.pinkarmy.ml");

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/base58.h#L276
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,3);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,28);
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/base58.h#L406
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128+3);
        // [PINK] HD wallet - how to use it for Pinkcoin? https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // [PINK] https://bitcoin.stackexchange.com/questions/70507/how-to-create-a-bech32-address-from-a-public-key
        // [PINK] It's a Segwit thing.
        bech32_hrp = "pink";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/checkpoints.cpp#L28
        checkpointData = {
            {
                { 50000, uint256S("0x000000000f794eac0e68cbd44f803cf6efb5eb31ce88444f9dd5b9f183c71b47")},
                {150000, uint256S("0x00000000016f84533d463b761dc14eb2efcc2f79ef6e799930da0880bd86bdaa")},
                {250000, uint256S("0x0000000000432587031f1a05ff8ca58a9dd2e19d34c4b473068b5a06307011c6")},
                {320000, uint256S("0x6d4459626f9f36ff37cc63cc30f10d3d5864e109554dbadce6c84b038008769e")},
                {400000, uint256S("0x6577afbb84a390974330442dc2f28959b7ec992b0717d11451f6e916eea81ab4")},
                {500000, uint256S("0x6ad56723745d551909fb772be2a23d4ad6ad5eddc93a47773f0631e3c341c936")},
                {580000, uint256S("0x0dc57093e52d8148e4da6660530e22e433dda6555d8c64bef781d92832a628a6")},
                {590000, uint256S("0x5727cf32aaba496383e1fd8fbc182eb7b2fbaa87150d2e965a2d9fee9ac7f270")},
                {600000, uint256S("0x000000000011c3a26936c1726549311b49dc1922d1ca8efeadc7133c435437da")},
                {610000, uint256S("0x0000000000090088e2b22d5ef14f2e63fb64b0ca6656a7b7d4b3e37da25f5d59")},
                {620000, uint256S("0x000000000000c4ba0b8b56e707858ec912014f7538f0fe3dd67f4ceac169fbc7")},
                {630000, uint256S("0x000000000023154bdf6cb7546ba146d9b973591cdd2e12638b711af66c004a1c")},
                {640000, uint256S("0x0000000000256123ab4a8ac67758f0a779dd31fb853f54bfc97ca6b97c653320")}
            }
        };

        // [PINK] Required for proper syncing estimations.
        // TODO: Estimate it when that wallet works and sync (using getchaintxtsats)
        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0000000000000000002e63058c023a9a1de233554f28c7b21380b6c9003f36a8
            /* nTime    */ 1540263406,
            /* nTxCount */ 704000,
            /* dTxRate  */ 0.2
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 846800;
        // [PINK] Set to null because P2SH was always enabled for Pinkcoin.
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 10000000;
        // [PINK] Not actvated on testnet
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 10000000;
        consensus.BIP66Height = 10000000;

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L42
        consensus.powLimit  = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit  = uint256S("003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fposLimit = uint256S("003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1176
        consensus.V2104Height = 315065;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2149
        consensus.V2104Time = 1513942698;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2241
        // [PINK] Sep 30 2018 0:00:00 (block 615543)
        // [PINK] TODO: Fix it for testnet (there are two times for different stuff):
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/kernel.cpp#L289
        consensus.V220Time = 1534208400;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L94
        // [PINK] Oct 29 2018 0:00:00 (block 646272)
        consensus.V221Time = 1540771200;

        // // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L54
        consensus.nHour1 = 15;  // 7am UTC-8
        consensus.nHour2 = 20;  // 12pm UTC-8
        consensus.nHour3 = 1;   // 5pm UTC-8
        consensus.nHour4 = 6;   // 10pm UTC-8

        consensus.nPowTargetTimespan = 60 * 60; // 60 minutes
        // [PINK] ?? https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L47
        consensus.nPowTargetSpacing = 2 * 60;   //  2 minutes

        // [PINK] ?? https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1037
        consensus.nPosTargetTimespan = 2 * 60 * 60; // 2 hours
        // [PINK] ?? https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L48
        consensus.nPosTargetSpacing = 6 * 60;       // 6 minutes

        // [PINK] ?? https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1038
        consensus.nFposTargetTimespan = 10 * 60; // 10 minutes
        // [PINK] ?? https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L49
        consensus.nFposTargetSpacing = 1 * 60;   // 1 minute

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        // [PINK] Not activated
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 0;

        // [PINK] Not activated
        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0;

        // [PINK] Not activated
        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0;

        // [PINK] TODO: set proper value
        // [PINK] TODO: Change work to trust
        // The best chain should have at least this much trust.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000007dbe94253893cbd463");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000037a8cd3e06cd5edbfe9dd1dbcc5dacab279376ef7cfc2b4c75"); //1354312

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/e569f7c9042b0dad13af46e9230d47835a7f1ec1/src/main.cpp#L2720
        pchMessageStart[0] = 0x02;
        pchMessageStart[1] = 0x04;
        pchMessageStart[2] = 0x05;
        pchMessageStart[3] = 0x0d;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/protocol.h#L19
        nDefaultPort = 19134;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 30;
        m_assumed_chain_state_size = 2;

        // [PINK] nTime: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2749
        // [PINK] nNonce: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2762
        // [PINK] nBits: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2761
        //               and https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2725
        // [PINK] nVersion: https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2759
        genesis = CreateGenesisBlock(1486329989, 23112, 0x1f00ffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.h#L44
        assert(consensus.hashGenesisBlock == uint256S("0x000076a007b949e5f8cdee6c18817d26bc224bfde575ce3f2ecb0dd000f7ec19"));
        // [PINK] ?? https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2786
        assert(genesis.hashMerkleRoot == uint256S("0x96f872319c330aadbdc18543e27a305c6ab046801cfc81e20a004f3b26fad891"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // vSeeds.emplace_back("testnet-seed.bitcoin.jonasschnelli.ch");
        // vSeeds.emplace_back("seed.tbtc.petertodd.org");
        // vSeeds.emplace_back("seed.testnet.bitcoin.sprovoost.nl");
        // vSeeds.emplace_back("testnet-seed.bluematt.me");

        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/base58.h#L278
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,55);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/base58.h#L406
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128+55);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tpink";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        // [PINK] No checkpoints in Testnet.
        checkpointData = {
            {
            }
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 4096 0000000000000037a8cd3e06cd5edbfe9dd1dbcc5dacab279376ef7cfc2b4c75
            /* nTime    */ 1540263406,
            /* nTxCount */ 604000,
            /* dTxRate  */ 0.2
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L1176
        consensus.V2104Height = 315065;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2149
        consensus.V2104Time = 1513942698;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L2241
        // [PINK] Sep 30 2018 0:00:00 (block 615543)
        consensus.V220Time = 1538265600;
        // [PINK] https://github.com/Pink2Dev/Pink2/blob/master/src/main.cpp#L94
        // [PINK] Oct 29 2018 0:00:00 (block 646272)
        consensus.V221Time = 1540771200;
        consensus.nHour1 = 15;  // 7am UTC-8
        consensus.nHour2 = 20;  // 12pm UTC-8
        consensus.nHour3 = 1;   // 5pm UTC-8
        consensus.nHour4 = 6;   // 10pm UTC-8
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fposLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60;     // 60 minutes
        consensus.nPowTargetSpacing = 2 * 60;       //  2 minutes
        consensus.nPosTargetTimespan = 2 * 60 * 60; // 2 hours
        consensus.nPosTargetSpacing = 6 * 60;       // 6 minutes
        consensus.nFposTargetTimespan = 10 * 60;    // 10 minutes
        consensus.nFposTargetSpacing = 1 * 60;      // 1 minute

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateVersionBitsParametersFromArgs(args);

        genesis = CreateGenesisBlock(1486329989, 6777712, 0x1e0fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000f79b700e6444665c4d090c9b8833664c4e2597c7087a6ba6391b956cc89"));
        assert(genesis.hashMerkleRoot == uint256S("0x96f872319c330aadbdc18543e27a305c6ab046801cfc81e20a004f3b26fad891"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "bcrt";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateVersionBitsParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateVersionBitsParametersFromArgs(const ArgsManager& args)
{
    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
