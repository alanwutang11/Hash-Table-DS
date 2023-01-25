#include <gtest/gtest.h>
#include "../include/HashTable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

///* Hash based off of sdbm. Original version available at
//   http://www.cse.yorku.ca/~oz/hash.html */
size_t StringKeyHash(const void *key){
   size_t HashValue = 0;
   unsigned char *Str = (unsigned char *)key;
   int Ch;
   while(*Str){
       Ch = *Str++;
       HashValue = Ch + (HashValue << 6) + (HashValue << 16) - HashValue;
   }
   return HashValue;
}

size_t CollisionTestHash(const void *key){
   return 1000;
}

void * StringCopy(const void *key_or_value){
   const char *Str = (const char *)key_or_value;
   if(Str){
       size_t StrLen = strlen(Str) + 1;
       char *RetStr = (char *)malloc(sizeof(char) * StrLen);
       strcpy(RetStr,Str);
       return (void *)RetStr;
   }
   return NULL;
}

SHashTableCreateData StringStringHashCreateTableData =
{
   StringKeyHash,
   (THashKeyCompareFunction)strcmp,
   StringCopy,
   free,
   StringCopy,
   free
};

SHashTableCreateData StringStringHashCreateTableData2 = //for collision testing/resize testing
       {
               CollisionTestHash,
               (THashKeyCompareFunction)strcmp,
               StringCopy,
               free,
               StringCopy,
               free
       };

TEST(HashTable, EmptyTest){
   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_EQ(HashTableSize(HashTable),0);
   HashTableDestroy(HashTable);
}

TEST(HashTable, EmptyGetTest){
   const void* key1 = "j";
   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   ASSERT_TRUE(HashTableGet(HashTable,key1) == NULL);
   HashTableDestroy(HashTable);
}


TEST(HashTable, InsertTest){
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";



   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_EQ(HashTableSize(HashTable),0);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_EQ(HashTableSize(HashTable),1);
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_EQ(HashTableSize(HashTable),2);
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   EXPECT_EQ(HashTableSize(HashTable),3);

   HashTableDestroy(HashTable);
}


TEST(HashTable, GetTest){
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";

   const void* result1;
   const void* result2;


   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   EXPECT_EQ(HashTableSize(HashTable),3);

   result1 = HashTableGet(HashTable,key1);
   result2 = HashTableGet(HashTable,key2);

   EXPECT_EQ(strcmp((char*)result1,"val1"),0);
   EXPECT_EQ(strcmp((char*)result2,"val2"),0);

   HashTableDestroy(HashTable);

}

TEST(HashTable, UpdateTest){
   const void* key1 = "A";
   const void* val1 = "val";
   const void* val2 = "update";

   const void* result;

   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   HashTableInsert(HashTable, key1, val1);
   HashTableGet(HashTable, key1);
   HashTableUpdate(HashTable, key1, val2);
   result = HashTableGet(HashTable, key1);
   EXPECT_EQ(strcmp((char*)result,"update"),0);
   HashTableDestroy(HashTable);

}

TEST(HashTable, DeleteTest){
   const void* key1 = "A";
   const void* val1 = "B";

   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   HashTableInsert(HashTable, key1, val1);
   HashTableDelete(HashTable,key1);
   ASSERT_TRUE(HashTableGet(HashTable, key1) == NULL);

   HashTableDestroy(HashTable);

}

TEST(HashTable, FindTest){ //iterator find test
   //begin, check if begin is equal to find (should be the first one so they should be equal)
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";

   SHashTableConstIteratorRef FoundIter, BeginIter;

   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   BeginIter = HashTableBegin(HashTable);
   FoundIter = HashTableFind(HashTable,key1);
   EXPECT_TRUE(BeginIter == FoundIter);
   HashTableDestroy(HashTable);

}

TEST(HashTable, ForwardIteratorTest){

   SHashTableConstIteratorRef Iter,EndIter;
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";

   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   EXPECT_EQ(HashTableSize(HashTable),3);
   Iter = HashTableBegin(HashTable);
   EndIter = HashTableEnd(HashTable);
   Iter = HashTableIteratorNext(Iter);
   Iter = HashTableIteratorNext(Iter);
   Iter = HashTableIteratorNext(Iter);
   EXPECT_TRUE(EndIter == Iter);
   HashTableDestroy(HashTable);
}

TEST(HashTable, ReverseIteratorTest){
   SHashTableConstIteratorRef BeginIter,Iter;
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";

   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   EXPECT_EQ(HashTableSize(HashTable),3);
   BeginIter = HashTableBegin(HashTable);
   Iter = HashTableEnd(HashTable);
   Iter = HashTableIteratorPrevious(Iter);
   Iter = HashTableIteratorPrevious(Iter);
   Iter = HashTableIteratorPrevious(Iter);
   EXPECT_TRUE(BeginIter == Iter);
   HashTableDestroy(HashTable);

}

TEST(HashTable, IteratorFindUpdateTest){
   SHashTableIteratorRef Iter;
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";
   const void* updateVal = "update";
   const void* updatedKey;
   const void* updatedVal;
   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   EXPECT_EQ(HashTableSize(HashTable),3);
   Iter = HashTableFind(HashTable, key2); //val2
   HashTableIteratorUpdate(Iter, updateVal);
   updatedKey = HashTableIteratorKey(Iter);
   updatedVal = HashTableIteratorValue(Iter);

   EXPECT_EQ(strcmp((char*)updatedKey,"key2"),0);
   EXPECT_EQ(strcmp((char*)updatedVal,"update"),0);
   HashTableDestroy(HashTable);
}

TEST(HashTable, IteratorFindDeleteTest){
   SHashTableIteratorRef Iter;
   const void* key1 = "key1";
   const void* key2 = "key2";
   const void* val1 = "val1";
   const void* val2 = "val2";
   const void* key3 = "key3";
   const void* val3 = "val3";

   const void* nextKey;
   const void* nextVal;


   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   EXPECT_TRUE(HashTableInsert(HashTable, key1, val1));
   EXPECT_TRUE(HashTableInsert(HashTable, key2, val2));
   EXPECT_TRUE(HashTableInsert(HashTable, key3, val3));
   EXPECT_EQ(HashTableSize(HashTable),3);
   Iter = HashTableFind(HashTable, key2); //val2
   Iter = HashTableIteratorDelete(Iter); //delete key2
   ASSERT_TRUE(HashTableFind(HashTable,key2) == NULL);
   EXPECT_EQ(HashTableSize(HashTable),2);
   nextKey = HashTableIteratorKey(Iter); //expect key3
   nextVal = HashTableIteratorValue(Iter); //expect val3
   EXPECT_EQ(strcmp((char*)nextKey,"key3"),0);
   EXPECT_EQ(strcmp((char*)nextVal,"val3"),0);

   HashTableDestroy(HashTable);

}

TEST(HashTable, InvalidParameterTest){
   SHashTableRef HashTable = HashTableCreate(&StringStringHashCreateTableData);
   SHashTableIteratorRef Iter;
   SHashTableConstIteratorRef IterEnd;
   const void* key1 = "key1";
   const void* val1 = "val1";

   const void* unisertedkey = "nothing";
   const void* unisertedval = "nothingval";

   ASSERT_FALSE(HashTableInsert(HashTable, NULL, "Hello"));
   ASSERT_FALSE(HashTableInsert(HashTable, "Hello", NULL));
   EXPECT_EQ(HashTableSize(HashTable),NULL);
   ASSERT_TRUE(HashTableGet(HashTable, NULL) == NULL);


   ASSERT_FALSE(HashTableDelete(HashTable, NULL));
   ASSERT_FALSE(HashTableDelete(NULL, key1));

   HashTableInsert(HashTable, key1, val1);
   ASSERT_TRUE(HashTableBegin(NULL) == NULL);
   ASSERT_TRUE(HashTableEnd(NULL) == NULL);

   ASSERT_TRUE(HashTableFind(NULL, key1) == NULL);
   ASSERT_TRUE(HashTableFind(HashTable, NULL) == NULL);

   Iter = HashTableBegin(HashTable);
   Iter = HashTableIteratorNext(Iter);
   Iter = HashTableIteratorNext(Iter);
   ASSERT_TRUE(Iter == NULL);

   Iter = HashTableBegin(HashTable);
   Iter = HashTableIteratorPrevious(Iter);
   ASSERT_TRUE(Iter == NULL);
   ASSERT_TRUE(HashTableIteratorKey(Iter) == NULL);
   ASSERT_TRUE(HashTableIteratorValue(Iter) == NULL);

   ASSERT_FALSE(HashTableUpdate(HashTable,NULL,val1));
   ASSERT_FALSE(HashTableUpdate(NULL,key1,val1));
   ASSERT_FALSE(HashTableUpdate(HashTable,unisertedkey,unisertedval));


   IterEnd = HashTableEnd(HashTable);
   EXPECT_TRUE(HashTableIteratorDelete((SHashTableIteratorRef)IterEnd) == NULL);

   EXPECT_FALSE(HashTableDestroy(NULL));
   HashTableDestroy(HashTable);

}
