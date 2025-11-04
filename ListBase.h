#ifndef LIST_BASE
#define LIST_BASE

#include <stdio.h>  // for print & scan
#include <stdlib.h> // for calloc  // for system()
#include <assert.h>

// ЗАВЕСТИ ПОЛЕ ГДЕ БЫЛ СОЗДАН
// ОТКЛЮЧАТЬ ВЕРИФИКАТОР
// ON_DEBUG(ListVerify())


const int MAX_INDEX = 1 << 4;
const int CANARY = 0xACDC;
const int POISON = 0xC0CE;
const int MAX_FILE_NAME = 100;

typedef int list_t;

typedef struct
{
    long free;
    list_t* data;
    long* next;
    long* prev;
    FILE* dump_file;
    int count_img;
    list_t dump_data;
    long dump_pos;
} list_s;

typedef enum
{
    LIST_OK   = 1 << 0,
    NULL_LIST = 1 << 1
} ListErr_t;

#endif // LIST_BASE
