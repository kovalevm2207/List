#ifndef LIST_BASE
#define LIST_BASE

#include <stdio.h>  // for print & scan
#include <stdlib.h> // for calloc  // for system()
#include <assert.h>

#include "color_print.h"
// ЗАВЕСТИ ПОЛЕ ГДЕ БЫЛ СОЗДАН

// Палитра цветов
// "#717171"
// "#00a606"
// "#9f00be"
// "#a00000"
// "#f79642"
// "#81e6ff"
// "#1114ff"
// "#87c58a"
// "#0f5f13"


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
    LIST_OK                   = 1 << 0,
    NULL_PTR                  = 1 << 1,
    NULL_LIST                 = 1 << 2,
    NULL_DATA                 = 1 << 3,
    NULL_NEXT                 = 1 << 4,
    NULL_PREV                 = 1 << 5,
    NULL_FILE                 = 1 << 6,
    POISON_DATA               = 1 << 7,
    NEXT_DATA_ERR             = 1 << 8,
    INVALID_NEXT_INDEX        = 1 << 9,
    INVALID_PREV_INDEX        = 1 << 10,
    NEXT_CYCLE_ERR            = 1 << 11,
    PREV_CYCLE_ERR            = 1 << 12,
    INVALID_FREE_INDEX        = 1 << 13,
    INVALID_INDEX_IN_FUNC_ARG = 1 << 14,
    FREE_ELEM_NOT_POISON      = 1 << 15
} ListErr_t;

#define DATA(index) list->data[index]
#define NEXT(index) list->next[index]
#define PREV(index) list->prev[index]
#define DATA_PTR list->data
#define NEXT_PTR list->next
#define PREV_PTR list->prev
#define FREE list->free
#define DUMP_FILE list->file.dump_file
#define COUNT_IMG list->file.count_img
#define DUMP_DATA list->file.dump_data
#define DUMP_POS list->file.dump_pos
#define DUMP_ELEM list->file.dump_free

#ifdef DEBUG
    #define ON_DEBUG(func) func;
    #define DEBUG_FILE_PRINT(color, text, ...) DUMP_FILE = fopen("dump.html", "a");                                          \
                                   assert(DUMP_FILE != NULL);                                                                \
                                   fprintf(DUMP_FILE,"<p style=\"color: %s;\"><b>" text "</b></p>", color, ##__VA_ARGS__);   \
                                   fclose(DUMP_FILE);

    #define TEST_PRINT(color, text, ...) list.file.dump_file = fopen("dump.html", "a");                                                \
                                   assert(list.file.dump_file != NULL);                                                                \
                                   fprintf(list.file.dump_file,"<p style=\"color: %s;\"><b>" text "</b></p>", color, ##__VA_ARGS__);   \
                                   fclose(list.file.dump_file);

    #define CHECK_STATUS_AND_IF_NOK_RETURN(return_value) if (*status != LIST_OK && *status != LIST_OK + POISON_DATA) \
                                       {                                                                             \
                                            ListDump(list, status);                                                  \
                                            return return_value;                                                     \
                                       }

    #define CHECK_PTR(param, name) if (param == NULL)            \
                                    {                            \
                                        *status |= NULL_##name;  \
                                    }

#else
    #define ON_DEBUG(func)
    #define DEBUG_FILE_PRINT(color, text, ...)
    #define TEST_PRINT(color, text, ...)
    #define CHECK_PTR(param, name) (void) status;
    #define CHECK_STATUS_AND_IF_NOK_RETURN(return_value)
#endif

#endif // LIST_BASE
