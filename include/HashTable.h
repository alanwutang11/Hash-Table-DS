#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct STRUCT_HASH_TABLE_TAG SHashTable;
typedef SHashTable * SHashTableRef;
typedef const SHashTable * SConstHashTableRef;

typedef struct STRUCT_HASH_TABLE_ITERATOR_TAG SHashTableIterator;
typedef SHashTableIterator * SHashTableIteratorRef;
typedef const SHashTableIterator * SHashTableConstIteratorRef;

typedef size_t (*THashFunction)(const void *key);
typedef int (*THashKeyCompareFunction)(const void *key1, const void *key2);
typedef void * (*THashElementCopyFunction)(const void *key_or_value);
typedef void (*THashElementDestroyFunction)(void *key_or_value);

typedef struct STRUCT_HASH_TABLE_CREATE_DATA_TAG{
    THashFunction DHashFunction;
    THashKeyCompareFunction DKeyCompareFunction;
    THashElementCopyFunction DKeyCopyFunction;
    THashElementDestroyFunction DKeyDestroyFunction;
    THashElementCopyFunction DValueCopyFunction;
    THashElementDestroyFunction DValueDestroyFunction;
} SHashTableCreateData;
typedef SHashTableCreateData * SHashTableCreateDataRef;
typedef const SHashTableCreateData * SConstHashTableCreateDataRef;

size_t HashFunctionOne(size_t key_as_num, size_t DTableSize);

size_t HashFunctionTwo(size_t key_as_num, size_t DTableSize);

size_t GetIdx(SConstHashTableRef table, const void* key, int whichHash);

void TableResize(SHashTableRef HashTable);

SHashTableIteratorRef HashTableEndMostNode(SHashTableRef HashTable, SHashTableConstIteratorRef iter);

SHashTableIteratorRef HashTableNodeCreate(SHashTableRef HashTable);

void HashTableNodeDestroy(SHashTableRef HashTable, SHashTableIteratorRef Node);



SHashTableRef HashTableCreate(SConstHashTableCreateDataRef data);

bool HashTableDestroy(SHashTableRef table);

size_t HashTableSize(SConstHashTableRef table);

const void *HashTableGet(SConstHashTableRef table, const void *key);

void NodePlace(SHashTableRef HashTable, SHashTableIteratorRef Node, size_t count);

bool HashTableNodeInsert(SHashTableRef HashTable, const void* key, const void* value);

bool HashTableInsert(SHashTableRef table, const void *key, const void *value);

bool HashTableUpdate(SHashTableRef table, const void *key, const void *value);

bool HashTableDelete(SHashTableRef table, const void *key);

SHashTableIteratorRef HashTableFind(SConstHashTableRef table, const void *key);

SHashTableIteratorRef HashTableBegin(SConstHashTableRef table);

SHashTableConstIteratorRef HashTableEnd(SConstHashTableRef table);

SHashTableIteratorRef HashTableIteratorNext(SHashTableConstIteratorRef iter);

SHashTableIteratorRef HashTableIteratorPrevious(SHashTableConstIteratorRef iter);

const void *HashTableIteratorKey(SHashTableConstIteratorRef iter);

const void *HashTableIteratorValue(SHashTableConstIteratorRef iter);

bool HashTableIteratorUpdate(SHashTableIteratorRef iter, const void *value);

SHashTableIteratorRef HashTableIteratorDelete(SHashTableIteratorRef iter);

#ifdef __cplusplus
}
#endif

#endif
