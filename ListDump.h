#include "ListBase.h"

#ifndef LIST_DUMP
#define LIST_DUMP

#include <time.h>

FILE* StartHTMLfile(void);
ListErr_t CreateDotFile(list_s* list);
void MakeNodes(list_s* list, FILE* file);
void SetOrder(list_s* list, FILE* file);
void MakeArrows(list_s* list, FILE* file);
void PrintList(list_s* list);
ListErr_t WriteInHtmlFile(list_s* list, const char* func, const char* file, int line);
int EndHTMLfile(list_s* list);

#endif // LIST_DUMP
