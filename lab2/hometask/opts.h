#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>

void status();
void help();
void info(char* var);
void set(char* statement);
void assign_value(char* var, char* val);
void del(char* var);
void clear();