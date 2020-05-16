//#include <gtest/gtest.h>
//#include "../include/HashTable.h"
//#include <string.h>
//
///* Hash based off of sdbm. Original version available at
//   http://www.cse.yorku.ca/~oz/hash.html */
//size_t StringKeyHash(const void *key){
//    size_t HashValue = 0;
//    unsigned char *Str = (unsigned char *)key;
//    int Ch;
//    while(*Str){
//        Ch = *Str++;
//        HashValue = Ch + (HashValue << 6) + (HashValue << 16) - HashValue;
//    }
//    return HashValue;
//}
//
//void * StringCopy(const void *key_or_value){
//    const char *Str = (const char *)key_or_value;
//    if(Str){
//        size_t StrLen = strlen(Str) + 1;
//        char *RetStr = (char *)malloc(sizeof(char) * StrLen);
//        strcpy(RetStr,Str);
//        return (void *)RetStr;
//    }
//    return NULL;
//}
//
//SHashTableCreateData StringStringHashCreateTableData =
//{
//    StringKeyHash,
//    (THashKeyCompareFunction)strcmp,
//    StringCopy,
//    free,
//    StringCopy,
//    free
//};
//
//TEST(HashTable, EmptyTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, EmptyGetTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, InsertTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, GetTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, UpdateTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, DeleteTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, FindTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, ForwardIteratorTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, ReverseIteratorTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, IteratorFindUpdateTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, IteratorFindDeleteTest){
//    /* YOUR CODE HERE */
//}
//
//TEST(HashTable, InvalidParameterTest){
//    /* YOUR CODE HERE */
//}
