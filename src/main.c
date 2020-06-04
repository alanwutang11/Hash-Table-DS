#include <stdio.h> 	 	    		
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "../include/HashTable.h"
#include "../include/Vector.h"

typedef struct STRUCT_COURSE_INFORMATION_TAG{
    int DTerm;
    int DCRN;
    char DSubject[4];
    char DCourse[6];
    char DSection[4];
    int DMaxSeats;
} SCourseInformation, *SCourseInformationRef;

typedef struct STRUCT_INSTRUCTOR_INFORMATION_TAG{
    int DTerm;
    int DCRN;
    int DIID;
} SInstructorInformation, *SInstructorInformationRef;

typedef struct STRUCT_FULL_COURSE_INFORMATION_TAG{
    int DTerm;
    int DCRN;
    char DSubject[4];
    char DCourse[6];
    char DSection[4];
    int DMaxSeats;
    int DIID;
} SFullCourseInformation, *SFullCourseInformationRef;

typedef struct STRUCT_TERM_CRN_TAG{
    int DTerm;
    int DCRN;
} STermCRN, *STermCRNRef;

SVectorRef LoadCourseData(const char *filename);
SVectorRef LoadInstructorData(const char *filename);
SHashTableRef HashCourseData(SVectorRef coursedata);
SHashTableRef HashInstructorData(SVectorRef instructordata);
SVectorRef MergeCourseData(SHashTableRef coursedata,  SHashTableRef instructordata);

size_t TermCRNHash(const void *key);
int TermCRNCompare(const void *key1, const void *key2);
void *TermCRNCopy(const void *key);
void *CourseInformationCopy(const void *value);
void *InstructorInformationCopy(const void *value);


int main(int argc, char *argv[]){
    SVectorRef CourseData, InstructorInfo, MergedData;
    SHashTableRef CourseHash, InstructorHash;
    clock_t StartTime, EndCourseHashTime, EndInstructorHashTime, EndMergeTime, EndSortTime;
    double CourseHashTime, InstructorHashTime, MergeTime, SortTime;
    if(3 > argc){
        fprintf(stderr,"Syntax Error: proj4 coursefile instructorfile\n");
        return EXIT_FAILURE;
    }
    CourseData = LoadCourseData(argv[1]);
    if(!CourseData){
        fprintf(stderr,"Failed to load data from %s\n",argv[1]);
        return EXIT_FAILURE;
    }
    InstructorInfo = LoadInstructorData(argv[2]);
    if(!InstructorInfo){
        fprintf(stderr,"Failed to load data from %s\n",argv[2]);
        VectorDestroy(CourseData);
        return EXIT_FAILURE;
    }
    StartTime = clock();
    CourseHash = HashCourseData(CourseData);
    if(!CourseHash){
        fprintf(stderr,"Failed to hash course data\n");
        VectorDestroy(InstructorInfo);
        VectorDestroy(CourseData);
        return EXIT_FAILURE;
    }
    EndCourseHashTime = clock();
    InstructorHash = HashInstructorData(InstructorInfo);
    if(!InstructorHash){
        fprintf(stderr,"Failed to hash instructor data\n");
        HashTableDestroy(CourseHash);
        VectorDestroy(InstructorInfo);
        VectorDestroy(CourseData);
        return EXIT_FAILURE;
    }
    EndInstructorHashTime = clock();
    MergedData = MergeCourseData(CourseHash,InstructorHash);
    if(!MergedData){
        fprintf(stderr,"Failed to merge data\n");
        HashTableDestroy(InstructorHash);
        HashTableDestroy(CourseHash);
        VectorDestroy(InstructorInfo);
        VectorDestroy(CourseData);
        return EXIT_FAILURE;
    }
    EndMergeTime = clock();
    VectorSort(MergedData,TermCRNCompare);
    EndSortTime = clock();
    printf("term,crn,subj_code,course_num,section,max_seats,iid\n");
    for(size_t Index = 0; Index < VectorSize(MergedData); Index++){
        SFullCourseInformationRef CourseInfo = VectorElementAs(MergedData,Index,SFullCourseInformation);
        printf("%d,%d,%s,%s,%s,%d,%d\n",CourseInfo->DTerm,CourseInfo->DCRN,CourseInfo->DSubject,CourseInfo->DCourse,CourseInfo->DSection,CourseInfo->DMaxSeats,CourseInfo->DIID);
    }
    CourseHashTime = (double)(EndCourseHashTime - StartTime)/CLOCKS_PER_SEC;
    InstructorHashTime = (double)(EndInstructorHashTime - EndCourseHashTime)/CLOCKS_PER_SEC;
    MergeTime = (double)(EndMergeTime - EndInstructorHashTime)/CLOCKS_PER_SEC;
    SortTime = (double)(EndSortTime - EndMergeTime)/CLOCKS_PER_SEC;
    printf("Course    : %lf\n",CourseHashTime);
    printf("Instructor: %lf\n",InstructorHashTime);
    printf("Merge     : %lf\n",MergeTime);
    printf("Sort      : %lf\n",SortTime);
    VectorDestroy(MergedData);
    HashTableDestroy(InstructorHash);
    HashTableDestroy(CourseHash);
    VectorDestroy(InstructorInfo);
    VectorDestroy(CourseData);
    return EXIT_SUCCESS;
}

SVectorRef LoadCourseData(const char *filename){
    FILE *FP = fopen(filename,"r");
    if(FP){
        char TermHeader[16], CRNHeader[16], SubjHeader[16], CourseHeader[16], SectionHeader[16], MaxSeatsHeader[16];
        if(6 == fscanf(FP,"%4s,%3s,%9s,%10s,%7s,%9s\n",TermHeader,CRNHeader,SubjHeader,CourseHeader,SectionHeader,MaxSeatsHeader)){
            SVectorRef Vector = VectorCreate(sizeof(SCourseInformation));
            if(Vector){
                while(!feof(FP)){
                    SCourseInformation CourseInfo;
                    if(6 == fscanf(FP,"%6d,%5d,%3s,%5s%*[ ],%3s,%d\n",&CourseInfo.DTerm,&CourseInfo.DCRN,CourseInfo.DSubject,CourseInfo.DCourse,CourseInfo.DSection,&CourseInfo.DMaxSeats)){
                        if(!VectorElementAppend(Vector,&CourseInfo)){
                            VectorDestroy(Vector);
                            Vector = NULL;
                            break;
                        }
                    }
                }
                fclose(FP);
                return Vector;
            }

        }
        fclose(FP);
    }
    return NULL;
}

SVectorRef LoadInstructorData(const char *filename){
    FILE *FP = fopen(filename,"r");
    if(FP){
        char TermHeader[16], CRNHeader[16], IIDHeader[16];
        if(3 == fscanf(FP,"%4s,%3s,%3s\n",TermHeader,CRNHeader,IIDHeader)){
            SVectorRef Vector = VectorCreate(sizeof(SInstructorInformation));
            if(Vector){
                while(!feof(FP)){
                    SInstructorInformation InstructorInfo;
                    if(3 == fscanf(FP,"%6d,%5d,%d\n",&InstructorInfo.DTerm,&InstructorInfo.DCRN,&InstructorInfo.DIID)){
                        if(!VectorElementAppend(Vector,&InstructorInfo)){
                            VectorDestroy(Vector);
                            Vector = NULL;
                            break;
                        }
                    }
                }
                fclose(FP);
                return Vector;
            }
        }
        fclose(FP);
    }
    return NULL;
}

const SHashTableCreateData CourseHashData = {TermCRNHash, TermCRNCompare, TermCRNCopy, free, CourseInformationCopy, free};

SHashTableRef HashCourseData(SVectorRef coursedata){
    SHashTableRef CourseHash = HashTableCreate(&CourseHashData);
    if(CourseHash){
        size_t CourseCount = VectorSize(coursedata);
        for(size_t Index = 0; Index < CourseCount; Index++){
            void *CourseInfo = VectorElement(coursedata,Index);
            if(!HashTableInsert(CourseHash,CourseInfo,CourseInfo)){
                HashTableDestroy(CourseHash);
                CourseHash = NULL;
                break;
            }
        }
    }
    return CourseHash;
}

const SHashTableCreateData InstructorHashData = {TermCRNHash, TermCRNCompare, TermCRNCopy, free, InstructorInformationCopy, free};

SHashTableRef HashInstructorData(SVectorRef instructordata){
    SHashTableRef InstructorHash = HashTableCreate(&InstructorHashData);
    if(InstructorHash){
        size_t InstructorCount = VectorSize(instructordata);
        for(size_t Index = 0; Index < InstructorCount; Index++){
            void *InstructorInfo = VectorElement(instructordata,Index);
            if(!HashTableInsert(InstructorHash,InstructorInfo,InstructorInfo)){
                HashTableDestroy(InstructorHash);
                InstructorHash = NULL;
                break;
            }
        }
    }
    return InstructorHash;
}

SVectorRef MergeCourseData(SHashTableRef coursedata,  SHashTableRef instructordata){
    SVectorRef MergedVector = VectorCreate(sizeof(SFullCourseInformation));
    SHashTableIteratorRef Iterator = HashTableBegin(coursedata);
    SFullCourseInformation TempCourseInfo;
    SInstructorInformationRef InstructorInfo;

    while(Iterator != HashTableEnd(coursedata)){
        memcpy(&TempCourseInfo, HashTableIteratorValue(Iterator), sizeof(SCourseInformation));
        InstructorInfo = (SInstructorInformationRef)HashTableGet(instructordata,&TempCourseInfo);
        if(InstructorInfo){
            TempCourseInfo.DIID = InstructorInfo->DIID;
            VectorElementAppend(MergedVector,&TempCourseInfo);
        }
        Iterator = HashTableIteratorNext(Iterator);
    }
    return MergedVector;
}

size_t TermCRNHash(const void *key){
    STermCRNRef TermCRN = (STermCRNRef)key;
    return TermCRN->DCRN ^ TermCRN->DTerm;
}

int TermCRNCompare(const void *key1, const void *key2){
    return memcmp(key1,key2,sizeof(STermCRN));
}

void *TermCRNCopy(const void *key){
    void *RetValue = malloc(sizeof(STermCRN));
    memcpy(RetValue,key,sizeof(STermCRN));
    return RetValue;
}

void *CourseInformationCopy(const void *value){
    void *RetValue = malloc(sizeof(SCourseInformation));
    memcpy(RetValue,value,sizeof(SCourseInformation));
    return RetValue;
}

void *InstructorInformationCopy(const void *value){
    void *RetValue = malloc(sizeof(SInstructorInformation));
    memcpy(RetValue,value,sizeof(SInstructorInformation));
    return RetValue;
}


