#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "misc.h"
#include "mem.h"
#include "buffer.h"

int main(int argc, char *argv[]){
  uint32_t k, i;
  uint8_t  s, header = 1;
  BUF *B;

  if(argc != 1){
    fprintf(stderr, "Usage: %s < input.fasta > output\n"
    "It shows read information of a FASTA or Multi-FASTA file format.\n", argv[0]);
    return EXIT_SUCCESS;
    }

  B = CreateBuffer(BUF_SIZE);

  while((k = fread(B->buf, 1, B->size, stdin)))
    for(i = 0 ; i < k ; ++i){
      s = B->buf[i];
      if(s == '>'){ header = 1; continue; }
      if(s == '\n' && header == 1){ header = 0; continue; }
      if(s == '\n') continue;
      if(header == 1) continue;
      if(s < 65 || s > 122) continue;


      putchar(s);
      }

  RemoveBuffer(B); 
  return EXIT_SUCCESS;
  }

