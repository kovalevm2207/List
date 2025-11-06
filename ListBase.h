#ifndef LIST_BASE
#define LIST_BASE

#include <stdio.h>  // for print & scan
#include <stdlib.h> // for calloc  // for system()
#include <assert.h>

#include "color_print.h"
// ЗАВЕСТИ ПОЛЕ ГДЕ БЫЛ СОЗДАН

const int MAX_INDEX = 1 << 4;
const int CANARY = 0xACDC;
const int POISON = 0xC0CE;
const int MAX_FILE_NAME = 100;

typedef int list_t;

typedef struct
{
    FILE* dump_file;
    int count_img;
    list_t dump_data;
    long dump_pos;
    long dump_free;
} dot_file_s;

typedef struct
{
    long free;
    list_t* data;
    long* next;
    long* prev;
    dot_file_s file;
} list_s;

typedef enum
{
    LIST_OK   = 1 << 0,
    NULL_LIST = 1 << 1,
    NULL_DATA = 1 << 2,
    NULL_NEXT = 1 << 3,
    NULL_PREV = 1 << 4,
    NULL_FILE = 1 << 5
} ListErr_t;

#define DATA(index) list->data[index]
#define NEXT(index) list->next[index]
#define PREV(index) list->prev[index]
#define FREE list->free
#define DUMP_FILE list->file.dump_file
#define COUNT_IMG list->file.count_img
#define DUMP_DATA list->file.dump_data
#define DUMP_POS list->file.dump_pos
#define DUMP_ELEM list->file.dump_free

#endif // LIST_BASE
