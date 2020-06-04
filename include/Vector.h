#ifndef VECTOR_H 	 	    		
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct STRUCT_VECTOR_TAG SVector;
typedef SVector *SVectorRef;

typedef int (*TVectorElementCompare)(const void *, const void *);

SVectorRef VectorCreate(size_t elemsize);

bool VectorDestroy(SVectorRef vector);

size_t VectorSize(SVectorRef vector);

void *VectorElement(SVectorRef vector, size_t index);

#define VectorElementAs(vect,idx,type)      ((type *)VectorElement((vect),(idx)))

bool VectorElementAppend(SVectorRef vector, void *elem);

void VectorSort(SVectorRef vector, TVectorElementCompare compare);


#ifdef __cplusplus
}
#endif

#endif