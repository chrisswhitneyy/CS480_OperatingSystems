/***************
HW5 Race Conditions: The purpose of this program is to demonstrate race conditions with a common resources, in this case the text file db.txt. The program forks 10 times an executable that adds and removes a record 10 time to the db.txt file. 
 @Authors: Clarissa Calderon and Chris Whitney
 ****************/

/***************
 Preprocessor directives
 ****************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

/*File paths of the database and the executable which adds and removes a record 10 times, then finally adds it once. */
#define DATABASE_FILE "db.txt"
#define EXECUTABLE "/Users/cdubs/Documents/xCodeProjects/cs480_hw5/cs480_hw5/child"

/***************
Type definition
****************/
typedef struct {
    char name[80];
    char number[10];
} PhoneBookRecord;

/***************
Prototype declarations
****************/
void printDB();

int main(int argn,char* argv[]) {
    
    int status = 0;
    int i = 10;
    int return_from_wait;
    
    if (argn==1) {
        printf("ERROR: Not enough arguments \n");
        return -1;
    }
    
    
    for (int i=0; i<10; i++) {
        pid_t pid;
        pid = fork(); //creates a new processes
        if (pid == 0){
            /* This is the child process, which calls the executable defined in the preprocessor directives. */
            execl (EXECUTABLE,EXECUTABLE,argv[1],NULL);
        }
        else if (pid < 0){
            /* The fork failed. Print error. */
            status = -1;
            printf("ERROR: %s \n",strerror(errno));
            return -1;
        }
    }
    
    while (i--) {
        return_from_wait = wait(&status);
        if (return_from_wait == -1) {
            i++;
        }else{
            printf("Process %d returned \n",return_from_wait);
        }
    }
    printDB();
    return status;

}

/****************
 printDB: This function simply prints out the content of the database to the standard output. This accomplished by looping record at a time through the file until the number of read reacord is equal to the number of record that exist.
 ****************/
void printDB() {
    
    int total;
    int record_count;
    int read_records = 0;
    PhoneBookRecord temp_record;
    
    int fd = open(DATABASE_FILE,O_RDWR,S_IREAD);
    if (fd<0) {
        printf("File not found \n");
    }
    //reads in record_count from the front of the db
    lseek(fd, 0, SEEK_CUR);
    read(fd, &record_count, sizeof(int));
    
    //calculates the total size of all the records
    total = record_count * sizeof(PhoneBookRecord);
    
    printf("======DATABASE======\n");
    //lops through each record and prints them out
    while (read_records<record_count) {
        read(fd, &temp_record, sizeof(temp_record));
        printf("Name: %s Number: %s \n",temp_record.name,temp_record.number);
        read_records++;
    }
 
}







