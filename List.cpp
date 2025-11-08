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

    ON_DEBUG(DUMP_FILE = StartHTMLfile();)
    CHECK_PTR(DUMP_FILE, FILE)
    CHECK_STATUS_AND_IF_NOK_RETURN(NULL_FILE)

    DUMP_DATA = 0;
    DUMP_POS = 0;
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
    DUMP_POS = 0;

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
    DUMP_POS = 0;

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

    DUMP_POS = 0;
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
    DUMP_POS = 0;

    return DUMP_ELEM;
}


ListErr_t ListDump_(list_s* list, int* status, const char* func, const char* file, int line)
{
    assert(func != NULL);
    assert(file != NULL);
    assert(status != NULL);

    if (*status & NULL_LIST || list == NULL)
    {
        ON_DEBUG(printf("ListDump from %s at %s:%d\n"
     RED_COLOR "NULL LIST POINTER\n" RESET, func, file, line));
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
    ON_DEBUG(printf("Идет поиск нулевых указателей на next, prev и data...\n"))
    CHECK_PTR(NEXT_PTR, NEXT)
    CHECK_PTR(PREV_PTR, PREV)
    CHECK_PTR(DATA_PTR, DATA)
    if (*status != LIST_OK && *status != LIST_OK + POISON_DATA) return (ListErr_t) *status;

    ON_DEBUG(printf("Проверка на случайное совпадение с POISON значений data у занятых элементов...\n"))
    if (DUMP_DATA == POISON) *status |= POISON_DATA;

    *status |= check_prev_and_next(list);
    ON_DEBUG(printf("Проверка на валидное значение free...\n"))
    if (FREE > MAX_INDEX || FREE < 0) *status |= INVALID_FREE_INDEX;

    ON_DEBUG(printf("Проверка на допустимое значение передаваемого аргумента функции вставки или удаления...\n"))
    if (DUMP_POS > MAX_INDEX || DUMP_POS < 0) *status |= INVALID_INDEX_IN_FUNC_ARG;

    if (DATA(FREE) != POISON) *status |= FREE_ELEM_NOT_POISON;

    return LIST_OK;
}


ListErr_t check_prev_and_next(list_s* list)
{
    #define INVALID_INDEX(index) errors[error].get_elem(list, index) > MAX_INDEX || errors[error].get_elem(list, index) < 0

    int status = LIST_OK;

    struct
    {
        ListErr_t invalid_index;
        ListErr_t cycle_err;
        long (* get_elem)(list_s* list, long index);
        const char* verify_name;
    } errors[] = {
        {INVALID_NEXT_INDEX, NEXT_CYCLE_ERR, get_next, "NEXT"},
        {INVALID_PREV_INDEX, PREV_CYCLE_ERR, get_prev, "PREV"}
    };


    for (size_t error = 0; error < sizeof(errors)/sizeof(errors[0]); error++)
    {
        ON_DEBUG(printf("Идет роверка массива %s:\n", errors[error].verify_name))

        for (int cur_index = 0; cur_index < MAX_INDEX + 1; cur_index++)
        {
            if (INVALID_INDEX(cur_index))
            {
                ON_DEBUG(printf(RED_COLOR "У элемента с индексом %d недопустимое значение %s\n" RESET, cur_index, errors[error].verify_name));
                status |= errors[error].invalid_index;
            }
            else if (DATA(cur_index) != POISON)
            {
                ON_DEBUG(printf(GREEN_COLOR " Значение %s индекса %d корректное\n" RESET, errors[error].verify_name, cur_index));
                long index = cur_index;
                for (int i = 0; i < MAX_INDEX + 1; i++)
                {
                    if (INVALID_INDEX(index))
                    {
                        ON_DEBUG(printf(RED_COLOR "У элемента с индексом %ld недопустимое значение %s\n" RESET, index, errors[error].verify_name));
                        status |= errors[error].invalid_index;
                        break;
                    }
                    if (index == 0)
                    {
                        ON_DEBUG(printf(GREEN_COLOR "Ложная циклическая зависимость не обнаружена\n" RESET));
                        break;
                    }
                    index = errors[error].get_elem(list, index);
                }

                if(index != 0 && !(INVALID_INDEX(index)))
                {
                    status |= errors[error].cycle_err;
                    ON_DEBUG(printf(RED_COLOR "Обнаружена ложная циклическая зависимость\n" RESET));
                }

            }
        }
    }

    return (ListErr_t) status;
}

long get_next(list_s* list, long index) {return NEXT(index);}
long get_prev(list_s* list, long index) {return PREV(index);}
long get_free(list_s* list, long index) {(void) index; return FREE;}
