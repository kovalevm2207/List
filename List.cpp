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

    list->data = (list_t*) calloc(MAX_INDEX + 2, sizeof(list_t));
    assert(list->data != NULL);

    list->next = (long*) calloc(MAX_INDEX + 2, sizeof(long));
    assert(list->next != NULL);

    list->prev = (long*) calloc(MAX_INDEX + 2, sizeof(long));
    assert(list->prev != NULL);

    list->dump_file = StartHTMLfile();
    assert(list->dump_file != NULL);

    list->data[0] = CANARY;
    list->data[1] = CANARY;
    for (int i = 2; i < MAX_INDEX + 2; i++)
    {
        list->data[i] = POISON;
        list->prev[i] = i - 1;
    }

    list->next[0] = 2; // free
    list->next[1] = 1;
    list->prev[1] = 1;

    for (int i = 2; i < MAX_INDEX + 1; i++)
    {
        list->next[i] = i + 1;
    }
    list->next[MAX_INDEX + 1] = 1;

    list->count_img = 1;

    ListDump(list);

    return LIST_OK;
}


FILE* StartHTMLfile(void)
{
    FILE* dump_file = fopen("dump.html", "w");
    if(dump_file == NULL) return NULL;

    fprintf(dump_file, "<!DOCTYPE html>\n"
                       "<html>\n"
                       "<head>\n"
                       "    <title>List Dump</title>\n"
                       "</head>\n"
                       "<body>\n");

    return  dump_file;
}


ListErr_t InsertAfter(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);
    long real_pos = pos + 1;

    long free = list->next[0];

    list->data[free] = value;
    list->next[0] = list->next[free];

    list->next[free] = list->next[real_pos];
    list->prev[free] = real_pos;

    list->prev[list->next[real_pos]] = free;
    list->next[real_pos] = free;

    ListDump(list);
    return LIST_OK;
}


ListErr_t ListDump_ (list_s* list, const char* func, const char* file, int line)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    CreateDotFile(list);
    char command[MAX_FILE_NAME];
    sprintf(command, "dot -Tsvg svg_dot/%ddump.dot -o svg_dot/%ddump.svg",
            list->count_img, list->count_img);
    system(command);
    write_in_html_file(list, func, file, line);
    return LIST_OK;
}

ListErr_t CreateDotFile(list_s* list)
{
    assert(list != NULL);

    FILE* dump_file = MakeFile(list);
    assert(dump_file != NULL);

    fprintf(dump_file, "digraph DUMP\n"
                       "{\n"
                       "    rankdir=RL;\n"
                       "    splines=true;\n"
                       "    node[shape=Mrecord];\n");

    MakeDataNodes(list, dump_file);
    MakeFreeNodes(list, dump_file);
    MakeArrows(list, dump_file);

    fprintf(dump_file,"}\n");

    fclose(dump_file);
    return LIST_OK;
}


FILE* MakeFile(list_s* list)
{
    char name[MAX_FILE_NAME] = {};
    assert(name != NULL);

    sprintf(name, "svg_dot/%ddump.dot", list->count_img);

    return fopen(name, "w");
}


void MakeFreeNodes(list_s* list, FILE* file)
{
    assert(list != NULL);

    long index = list->next[0];

    //printf("free:\n");
    do
    {
        //printf("\tindex = %ld\n", index);
        fprintf(file, "    index_%ld [style=\"filled\", "
                                     "fillcolor=\"lightcoral\", "
                                     "label = \"<h> index_%ld |"
                                     " <d> data = %d |"
                                     " { <n> next = %ld | <p> prev = %ld }\"]\n",
                            index - 1, index - 1, list->data[index], list->next[index] - 1, list->prev[index] - 1);

        index = list->next[index];
    } while (index != 1);
}


void MakeDataNodes(list_s* list, FILE* file)
{
    assert(list != NULL);

    long index = 1;

    //printf("data:\n");
    do
    {
        //printf("\tindex = %ld\n", index);
        fprintf(file, "    index_%ld [style=\"filled\", "
                                     "fillcolor=\"lightgreen\", "
                                     "label = \"<h> index_%ld |"
                                     " <d> data = %d |"
                                     " { <n> next = %ld | <p> prev = %ld }\"]\n",
                      index - 1, index - 1, list->data[index], list->next[index] - 1, list->prev[index] - 1);

        index = list->next[index];
    } while (index != 1);
}


void MakeArrows(list_s* list, FILE* file)
{
    assert(list != NULL);

    for (int i = 2; i < MAX_INDEX + 2; i++)
    {
        fprintf(file, "    index_%d:n -> index_%ld:h [color=\"magenta\", "
                                                          "style=\"bold\", "
                                                          "arrowhead=\"normal\"];\n",
                           i - 1, list->next[i] - 1);

        fprintf(file, "    index_%d:p -> index_%ld:h [color=\"lightgrey\", "
                                                          "style=\"bold,dashed\", "
                                                          "arrowhead=\"normal\"];\n",
                           i - 1, list->prev[i] - 1);
    }
}


ListErr_t write_in_html_file(list_s* list,  const char* func, const char* file, int line)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    fprintf(list->dump_file, "    <pre>ListDump from %s at %s:%d</pre>\n",
                             func, file, line);
    PrintList(list);
    fprintf(list->dump_file, "    <img src=\"svg_dot/%ddump.svg\">\n",
                             list->count_img++);

    return LIST_OK;
}


void PrintList(list_s* list)
{
    assert(list != NULL);

    FILE* file = list->dump_file;
    assert(file != NULL);

    fprintf(file, "    <pre>data [");
    for (int i = 0; i < MAX_INDEX + 2; i++)
    {
        fprintf(file, " %5d;", list->data[i]);
    }
    fprintf(file, "]</pre>\n"
                  "    <pre>next [");
    for (int i = 0; i < MAX_INDEX + 2; i++)
    {
        fprintf(file, " %5ld;", list->next[i]);
    }
    fprintf(file, "]</pre>\n"
                  "    free---------^"
                  "    <pre>prev [");
    for (int i = 0; i < MAX_INDEX + 2; i++)
    {
        fprintf(file, " %5ld;", list->prev[i]);
    }
    fprintf(file, "]</pre>\n");
}

ListErr_t ListDtor(list_s* list)
{
    if (list == NULL) return NULL_LIST;

    EndHTMLfile(list->dump_file);

    free(list->prev);
    free(list->next);
    free(list->data);

    list->free = 0;

    return LIST_OK;
}

int EndHTMLfile(FILE* file)
{
    fprintf(file, "</body>\n"
                  "</html>\n");

    return fclose(file);
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
