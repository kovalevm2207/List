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

    list->dump_file = StartHTMLfile();
    assert(list->dump_file != NULL);

    list->dump_data = POISON;
    list->dump_pos = -1;
    list->free = 1;
    list->data[0] = POISON;
    list->next[0] = 0;
    list->prev[0] = 0;

    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        list->data[i] = POISON;
        list->prev[i] = i - 1;
        list->next[i] = i + 1;
    }

    list->next[MAX_INDEX] = 0;

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
                       "<body>\n"
                       "<style>\n"
                       "    * {\n"
                       "       font-size: 20px;\n"
                       "    }\n"
                       "</style>\n");

    return  dump_file;
}


ListErr_t InsertAfter(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    list->dump_data = value;
    list->dump_pos = pos;

    long free = list->free;

    list->data[free] = value;
    list->free = list->next[free];

    list->next[free] = list->next[pos];
    list->prev[free] = pos;

    list->prev[list->next[pos]] = free;
    list->next[pos] = free;

    ListDump(list);

    list->dump_data = POISON;
    list->dump_pos = -1;

    return LIST_OK;
}


ListErr_t InsertBefore(long pos, list_t value, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    list->dump_data = value;
    list->dump_pos = pos;

    long free = list->free;

    list->data[free] = value;
    list->free = list->next[free];

    list->prev[free] = list->next[pos];
    list->next[free] = pos;

    list->next[list->prev[pos]] = free;
    list->prev[pos] = free;

    ListDump(list);

    list->dump_data = POISON;
    list->dump_pos = -1;

    return LIST_OK;
}


ListErr_t DeleteAfter(long pos, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);

    list->dump_pos = pos;

    long deleting_elem = list->next[pos];
    long next_elem = list->next[deleting_elem];
    long last_free = list->prev[list->free];

    list->prev[list->free] = deleting_elem;

    list->next[pos] = next_elem;
    list->prev[next_elem] = pos;

    list->next[deleting_elem] = list->free;
    list->prev[deleting_elem] = last_free;

    list->free = deleting_elem;
    list->data[deleting_elem] = POISON;
// prev(next , pos);  DSL   Domen Specific Language  #define -> caps

    list->dump_pos = -1;

    ListDump(list);
    return LIST_OK;
}


ListErr_t DeleteBefore(long pos, list_s* list)
{
    assert(list != 0);
    assert(pos <= MAX_INDEX);


    list->dump_pos = pos;

    long deleting_elem = list->prev[pos];
    long prev_elem = list->prev[deleting_elem];
    long last_free = list->prev[list->free];

    list->prev[list->free] = deleting_elem;

    list->prev[pos] = prev_elem;
    list->next[prev_elem] = pos;

    list->next[deleting_elem] = list->free;
    list->prev[deleting_elem] = last_free;

    list->free = deleting_elem;
    list->data[deleting_elem] = POISON;

    ListDump(list);


    list->dump_pos = -1;

    return LIST_OK;
}

ListErr_t ListDump_ (list_s* list, const char* func, const char* file, int line)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    CreateDotFile(list);
    char command[MAX_FILE_NAME];
    sprintf(command, "dot -Tsvg svg_dot/dump.dot -o svg_dot/%ddump.svg", list->count_img);
    system(command);
    WriteInHtmlFile(list, func, file, line);
    return LIST_OK;
}

ListErr_t CreateDotFile(list_s* list)
{
    assert(list != NULL);

    FILE* dump_file = fopen("svg_dot/dump.dot", "w");
    assert(dump_file != NULL);

    fprintf(dump_file, "digraph DUMP\n"
                       "{\n"
                       "    rankdir=LR;\n"
                       "    splines=true;\n"
                       "    node[shape=hexagon,"
                                "style=\"filled\","
                                "fillcolor=\"red\","
                                "fontcolor=\"white\","
                                "fontname=\"Arial\","
                                "fontsize=24,"
                                "width=1.2,"
                                "height=1.8];\n");

    MakeNodes(list, dump_file);
    SetOrder(list, dump_file);
    MakeArrows(list, dump_file);

    fprintf(dump_file,"}\n");

    fclose(dump_file);
    return LIST_OK;
}


void MakeNodes(list_s* list, FILE* file)
{
    assert(list != NULL);

    fprintf(file, "    index_0 [shape=Mrecord, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12,"
                               "width=1.2, height=1.2,"
                               "fillcolor = \"#f79642ff\","
                               "label = \"<h> index_0 |"
                               " <d> data = %d |"
                               " { <p> TAIL = %ld | <n> HEAD = %ld }\"];\n",
                  list->data[0], list->prev[0], list->next[0]);

    fprintf(file, "    FREE [shape=box, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12, "
                            "width=1, height=0.5, "
                            "fillcolor = \"#f79642ff\","
                            "label = \"FREE = %ld\"];\n",
                  list->free);
    fprintf(file, "    TAIL [shape=box, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12, "
                            "width=1, height=0.5, "
                            "fillcolor = \"#f79642ff\","
                            "label = \"TAIL = %ld\"];\n",
                  list->prev[0]);

    fprintf(file, "    HEAD [shape=box, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12, "
                            "width=1, height=0.5, "
                            "fillcolor = \"#f79642ff\","
                            "label = \"HEAD = %ld\"];\n",
                  list->next[0]);

    for(long index = 1; index < MAX_INDEX + 1; index++)
    {
        const char* shape_color = (list->data[index] == POISON) ? "\"palegreen\"" : "\"#81e6ffff\"";
        fprintf(file, "    index_%ld [shape=Mrecord, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12,"
                                     "width=1.2, height=1.2,"
                                     "fillcolor=%s,"
                                     "label = \"<h> index_%ld |"
                                     " <d> data = %d |"
                                     " { <p> prev = %ld | <n> next = %ld }\"];\n",
                      index, shape_color, index, list->data[index], list->prev[index], list->next[index]);
    }
}


void SetOrder(list_s* list, FILE* file)
{
    fprintf(file, "  index_0");
    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        if (list->data[i] != POISON)
        {
            fprintf(file, " -> index_%d", i);
        }
    }

    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        if (list->data[i] == POISON)
        {
            fprintf(file, " -> index_%d", i);
        }
    }
    fprintf(file, "[weight=1000, color=\"red\", style=\"invis\"];\n");
}

void MakeArrows(list_s* list, FILE* file)
{
    assert(list != NULL);

    fprintf(file, "    FREE -> index_%ld [color=\"#f79642ff\", "
                                        "style=\"bold,dashed\", "
                                        "arrowed=\"normal\"];",
                  list->free);
    fprintf(file, "    HEAD -> index_%ld [color=\"#f79642ff\", "
                                        "style=\"bold,dashed\", "
                                        "arrowed=\"normal\"];",
                  list->next[0]);
    fprintf(file, "    TAIL -> index_%ld [color=\"#f79642ff\", "
                                        "style=\"bold,dashed\", "
                                        "arrowed=\"normal\"];",
                  list->prev[0]);

    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        const char* next_color = (list->data[i] == POISON) ? "#0f5f13ff" : "#1114ff";
        const char* prev_color = (list->data[i] == POISON) ? "#91ca8aff" : "#73ceffff";

        fprintf(file, "    index_%d:p -> index_%ld:h [color=\"%s\", "
                                                     "style=\"bold,dashed\", "
                                                     "arrowhead=\"normal\"];\n",
                           i, list->prev[i], prev_color);
        if (i != MAX_INDEX)
        {
            fprintf(file, "    index_%d:n -> index_%ld:h [color=\"%s\", "
                                                         "style=\"bold\", "
                                                         "arrowhead=\"normal\"];\n",
                               i, list->next[i], next_color);
        }
    }
}


ListErr_t WriteInHtmlFile(list_s* list,  const char* func, const char* file, int line)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    fprintf(list->dump_file, "    <pre><b>ListDump(%d) from %s(value = %d, pos = %d) at %s:%d</b></pre>\n",
                             list->count_img, func, list->dump_data, list->dump_pos, file, line);
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

    fprintf(file, "    <pre>__________________________________________\n"
                       "<u>|   index  |   data   |   next  |   prev  |</u>\n");

    fprintf(file, "<span style=\"background-color: #f79642ff;\">"
                  "| %8d | %8d | %8ld| %8ld|\n"
                  "</span>",
                  0, list->data[0], list->next[0], list->prev[0]);

    for (int i = 1; i < MAX_INDEX + 1   ; i++)
    {
        const char* background_color = (list->data[i] == POISON) ? "palegreen" : "#81e6ffff";
        if(i == MAX_INDEX) fprintf(file, "<u>");
        fprintf(file, "<span style=\"background-color: %s;\">"
                      "| %8d | %8d | %8ld| %8ld|"
                      "</span>\n",
                      background_color, i, list->data[i], list->next[i], list->prev[i]);
        if(i == MAX_INDEX) fprintf(file, "</u>");
    }
    fprintf(file, "</pre>\n");
}

ListErr_t ListDtor(list_s* list)
{
    if (list == NULL) return NULL_LIST;

    EndHTMLfile(list->dump_file);

    free(list->prev);
    free(list->next);
    free(list->data);

    list->free = 0;
    list->dump_data = 0;
    list->dump_pos = 0;

    return LIST_OK;
}

int EndHTMLfile(FILE* file)
{
    fprintf(file, "</body>\n"
                  "</html>\n");

    return fclose(file);
}



/*
ListErr_t ListVerify(list_t data, list_s  list)
{
    (void) list;

    return LIST_OK;
}
*/
