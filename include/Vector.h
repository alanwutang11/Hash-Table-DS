#ifndef VECTOR_H 	 	    		
#define VECTOR_H

#include "../../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/stdlib.h"
#include "../../../../../../Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/10.0.1/include/stdbool.h"

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