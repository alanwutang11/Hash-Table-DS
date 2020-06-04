#include "../include/Vector.h"
#include <stdint.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

struct STRUCT_VECTOR_TAG{
    size_t DElementSize;
    size_t DSize;
    size_t DCount;
    uint8_t *DData;
};

#define VECTOR_MIN_SIZE     16

SVectorRef VectorCreate(size_t elemsize){
    if(elemsize){
        SVectorRef Vector = (SVectorRef)malloc(sizeof(SVector));
        Vector->DSize = VECTOR_MIN_SIZE;
        Vector->DCount = 0;
        Vector->DElementSize = elemsize;
        Vector->DData = (uint8_t *)malloc(Vector->DSize * Vector->DElementSize);
        return Vector;
    }
    return NULL;
}

bool VectorDestroy(SVectorRef vector){
    if(vector){
        if(vector->DData){
            free(vector->DData);
            free(vector);
            return true;
        }
    }
    return false;
}

size_t VectorSize(SVectorRef vector){
    if(vector){
        return vector->DCount;
    }
    return 0;
}

void *VectorElement(SVectorRef vector, size_t index){
    if(vector){
        if(index < vector->DCount){
            return (void *)(vector->DData + index * vector->DElementSize);
        }
    }
    return NULL;
}

bool VectorElementAppend(SVectorRef vector, void *elem){
    if(vector && elem){
        if(vector->DCount == vector->DSize){ //this is resizing
            size_t OldSize = vector->DSize;
            uint8_t *OldData = vector->DData;
            vector->DSize *= 2;
            vector->DData = (uint8_t *)malloc(vector->DSize * vector->DElementSize);
            memcpy(vector->DData,OldData,OldSize * vector->DElementSize);
            free(OldData);
        }
        memcpy(vector->DData + vector->DCount * vector->DElementSize, elem, vector->DElementSize);
        vector->DCount++;
        return true;
    }
    return false;
}

void VectorSort(SVectorRef vector, TVectorElementCompare compare){
    qsort(vector->DData,vector->DCount,vector->DElementSize,compare);
}
