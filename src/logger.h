#ifndef LOGGER_H
#define LOGGER_H
#include <stdbool.h>

bool log_append_int(int value);
bool log_append_string(char* text);

#endif