// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>


#include <sstream>
#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

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
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "BBC NEWS 22/Jun/2018 Zimbabwe's President Mnangagwa appeals for racial unity ahead of election";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 710000;
        consensus.BIP34Hash = uint256S("fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf");
        consensus.BIP65Height = 918684; // bab3041e8977e0dc3eeff63fe707b92bde1dd449d8efafb248c27c8264cc311a
        consensus.BIP66Height = 811879; // 7aceee012833fa8952f8835d8b1b3ae233cd6ab08fdb27a771d2bd7bdc491894
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 10 * 30; // 3.5 days
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

      /*   consensus.powForkTime = POW_FORK_TIME;              // Time of PoW hash method change
        consensus.lastScryptBlock = LAST_SCRYPT_BLOCK;      // Height of last scrypt block
        consensus.powLimitSHA = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");   // Initial hash target at fork
        consensus.slowStartBlocks = 2000;                    // Scale post-fork block reward up over this many blocks
        consensus.premineAmount = 550000;                   // Premine amount (less than 1% of issued currency at fork time)
        std::vector<unsigned char> vch = ParseHex("76a914c9f3305556963e2976ccf3348b89a6cc736b6a4e88ac");
        consensus.premineOutputScript = CScript(vch.begin(), vch.end());    // Output script for premine block */


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000de1e4e93317241177b5f1d72fc151c6e76815e9b0be4961dfd309d60"); //1353397

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xd0;
        pchMessageStart[1] = 0xe4;
        pchMessageStart[2] = 0xba;
        pchMessageStart[3] = 0xc6;
        nDefaultPort = 62458;
        nPruneAfterHeight = 100000;


        genesis = CreateGenesisBlock(1532243130, 2085169741, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        LogPrintf("Genesis Block Hash : %s" , consensus.hashGenesisBlock.ToString());
        LogPrintf("merkle Hash : %s" , genesis.hashMerkleRoot.ToString());
        assert(consensus.hashGenesisBlock == uint256S("0xba4cc371a02e3f3287124788c5071930d7e20f0179ff7f72fb4192f1b51a434e"));
        assert(genesis.hashMerkleRoot == uint256S("0xa80fe9daa279f405437af5fcdc2e7ec517bb50ae600d497f26187f2fb552d4cb"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        // vSeeds.emplace_back("seed-a.FBTC.loshan.co.uk", true);
         vSeeds.emplace_back("206.81.15.128:62458", true);
        vSeeds.emplace_back("167.99.145.155:62458", true);
      //  vSeeds.emplace_back("dnsseed.FBTCtools.com", false);
        //vSeeds.emplace_back("dnsseed.FBTCpool.org", false);
        //vSeeds.emplace_back("dnsseed.koin-project.com", false);

    
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,35);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,50);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xff, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0xff, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {

            {
                {  0, uint256S("0x52e1c3b79ae576818f29b5534e9be87ed5d5fa4dc1593c3b086691cbcc7aded2")}
            /*   {  1500, uint256S("0x841a2965955dd288cfa707a755d05a54e45f8bd476835ec9af4402a2b59a2967")},
                {  4032, uint256S("0x9ce90e427198fc0ef05e5905ce3503725b80e26afd35a987965fd7e3d9cf0846")},
                {  8064, uint256S("0xeb984353fc5190f210651f150c40b8a4bab9eeeff0b729fcb3987da694430d70")},
                { 16128, uint256S("0x602edf1859b7f9a6af809f1d9b0e6cb66fdc1d4d9dcd7a4bec03e12a1ccd153d")},
                { 23420, uint256S("0xd80fdf9ca81afd0bd2b2a90ac3a9fe547da58f2530ec874e978fce0b5101b507")},
                { 50000, uint256S("0x69dc37eb029b68f075a5012dcc0419c127672adb4f3a32882b2b3e71d07a20a6")},
                { 80000, uint256S("0x4fcb7c02f676a300503f49c764a89955a8f920b46a8cbecb4867182ecdb2e90a")},
                {120000, uint256S("0xbd9d26924f05f6daa7f0155f32828ec89e8e29cee9e7121b026a7a3552ac6131")},
                {161500, uint256S("0xdbe89880474f4bb4f75c227c77ba1cdc024991123b28b8418dbbf7798471ff43")},
                {179620, uint256S("0x2ad9c65c990ac00426d18e446e0fd7be2ffa69e9a7dcb28358a50b2b78b9f709")},
                {240000, uint256S("0x7140d1c4b4c2157ca217ee7636f24c9c73db39c4590c4e6eab2e3ea1555088aa")},
                {383640, uint256S("0x2b6809f094a9215bafc65eb3f110a35127a34be94b7d0590a096c3f126c6f364")},
                {409004, uint256S("0x487518d663d9f1fa08611d9395ad74d982b667fbdc0e77e9cf39b4f1355908a3")},
                {456000, uint256S("0xbf34f71cc6366cd487930d06be22f897e34ca6a40501ac7d401be32456372004")},
                {638902, uint256S("0x15238656e8ec63d28de29a8c75fcf3a5819afc953dcd9cc45cecc53baec74f38")},
                {721000, uint256S("0x198a7b4de1df9478e2463bd99d75b714eab235a2e63e741641dc8a759a9840e5")},*/
            }
        }; 


         /* checkpointData = (CCheckpointData) {
            {
                {  1500, uint256S("0x841a2965955dd288cfa707a755d05a54e45f8bd476835ec9af4402a2b59a2967")},
                {  4032, uint256S("0x9ce90e427198fc0ef05e5905ce3503725b80e26afd35a987965fd7e3d9cf0846")},
                {  8064, uint256S("0xeb984353fc5190f210651f150c40b8a4bab9eeeff0b729fcb3987da694430d70")},
                { 16128, uint256S("0x602edf1859b7f9a6af809f1d9b0e6cb66fdc1d4d9dcd7a4bec03e12a1ccd153d")},
                { 23420, uint256S("0xd80fdf9ca81afd0bd2b2a90ac3a9fe547da58f2530ec874e978fce0b5101b507")},
                { 50000, uint256S("0x69dc37eb029b68f075a5012dcc0419c127672adb4f3a32882b2b3e71d07a20a6")},
                { 80000, uint256S("0x4fcb7c02f676a300503f49c764a89955a8f920b46a8cbecb4867182ecdb2e90a")},
                {120000, uint256S("0xbd9d26924f05f6daa7f0155f32828ec89e8e29cee9e7121b026a7a3552ac6131")},
                {161500, uint256S("0xdbe89880474f4bb4f75c227c77ba1cdc024991123b28b8418dbbf7798471ff43")},
                {179620, uint256S("0x2ad9c65c990ac00426d18e446e0fd7be2ffa69e9a7dcb28358a50b2b78b9f709")},
                {240000, uint256S("0x7140d1c4b4c2157ca217ee7636f24c9c73db39c4590c4e6eab2e3ea1555088aa")},
                {383640, uint256S("0x2b6809f094a9215bafc65eb3f110a35127a34be94b7d0590a096c3f126c6f364")},
                {409004, uint256S("0x487518d663d9f1fa08611d9395ad74d982b667fbdc0e77e9cf39b4f1355908a3")},
                {456000, uint256S("0xbf34f71cc6366cd487930d06be22f897e34ca6a40501ac7d401be32456372004")},
                {638902, uint256S("0x15238656e8ec63d28de29a8c75fcf3a5819afc953dcd9cc45cecc53baec74f38")},
                {721000, uint256S("0x198a7b4de1df9478e2463bd99d75b714eab235a2e63e741641dc8a759a9840e5")} // LitecoinCash: Premine block
            }
        }; */

        chainTxData = ChainTxData{
            // Data as of block 59c9b9d3fec105bdc716d84caa7579503d5b05b73618d0bf2d5fa639f780a011 (height 1353397).
            1529119875, // * UNIX timestamp of last known number of transactions
            0,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.0     // * estimated number of transactions per second after that timestamp
        };
    }


};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573");
        consensus.BIP65Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 30; // 3.5 days
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018



    /*   consensus.powForkTime = 1518355163;                 // Time of PoW hash method change
        consensus.lastScryptBlock = 1366830;                // Height of last scrypt block
        consensus.powLimitSHA = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");   // Initial hash target at fork
        consensus.slowStartBlocks = 400;                    // Scale post-fork block reward up over this many blocks
        consensus.premineAmount = 550000;                   // Premine amount (less than 1% of issued currency at fork time)
        std::vector<unsigned char> vch = ParseHex("76a914c9f3305556963e2976ccf3348b89a6cc736b6a4e88ac");
        consensus.premineOutputScript = CScript(vch.begin(), vch.end());        // Output script for premine block */


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x09a5ca9513d4f9cce6f0c083a897219c44aebd5c65713c6903ae558022cd5921"); //343833


        pchMessageStart[0] = 0xb6;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0xd3;
        pchMessageStart[3] = 0xcf;
        nDefaultPort = 19335;
        nPruneAfterHeight = 1000;



        genesis = CreateGenesisBlock(1529119875, 2085613887, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x9716ed9f8103813f4670addb840e69b772185171c5d19380970720ee5f47ec97"));
        assert(genesis.hashMerkleRoot == uint256S("0x60f15e0979392d65025049ae21f42521e5c53976bbc41477c00278435d1a03d0"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
          vSeeds.emplace_back("206.81.9.7:62458", true);
        vSeeds.emplace_back("206.81.14.114:62458", true);
      //  vSeeds.emplace_back("dnsseed-testnet.thrasher.io", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,50);

        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {  0, uint256S("0x52e1c3b79ae576818f29b5534e9be87ed5d5fa4dc1593c3b086691cbcc7aded2")}
             
            }
        };



        chainTxData = ChainTxData{
            // Data as of block a0afbded94d4be233e191525dc2d467af5c7eab3143c852c3cd549831022aad6 (height 343833)
            1529119875,
            0,
            0.0
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 30; // two weeks
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1520871009, 2085038854, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x52e1c3b79ae576818f29b5534e9be87ed5d5fa4dc1593c3b086691cbcc7aded2"));
        assert(genesis.hashMerkleRoot == uint256S("0x0de09575a6bd750929f83bb6a7120b9f25f897d718a18b64259c6d606dabde37"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData) {
            {
                {  0, uint256S("0x52e1c3b79ae576818f29b5534e9be87ed5d5fa4dc1593c3b086691cbcc7aded2")}
            }
        };

        chainTxData = ChainTxData{
            1520871009,
            0,
            0.0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}