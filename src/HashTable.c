#include "../include/HashTable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/Vector.h"

//one table, two hash functions. Cuckoo Hash

struct STRUCT_HASH_TABLE_ITERATOR_TAG{
    SHashTableRef DHashTable;
    void* DKey;
    void* DValue;
};

struct STRUCT_HASH_TABLE_TAG{
    SHashTableIteratorRef* DTable;
    SHashTableIterator DEnd;
    size_t DTableSize;
    size_t DTableNodeCount;
    size_t DNodeSize;

    THashFunction DHashFunction;
    THashKeyCompareFunction DCompareFunction;
    THashElementCopyFunction DCopyKeyFunction;
    THashElementCopyFunction DCopyValFunction;
    THashElementDestroyFunction DDestroyKeyFunction;
    THashElementDestroyFunction DDestroyValFunction;
};
size_t HashFunctionOne(size_t key_as_num, size_t DTableSize){ //returns index to "table 1"
    return key_as_num % DTableSize;
}

size_t HashFunctionTwo(size_t key_as_num, size_t DTableSize){ //returns index to "table 2"
    return (key_as_num / DTableSize)%DTableSize; //use table size
}

size_t GetIdx(SConstHashTableRef table, const void* key, int whichHash){
    size_t key_as_num = table->DHashFunction(key);
    size_t table_idx;
    if (whichHash == 1){
        table_idx = HashFunctionOne(key_as_num, table->DTableSize);
    }
    else{ //2nd hash function, whichHash == 2
        table_idx = HashFunctionTwo(key_as_num, table->DTableSize);
    }

    return table_idx;
}


void TableResize(SHashTableRef HashTable){
    size_t OldSize = HashTable->DTableSize;
    SHashTableIteratorRef* OldData = HashTable->DTable;
    HashTable->DTableSize *= 2;
    HashTable->DTable = (SHashTableIteratorRef*)malloc(HashTable->DTableSize*HashTable->DNodeSize);

    for (int i = 0; i < HashTable->DTableSize; i++){
        HashTable->DTable[i] = NULL; //initialize new table to empty for each node
    }

    for (int i = 0; i < OldSize; i++){
        if (OldData[i]){ //if node exists in the old table
            NodePlace(HashTable,OldData[i],0); //place the node with the new table size
        }
    }

    free(OldData);
}

SHashTableIteratorRef HashTableEndMostNode(SHashTableRef HashTable, SHashTableConstIteratorRef iter){ //left most node
    int idx = HashTable->DTableSize;
    int not_ob = 0; //out of bounds
    while (!HashTable->DTable[idx]){
        idx--;
        not_ob = 1;
    }
    if (not_ob){ //doesnt go out of bounds, iterator to return
        return HashTable->DTable[idx];
    }
    return NULL;
}

SHashTableIteratorRef HashTableNodeCreate(SHashTableRef HashTable){
    SHashTableIteratorRef Node = malloc(sizeof(SHashTableIterator));
    memset(Node,0,sizeof(SHashTableIterator));
    Node->DHashTable = HashTable;
    return Node;
}

void HashTableNodeDestroy(SHashTableRef HashTable, SHashTableIteratorRef Node){
    HashTable->DDestroyKeyFunction(Node->DKey);
    HashTable->DDestroyValFunction(Node->DValue);
    free(Node);
    HashTable->DTableNodeCount--;
}



void NodePlace(SHashTableRef HashTable, SHashTableIteratorRef Node, size_t count){ //have a count for cycles
    SHashTableIteratorRef DisplacedNode, DisplacedNode2;
    size_t table_idx;
    if (count == HashTable->DTableSize){
        TableResize(HashTable); //table re-sized, now INSERT
        NodePlace(HashTable,Node,0); //count will no longer equal table size so it goes to the other ones
        return;
    }

    table_idx = GetIdx(HashTable,Node->DKey,1);
    if (!HashTable->DTable[table_idx]){ //if spot if free, place. Then return
        HashTable->DTable[table_idx] = Node;
        return; //done finish
    }
    else{//spot is not free, swap. Move current into temp
        DisplacedNode = HashTable->DTable[table_idx];
        HashTable->DTable[table_idx] = Node;
        table_idx = GetIdx(HashTable, DisplacedNode->DKey, 2);

        if (!HashTable->DTable[table_idx]){ //if spot is free, place. Then return
            HashTable->DTable[table_idx] = DisplacedNode;
            return;
        }
        else{ //2nd spot is occupied
            DisplacedNode2 = HashTable->DTable[table_idx];
            HashTable->DTable[table_idx] = DisplacedNode;
            count += 1;
            NodePlace(HashTable,DisplacedNode2, count); //both spots occupied, try again with displaced second node
        }
    }
}

bool HashTableNodeInsert(SHashTableRef HashTable, const void* key, const void* value) {
    SHashTableIteratorRef NewNode, CompNode;

    HashTable->DTableNodeCount++;
    if (HashTable->DTableNodeCount == HashTable->DTableSize){ //if table is full
        TableResize(HashTable);
    }

    //check if key exists
    CompNode = HashTableFind(HashTable,key);
    if (CompNode){
        if(!HashTable->DCompareFunction(CompNode->DKey,key)){ //if key is the same, return false
            return false;
        }
    }


    //create new node
    NewNode = HashTableNodeCreate(HashTable);
    NewNode->DKey = HashTable->DCopyKeyFunction(key);
    NewNode->DValue = HashTable->DCopyValFunction(value);

    NodePlace(HashTable,NewNode,0);
    return true;
}


SHashTableRef HashTableCreate(SConstHashTableCreateDataRef data) {
    SHashTableRef HashTable = malloc(sizeof(SHashTable));
    HashTable->DNodeSize = sizeof(SHashTableIteratorRef);
    HashTable->DTable = (SHashTableIteratorRef*)malloc(2048*HashTable->DNodeSize);
    memset(&HashTable->DEnd,0,sizeof(SHashTableIterator));
    HashTable->DEnd.DHashTable = HashTable;
    HashTable->DTableSize = 2048;
    HashTable->DTableNodeCount = 0;

    for (int i = 0; i < HashTable->DTableSize; i++){
        HashTable->DTable[i] = NULL;
    }


    HashTable->DHashFunction = data->DHashFunction;
    HashTable->DCompareFunction = data->DKeyCompareFunction;
    HashTable->DCopyKeyFunction = data->DKeyCopyFunction;
    HashTable->DDestroyKeyFunction = data->DKeyDestroyFunction;
    HashTable->DCopyValFunction = data->DValueCopyFunction;
    HashTable->DDestroyValFunction = data->DValueDestroyFunction;

    return HashTable;
}

bool HashTableDestroy(SHashTableRef table){
    if (!table) return false;

    for (int i = 0; i < table->DTableSize; i++){
        if (table->DTable[i]){
            HashTableNodeDestroy(table,table->DTable[i]);
            table->DTable[i] = NULL;
            if (!table->DTable[i]){
            }
            if (table->DTable[i]){
            }
        }

    }
    free(table->DTable);
    free(table);
    return true;
}

size_t HashTableSize(SConstHashTableRef table){
    if (table == NULL) return 0;
    return table->DTableNodeCount;
}

const void *HashTableGet(SConstHashTableRef table, const void *key){
    if (!table || !key) return NULL;
    size_t table_idx, table_idx2;
    table_idx = GetIdx(table,key,1);
    table_idx2 = GetIdx(table,key,2);

    if (table->DTableNodeCount == 0) return NULL;

    if (table->DTable[table_idx]){
        return (const void*)(table->DTable[table_idx]->DValue);
    }

    if (table->DTable[table_idx2]){
        return (const void*)(table->DTable[table_idx2]->DValue);
    }


    return NULL;
}

bool HashTableInsert(SHashTableRef table, const void *key, const void *value){
    if (!table || !key || !value) return false;
    return HashTableNodeInsert(table, key, value);
}

bool HashTableUpdate(SHashTableRef table, const void *key, const void *value){
    SHashTableIteratorRef Node;
    size_t table_idx, table_idx2;
    if (!table || !key || !value) return false;
    table_idx = GetIdx(table,key,1);
    table_idx2 = GetIdx(table,key,2);

    if (table->DTable[table_idx]){ //if exists in pos 1

        table->DDestroyValFunction(table->DTable[table_idx]->DValue); //delete value
        table->DTable[table_idx]->DValue = table->DCopyValFunction(value); //copy updated value
        return true;
    }

    if (table->DTable[table_idx2]){ //if exists in pos 2
        table->DDestroyValFunction(table->DTable[table_idx2]->DValue); //delete value
        table->DTable[table_idx2]->DValue = table->DCopyValFunction(value); //copy updated value
        return true;
    }
    //key not found
    return false;
}

bool HashTableDelete(SHashTableRef table, const void *key){
    if (!table || !key) return false;
    size_t table_idx, table_idx2;
    table_idx = GetIdx(table,key,1);
    table_idx2 = GetIdx(table,key,2);
    if (table->DTable[table_idx]){
        HashTableNodeDestroy(table,table->DTable[table_idx]); //destroy node
        table->DTable[table_idx] = NULL;
        return true;
    }
    else if (table->DTable[table_idx2]){
        HashTableNodeDestroy(table,table->DTable[table_idx2]);
        table->DTable[table_idx2] = NULL;
        return true;
    }
    //key not found
    return false;
}

SHashTableIteratorRef HashTableFind(SConstHashTableRef table, const void *key){
    size_t table_idx, table_idx2;
    if (!table || !key) return NULL;
    table_idx = GetIdx(table,key,1);
    table_idx2 = GetIdx(table,key,2);

    if (table->DTable[table_idx]){
        return table->DTable[table_idx];
    }
    if (table->DTable[table_idx2]){
        return table->DTable[table_idx2];
    }

    return NULL;
}

SHashTableIteratorRef HashTableBegin(SConstHashTableRef table){
    //returns the first valid node closest to the start of the array
    int idx = 0;
    int beg_exists = 0;
    if (!table) return NULL;

    while(!table->DTable[idx]){ //while the value is no good
        idx++;
        beg_exists = 1; //a node has been found
    }
    if (beg_exists){
        return table->DTable[idx];
    }
    //a node has not been found, return end
    return &((SHashTableRef)table)->DEnd;
}

SHashTableConstIteratorRef HashTableEnd(SConstHashTableRef table){
    if (!table) return NULL;
    return &((SHashTableRef)table)->DEnd; //end means out of bounds
}

SHashTableIteratorRef HashTableIteratorNext(SHashTableConstIteratorRef iter){
    size_t table_idx, table_idx2;
    if (!iter) return NULL;
    if(&iter->DHashTable->DEnd == iter) {
        return NULL;
    }

    table_idx = GetIdx(iter->DHashTable,iter->DKey,1);
    table_idx2 = GetIdx(iter->DHashTable,iter->DKey,2);
    table_idx = iter->DHashTable->DTable[table_idx] ? table_idx : table_idx2; //index resides in either table 1 or 2


    for (int i = table_idx + 1; i < iter->DHashTable->DTableSize; i++) { //keep iterating through table until the next vaild node is found
        if (iter->DHashTable->DTable[i]){
            return iter->DHashTable->DTable[i];
        }
    }

    return &iter->DHashTable->DEnd; //if no valid nodes are left, then return end
}

SHashTableIteratorRef HashTableIteratorPrevious(SHashTableConstIteratorRef iter){ //same thing as forward, but in reverse
    size_t table_idx, table_idx2;
    if (!iter) return NULL;
    if (HashTableBegin(iter->DHashTable) == iter){
        return NULL;
    }
    if (iter == &iter->DHashTable->DEnd){
        return HashTableEndMostNode(iter->DHashTable,iter);
    }
    table_idx = GetIdx(iter->DHashTable,iter->DKey,1);
    table_idx2 = GetIdx(iter->DHashTable,iter->DKey,2);

    table_idx = iter->DHashTable->DTable[table_idx] ? table_idx : table_idx2; //index resides in either table 1 or 2

    for (int i = table_idx - 1; i < iter->DHashTable->DTableSize; i--) {
        if (iter->DHashTable->DTable[i]){
            return iter->DHashTable->DTable[i];
        }
    }

    return NULL;

}

const void *HashTableIteratorKey(SHashTableConstIteratorRef iter){
    if (!iter) return NULL;
    return (const void*)iter->DKey;
}

const void *HashTableIteratorValue(SHashTableConstIteratorRef iter){
    if (!iter) return NULL;
    return (const void*)iter->DValue;
}

bool HashTableIteratorUpdate(SHashTableIteratorRef iter, const void *value){
    if (!iter || !value) return false;
    iter->DHashTable->DDestroyValFunction(iter->DValue);

    iter->DValue = iter->DHashTable->DCopyValFunction(value);
    return true;
}

SHashTableIteratorRef HashTableIteratorDelete(SHashTableIteratorRef iter){
    size_t table_idx, table_idx2;
    SHashTableIteratorRef RetNode;
    if (!iter) return NULL;
    if(&iter->DHashTable->DEnd == iter) {
        return NULL;
    }
    //check if the node is an end node
    RetNode = HashTableIteratorNext(iter);

    table_idx = GetIdx(iter->DHashTable,iter->DKey,1);
    table_idx2 = GetIdx(iter->DHashTable,iter->DKey,2);
    table_idx = iter->DHashTable->DTable[table_idx] ? table_idx : table_idx2;
    HashTableNodeDestroy(iter->DHashTable,iter);
    iter->DHashTable->DTable[table_idx] = NULL; //doing all this so I can set the node to NULL
    return RetNode;
}