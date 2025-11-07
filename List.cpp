#include "ListDump.h"
#include "List.h"

ListErr_t ListCtor(list_s*  list)
{
    int status_value = LIST_OK;
    int* status = &status_value;

    CHECK_PTR(list, LIST)
    CHECK_STATUS_AND_IF_NOK_RETURN(NULL_LIST)
                                                                    // (ну вообще может это и не проблема)
    DATA_PTR = (list_t*) calloc(MAX_INDEX + 1, sizeof(list_t));   // Вот проблема такая: list->data это указатель
    CHECK_PTR(DATA_PTR, DATA)                                     // а list->data[0] это уже конкретное значение
                                                                    // я вот не понимаю, как мне сделать макрос , ну или его обработку
    NEXT_PTR = (long*) calloc(MAX_INDEX + 1, sizeof(long));       // так что бы можно было и то и то подставлять или же мне лучше просто писать
    CHECK_PTR(NEXT_PTR, NEXT)                                     // list->data ???

    PREV_PTR = (long*) calloc(MAX_INDEX + 1, sizeof(long));
    CHECK_PTR(PREV_PTR, PREV)

    CHECK_STATUS_AND_IF_NOK_RETURN((ListErr_t) *status)

    DUMP_FILE = StartHTMLfile();
    CHECK_PTR(DUMP_FILE, FILE)
    CHECK_STATUS_AND_IF_NOK_RETURN(NULL_FILE)

    DUMP_DATA = 0;
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

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN((ListErr_t) *status)
    ON_DEBUG(ListDump(list, status))

    DUMP_DATA = POISON;

    return (ListErr_t) *status;
}


long InsertAfter_(long pos, list_t value, list_s* list)
{
    int status_value = LIST_OK;
    int* status = &status_value;

    CHECK_PTR(list, LIST)
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    DUMP_DATA = value;
    DUMP_POS = pos;

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    DUMP_ELEM = FREE;

    DATA(DUMP_ELEM) = value;
    FREE = NEXT(DUMP_ELEM);

    NEXT(DUMP_ELEM) = NEXT(pos);
    PREV(DUMP_ELEM) = pos;

    PREV(NEXT(pos)) = DUMP_ELEM;
    NEXT(pos) = DUMP_ELEM;

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    ON_DEBUG(ListDump(list, status))

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return DUMP_ELEM;
}


long InsertBefore_(long pos, list_t value, list_s* list)
{
    int status_value = LIST_OK;
    int* status = &status_value;

    CHECK_PTR(list, LIST)
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    DUMP_DATA = value;
    DUMP_POS = pos;

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    DUMP_ELEM = FREE;

    DATA(DUMP_ELEM) = value;
    FREE = NEXT(DUMP_ELEM);

    PREV(DUMP_ELEM) = NEXT(pos);
    NEXT(DUMP_ELEM) = pos;

    NEXT(PREV(pos)) = DUMP_ELEM;
    PREV(pos) = DUMP_ELEM;

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    ON_DEBUG(ListDump(list, status))

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return DUMP_ELEM;
}


long DeleteAfter_(long pos, list_s* list)
{
    int status_value = LIST_OK;
    int* status = &status_value;

    CHECK_PTR(list, LIST)
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    DUMP_DATA = 0;
    DUMP_POS = pos;
    DUMP_ELEM = NEXT(pos);

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    NEXT(pos) = NEXT(DUMP_ELEM);
    PREV(NEXT(DUMP_ELEM)) = pos;

    NEXT(DUMP_ELEM) = FREE;
    PREV(DUMP_ELEM) = PREV(FREE);

    PREV(FREE) = DUMP_ELEM;

    FREE = DUMP_ELEM;
    DATA(DUMP_ELEM) = POISON;

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    ON_DEBUG(ListDump(list, status))

    DUMP_POS = -1;
    DUMP_DATA = POISON;

    return DUMP_ELEM;
}


long DeleteBefore_(long pos, list_s* list)
{
    int status_value = LIST_OK;
    int* status = &status_value;

    CHECK_PTR(list, LIST)
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    DUMP_DATA = 0;
    DUMP_POS = pos;
    DUMP_ELEM = PREV(pos);

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    NEXT(DUMP_ELEM) = FREE;

    PREV(pos) = PREV(DUMP_ELEM);
    NEXT(PREV(DUMP_ELEM)) = pos;

    PREV(DUMP_ELEM) = PREV(FREE);
    PREV(FREE) = DUMP_ELEM;

    FREE = DUMP_ELEM;
    DATA(DUMP_ELEM) = POISON;

    ON_DEBUG(ListVerify(list, status))
    CHECK_STATUS_AND_IF_NOK_RETURN(-1)

    ON_DEBUG(ListDump(list, status))

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return DUMP_ELEM;
}

// "#717171"
// "#9a0000"

ListErr_t ListDump_(list_s* list, int* status, const char* func, const char* file, int line)
{
    assert(func != NULL);
    assert(file != NULL);
    assert(status != NULL);

    if (*status & NULL_LIST || list == NULL)
    {
        printf("ListDump from %s at %s:%d\n"
     RED_COLOR "NULL LIST POINTER\n" RESET, func, file, line);
        return NULL_LIST;
    }

    if (!(*status & NULL_DATA || *status & NULL_NEXT || *status & NULL_PREV))
    {
        CreateDotFile(list);
        char command[MAX_FILE_NAME];
        sprintf(command, "dot -Tsvg svg_dot/dump.dot -o svg_dot/%ddump.svg", COUNT_IMG);
        system(command);
    }

    WriteInHtmlFile(list, (ListErr_t*) status, func, file, line);

    return LIST_OK;
}


ListErr_t ListDtor_(list_s* list)
{
    int status_value = LIST_OK;
    int* status = &status_value;

    CHECK_PTR(list, LIST)
    CHECK_STATUS_AND_IF_NOK_RETURN(NULL_LIST)

    EndHTMLfile(list);

    free(PREV_PTR);
    free(NEXT_PTR);
    free(DATA_PTR);

    FREE = 0;
    DUMP_DATA = 0;
    DUMP_POS = 0;
    DUMP_ELEM = 0;
    DUMP_FILE = NULL;
    COUNT_IMG = 0;

    return LIST_OK;
}

// У меня определяется свободный это
// элемент или нет через значение data, но тогда
// POISON_DATA сбивает мне это , может можно
// как-то по другому проверять свободный это элемент или
// нет ?

ListErr_t ListVerify(list_s* list, int* status)
{
    CHECK_PTR(NEXT_PTR, NEXT)
    CHECK_PTR(PREV_PTR, PREV)
    CHECK_PTR(DATA_PTR, DATA)

    if (DUMP_DATA == POISON) *status |= POISON_DATA;

    return LIST_OK;
}
