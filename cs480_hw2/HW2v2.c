#include <stdio.h>
#include <stdlib.h>

char** string_central(char*(*array_ptr[])(char*), char* str);

char* char_double(char*);
char* char_shift(char*);
char char_flip(char*);

int main(int argc, char*argv[]){
	char*(array_ptr[])(char*) = {char_double, char_shift, char flip}; //Array of functions
	char* input = (argv+1);
    char* a = string_central(array_ptr, input);
    while (*a)
    {
        printf("%s\n", *(a++));
    }
    printf("\n");
    return 0;
}

char**string_central(char*(*array_ptr[])(char*), char* str)
{
    char** amount = (char**)malloc(1024);
    char** return_amount = amount;
    while (*array_ptr)
    {
        (*amount++) = (*(array_ptr)++)(str);
    }
    *amount = NULL;
    return return_amount;
}
    


