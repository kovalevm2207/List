  /*В массиве next в каждой ячейке находятся номер ячейки элемента идущего за ним,
а то что не занято заполняется номерами ячеек следующих свободных элементов.
Изначально (при создании) в массиве next идут числа 0*, 2, 3, 4, 5, ... , MAX_INDEX, 0; (* - заполнено)

    В переменной free находится тот номер ячейки массива data в которую мы в следующий
будем что-то класть.
Изначально (при создании) free = 1; (т.е. первая ячейка data в которую мы будем писать будет
иметь номер 1)

    В массиве prev в каждой ячейке лежит номер ячейки предыдущего элемента,
а то ,что не занято заполняется номерами предыдущих свободных элементов;
Изначально (при создании) в массиве prev идут числа 0*, 0*, 1, 2, 3, ... , MAX_INDEX - 1, MAX_INDEX; (* - заполнено)

    В переменной header находится номер самой первой заполненной ячейки;
Т.е. изначально header = 1;
    Также есть переменная tail в котрой находится номер ячейки последнего элемента, которого мы записали
Т.е. изначально tail = 1;

Наша программа должна выглядеть примерно так:

    Вы создаете массив list_t data (Заполненный нулями или иным POISON значением)
(изначально data заполнена числами 0, 0, 0, 0, 0, ...)и структуру list_s list;
отправляете структуру в ListCtor; Там эта структура принимает изначальные значения описанные выше;

    Теперь вы хотите добавить в список (т.е. в массив data) элемент (очевидно на первое место), например значение 10;
Пишете AddElement(значение(в примере это 10), номер места, рабочие поля...);

    После этой операции у вас должны быть следующие значения:

    в воображаемом списке у вас 10 0 0 0 0 0 0 0 0 0 0 ...

    Но в реале:

    data: 0 3 0 0 0 0 0 0 0 0 ...
    next: 0 0 2 4 5 6 7 8 9 10 ...
    prev: 0 0 1 2 3 4 5 6 7 8 ...
    free: 2
    tail: 1
    header: 1

    Теперь допустим значение 20 мы кладем на место 2:

    в воображаемом списке у вас 10 20 0 0 0 0 0 0 0 0 0 ...

    Но в реале:

    data: 0 10 20 0 0 0 0 0 0  0 ...
    next: 0  2  0 4 5 6 7 8 9 10 ...
    prev: 0  0  1 2 3 4 5 6 7  8 ...
    free: 3
    tail: 2
    header: 1

    И наконец значение 30 мы кладем на место 3:

    в воображаемом списке у вас 10 20 30 0 0 0 0 0 0 0 0 ...

    Но в реале:

    data: 0 10 20 30     0 0 0 0 0  0 ...
    next: 0  2  3  0     5 6 7 8 9 10 ...
    prev: 0  0  1  2     3 4 5 6 7  8 ...
    free: 4
    tail: 3
    header: 1

    Как это работает?
    1) Сначала мы должны положить в массив data на следующее свободное место значение, т.е.:
        data[free] = значение;

    2) Теперь нам надо в free положить номер следующего свободного элемента, который мы можем найти из
*/

#include "ListDump.h"
#include "List.h"

ListErr_t ListCtor(list_s*  list)
{
    assert(list != NULL);

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


ListErr_t InsertAfter(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    DUMP_DATA = value;
    DUMP_POS = pos;

    long free = FREE;

    DATA(free) = value;
    FREE = NEXT(free);

    NEXT(free) = NEXT(pos);
    PREV(free) = pos;

    PREV(NEXT(pos)) = free;
    NEXT(pos) = free;

    ListDump(list);

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return LIST_OK;
}


ListErr_t InsertBefore(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    DUMP_DATA = value;
    DUMP_POS = pos;

    long free = FREE;

    DATA(free) = value;
    FREE = NEXT(free);

    PREV(free) = NEXT(pos);
    NEXT(free) = pos;

    NEXT(PREV(pos)) = free;
    PREV(pos) = free;

    ListDump(list);

    DUMP_DATA = POISON;
    DUMP_POS = -1;

    return LIST_OK;
}


ListErr_t DeleteAfter(long pos, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    DUMP_POS = pos;

    long deleting_elem = NEXT(pos);
    long next_elem = NEXT(deleting_elem);
    long last_free = PREV(FREE);

    PREV(FREE) = deleting_elem;

    NEXT(pos) = next_elem;
    PREV(next_elem) = pos;

    NEXT(deleting_elem) = FREE;
    PREV(deleting_elem) = last_free;

    FREE = deleting_elem;
    DATA(deleting_elem) = POISON;
// prev(next , pos);  DSL   Domen Specific Language  #define -> caps

    ListDump(list);

    DUMP_POS = -1;
    return LIST_OK;
}


ListErr_t DeleteBefore(long pos, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);


    DUMP_POS = pos;

    long deleting_elem = PREV(pos);
    long prev_elem = PREV(deleting_elem);
    long last_free = PREV(FREE);

    PREV(FREE) = deleting_elem;

    PREV(pos) = prev_elem;
    NEXT(prev_elem) = pos;

    NEXT(deleting_elem) = FREE;
    PREV(deleting_elem) = last_free;

    FREE = deleting_elem;
    DATA(deleting_elem) = POISON;

    ListDump(list);


    DUMP_POS = -1;

    return LIST_OK;
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
