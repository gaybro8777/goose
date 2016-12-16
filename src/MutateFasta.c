#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "defs.h"
#include "args.h"
#include "misc.h"
#include "parser.h"
#include "buffer.h"

int main(int argc, char *argv[]){
  uint32_t k, i;
  int s, r, seed = 0, nSymbols = 4;
  double mutationRate = 0, deletionRate = 0, insertionRate = 0;
  PARSER *PA = CreateParser();
  char *bases = "ACGT";
  BUF *B;

  if(ArgBin(0, argv, argc, "-h")){
    fprintf(stderr, "Usage: %s < input > output\n", argv[0]);
    fprintf(stderr, " Arguments:                   \n");
    fprintf(stderr, " -s  <seed>                   \n");
    fprintf(stderr, " -mr <mutationRate>  (def:0.0)\n");
    fprintf(stderr, " -dr <deletionRate>  (def:0.0)\n");
    fprintf(stderr, " -ir <insertionRate> (def:0.0)\n");
    fprintf(stderr, " -a5 (use ACGTN alphabet)     \n");
    fprintf(stderr, "It works on FASTA domain.     \n");
    return EXIT_SUCCESS;
    }

  seed          = ArgNum(0, argv, argc, "-s", 1, UINT_MAX);
  mutationRate  = ArgDbl(0, argv, argc, "-mr");
  deletionRate  = ArgDbl(0, argv, argc, "-dr");
  insertionRate = ArgDbl(0, argv, argc, "-ir");
  if(ArgBin(0, argv, argc, "-a5") == 1){
    bases = "ACGTN";
    nSymbols = 5;
    }

  FileType(PA, stdin);
  if(PA->type != 1){
    fprintf(stderr, "ERROR: This is not a FASTA file!\n");
    exit(1);
    }

  srand(seed);
  B = CreateBuffer(BUF_SIZE);
  while((k = fread(B->buf, 1, B->size, stdin)))
    for(i = 0 ; i < k ; ++i){

      if(nSymbols == 4){
        if(ParseSym(PA, (s = B->buf[i])) == -1){
          putchar(s);
          continue;
          }
        }
      else{
        if(ParseSymN(PA, (s = B->buf[i])) == -1){
          putchar(s);
          continue;
          }
        }
       
      if(rand() / (RAND_MAX + 1.0) < mutationRate){
        while((r = bases[rand() % nSymbols]) == s)
          ;
        putchar(r);
        continue;
        }
      if(rand() / (RAND_MAX + 1.0) < deletionRate)
        continue;
      if(rand() / (RAND_MAX + 1.0) < insertionRate)
        putchar(bases[rand() % nSymbols]);
      putchar(s);
      }
  RemoveBuffer(B);
  RemoveParser(PA);
  return 0;
  }
