#ifndef LIST
#define LIST

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
} list_s;

typedef enum
{
    LIST_OK   = 1 << 0,
    NULL_LIST = 1 << 1
} ListErr_t;

ListErr_t ListCtor(list_s* indexes);
    FILE* StartHTMLfile(void);  // to do: перенести

ListErr_t InsertAfter(long pos, list_t value, list_s* list);
ListErr_t InsertBefore(long pos, list_t value, list_s* list);
ListErr_t DeleteAfter(long pos, list_s* list);
ListErr_t DeleteBefore(long pos, list_s* list);


ListErr_t ListDump_ (list_s* indexes, const char* func, const char* file, int line);
    ListErr_t CreateDotFile(list_s* list);
    void MakeNodes(list_s* list, FILE* file);           //
    void MakeArrows(list_s* list, FILE* file);
    ListErr_t WriteInHtmlFile(list_s* list, const char* func, const char* file, int line);
    void PrintList(list_s* list);

ListErr_t ListDtor (list_s* indexes);
    int EndHTMLfile(FILE* file);

#define ListDump(list) ListDump_(list, __func__, __FILE__, __LINE__);

/*
ListErr_t ListVerify(list_t data, list_s indexes);
*/

#endif // LIST
