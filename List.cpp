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


# include "List.h"

ListErr_t ListCtor(list_s*  list)
{
    assert(list != NULL);

    list->data = (list_t*) calloc(MAX_INDEX + 1, sizeof(list_t));
    assert(list->data != NULL);

    list->next = (long*) calloc(MAX_INDEX + 1, sizeof(long));
    assert(list->next != NULL);
    list->prev = (long*) calloc(MAX_INDEX + 1, sizeof(long));
    assert(list->prev != NULL);

    for (int i = 1; i < MAX_INDEX; i++)
    {
        list->next[i] = i + 1;
        list->prev[i] = i - 1;
    }
    list->free = 1;


    return LIST_OK;
}


ListErr_t ListDump_ (list_s list, const char* func, const char* file, int line)
{
    // сначала создадим файл с макросами для построения графов
    CreateDotCmdFile(list);
    // отправим его в препроцессор, чтобы подставились макросы
    system("g++ -x c -E -P nprocessed_dump.dot -o dump.dot");
    // отправим наш файл переделываться в граф и выведем в виде png
    system("dot -Tpng dump.dot -o dump.png && start dump.png");
    // Пока на этом все

    return LIST_OK;
}

ListErr_t CreateDotCmdFile(list_s list)
{
    FILE* dump_file = fopen("nprocessed_dump.dot", "w");
    assert(dump_file != NULL);

    fprintf(dump_file, "#include \"DotCmd.h\"\n"
                       "\n"
                       "digraph DUMP\n"
                       "{\n"
                       "    rankdir=LR;\n"
                       "    node[shape=Mrecord];\n"
                       "\n");

    for (int i = 0; i <= MAX_INDEX; i++)
    {
        fprintf(dump_file, "    NODE(index_%d, data = %d, next = %ld, prev = %ld)\n", i, list.data[i], list.next[i], list.prev[i]);
        fprintf(dump_file, "    index_%d:n -> index_%ld:h;\n", i, list.next[i]);
        fprintf(dump_file, "    index_%d:p -> index_%ld:h;\n", i, list.prev[i]);
    }

    fprintf(dump_file, "}\n");

    fclose(dump_file);

    return LIST_OK;
}

ListErr_t ListDtor(list_s* list)
{
    if (list != NULL) return NULL_LIST;

    free(list->prev);
    free(list->next);
    free(list->data);

    list->free = 0;

    return LIST_OK;
}

/*
ListErr_t AddElement(list_t elem, long num, list_t* data, list_s* list)
{
    data[num] = elem;

    list->next = list->tail; //
    list->prev = list->;
    list->tail =;
    list->free =;
}

ListErr_t DelElement(list_t data, list_s  list);
*/


/*
ListErr_t ListVerify(list_t data, list_s  list)
{
    (void) data; (void) list;

    return LIST_OK;
}
*/
