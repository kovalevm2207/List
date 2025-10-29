#ifndef LIST
#define LIST

#include <stdio.h>  // for print & scan
#include <stdlib.h> // for calloc  // for system()
#include <assert.h>

#include "DotCmd.h"

const int MAX_INDEX = 1 << 3;
const int POISON = 0xBADDED;

typedef int list_t;

typedef struct
{
    long free;
    list_t* data;
    long* next;
    long* prev;
} list_s;

typedef enum
{
    LIST_OK   = 1 << 0,
    NULL_LIST = 1 << 1
} ListErr_t;

ListErr_t ListCtor(list_s* indexes);

ListErr_t ListDump_ (list_s indexes, const char* func, const char* file, int line);
ListErr_t CreateDotCmdFile(list_s list);

ListErr_t ListDtor (list_s* indexes);

#define ListDump(list) ListDump_(list, __func__, __FILE__, __LINE__);

/*
ListErr_t InsertAfter(list_t* data, list_s* indexes);
ListErr_t InsertBefore(list_t* data, list_S* indexes);

ListErr_t ListVerify(list_t data, list_s indexes);
*/

#endif // LIST
