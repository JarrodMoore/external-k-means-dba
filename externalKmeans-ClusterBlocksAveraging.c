#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
//#include <fcntl.h>
#define lineMem 512*3
#define iterLimit 1000
#define delims ",\n"
int bufferFillCount;

int fillBuffer(FILE* inputFile, unsigned long fileSize, char* buffer){
  bufferFillCount += 1;
  unsigned int lastRead = 0, breakMe = 0;
  unsigned long readAmt=0, readLen=0;
  unsigned long p=0, q=0, r=0;
  readAmt = memBlock - sizeof(char);
        if(readAmt > (fileSize - ftell(inputFile))){
          readAmt = fileSize - ftell(inputFile);
          lastRead = 1;
        }
        readLen = fread(buffer, sizeof(char),  readAmt,  inputFile);
        buffer[readLen] = '\0';
        if(!lastRead){
          for(p = strlen(buffer)-1; p>=0; --p){
            for(r=0; r<strlen(delims); ++r){
              if(buffer[p] == delims[r]){
                buffer[p] = '\0';
                breakMe = 1;
                break;
              }
            }
            if(breakMe){
              breakMe = 0;
              break;
            }
          }
          q = readLen-p-1;
          fseek(inputFile, -q, SEEK_CUR);
        }
  return lastRead;
}

double euclidDist(double* p1, double* p2, long dim){
  double accumulator=0;
  long i;
  for(i=0; i<dim; ++i)
    accumulator+=pow(p1[i]-p2[i], 2);
  return sqrt(accumulator);
}

int findClosest(double* p1, double* centers, long dim, long k){
  long closest = 0, i;
  double minDist=0, testDist=0;
  minDist = euclidDist(p1, centers, dim);
  for(i=1; i<k; ++i){
    testDist = euclidDist(p1, centers+(i*dim), dim);
    if( minDist > testDist){
      closest = i;
      minDist = testDist;
    }
  }
  return closest;
}

int centerDiff(double* centers1, double* centers2, double tol, long k, long dim){
  long i;
  double accum=0;
  for(i=0; i<k; ++i){
    accum += euclidDist(centers1+(i*dim), centers2+(i*dim), dim);
  }
  if(accum > tol)
    return 1;
  else
    return 0;
}


int main(int argc, char* argv[]){
  bufferFillCount =0;
  FILE *inputFile; //done
  int breakMe = 0, lastRead = 0, iterCount=0, firstIter=1, initReadCount;
  char *buffer, *tmpChar; //done
  long i=0, j=0, l=0, colAmt=0, rowAmt=0, fileSize=0, k=atoi(argv[2]);
  long closePoint = 0;
  double *M, *centers1, *centers2, *tmpCenters, *centerWeights, *averagedCenters, *averagedWeights; //done
//Variable allocation and initialization
  inputFile = fopen(argv[1], "r");
  buffer = malloc(memBlock);
  M = malloc(memBlock);
  srand(time(NULL));
  fgets(buffer, memBlock, inputFile);
  tmpChar = strtok(buffer, delims);
  while(tmpChar != NULL){
    ++colAmt;
    tmpChar = strtok(NULL, delims);
  }
  rowAmt = memBlock/(sizeof(double)*colAmt);
  fseek(inputFile, 0, SEEK_END);
  fileSize = ftell(inputFile);
  rewind(inputFile);
  centers1 = (double*) malloc(sizeof(double)*k*colAmt);
  centers2 = (double*) malloc(sizeof(double)*k*colAmt);
  averagedCenters = (double*) malloc(sizeof(double)*k*colAmt);
  centerWeights = (double*) malloc(sizeof(double)*k);
  averagedWeights = (double*) malloc(sizeof(double)*k);
/*******SEEDING**********************/
  tmpChar = NULL;
  lastRead = 0;
  for(i=0; i<k;++i){
    averagedWeights[i] = 0;
  }
  for(i=0; i<k*colAmt;++i){
    averagedCenters[i] = 0;
  }
  // lastRead = fillBuffer(inputFile, fileSize, buffer);
  // tmpChar = strtok(buffer, delims);

  do{
    do{
      for(i=0;i<(rowAmt*colAmt); ++i){
        if(tmpChar == NULL){
          if(lastRead){
            breakMe=1;
            break;
          }
          else{
            lastRead  = fillBuffer(inputFile, fileSize, buffer);
            tmpChar = strtok(buffer, delims);

          }
        }
        // printf("%s\n", tmpChar);
        // fflush(stdout);
        M[i] = atof(tmpChar);
        tmpChar = strtok(NULL, delims);
      }
    }while(i<(rowAmt*colAmt) && !breakMe);
    for(j=0; j<k; ++j){
      centerWeights[j] = 0;
      memmove(centers1+(j*colAmt), M+((rand()%(i/colAmt))*colAmt), sizeof(double)*colAmt);
      memmove(centers2+(j*colAmt), centers1+(j*colAmt), sizeof(double)*colAmt);
    }
    do{
      for(j=0; j<(i/colAmt); ++j){
        closePoint = findClosest(M+(j*colAmt), centers1, colAmt, k);
        for(l=0; l<colAmt; ++l){
          centers2[closePoint*colAmt+l] = ((centers2[closePoint*colAmt+l]
              * centerWeights[closePoint]) + M[j*colAmt + l]) / (centerWeights[closePoint] +1);
        }
        ++centerWeights[closePoint];
      }
      for(l=0;l<k;++l)
        centerWeights[l] = 0;
      tmpCenters = centers1;
      centers1 = centers2;
      centers2 = tmpCenters;
      iterCount += 1;
                  //printf("%d : %zu : %lu\r", iterCount, ftell(inputFile), fileSize);

      if(iterCount>iterLimit) break;
    }while(centerDiff(centers1, centers2, .01, k, colAmt));
    iterCount = 0;
    /*****Average the centers here*************/
    if(firstIter){
      for(i=0; i<k;++i){
        memmove(averagedCenters+(i*colAmt), centers1+(i*colAmt), sizeof(double)*colAmt);
        firstIter=0;
      }
    }
    else{
      for(i=0; i<k; ++i){
        closePoint = findClosest(centers1+(i*colAmt), averagedCenters, colAmt, k);
        for(j=0; j<colAmt; ++j){
          averagedCenters[closePoint*colAmt+j] = ((averagedCenters[closePoint*colAmt+j]
              * averagedWeights[closePoint])
               + centers1[closePoint*colAmt + j]) 
          / (averagedWeights[closePoint] + 1);
        }
        ++averagedWeights[closePoint];
      }

    }

  }while(!breakMe);

  for(i=0; i<k; ++i){
      centerWeights[i] = 0;
      memmove(centers1+(i*colAmt), averagedCenters+(i*colAmt), sizeof(double)*colAmt);
      memmove(centers2+(i*colAmt), centers1+(i*colAmt), sizeof(double)*colAmt);
    }

  // printf("---CENTERS---\n");
  // for(j=0;j<k; ++j){
  //   printf("%lu: ", j+1);
  //   for(l=0; l<colAmt; ++l)
  //     printf("%f ", averagedCenters[j*colAmt + l]);
  //   printf("\n");
  // }
  initReadCount = bufferFillCount;
/********CLUSTERING**********************/  
  rewind(inputFile);
  tmpChar = NULL;
  lastRead = 0;
  breakMe = 0;
  iterCount = 0;
    do{
      do{
        for(i=0;i<(rowAmt*colAmt); ++i){
          if(tmpChar == NULL){
            if(lastRead){
              breakMe=1;
              break;
            }
            else{
              //printf("%zu : %lu\r", ftell(inputFile), fileSize);
              lastRead  = fillBuffer(inputFile, fileSize, buffer);
              tmpChar = strtok(buffer, delims);
            }
          }
          M[i] = atof(tmpChar);
          tmpChar = strtok(NULL, delims);
        }
        for(j=0; j<(i/colAmt); ++j){
          closePoint = findClosest(M+(j*colAmt), centers1, colAmt, k);
          for(l=0; l<colAmt; ++l){
            centers2[closePoint*colAmt+l] = ((centers2[closePoint*colAmt+l]
                * centerWeights[closePoint]) + M[j*colAmt + l]) / (centerWeights[closePoint] +1);
          }
          ++centerWeights[closePoint];
        }
      }while(!breakMe);

      for(i=0;i<k;++i)
        centerWeights[i] = 0;
      tmpCenters = centers1;
      centers1 = centers2;
      centers2 = tmpCenters;
      breakMe = 0;
      lastRead = 0;
      rewind(inputFile);
      ++iterCount;
      // if(iterCount > iterLimit){
      //   printf("ITERLIMITBREAK\n");
      //   break;
      // }
    }while(centerDiff(centers1, centers2, .0001, k, colAmt));
   printf("---CENTERS---\n");
   for(j=0;j<k; ++j){
      printf("%lu: ", j+1);
     for(l=0; l<colAmt; ++l)
       printf("%f ", centers2[j*colAmt + l]);
     printf("\n");
    }
  //printf("%d ", initReadCount);
  printf("%d\n", bufferFillCount);

  // printf("Memory Row Capacity: %zu\n", rowAmt);
  // printf("Memory Col Capacity: %zu\n", colAmt);
  fclose(inputFile);
  free(buffer);
  free(M);
  free(centers1);
  free(centers2);
  free(centerWeights);
  free(averagedCenters);
  free(averagedWeights);
  return 1;
}
