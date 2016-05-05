//
//  main.c
//  CS480_HW2
//
//  The purpose of this assignment is to demonstrate how to use pointers to functions.
//  In the main function an array of functions is creates {char_double,char_shift,char_flip}
//  a string is then passed in through the commnand line through agrv, these two arrays are
//  then passed into central_string which evaluates them.
//
//  Created by Clarissa Calderon & Christopher Whitney on 2/15/16.
//  Copyright © 2016 Clarissa Calderon & Christopher Whitney. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ASCIIZ '\0'

char** string_central(char*(*array_ptr[])(char*), char* str);
char* char_double(char*);
char* char_shift(char*);
char* char_flip(char*);

int main(int argc, char*argv[]){
    char* strs = *((argv+1)); //increments argv to ignore file path
    char*(*func_ptr[4])(char*) = {char_double, char_shift, char_flip,NULL}; //declares array of ptrs to functions
    char** result = string_central(func_ptr, strs);//calls string_central on func_ptr, and argv strs
    while (*result!=NULL) {//prints out the content of the return value from string_central
        printf("%s",*result);
        printf("\n");
        result++;
    }
    return 0;
}
/*string_central: is the central function in this program, because it takes in an array of function
 pointers and a string then it runs all of the functions on the strings then returns all the results*/
char**string_central(char*(*func_ptr[])(char*), char* str)
{
    char** out_str = (char**)malloc(1024);//allocates memory
    char** return_out = out_str; //re-assigns address of out_str to return_out
    while (*func_ptr!=NULL){ //loops through the func_ptr array until null terminated
        char* value = (*func_ptr++)(str); //calls the first element of that array of str and post increments
        *out_str++ = value; //assigns that return value from the func to the out_str
    }//end of while
    out_str++; //increments out_str
    out_str = NULL; //assigns null to end of out_str
    return return_out; //returns address of the begining of out_str
}
/*char_double: this function takes in a ptr to a string and doubles
every char in that string, then returns a ptr to the new string*/
char* char_double(char* str){
    char* value = (char*) malloc(80);
    char* return_value = value;
    char* cur = str;
    while (*cur!=0) {
        *value++ = *cur;
        *value++ = *cur++;
    }
    value = ASCIIZ;
    return return_value;
}
/*char_shift: this function takes in a ptr to a string and then forever
 other char the ascii value is incremented */
char* char_shift(char* str){
    char* value = (char*) malloc(80); //allocates memory
    char* return_value = value; //assigns the address of value to return
    char* cur = str;//defines cur and assigns it to str
    for (int i=0; i<strlen(str); i++) { //loops through the length of str
        *value++ = *cur; //increments value and assigns the content cur to it
        if (i%2==0) { //if its an even char in the string
            int ascii_value = (int) ((char)*cur);//converts char to ascii dec number
            ascii_value++;  //increments the ascii_value
            char new_char = (char) ascii_value; //converts ascii_value back to char
            *value++ = new_char;//increments value and assigns new_char to it
        }
        cur++;//increments current
    }//end for
    value = ASCIIZ;
    return return_value;
}
/*char_flip: this function takes in string and converts any lowercase letter to 
 uppercase and vice versa*/
char* char_flip(char* str){
    char* value = (char*) malloc(80);//locates memory
    char* return_value = value; //return address
    char* cur = str; //sets current to str
    for (int i=0; i<strlen(str); i++) {
        int ascii_value = (int)((char)*cur); //converts char to ascii dec number
        if (ascii_value>=97 && ascii_value<=122) {//checks with the ascii code is lower case
            ascii_value=ascii_value-32; //substracts 32 to convert to upercase
        }else {//otherwise its uppercase or not a letter
            ascii_value=ascii_value+32; //adds 32 to convert to lowercase
        }
        char new_char = (char) ascii_value; //converts ascii dec number back to char
        *value++ = new_char; //increments value and assigns new_char to it
        cur++; //increments current
        
    }//end for
    value = ASCIIZ;
    return return_value;
}
