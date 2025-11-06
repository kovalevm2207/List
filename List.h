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

#ifdef DEBUG
    #define ON_DEBUG(func) func;
    #define DEBUG_FILE_PRINT(color, text, ...) DUMP_FILE = fopen("dump.html", "a");                                                      \
                                   assert(DUMP_FILE != NULL);                                                                \
                                   fprintf(DUMP_FILE,"<p style=\"color: %s;\"><b>" text "</b></p>", color, ##__VA_ARGS__); \
                                   fclose(DUMP_FILE);
    #define DEBUG_CONSOL_PRINT(text, ...) printf(text, ##__VA_ARGS__);

    #define CHECK_STATUS_AND_IF_NOK_RETURN(return_value) if (*status != LIST_OK) \
                                       {                                         \
                                            ListDump(list, status);              \
                                            return return_value;                 \
                                       }

    #define CHECK_PTR(param, name) if (param == NULL)            \
                                    {                            \
                                        *status |= NULL_##name;  \
                                    }

#else
    #define ON_DEBUG(func)
    #define DEBUG_FILE_PRINT(text, ...)
    #define DEBUG_CONSOL_PRINT(text, ...)
    #define CHECK_PTR(param, name)
    #define CHECK_STATUS_AND_IF_NOK_RETURN(return_value)
#endif

#endif // LIST

