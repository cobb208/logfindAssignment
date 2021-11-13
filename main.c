#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>

#include "dbg.h"

#define MAX_WORD_SIZE 100

int get_file_char_count(FILE *f, char *filename);
int searchForKeyword(char *keyword, char *check_string);
int findLogFilename(char *filename);
int setIsOr(int argc, char *argv[]);
char** logFileNames();

int main(int argc, char *argv[])
{
    check((argc == 0), "Arguements required, please enter the search terms you want to look for in the log\nUse the -o to search for terms in one file.");

    unsigned int is_or = setIsOr(argc, argv); // set the and/or operator from -o flag
    FILE *fp;

    
    // Gets all file names in the log folder that end with .log
    char **logfile = logFileNames();
    check(logfile == NULL, "Failed to open directory and find log files!");
    
    int i = 0;
    while(logfile[i] != NULL)
    {

        char f_path[100];
        strcpy(f_path, "./log/");
        strcat(f_path, logfile[i]);


        fp = fopen(f_path, "r");
        int file_length = get_file_char_count(fp, logfile[i]);

        rewind(fp);

        char *file_contents = malloc(sizeof(char) * file_length); // dynamically allocated because of file size
        fgets(file_contents, file_length, fp); // write the stream to the variable

        if(is_or)
        {
            int j = 1; 
            for(j = 1; j < argc; j++)
            {
                int result = searchForKeyword(argv[j], file_contents);
                if(result == 0) {
                    printf("Match has been found in file: %s\n", logfile[i]);
                    break;
                }
            }
        } else {
            int j = 1;
            int found_all = 0; // bool to hold if all words were found in file
            for(j = 1; j < argc; j++)
            {
                int result = searchForKeyword(argv[j], file_contents);
                if(result == 0) {
                    found_all = 1;
                } else {
                    found_all = 0;
                    break;
                }
            }
            if(found_all)
                printf("Match has been found, for all words, in file: %s\n", logfile[i]);
        }
        

        free(file_contents);
        fclose(fp);

        i++;
    }

    free(logfile);

    return 0;

error:
    return -1;
}


int get_file_char_count(FILE *f, char *filename) 
{
    int returnVal = 0;
    char cursor = ' ';

    while(cursor != EOF)
    {
        cursor = fgetc(f);
        returnVal++;
    }

    return returnVal;
}

int searchForKeyword(char *keyword, char *check_string)
{
    regex_t regex;

    int reg_setup;
    int reg_return_result;

    reg_setup = regcomp(&regex, keyword, 0);
    reg_return_result = regexec(&regex, check_string, 0, NULL, 0);

    return reg_return_result;
}

int findLogFilename(char *filename)
{
    regex_t regex;

    int reg_setup;
    int reg_return_result;

    reg_setup = regcomp(&regex, ".log", 0);
    reg_return_result = regexec(&regex, filename, 0, NULL, 0);

    return reg_return_result;
}

int setIsOr(int argc, char *argv[]) 
{
    int i = 0;
    int ret_int = 0;

    for(i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-o") == 0)
        {
            ret_int = 1; // sets the or flag to true
        }
    }

    return ret_int;
}


char** logFileNames() {
    DIR *d;
    struct dirent *dir;
    int isLog = 1;

    char** retArr;
    
    d = opendir("./log");
    check(d, "Could not open");

    int fileCount = 0;

    if(d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            isLog = findLogFilename(dir->d_name);
            if(isLog == 0)
            {
                fileCount++;
            }
        }

        retArr = malloc(sizeof(*retArr) * fileCount);
        check_mem(retArr);

        rewinddir(d);

        int logFilePos = 0;
        while((dir = readdir(d)) != NULL)
        {
            isLog = findLogFilename(dir->d_name);
            if(isLog == 0)
            {
                retArr[logFilePos] = dir->d_name;
                logFilePos++;
            }
        }

        closedir(d);

        retArr[logFilePos + 1] = NULL;

        return retArr;

    }
    return NULL;
    

error:
    closedir(d);
    return NULL;
}