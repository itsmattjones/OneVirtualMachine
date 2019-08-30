#include "logger.h"
#include <time.h>
#include <stdio.h>

const char* log_file = "General.log";

bool log_append_int(int value)
{
    // Create/Get the log file.
    FILE *file;
    file = fopen(log_file, "a+");

    if (file == NULL) 
        return false;

    // Write DateTime + Value.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "%d-%d-%d %d:%d:%d :: %d\n", tm.tm_year + 1900, 
        tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, value);

    // We're done.
    fclose(file);
    return true;
}

bool log_append_string(char* text)
{
    // Create/Get the log file.
    FILE *file;
    file = fopen(log_file, "a+");

    if (file == NULL) 
        return false;

    // Write DateTime + String.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "%d-%d-%d %d:%d:%d :: %s\n", tm.tm_year + 1900, 
        tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text);

    // We're done.
    fclose(file);
    return true;
}