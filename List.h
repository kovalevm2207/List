#ifndef LIST
#define LIST

#include <stdio.h>  // for print & scan
#include <stdlib.h> // for calloc  // for system()
#include <assert.h>


const int MAX_INDEX = 1 << 3;
const int CANARY = 0xACDC;
const int POISON = 0xBADDED;
const int MAX_FILE_NAME = 1000;

typedef int list_t;

typedef struct
{
    long free;
    list_t* data;
    long* next;
    long* prev;
    FILE* dump_file;
    int count_img;
} list_s;

typedef enum
{
    LIST_OK   = 1 << 0,
    NULL_LIST = 1 << 1
} ListErr_t;

ListErr_t ListCtor(list_s* indexes);
    FILE* StartHTMLfile(void);

ListErr_t InsertAfter(long pos, list_t value, list_s* list);

ListErr_t ListDump_ (list_s* indexes, const char* func, const char* file, int line);

    ListErr_t CreateDotFile(list_s* list);
    FILE* MakeFile(list_s* list);
    void MakeFreeNodes(list_s* list, FILE* file);
    void MakeDataNodes(list_s* list, FILE* file);
    void MakeArrows(list_s* list, FILE* file);
    ListErr_t write_in_html_file(list_s* list, const char* func, const char* file, int line);
    void PrintList(list_s* list);

ListErr_t ListDtor (list_s* indexes);
    int EndHTMLfile(FILE* file);

#define ListDump(list) ListDump_(list, __func__, __FILE__, __LINE__);

/*
ListErr_t InsertAfter(list_t* data, list_s* indexes);
ListErr_t InsertBefore(list_t* data, list_S* indexes);

ListErr_t ListVerify(list_t data, list_s indexes);
*/

#endif // LIST
