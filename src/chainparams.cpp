// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018 The Escrow developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

 bool CheckProof(uint256 hash, unsigned int nBits)
 {
     bool fNegative;
     bool fOverflow;
     uint256 bnTarget;


     bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

     // Check range
     if (fNegative || bnTarget == 0 || fOverflow)
         return false; //error("CheckProofOfWork() : nBits below minimum work");

     // Check proof of work matches claimed amount
     if (hash > bnTarget)
         return false; //error("CheckProofOfWork() : hash doesn't match nBits");

     return true;
 }

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x001"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1546322010, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1545653389,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1545653389,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x45;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0x67;
        vAlertPubKey = ParseHex("04b5c5074ffb74f72557ca30681ef423e160f6a509b0591ce93c2d030f3b3306437b2ec5e95270b5aa3b172d04a8932e379ff1a51d80cc45504d4800b6cba872f6");
        nDefaultPort = 12929;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Escrow starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Escrow: 1 day
        nTargetSpacing = 1 * 60;  // Escrow: 1 minute
        nMaturity = 10;
        nMasternodeCountDrift = 20;
        nMasternodeCollateralLimit = 2000;
        nMaxMoneyOut = 30000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 30;
        nModifierUpdateBlock = 2147483646;
        nZerocoinStartHeight = 500000;
        nZerocoinStartTime = 1547546400;        // 11/24/2018 @ 8:00pm (UTC)
        nBlockEnforceSerialRange = 201;         // Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1;     // Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1;             // First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1;          // Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 2147483646;  // Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN;      // Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 2147483646;          //!>The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "new genesis v3 2019";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040d47582383aa89ac28b0b974b13973e79cbcefd6abfe2a754bea4855c8a9ca85a9acd06713f103a9319e4d3bcbf3e658501c732482e52772cf5722c228838ec5") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1546322010;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 6100683;
        /*
        std::cout << "Main net" << std::endl;
        while (!CheckProof(genesis.GetHash(), genesis.nBits)) {
            genesis.nNonce ++;
        }

        std::cout << genesis.nNonce << std::endl;
        std::cout << genesis.GetHash().GetHex() << std::endl;
        std::cout << genesis.hashMerkleRoot.GetHex() << std::endl;
        */
        hashGenesisBlock =  uint256("0x0000043280d799d67e373e66466cd2e3430fde201d91e39ceba5baf20399787a");


        hashGenesisBlock = genesis.GetHash();
        //printf("%s\n", hashGenesisBlock.ToString().c_str());
        //printf("%s\n", genesis.hashMerkleRoot.ToString().c_str());
        assert(hashGenesisBlock == uint256("0x0000043280d799d67e373e66466cd2e3430fde201d91e39ceba5baf20399787a"));
        assert(genesis.hashMerkleRoot == uint256("0x14dddc020976f50e254013d373f70ef5987880858496bd3afc6100a3c61ff946"));


        //vSeeds.push_back(CDNSSeedData("Node-1", "159.89.4.8"));      // Single node address
        //vSeeds.push_back(CDNSSeedData("Node-2", "159.89.4.9"));       // Single node address
        //vSeeds.push_back(CDNSSeedData("Node-3", "159.89.4.10"));       // Single node address
        //vSeeds.push_back(CDNSSeedData("Node-4", "159.65.226.218"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 33);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 85);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 153);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;            // default true
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;          // default false
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04ce17d065aeec5072d4b3a4e8b7c83a95e65069c743d783b89acf92f39a8ab6302c15150fedd3f214bd4a89c323efc87e6a0116f142c1160fee5ca5a051e6dadd";
        strObfuscationPoolDummyAddress = "EbajAPnKBpY6iq626E2sSrCiovi9q3w351";
        nStartMasternodePayments = 1546322010 + 10000; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7;// Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT;      // high fee required for zerocoin mints
        nMintRequiredConfirmations = 20;     // the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100;         // full security level for accumulators
        nZerocoinHeaderVersion = 4;          // Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200;   // The required confirmations for a zpiv to be stakable
        nBudget_Fee_Confirmations = 6;       // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x74;
        pchMessageStart[1] = 0x75;
        pchMessageStart[2] = 0xd6;
        pchMessageStart[3] = 0xa0;
        vAlertPubKey = ParseHex("04ed93a5681596af02e438528699e7d841da484dc37ecd99123bde9b5a91ac06e6d1261fb975d357160f0d452abda7dc8b11b24e341c4994de162ba9b1b4b08272");
        nDefaultPort = 12935;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Escrow: 1 day
        nTargetSpacing = 1 * 60;  // Escrow: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nMasternodeCollateralLimit = 2000;
        nModifierUpdateBlock = 51197;
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 2147483646;
        nZerocoinStartTime = 1545653389;        // 12/31/2018 @ 12:00am (UTC)
        nBlockEnforceSerialRange = 1;           // Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1;     // Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1;             // First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1;          // Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 2147483646;  // Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN;      // Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 2147483646;          //!> The block that zerocoin v2 becomes active

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1546322010;
        genesis.nNonce = 1332963;

        /*
        std::cout << "Main test" << std::endl;
        while (!CheckProof(genesis.GetHash(), genesis.nBits)) {
            genesis.nNonce ++;
        }

        std::cout << genesis.nNonce << std::endl;
        std::cout << genesis.GetHash().GetHex() << std::endl;
        std::cout << genesis.hashMerkleRoot.GetHex() << std::endl;
        */
        hashGenesisBlock = genesis.GetHash();

        //assert(hashGenesisBlock == uint256("0x0000034cffef200ff58675a7a7efe48a5862519ce8b61f5ab435a45be97a3070"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet escrow addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet escrow script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet escrow BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet escrow BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet escrow BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0462f9e4c0d0a45a22f9eec7ccf48d374160dcedcceb160f9450c01e87725f73ecee5f857682bec1aed688132fb0c7a7531a9add30614d92f679cb4994c0c9cece";
        strObfuscationPoolDummyAddress = "yDoyykG4KVZi3VrHJoVTCZ8JEnJQcRqt6u";
        nStartMasternodePayments = 1545653389; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xf4;
        pchMessageStart[1] = 0xf6;
        pchMessageStart[2] = 0x40;
        pchMessageStart[3] = 0x57;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Escrow: 1 day
        nTargetSpacing = 1 * 60;        // Escrow: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1546322010;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 4;
        /*
        std::cout << "Main regnet" << std::endl;
        while (!CheckProof(genesis.GetHash(), genesis.nBits)) {
            genesis.nNonce ++;
        }

        std::cout << genesis.nNonce << std::endl;
        std::cout << genesis.GetHash().GetHex() << std::endl;
        std::cout << genesis.hashMerkleRoot.GetHex() << std::endl;
        */
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 12936;

        //assert(hashGenesisBlock == uint256("0x6ad9b6f4971d200e30833e55bb1b53ad0be72fd8f5590935494429c044b307eb"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 12937;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
