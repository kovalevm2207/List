#ifndef LIST
#define LIST

#include "ListBase.h"

ListErr_t ListCtor(list_s* indexes);
long InsertAfter_(long pos, list_t value, list_s* list);
long InsertBefore_(long pos, list_t value, list_s* list);
long DeleteAfter_(long pos, list_s* list);
long DeleteBefore_(long pos, list_s* list);
ListErr_t ListDump_ (list_s* indexes, int* status, const char* func, const char* file, int line);
ListErr_t ListDtor_ (list_s* indexes);
ListErr_t ListVerify(list_s* list, int* status);

#define InsertAfter(pos, value, list) InsertAfter_(pos, value, list)
#define InsertBefore(pos, value, list) InsertBefore_(pos, value, list)
#define DeleteAfter(pos, list) DeleteAfter_(pos, list)
#define DeleteBefore(pos, list) DeleteBefore_(pos, list)
#define ListDtor(list) ListDtor_(list)
#define ListDump(list, status) ListDump_(list, status, __func__, __FILE__, __LINE__)

#endif // LIST

