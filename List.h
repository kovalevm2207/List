#include <stdio.h>

const int MAX_INDEX = 1 << 5;
const int POISON = BADDED;

typedef int list_t;

typedef struct
{
    long long header;
    long long tail;
    long long free;
    list_t* data;
    long long* next;
    long long* pref;
} list_s;

typedef enum
{
    LIST_OK = 1 << 0
} ListErr_t;

ListErr_t ListCtor(list_s* indexes);
ListErr_t ListDtor(list_s* indexes);

ListErr_t InsertAfter(list_t* data, list_s* indexes);
ListErr_t InsertBefore(list_t* data, list_S* indexes);

ListErr_t ListDump(list_t data, list_s indexes);
ListErr_t ListVerify(list_t data, list_s indexes);
