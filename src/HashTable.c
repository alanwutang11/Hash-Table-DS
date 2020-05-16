#include "../include/HashTable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct STRUCT_HASH_TABLE_ITERATOR_TAG{
    /* ENTER YOUR IMPLEMENTATION HERE */
};

struct STRUCT_HASH_TABLE_TAG{
    /* ENTER YOUR IMPLEMENTATION HERE */
};

SHashTableRef HashTableCreate(SConstHashTableCreateDataRef data){
    /* YOUR CODE HERE */
}

bool HashTableDestroy(SHashTableRef table){
    /* YOUR CODE HERE */
}

size_t HashTableSize(SConstHashTableRef table){
    /* YOUR CODE HERE */
}

const void *HashTableGet(SConstHashTableRef table, const void *key){
    /* YOUR CODE HERE */
}

bool HashTableInsert(SHashTableRef table, const void *key, const void *value){
    /* YOUR CODE HERE */
}

bool HashTableUpdate(SHashTableRef table, const void *key, const void *value){
    /* YOUR CODE HERE */
}

bool HashTableDelete(SHashTableRef table, const void *key){
    /* YOUR CODE HERE */
}

SHashTableIteratorRef HashTableFind(SConstHashTableRef table, const void *key){
    /* YOUR CODE HERE */
}

SHashTableIteratorRef HashTableBegin(SConstHashTableRef table){
    /* YOUR CODE HERE */
}

SHashTableConstIteratorRef HashTableEnd(SConstHashTableRef table){
    /* YOUR CODE HERE */
}

SHashTableIteratorRef HashTableIteratorNext(SHashTableConstIteratorRef iter){
    /* YOUR CODE HERE */
}

SHashTableIteratorRef HashTableIteratorPrevious(SHashTableConstIteratorRef iter){
    /* YOUR CODE HERE */
}

const void *HashTableIteratorKey(SHashTableConstIteratorRef iter){
    /* YOUR CODE HERE */
}

const void *HashTableIteratorValue(SHashTableConstIteratorRef iter){
    /* YOUR CODE HERE */
}

bool HashTableIteratorUpdate(SHashTableIteratorRef iter, const void *value){
    /* YOUR CODE HERE */
}

SHashTableIteratorRef HashTableIteratorDelete(SHashTableIteratorRef iter){
    /* YOUR CODE HERE */
}