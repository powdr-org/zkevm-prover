#ifndef PROOF2ZKIN__STARK_HPP
#define PROOF2ZKIN__STARK_HPP

#include <nlohmann/json.hpp>
#include "friProof.hpp"

using ordered_json = nlohmann::ordered_json;

ordered_json proof2zkinStark(ordered_json &fproof);
ordered_json joinzkinBatchRecursive2(ordered_json &zkin1, ordered_json &zkin2, ordered_json &verKey, uint64_t steps);
ordered_json joinzkinBlobOuterRecursive2(ordered_json &zkin1, ordered_json &zkin2, ordered_json &verKey, uint64_t steps);
ordered_json joinzkinBlobOuter(ordered_json &zkinBatch, ordered_json &zkinBlobInnerRecursive1, ordered_json &verKey, std::string chainId, uint64_t steps);

#endif