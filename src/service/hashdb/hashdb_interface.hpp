#ifndef HASHDB_INTERFACE_HPP
#define HASHDB_INTERFACE_HPP

#include "goldilocks_base_field.hpp"
#include "database.hpp"
#include "database_map.hpp"
#include "config.hpp"
#include "smt.hpp"
#include <mutex>
#include "zkresult.hpp"
#include "persistence.hpp"
#include "database_64.hpp"
#include "key_value.hpp"
#include "hash_value_gl.hpp"

class HashDBInterface
{
public:

    virtual ~HashDBInterface(){};
    virtual zkresult getLatestStateRoot (Goldilocks::Element (&stateRoot)[4]) = 0;
    virtual zkresult set                (const string &batchUUID, uint64_t block, uint64_t tx, const Goldilocks::Element (&oldRoot)[4], const Goldilocks::Element (&key)[4], const mpz_class &value, const Persistence persistence, Goldilocks::Element (&newRoot)[4], SmtSetResult *result, DatabaseMap *dbReadLog) = 0;
    virtual zkresult get                (const string &batchUUID, const Goldilocks::Element (&root)[4], const Goldilocks::Element (&key)[4], mpz_class &value, SmtGetResult *result, DatabaseMap *dbReadLog) = 0;
    virtual zkresult setProgram         (const string &batchUUID, uint64_t block, uint64_t tx, const Goldilocks::Element (&key)[4], const vector<uint8_t> &data, const Persistence persistence) = 0;
    virtual zkresult getProgram         (const string &batchUUID, const Goldilocks::Element (&key)[4], vector<uint8_t> &data, DatabaseMap *dbReadLog) = 0;
    virtual void     loadDB             (const DatabaseMap::MTMap &input, const bool persistent, const Goldilocks::Element (&stateRoot)[4]) = 0;
    virtual void     loadProgramDB      (const DatabaseMap::ProgramMap &input, const bool persistent) = 0;
    virtual void     finishTx           (const string &batchUUID, const string &newStateRoot, const Persistence persistence) = 0;
    virtual void     startBlock         (const string &batchUUID, const string &oldStateRoot, const Persistence persistence) = 0;
    virtual void     finishBlock        (const string &batchUUID, const string &newStateRoot, const Persistence persistence) = 0;
    virtual zkresult flush              (const string &batchUUID, const string &newStateRoot, const Persistence persistence, uint64_t &flushId, uint64_t &storedFlushId) = 0;
    virtual zkresult purge              (const string &batchUUID, const Goldilocks::Element (&newStateRoot)[4], const Persistence persistence) = 0;
    virtual zkresult consolidateState   (const Goldilocks::Element (&virtualStateRoot)[4], const Persistence persistence, Goldilocks::Element (&consolidatedStateRoot)[4], uint64_t &flushId, uint64_t &storedFlushId) = 0;
    virtual zkresult getFlushStatus     (uint64_t &storedFlushId, uint64_t &storingFlushId, uint64_t &lastFlushId, uint64_t &pendingToFlushNodes, uint64_t &pendingToFlushProgram, uint64_t &storingNodes, uint64_t &storingProgram, string &proverId) = 0;
    virtual zkresult getFlushData       (uint64_t flushId, uint64_t &storedFlushId, unordered_map<string, string> (&nodes), unordered_map<string, string> (&program), string &nodesStateRoot) = 0;
    virtual void     clearCache         (void) = 0;
    virtual zkresult readTree           (const Goldilocks::Element (&root)[4], vector<KeyValue> &keyValues, vector<HashValueGL> &hashValues) = 0;
    virtual zkresult writeTree          (const Goldilocks::Element (&oldRoot)[4], const vector<KeyValue> &keyValues, Goldilocks::Element (&newRoot)[4], const bool persistent) = 0;
    virtual zkresult cancelBatch        (const string &batchUUID) = 0;
    virtual zkresult resetDB            (void) = 0;

};

#endif