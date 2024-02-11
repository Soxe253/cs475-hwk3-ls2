#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"
#include "stdbool.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "
#define MAX 1000

// TODO: Function declarations here for ls2
void ls2(char* path, char* emp, int indent);
bool mode2(stack_t *list, char* path, char* emp, int indent);
#endif
