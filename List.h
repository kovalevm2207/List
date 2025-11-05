#ifndef LIST
#define LIST

#include "ListBase.h"

ListErr_t ListCtor(list_s* indexes);
long InsertAfter(long pos, list_t value, list_s* list);
long InsertBefore(long pos, list_t value, list_s* list);
long DeleteAfter(long pos, list_s* list);
long DeleteBefore(long pos, list_s* list);
ListErr_t ListDump_ (list_s* indexes, const char* func, const char* file, int line);
ListErr_t ListDtor (list_s* indexes);

#define ListDump(list) ListDump_(list, __func__, __FILE__, __LINE__);

/*
ListErr_t ListVerify(list_t data, list_s indexes);
*/

#endif // LIST
