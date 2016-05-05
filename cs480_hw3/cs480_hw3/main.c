//
//  main.c
//  cs480_hw3
//  Purpose: The purpose of this program is to take in two file paths
//  a source and destination taking the blocks of the source reversing
//  them into the destination file.
//  Created by Christopher Whitney on 3/20/16.
//  Copyright © 2016 Christopher Whitney and Clarissa Calderon.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 3

char *strrev(char *str);

int main(int argc, char *argv[]){
     
     //Open source and destination files
     int source = open(argv[1],O_RDONLY);
     int destin = open(argv[2],O_CREAT|O_WRONLY,S_IREAD|S_IWRITE);
     //An error accured when opening
     if (source<0) {
          printf("ERROR: Source file does not exist");
          exit(EXIT_FAILURE);
     }
     
     //Uses the stat system call to get files block size
     struct stat fileStat; //declares the fileStat struct
     stat(argv[1], &fileStat); //calls stat which gets info on the source file
     //access size from stat struct and assigns it to flsize
     int size = (int)fileStat.st_size;
     
     //Number of blocks and blocks read
     int blk = size/BLOCK_SIZE;
     int blkread = 0;
     //Loops through the source file
     for (int i=0; i<blk+1; i++) {
          char* data[1];
          //Adjusts file pointer
          lseek(source,size-BLOCK_SIZE*blkread,SEEK_SET);
          read(source,data,BLOCK_SIZE);
          //Here is where the reverse function call
          data[0]=strrev(data[0]);
          write(destin,data,BLOCK_SIZE);
          blkread++;
     }
     //If overflow writes the first char, because size not divisible by 2
     if (size%2==1 || BLOCK_SIZE%2==1) {
          char* data[1];
          lseek(source, 0, SEEK_SET);//sets file pointer to the front
          read(source,data,1); //reads in the overflow byte
          write(destin,data,1); //writes the overflow byte
     }
     close(source);
     close(destin);
     exit(EXIT_SUCCESS);
}
char *strrev(char *str){
     char *p1, *p2;
     if (!str)
          return str;
     for (p1=str,p2=str+strlen(str)-1; p2>p1; ++p1,--p2){
          *p1 ^= *p2;
          *p2 ^= *p1;
          *p1 ^= *p2;
     }
     return str;
}

