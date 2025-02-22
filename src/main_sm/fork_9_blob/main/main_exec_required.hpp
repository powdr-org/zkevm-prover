#ifndef MAIN_EXEC_REQUIRED_HPP_fork_9_blob
#define MAIN_EXEC_REQUIRED_HPP_fork_9_blob

#include <string>
#include "goldilocks_base_field.hpp"
#include "sm/storage/smt_action.hpp"
#include "sm/binary/binary_action.hpp"
#include "sm/arith/arith_action.hpp"
#include "sm/padding_kk/padding_kk_executor.hpp"
#include "sm/padding_kkbit/padding_kkbit_executor.hpp"
#include "sm/bits2field/bits2field_executor.hpp"
#include "sm/keccak_f/keccak_f_executor.hpp"
#include "sm/padding_sha256/padding_sha256_executor.hpp"
#include "sm/padding_sha256bit/padding_sha256bit_executor.hpp"
#include "sm/bits2field_sha256/bits2field_sha256_executor.hpp"
#include "sm/sha256_f/sha256_f_executor.hpp"
#include "sm/memory/memory_executor.hpp"
#include "sm/padding_pg/padding_pg_executor.hpp"
#include "sm/mem_align/mem_align_executor.hpp"
#include "sm/climb_key/climb_key_executor.hpp"

using namespace std;

namespace fork_9_blob
{

class MainExecRequired
{
public:
    vector<SmtAction> Storage;
    vector<MemoryAccess> Memory;
    vector<BinaryAction> Binary;
    vector<ArithAction> Arith;
    vector<PaddingKKExecutorInput> PaddingKK;
    vector<PaddingKKBitExecutorInput> PaddingKKBit;
    vector<Bits2FieldExecutorInput> Bits2Field;
    vector<vector<Goldilocks::Element>> KeccakF;
    vector<PaddingSha256ExecutorInput> PaddingSha256;
    vector<PaddingSha256BitExecutorInput> PaddingSha256Bit;
    vector<Bits2FieldSha256ExecutorInput> Bits2FieldSha256;
    vector<Sha256FExecutorInput> Sha256F;
    vector<PaddingPGExecutorInput> PaddingPG;
    vector<array<Goldilocks::Element, 17>> PoseidonG; // The 17th fe is the permutation
    vector<array<Goldilocks::Element, 17>> PoseidonGFromPG; // The 17th fe is the permutation
    vector<array<Goldilocks::Element, 17>> PoseidonGFromST; // The 17th fe is the permutation
    vector<ClimbKeyAction> ClimbKey; // The 17th fe is the permutation
    vector<MemAlignAction> MemAlign;
};

} // namespace

#endif