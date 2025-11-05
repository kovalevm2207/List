#include "ListDump.h"
#include "List.h"

ListErr_t ListCtor(list_s*  list)
{
    assert(list != NULL);
                                                                    // (ну вообще может это и не проблема)
    list->data = (list_t*) calloc(MAX_INDEX + 1, sizeof(list_t));   // Вот проблема такая: list->data это указатель
    assert(list->data != NULL);                                     // а list->data[0] это уже конкретное значение
                                                                    // я вот не понимаю, как мне сделать макрос , ну или его обработку
    list->next = (long*) calloc(MAX_INDEX + 1, sizeof(long));          // так что бы можно было и то и то подставлять или же мне лучше просто писать
    assert(list->next != NULL);                                           // list->data ???

    list->prev = (long*) calloc(MAX_INDEX + 1, sizeof(long));
    assert(list->prev != NULL);

    DUMP_FILE = StartHTMLfile();
    assert(DUMP_FILE != NULL);

    DUMP_DATA = POISON;
    DUMP_POS = -1;
    DUMP_ELEM = -1;

    FREE = 1;
    DATA(0) = POISON;
    NEXT(0) = 0;
    PREV(0) = 0;

    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        DATA(i) = POISON;
        PREV(i) = i - 1;
        NEXT(i) = i + 1;
    }

    NEXT(MAX_INDEX) = 0;

    COUNT_IMG = 1;

    ListDump(list);

    return LIST_OK;
}


long InsertAfter(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    DUMP_DATA = value;
    DUMP_POS = pos;

    DUMP_ELEM = FREE;

    DATA(DUMP_ELEM) = value;
    FREE = NEXT(DUMP_ELEM);

    NEXT(DUMP_ELEM) = NEXT(pos);
    PREV(DUMP_ELEM) = pos;

    PREV(NEXT(pos)) = DUMP_ELEM;
    NEXT(pos) = DUMP_ELEM;

    ListDump(list);

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return DUMP_ELEM;
}


long InsertBefore(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    DUMP_DATA = value;
    DUMP_POS = pos;

    DUMP_ELEM = FREE;

    DATA(DUMP_ELEM) = value;
    FREE = NEXT(DUMP_ELEM);

    PREV(DUMP_ELEM) = NEXT(pos);
    NEXT(DUMP_ELEM) = pos;

    NEXT(PREV(pos)) = DUMP_ELEM;
    PREV(pos) = DUMP_ELEM;

    ListDump(list);

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return DUMP_ELEM;
}


long DeleteAfter(long pos, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    DUMP_POS = pos;
    DUMP_ELEM = NEXT(pos);

    NEXT(pos) = NEXT(DUMP_ELEM);
    PREV(NEXT(DUMP_ELEM)) = pos;

    NEXT(DUMP_ELEM) = FREE;
    PREV(DUMP_ELEM) = PREV(FREE);

    PREV(FREE) = DUMP_ELEM;

    FREE = DUMP_ELEM;
    DATA(DUMP_ELEM) = POISON;

    ListDump(list);

    DUMP_POS = -1;
    return DUMP_ELEM;
}


long DeleteBefore(long pos, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);


    DUMP_POS = pos;
    DUMP_ELEM = PREV(pos);

    NEXT(DUMP_ELEM) = FREE;

    PREV(pos) = PREV(DUMP_ELEM);
    NEXT(PREV(DUMP_ELEM)) = pos;

    PREV(DUMP_ELEM) = PREV(FREE);
    PREV(FREE) = DUMP_ELEM;

    FREE = DUMP_ELEM;
    DATA(DUMP_ELEM) = POISON;


    ListDump(list);


    DUMP_POS = -1;

    return DUMP_ELEM;
}

ListErr_t ListDump_ (list_s* list, const char* func, const char* file, int line)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    CreateDotFile(list);
    char command[MAX_FILE_NAME];
    sprintf(command, "dot -Tsvg svg_dot/dump.dot -o svg_dot/%ddump.svg", COUNT_IMG);
    system(command);
    WriteInHtmlFile(list, func, file, line);
    return LIST_OK;
}


ListErr_t ListDtor(list_s* list)
{
    if (list == NULL) return NULL_LIST;

    EndHTMLfile(DUMP_FILE);

    free(list->prev);
    free(list->next);
    free(list->data);

    FREE = 0;
    DUMP_DATA = 0;
    DUMP_POS = 0;

    return LIST_OK;
}


/*
ListErr_t ListVerify(list_t data, list_s  list)
{
    (void) list;

    return LIST_OK;
}
*/
