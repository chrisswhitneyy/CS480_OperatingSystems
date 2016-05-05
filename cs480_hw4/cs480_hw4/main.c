/*
    main.c
    cs480_hw4
 
    Purpose: The purpose of this program is to implement 
    simple phone book record datbase which uses a text file
    to store and retreve info based on a given number.
    This is accomplished by using "special char" which represent
    the beginning and end of the record, name and number.
    
    The special char are the following:
    { - represents the begining of a record
    : - represents the begining of a name
    # - represents the begining of a number
    < - represents the end of a record/number
    
    Created by Christopher Whitney & Clarissa Calderon on 3/28/16.
    Copyright © 2016 All rights reserved.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ASCIIZ "\0"

//The struct for the phone book record
typedef struct phone_book_record {
    char name[80];
    char number[11];
} PhoneBookRecord;//typedef

void add_record(PhoneBookRecord* a_record);
int get_record(char* number, PhoneBookRecord* returned_record);
int remove_record(char* number);
void printDB();
char* db_file_path;

int main(int argc, char * argv[]) {
    
    db_file_path = argv[1];//set global file path
    
    /*Below is code which demostrates the functionality of this program.
     The function get_record is not called directly, however it is called
     from within the remove_record function to check if it exists */
    
    //adds 5 records to db
    PhoneBookRecord suhdude;
    // adding records to the db
//    strcpy(suhdude.name, "Suh Dude1");
//    strcpy(suhdude.number, "5206783055");
//    add_record(&suhdude);
//    strcpy(suhdude.name, "Suh Dude2");
//    strcpy(suhdude.number, "5206783044");
//    add_record(&suhdude);
//    strcpy(suhdude.name, "Suh Dude3");
//    strcpy(suhdude.number, "5206783043");
//    add_record(&suhdude);
//    strcpy(suhdude.name, "Suh Dude4");
//    strcpy(suhdude.number, "5206783042");
//    add_record(&suhdude);
//    strcpy(suhdude.name, "Suh Dude5");
//    strcpy(suhdude.number, "5206783041");
//    add_record(&suhdude);

    get_record("5206783055",&suhdude); //deletes 2 entry
//    get_record("5206783055",&suhdude);
//    get_record("5206783043",&suhdude);
    printDB();//prints db
    return 0;
}
/*Add_record is a function which talks in a record and adds
 it to the "database" text file. If the file does not exist
 it also creates the text file*/
void add_record(PhoneBookRecord* a_record){
    //gets db text file handle
    int db_handle = open(db_file_path,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
    if (db_handle<0) {
        printf("ERROR: File not found \n");
        close(db_handle); //closes file
    }else{
    
        //Uses the stat system call to get files size
        struct stat fileStat; //declares the fileStat struct
        stat(db_file_path, &fileStat);//calls stat get file info
        int size = (int)fileStat.st_size;//get size
        
        //gets length info and converts it to str
        int name_length = (int) strlen(a_record->name);
        int num_length = (int) strlen(a_record->number);
        
        /*  writes the content of the struct to the file using the special chars 
            if the file is not empty then the content needs to be shifted and the
            new record added to the front */
        if (size==0) {
            write(db_handle, "{", 1);
            write(db_handle, ":",2);
            write(db_handle, a_record->name, name_length);
            write(db_handle, "#",2);
            write(db_handle, a_record->number, num_length);
            write(db_handle, "<", 1);
            close(db_handle);
        }else{
            char* old_file[size];
            read(db_handle, old_file, size);
            lseek(db_handle, 0, SEEK_SET);
            write(db_handle, "{", 1);
            write(db_handle, ":",2);
            write(db_handle, a_record->name, name_length);
            write(db_handle, "#",2);
            write(db_handle, a_record->number, num_length);
            write(db_handle, "<", 1);
            write(db_handle, old_file, size);
            close(db_handle);
        }
        printf("SUCCESFULL ADDED RECORD\n");
        printf("Name:%s \n",a_record->name);
        printf("Number:%s \n",a_record->number);
    }//end of else
}//end of add_record()

/*printDB: prints out all the records in the db text file*/
void printDB(){
    //get file handle
    int db_handle = open(db_file_path,O_RDONLY,S_IREAD);
    if (db_handle<0) {
        printf("ERROR: File not found \n");
        close(db_handle); //closes file
    }else{
        printf("====DATABASE====\n");
        //Uses the stat system call to get files size
        struct stat fileStat; //declares the fileStat struct
        stat(db_file_path, &fileStat);//calls stat get file info
        int size = (int)fileStat.st_size;//get size
        //loops through the entire file
        for (int i=0; i<size; i++) {
            char buffer[1];
            read(db_handle, buffer, 1);
            //buffer hits a new record
            if (buffer[0]=='{') {
                //loops until it hits the name char
                while (buffer[0]!=':') { read(db_handle, buffer, 1);}
                printf("NAME:");
                //loops through the name until it hits the number char
                while (buffer[0]!='#') {
                    read(db_handle, buffer, 1);
                    if (buffer[0]!='#') {
                        printf("%c",buffer[0]); //stdout the name char
                    }
                }//end of while
                printf("  NUMBER: ");
                //loops until it hits the end number char
                while(buffer[0]!='<'){
                    read(db_handle, buffer, 1);
                    if (buffer[0]!='<') {
                        printf("%c",buffer[0]); //stdout numbers
                    }
                }//end of while
                printf("\n");
            }//end of id
        }//end of for
        close(db_handle);//close file
    }//end of else
}//end of printDB()

/*get_record: gets a given PhoneBookRecord based on a number */
int get_record(char* number, PhoneBookRecord* returned_record){
    int db_handle = open(db_file_path,O_RDONLY,S_IREAD); //get file handle
    if (db_handle<0) {
        printf("ERROR: File not found \n");
        return -1;
    }else{
        //Uses the stat system call to get files size
        struct stat fileStat; //declares the fileStat struct
        stat(db_file_path, &fileStat);//calls stat get file info
        int size = (int)fileStat.st_size;//get size
        //loops through the entire file
        for (int i=0; i<size+1; i++) {
            char buffer[1];
            read(db_handle, buffer, 1);
            //buffer hits a new record
            if (buffer[0]=='{') {
                
                while (buffer[0]!=':') {read(db_handle, buffer, 1);}

                char* db_name = malloc(80);
                int* name_length = malloc(2);
                //loops through the name until it hits the number char
                while (buffer[0]!='#') {
                    read(db_handle,buffer,1);
                    if (buffer[0]!='#') {
                        *db_name = buffer[0];
                        db_name++;
                        (*name_length)++;
                    }
                }
                
                //db_name= db_name-name_length;//resets db_name pt
                db_name= db_name-(--(*name_length));
                char* db_num = malloc(10);
                //loops through the name until it hits the end number char
                while (buffer[0]!='<') {
                    read(db_handle, buffer, 1);
                    if (buffer[0]!='<') {
                        *db_num = buffer[0];
                        db_num++;
                    }
                }
                db_num = db_num-10; //resets the db_num address
                //checks if db number matchs arg number
                
                if (strcmp(db_num, number)==0) {
                    //copys number and name into return_record
                    strcpy(returned_record->name,db_name);
                    strcpy(returned_record->number,db_num);
                    //prints out info
                    printf("===RECORD FOUND===\n");
                    printf("Name: %s\n",db_name);
                    printf("Number: %s\n",db_num);
                    close(db_handle);//close file
                    return 0;
                }
            }
        }
        close(db_handle);//close file
        printf("No records found with the number: %s \n",number);
        return -1;
    }//end of else
    return -1;
}
/*  @remove_record:removes the entry with the given number from the database.
    close the gap, if there is one left,print the deleted entry's full information
    to stdout for feedback return 0 upon success, -1 otherwise (i.e. there is no
    record with that number)
*/
int remove_record(char* number){
    //gets db text file handle
    int db_handle = open(db_file_path,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
    PhoneBookRecord* return_record = malloc(91);
    if (get_record(number, return_record)!=0) {
        return -1;
    }else if(db_handle<0){
        printf("FILE NOT FOUND \n");
        return -1;
    }
    //Uses the stat system call to get files size
    struct stat fileStat; //declares the fileStat struct
    stat(db_file_path, &fileStat);//calls stat get file info
    int size = (int)fileStat.st_size;//get size
    //loops through the entire file
    for (int i=0; i<size+1; i++) {
        char buffer[1];
        read(db_handle, buffer, 1);
        //buffer hits a new record
        if (buffer[0]=='{') {
            //loops until it hits the start of the name char
            while (buffer[0]!='#') {read(db_handle, buffer, 1);}
            
            char* db_num = malloc(10);
                       //loops through the name until it hits the number char
            while (buffer[0]!='<') {
                read(db_handle,buffer,1);
                if (buffer[0]!='<') {
                    *db_num = buffer[0];
                    db_num++;
                }
            }
            db_num = db_num-10;//resets db_num pt to point to first number
            
            //db_num matchs the return_records number
            if (strcmp(db_num, return_record->number)==0) {
                //location at the end of the deleted record
                long end_del = lseek(db_handle, 1, SEEK_CUR);//fp at the end of the deleted record
                lseek(db_handle, end_del, SEEK_SET);//move fp to end of del record
                long left = size-end_del;//the size of the content to the right of the deleted record
                long record_size = strlen(return_record->name)+strlen(return_record->number)+4; //gets the total size of the record
                
                char white_space[record_size];
                //loop to create white space
                for (int i=0; i<record_size+2; i++) {
                    white_space[i] = ' ';
                }
                
                if (left<0) {
                    lseek(db_handle,end_del-3, SEEK_SET);//set fp to begining of deleted record
                    write(db_handle, white_space, record_size);
                    
                }else{
                    char buffer_end[left]; //buffer for the end of the file
                    read(db_handle, buffer_end, left); //reads in the end
                    lseek(db_handle,end_del-record_size, SEEK_SET);//set fp to begining of deleted record
                    write(db_handle, buffer_end, left);//rewrites the end
                    write(db_handle, white_space, record_size);
                    lseek(db_handle, -record_size, SEEK_CUR);
                }
                
                close(db_handle);
                printf("===SUCCESSFULLY REMOVED=== \n");
                printf("Name: %s \n",return_record->name);
                printf("Number: %s \n",return_record->number);
                return 0;
            }
            
        }
    }
    return -1;
}

