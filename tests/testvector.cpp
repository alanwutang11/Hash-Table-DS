#include <gtest/gtest.h>
#include "../include/Vector.h"
#include <stdio.h>
#include <stdlib.h>


TEST(Vector, EmptyTest){
   SVectorRef Vector = VectorCreate(sizeof(int));

   ASSERT_TRUE(Vector != nullptr);
   EXPECT_EQ(VectorSize(Vector),0);
   EXPECT_TRUE(VectorDestroy(Vector));
}

TEST(Vector, AppendTest){
   SVectorRef Vector = VectorCreate(sizeof(int));
   int Temp;
   ASSERT_TRUE(Vector != nullptr);
   EXPECT_EQ(VectorSize(Vector),0);
   Temp = 4;
   EXPECT_TRUE(VectorElementAppend(Vector,&Temp));
   EXPECT_EQ(VectorSize(Vector),1);
   EXPECT_TRUE(VectorDestroy(Vector));
}

TEST(Vector, ElementTest){
   SVectorRef Vector = VectorCreate(sizeof(int));
   int Temp, *TempPtr;
   ASSERT_TRUE(Vector != nullptr);
   EXPECT_EQ(VectorSize(Vector),0);
   Temp = 4;
   EXPECT_TRUE(VectorElementAppend(Vector,&Temp));
   EXPECT_EQ(VectorSize(Vector),1);
   TempPtr = VectorElementAs(Vector,0,int);
   ASSERT_TRUE(TempPtr != nullptr);
   EXPECT_EQ(*TempPtr,4);
   EXPECT_TRUE(VectorDestroy(Vector));
}

TEST(Vector, LargeAppendTest){
   SVectorRef Vector = VectorCreate(sizeof(int));
   ASSERT_TRUE(Vector != nullptr);
   for(int Index = 0; Index < 100; Index++){
       int *TempPtr;
       EXPECT_EQ(VectorSize(Vector),Index);
       EXPECT_TRUE(VectorElementAppend(Vector,&Index));
       TempPtr = VectorElementAs(Vector,Index,int);
       ASSERT_TRUE(TempPtr != nullptr);
       EXPECT_EQ(*TempPtr,Index);
   }
   EXPECT_TRUE(VectorDestroy(Vector));
}

int RevIntCompare(const void *ptr1, const void *ptr2){
   const int *Ptr1 = (const int *)ptr1;
   const int *Ptr2 = (const int *)ptr2;
   return *Ptr2 - *Ptr1;
}

TEST(Vector, SortTest){
   SVectorRef Vector = VectorCreate(sizeof(int));
   ASSERT_TRUE(Vector != nullptr);
   srand(time(NULL));

   for(int Index = 0; Index < 100; Index++){
       int *TempPtr;
       int R = rand();
       VectorElementAppend(Vector,&R);
   }

   VectorSort(Vector,RevIntCompare);

   for(int Index = 0; Index < 99; Index++){
       int *val = VectorElementAs(Vector,Index,int);
       int *valnext = VectorElementAs(Vector,Index+1,int);
       EXPECT_TRUE(*val > *valnext);
   }
   EXPECT_TRUE(VectorDestroy(Vector));

}

TEST(Vector, InvalidParameterTest){
   SVectorRef Vector = VectorCreate(sizeof(int));

   EXPECT_EQ(VectorCreate(0),nullptr);

   EXPECT_FALSE(VectorDestroy(nullptr));

   EXPECT_EQ(VectorSize(nullptr),0);

   EXPECT_EQ(VectorElement(nullptr,0),nullptr);
   EXPECT_EQ(VectorElement(Vector,0),nullptr);

   EXPECT_FALSE(VectorElementAppend(nullptr,nullptr));
   EXPECT_FALSE(VectorElementAppend(Vector,nullptr));
   EXPECT_TRUE(VectorDestroy(Vector));
}
