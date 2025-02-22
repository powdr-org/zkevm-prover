

#include <string>
#include <iostream>
#include "proof2zkinStark.hpp"
using namespace std;

ordered_json proof2zkinStark(ordered_json &proof)
{
    ordered_json zkinOut = ordered_json::object();

    zkinOut["root1"] = proof["root1"];
    zkinOut["root2"] = proof["root2"];
    zkinOut["root3"] = proof["root3"];
    zkinOut["root4"] = proof["root4"];
    zkinOut["evals"] = proof["evals"];

    ordered_json friProof = proof["fri"];
    for (uint i = 1; i < friProof.size() - 1; i++)
    {
        zkinOut["s" + std::to_string(i) + "_root"] = friProof[i]["root"];
        zkinOut["s" + std::to_string(i) + "_vals"] = ordered_json::array();
        zkinOut["s" + std::to_string(i) + "_siblings"] = ordered_json::array();
        for (uint q = 0; q < friProof[0]["polQueries"].size(); q++)
        {
            zkinOut["s" + std::to_string(i) + "_vals"][q] = friProof[i]["polQueries"][q][0];
            zkinOut["s" + std::to_string(i) + "_siblings"][q] = friProof[i]["polQueries"][q][1];
        }
    }

    zkinOut["s0_vals1"] = ordered_json::array();
    if (friProof[0]["polQueries"][0][1][0].size())
    {
        zkinOut["s0_vals2"] = ordered_json::array();
    }
    if (friProof[0]["polQueries"][0][2][0].size())
    {
        zkinOut["s0_vals3"] = ordered_json::array();
    }

    zkinOut["s0_vals4"] = ordered_json::array();
    zkinOut["s0_valsC"] = ordered_json::array();
    zkinOut["s0_siblings1"] = ordered_json::array();
    if (friProof[0]["polQueries"][0][1][0].size())
    {
        zkinOut["s0_siblings2"] = ordered_json::array();
    }
    if (friProof[0]["polQueries"][0][2][0].size())
    {
        zkinOut["s0_siblings3"] = ordered_json::array();
    }
    zkinOut["s0_siblings4"] = ordered_json::array();
    zkinOut["s0_siblingsC"] = ordered_json::array();

    for (uint i = 0; i < friProof[0]["polQueries"].size(); i++)
    {

        zkinOut["s0_vals1"][i] = friProof[0]["polQueries"][i][0][0];
        zkinOut["s0_siblings1"][i] = friProof[0]["polQueries"][i][0][1];

        if (friProof[0]["polQueries"][0][1][0].size())
        {
            zkinOut["s0_vals2"][i] = friProof[0]["polQueries"][i][1][0];
            zkinOut["s0_siblings2"][i] = friProof[0]["polQueries"][i][1][1];
        }
        if (friProof[0]["polQueries"][0][2][0].size())
        {
            zkinOut["s0_vals3"][i] = friProof[0]["polQueries"][i][2][0];
            zkinOut["s0_siblings3"][i] = friProof[0]["polQueries"][i][2][1];
        }

        zkinOut["s0_vals4"][i] = friProof[0]["polQueries"][i][3][0];
        zkinOut["s0_siblings4"][i] = friProof[0]["polQueries"][i][3][1];

        zkinOut["s0_valsC"][i] = friProof[0]["polQueries"][i][4][0];
        zkinOut["s0_siblingsC"][i] = friProof[0]["polQueries"][i][4][1];
    }

    zkinOut["finalPol"] = friProof[friProof.size() - 1];

    return zkinOut;
};

ordered_json joinzkinBatchRecursive2(ordered_json &zkin1, ordered_json &zkin2, ordered_json &verKey, uint64_t steps)
{
    ordered_json zkinOut = ordered_json::object();

    // Batch Publics
    uint32_t oldBatchStatePos = 0;
    uint32_t oldBatchAccInputHashPos = 8;
    uint32_t oldBatchNumPos = 16;
    uint32_t oldL1InfoTreeRootPos = 17;
    uint32_t oldL1InfoTreeIndexPos = 25;
    uint32_t chainIdPos = 26;
    uint32_t forkIdPos = 27;
    uint32_t newBatchStatePos = 28;
    uint32_t newBatchAccInputHashPos = 36;
    uint32_t newBatchNumPos = 44;
    uint32_t newL1InfoTreeRootPos = 45;
    uint32_t newL1InfoTreeIndexPos = 53;
    uint32_t newLocalExitRootPos = 54;
    uint32_t newLastTimestampPos = 62;

    // Define output publics
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][oldBatchStatePos + i] = zkin1["publics"][oldBatchStatePos + i];
        zkinOut["publics"][oldBatchAccInputHashPos + i] = zkin1["publics"][oldBatchAccInputHashPos + i];
        zkinOut["publics"][oldL1InfoTreeRootPos + i] = zkin1["publics"][oldL1InfoTreeRootPos + i];

        zkinOut["publics"][newBatchStatePos + i] = zkin2["publics"][newBatchStatePos + i];
        zkinOut["publics"][newBatchAccInputHashPos + i] = zkin2["publics"][newBatchAccInputHashPos + i];
        zkinOut["publics"][newL1InfoTreeRootPos + i] = zkin2["publics"][newL1InfoTreeRootPos + i];

        zkinOut["publics"][newLocalExitRootPos + i] = zkin2["publics"][newLocalExitRootPos + i];
    }

    zkinOut["publics"][oldBatchNumPos] = zkin1["publics"][oldBatchNumPos];
    zkinOut["publics"][oldL1InfoTreeIndexPos] = zkin1["publics"][oldL1InfoTreeIndexPos];

    zkinOut["publics"][chainIdPos] = zkin1["publics"][chainIdPos];
    zkinOut["publics"][forkIdPos] = zkin1["publics"][forkIdPos];

    zkinOut["publics"][newBatchNumPos] = zkin2["publics"][newBatchNumPos];
    zkinOut["publics"][newL1InfoTreeIndexPos] = zkin2["publics"][newL1InfoTreeIndexPos];
    zkinOut["publics"][newLastTimestampPos] = zkin2["publics"][newLastTimestampPos];

    // Add first recursive proof inputs
    zkinOut["a_publics"] = zkin1["publics"];
    zkinOut["a_root1"] = zkin1["root1"];
    zkinOut["a_root2"] = zkin1["root2"];
    zkinOut["a_root3"] = zkin1["root3"];
    zkinOut["a_root4"] = zkin1["root4"];
    zkinOut["a_evals"] = zkin1["evals"];
    zkinOut["a_s0_vals1"] = zkin1["s0_vals1"];
    zkinOut["a_s0_vals3"] = zkin1["s0_vals3"];
    zkinOut["a_s0_vals4"] = zkin1["s0_vals4"];
    zkinOut["a_s0_valsC"] = zkin1["s0_valsC"];
    zkinOut["a_s0_siblings1"] = zkin1["s0_siblings1"];
    zkinOut["a_s0_siblings3"] = zkin1["s0_siblings3"];
    zkinOut["a_s0_siblings4"] = zkin1["s0_siblings4"];
    zkinOut["a_s0_siblingsC"] = zkin1["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["a_s" + std::to_string(i) + "_root"] = zkin1["s" + std::to_string(i) + "_root"];
        zkinOut["a_s" + std::to_string(i) + "_siblings"] = zkin1["s" + std::to_string(i) + "_siblings"];
        zkinOut["a_s" + std::to_string(i) + "_vals"] = zkin1["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["a_finalPol"] = zkin1["finalPol"];

    // Add second recursive proof inputs
    zkinOut["b_publics"] = zkin2["publics"];
    zkinOut["b_root1"] = zkin2["root1"];
    zkinOut["b_root2"] = zkin2["root2"];
    zkinOut["b_root3"] = zkin2["root3"];
    zkinOut["b_root4"] = zkin2["root4"];
    zkinOut["b_evals"] = zkin2["evals"];
    zkinOut["b_s0_vals1"] = zkin2["s0_vals1"];
    zkinOut["b_s0_vals3"] = zkin2["s0_vals3"];
    zkinOut["b_s0_vals4"] = zkin2["s0_vals4"];
    zkinOut["b_s0_valsC"] = zkin2["s0_valsC"];
    zkinOut["b_s0_siblings1"] = zkin2["s0_siblings1"];
    zkinOut["b_s0_siblings3"] = zkin2["s0_siblings3"];
    zkinOut["b_s0_siblings4"] = zkin2["s0_siblings4"];
    zkinOut["b_s0_siblingsC"] = zkin2["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["b_s" + std::to_string(i) + "_root"] = zkin2["s" + std::to_string(i) + "_root"];
        zkinOut["b_s" + std::to_string(i) + "_siblings"] = zkin2["s" + std::to_string(i) + "_siblings"];
        zkinOut["b_s" + std::to_string(i) + "_vals"] = zkin2["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["b_finalPol"] = zkin2["finalPol"];

    // Add rootC Recursive2 to publics
    zkinOut["rootC"] = ordered_json::array();
    for (int i = 0; i < 4; i++)
    {
        zkinOut["rootC"][i] = to_string(verKey["constRoot"][i]);
    }

    return zkinOut;
}

ordered_json joinzkinBlobOuterRecursive2(ordered_json &zkin1, ordered_json &zkin2, ordered_json &verKey, uint64_t steps)
{
    ordered_json zkinOut = ordered_json::object();

    // Blob outer publics
    uint32_t oldBlobOuterStateRootPos = 0;
    uint32_t oldBlobOuterBlobStateRootPos = 8;
    uint32_t oldBlobOuterAccInputHashPos = 16;
    uint32_t oldBlobOuterNumPos = 24;
    uint32_t blobOuterChainIdPos = 25;
    uint32_t blobOuterForkIdPos = 26;
    uint32_t newBlobOuterStateRootPos = 27;
    uint32_t newBlobOuterBlobStateRootPos = 35;
    uint32_t newBlobOuterAccInputHashPos = 43;
    uint32_t newBlobOuterNumPos = 51;
    uint32_t newBlobOuterLocalExitRootPos = 52;

    // Define output publics
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][oldBlobOuterStateRootPos + i] = zkin1["publics"][oldBlobOuterStateRootPos + i];
        zkinOut["publics"][oldBlobOuterBlobStateRootPos + i] = zkin1["publics"][oldBlobOuterBlobStateRootPos + i];
        zkinOut["publics"][oldBlobOuterAccInputHashPos + i] = zkin1["publics"][oldBlobOuterAccInputHashPos + i];

        zkinOut["publics"][newBlobOuterStateRootPos + i] = zkin2["publics"][newBlobOuterStateRootPos + i];
        zkinOut["publics"][newBlobOuterBlobStateRootPos + i] = zkin2["publics"][newBlobOuterBlobStateRootPos + i];
        zkinOut["publics"][newBlobOuterAccInputHashPos + i] = zkin2["publics"][newBlobOuterAccInputHashPos + i];

        zkinOut["publics"][newBlobOuterLocalExitRootPos + i] = zkin2["publics"][newBlobOuterLocalExitRootPos + i];
    }

    zkinOut["publics"][oldBlobOuterNumPos] = zkin1["publics"][oldBlobOuterNumPos];

    zkinOut["publics"][blobOuterChainIdPos] = zkin1["publics"][blobOuterChainIdPos];
    zkinOut["publics"][blobOuterForkIdPos] = zkin1["publics"][blobOuterForkIdPos];

    zkinOut["publics"][newBlobOuterNumPos] = zkin2["publics"][newBlobOuterNumPos];
    
    // Add first recursive proof inputs
    zkinOut["a_publics"] = zkin1["publics"];
    zkinOut["a_root1"] = zkin1["root1"];
    zkinOut["a_root2"] = zkin1["root2"];
    zkinOut["a_root3"] = zkin1["root3"];
    zkinOut["a_root4"] = zkin1["root4"];
    zkinOut["a_evals"] = zkin1["evals"];
    zkinOut["a_s0_vals1"] = zkin1["s0_vals1"];
    zkinOut["a_s0_vals3"] = zkin1["s0_vals3"];
    zkinOut["a_s0_vals4"] = zkin1["s0_vals4"];
    zkinOut["a_s0_valsC"] = zkin1["s0_valsC"];
    zkinOut["a_s0_siblings1"] = zkin1["s0_siblings1"];
    zkinOut["a_s0_siblings3"] = zkin1["s0_siblings3"];
    zkinOut["a_s0_siblings4"] = zkin1["s0_siblings4"];
    zkinOut["a_s0_siblingsC"] = zkin1["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["a_s" + std::to_string(i) + "_root"] = zkin1["s" + std::to_string(i) + "_root"];
        zkinOut["a_s" + std::to_string(i) + "_siblings"] = zkin1["s" + std::to_string(i) + "_siblings"];
        zkinOut["a_s" + std::to_string(i) + "_vals"] = zkin1["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["a_finalPol"] = zkin1["finalPol"];

    // Add second recursive proof inputs
    zkinOut["b_publics"] = zkin2["publics"];
    zkinOut["b_root1"] = zkin2["root1"];
    zkinOut["b_root2"] = zkin2["root2"];
    zkinOut["b_root3"] = zkin2["root3"];
    zkinOut["b_root4"] = zkin2["root4"];
    zkinOut["b_evals"] = zkin2["evals"];
    zkinOut["b_s0_vals1"] = zkin2["s0_vals1"];
    zkinOut["b_s0_vals3"] = zkin2["s0_vals3"];
    zkinOut["b_s0_vals4"] = zkin2["s0_vals4"];
    zkinOut["b_s0_valsC"] = zkin2["s0_valsC"];
    zkinOut["b_s0_siblings1"] = zkin2["s0_siblings1"];
    zkinOut["b_s0_siblings3"] = zkin2["s0_siblings3"];
    zkinOut["b_s0_siblings4"] = zkin2["s0_siblings4"];
    zkinOut["b_s0_siblingsC"] = zkin2["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["b_s" + std::to_string(i) + "_root"] = zkin2["s" + std::to_string(i) + "_root"];
        zkinOut["b_s" + std::to_string(i) + "_siblings"] = zkin2["s" + std::to_string(i) + "_siblings"];
        zkinOut["b_s" + std::to_string(i) + "_vals"] = zkin2["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["b_finalPol"] = zkin2["finalPol"];

    // Add rootC blobOuter to publics
    zkinOut["rootC"] = ordered_json::array();
    for (int i = 0; i < 4; i++)
    {
        zkinOut["rootC"][i] = to_string(verKey["constRoot"][i]);
    }

    return zkinOut;
}

ordered_json joinzkinBlobOuter(ordered_json &zkinBatch, ordered_json &zkinBlobInnerRecursive1, ordered_json &verKey, std::string chainId, uint64_t steps)
{
    ordered_json zkinOut = ordered_json::object();

    // Batch publics
    uint32_t oldBatchStatePos = 0;
    uint32_t oldBatchAccInputHashPos = 8;
    uint32_t oldBatchNumPos = 16;
    uint32_t oldL1InfoTreeRootPos = 17;
    uint32_t oldL1InfoTreeIndexPos = 25;
    uint32_t chainIdPos = 26;
    uint32_t forkIdPos = 27;
    uint32_t newBatchStatePos = 28;
    uint32_t newBatchAccInputHashPos = 36;
    uint32_t newBatchNumPos = 44;
    uint32_t newL1InfoTreeRootPos = 45;
    uint32_t newL1InfoTreeIndexPos = 53;
    uint32_t newLocalExitRootPos = 54;
    uint32_t newLastTimestampPos = 62;

    // Blob inner publics
    uint32_t oldBlobInnerBlobStateRootPos = 0;
    uint32_t oldBlobInnerAccInputHashPos = 8;
    uint32_t oldBlobInnerNumPos = 16;
    uint32_t oldBlobInnerStateRootPos = 17;
    uint32_t forkIdBlobInnerPos = 25;
    uint32_t newBlobInnerBlobStateRootPos = 26;
    uint32_t newBlobInnerAccInputHashPos = 34;
    uint32_t newBlobInnerNumPos = 42;
    uint32_t finalAccBatchHashDataPos = 43;
    uint32_t localExitRootFromBlobInnerPos = 51;
    uint32_t isInvalidBlobInnerPos = 59;
    uint32_t timestampLimitPos = 60;
    uint32_t lastL1InfoTreeRootPos = 61;
    uint32_t lastL1InfoTreeIndexPos = 69;

    // Blob outer publics
    uint32_t oldBlobOuterStateRootPos = 0;
    uint32_t oldBlobOuterBlobStateRootPos = 8;
    uint32_t oldBlobOuterAccInputHashPos = 16;
    uint32_t oldBlobOuterNumPos = 24;
    uint32_t blobOuterChainIdPos = 25;
    uint32_t blobOuterForkIdPos = 26;
    uint32_t newBlobOuterStateRootPos = 27;
    uint32_t newBlobOuterBlobStateRootPos = 35;
    uint32_t newBlobOuterAccInputHashPos = 43;
    uint32_t newBlobOuterNumPos = 51;
    uint32_t newBlobOuterLocalExitRootPos = 52;

    // Define output publics
    for (int i = 0; i < 8; i++)
    {
        zkinOut["publics"][oldBlobOuterBlobStateRootPos + i] = zkinBlobInnerRecursive1["publics"][oldBlobInnerBlobStateRootPos + i]; 
        zkinOut["publics"][oldBlobOuterAccInputHashPos + i] = zkinBlobInnerRecursive1["publics"][oldBlobInnerAccInputHashPos + i];

        zkinOut["publics"][newBlobOuterBlobStateRootPos + i] = zkinBlobInnerRecursive1["publics"][newBlobInnerBlobStateRootPos + i];
        zkinOut["publics"][newBlobOuterAccInputHashPos + i] = zkinBlobInnerRecursive1["publics"][newBlobInnerAccInputHashPos + i];
    }
    
    zkinOut["publics"][oldBlobOuterNumPos] = zkinBlobInnerRecursive1["publics"][oldBlobInnerNumPos];
    zkinOut["publics"][newBlobOuterNumPos] = zkinBlobInnerRecursive1["publics"][newBlobInnerNumPos];
    zkinOut["publics"][blobOuterForkIdPos] = zkinBatch["publics"][forkIdPos];

    bool isInvalidFinalAccBatchHashData = true;
    for(int i = 0; i < 8; i++) 
    {
        if(zkinBlobInnerRecursive1["publics"][finalAccBatchHashDataPos + i] != to_string(0)) {
            isInvalidFinalAccBatchHashData = false;
            break;
        }
    }

    bool isInvalidBlob = zkinBlobInnerRecursive1["publics"][isInvalidBlobInnerPos] == to_string(1);

    bool isInvalidTimestamp = zkinBatch["publics"][newLastTimestampPos] > zkinBlobInnerRecursive1["publics"][timestampLimitPos];

    bool isInvalidL1InfoTreeIndex = zkinBatch["publics"][newL1InfoTreeIndexPos] != zkinBlobInnerRecursive1["publics"][lastL1InfoTreeIndexPos];
    
    bool isInvalid = isInvalidFinalAccBatchHashData || isInvalidBlob || isInvalidTimestamp || isInvalidL1InfoTreeIndex;

    for(int i = 0; i < 8; i++) 
    {
        if(isInvalid) {
            zkinOut["publics"][oldBlobOuterStateRootPos + i] = zkinBlobInnerRecursive1["publics"][oldBlobInnerStateRootPos + i];
            zkinOut["publics"][newBlobOuterStateRootPos + i] = zkinBlobInnerRecursive1["publics"][oldBlobInnerStateRootPos + i];
            zkinOut["publics"][newBlobOuterLocalExitRootPos + i] = zkinBlobInnerRecursive1["publics"][localExitRootFromBlobInnerPos + i];
        } else {
            zkinOut["publics"][oldBlobOuterStateRootPos + i] = zkinBatch["publics"][oldBatchStatePos + i];
            zkinOut["publics"][newBlobOuterStateRootPos + i] = zkinBatch["publics"][newBatchStatePos + i];
            zkinOut["publics"][newBlobOuterLocalExitRootPos + i] = zkinBatch["publics"][newLocalExitRootPos + i];
        }
    }

    if(isInvalid) {
        zkinOut["publics"][blobOuterChainIdPos] = chainId;
    } else {
        zkinOut["publics"][blobOuterChainIdPos] = zkinBatch["publics"][chainIdPos];
    }


    // Add batch proof inputs
    zkinOut["batch_publics"] = zkinBatch["publics"];
    zkinOut["batch_root1"] = zkinBatch["root1"];
    zkinOut["batch_root2"] = zkinBatch["root2"];
    zkinOut["batch_root3"] = zkinBatch["root3"];
    zkinOut["batch_root4"] = zkinBatch["root4"];
    zkinOut["batch_evals"] = zkinBatch["evals"];
    zkinOut["batch_s0_vals1"] = zkinBatch["s0_vals1"];
    zkinOut["batch_s0_vals3"] = zkinBatch["s0_vals3"];
    zkinOut["batch_s0_vals4"] = zkinBatch["s0_vals4"];
    zkinOut["batch_s0_valsC"] = zkinBatch["s0_valsC"];
    zkinOut["batch_s0_siblings1"] = zkinBatch["s0_siblings1"];
    zkinOut["batch_s0_siblings3"] = zkinBatch["s0_siblings3"];
    zkinOut["batch_s0_siblings4"] = zkinBatch["s0_siblings4"];
    zkinOut["batch_s0_siblingsC"] = zkinBatch["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["batch_s" + std::to_string(i) + "_root"] = zkinBatch["s" + std::to_string(i) + "_root"];
        zkinOut["batch_s" + std::to_string(i) + "_siblings"] = zkinBatch["s" + std::to_string(i) + "_siblings"];
        zkinOut["batch_s" + std::to_string(i) + "_vals"] = zkinBatch["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["batch_finalPol"] = zkinBatch["finalPol"];

    // Add blob inner proof inputs
    zkinOut["blob_inner_publics"] = zkinBlobInnerRecursive1["publics"];
    zkinOut["blob_inner_root1"] = zkinBlobInnerRecursive1["root1"];
    zkinOut["blob_inner_root2"] = zkinBlobInnerRecursive1["root2"];
    zkinOut["blob_inner_root3"] = zkinBlobInnerRecursive1["root3"];
    zkinOut["blob_inner_root4"] = zkinBlobInnerRecursive1["root4"];
    zkinOut["blob_inner_evals"] = zkinBlobInnerRecursive1["evals"];
    zkinOut["blob_inner_s0_vals1"] = zkinBlobInnerRecursive1["s0_vals1"];
    zkinOut["blob_inner_s0_vals3"] = zkinBlobInnerRecursive1["s0_vals3"];
    zkinOut["blob_inner_s0_vals4"] = zkinBlobInnerRecursive1["s0_vals4"];
    zkinOut["blob_inner_s0_valsC"] = zkinBlobInnerRecursive1["s0_valsC"];
    zkinOut["blob_inner_s0_siblings1"] = zkinBlobInnerRecursive1["s0_siblings1"];
    zkinOut["blob_inner_s0_siblings3"] = zkinBlobInnerRecursive1["s0_siblings3"];
    zkinOut["blob_inner_s0_siblings4"] = zkinBlobInnerRecursive1["s0_siblings4"];
    zkinOut["blob_inner_s0_siblingsC"] = zkinBlobInnerRecursive1["s0_siblingsC"];
    for (uint64_t i = 1; i < steps; i++)
    {
        zkinOut["blob_inner_s" + std::to_string(i) + "_root"] = zkinBlobInnerRecursive1["s" + std::to_string(i) + "_root"];
        zkinOut["blob_inner_s" + std::to_string(i) + "_siblings"] = zkinBlobInnerRecursive1["s" + std::to_string(i) + "_siblings"];
        zkinOut["blob_inner_s" + std::to_string(i) + "_vals"] = zkinBlobInnerRecursive1["s" + std::to_string(i) + "_vals"];
    }
    zkinOut["blob_inner_finalPol"] = zkinBlobInnerRecursive1["finalPol"];

    // Add rootC blobOuter to publics
    zkinOut["rootC"] = ordered_json::array();
    for (int i = 0; i < 4; i++)
    {
        zkinOut["rootC"][i] = to_string(verKey["constRoot"][i]);
    }

    return zkinOut;
}