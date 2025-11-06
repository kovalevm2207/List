#include "ListDump.h"

FILE* StartHTMLfile(void)
{
    FILE* dump_file = fopen("dump.html", "w");
    if(dump_file == NULL) return NULL;

    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);

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
                       "</style>\n"
                       "<pre> Creation time (hours:min): %02d:%02d\n"
                            " data (day:month:year): %02d:%02d:%d </pre>",
                        time_info->tm_hour, time_info->tm_min,
                        time_info->tm_mday, time_info->tm_mon + 1,
                        time_info->tm_year + 1900);
    fclose(dump_file);
    return  dump_file;
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
                                "height=1.8];\n"
                       "    edge [style=\"bold\", "
                                 "arrowhead=\"normal\", "
                                 "splines=line, "
                                 "dir=normal]\n");

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
                  DATA(0), PREV(0), NEXT(0));

    struct BaseArrow
    {
        const char* name;
        long value;
    } Arrows[] =
    {
        {"FREE", FREE},
        {"HEAD", NEXT(0)},
        {"TAIL", PREV(0)}
    };

    for (size_t i = 0; i < sizeof(Arrows)/sizeof(BaseArrow); i++)
    {
        fprintf(file, "    %s [shape=box, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12, "
                                "width=1, height=0.5, "
                                "fillcolor = \"#f79642ff\","
                                "label = \"FREE = %ld\"];\n",
                    Arrows[i].name, Arrows[i].value);
    }

    for(long index = 1; index < MAX_INDEX + 1; index++)
    {
        const char* shape_color = (DATA(index) == POISON) ? "\"palegreen\"" : "\"#81e6ffff\"";
        fprintf(file, "    index_%ld [shape=Mrecord, style=\"filled\", fontcolor=\"black\", fontname=\"Arial\", fontsize=12,"
                                     "width=1.2, height=1.2,"
                                     "fillcolor=%s,"
                                     "label = \"<h> index_%ld |"
                                     " <d> data = %d |"
                                     " { <p> prev = %ld | <n> next = %ld }\"];\n",
                      index, shape_color, index, DATA(index), PREV(index), NEXT(index));
    }
}


void SetOrder(list_s* list, FILE* file)
{
    fprintf(file, "  index_0");
    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        if (DATA(i) != POISON)
        {
            fprintf(file, " -> index_%d", i);
        }
    }

    for (int i = 1; i < MAX_INDEX + 1; i++)
    {
        if (DATA(i) == POISON)
        {
            fprintf(file, " -> index_%d", i);
        }
    }
    fprintf(file, "[weight=1000, color=\"red\", style=\"invis\"];\n");
}


#define MAKE_BASE_ARROW()


void MakeArrows(list_s* list, FILE* file)
{
    assert(list != NULL);

    struct BaseArrow
    {
        const char* name;
        long to;
    } Arrows[] =
    {
        {"FREE", FREE},
        {"HEAD", NEXT(0)},
        {"TAIL", PREV(0)}
    };

    for (size_t i = 0; i < sizeof(Arrows)/sizeof(BaseArrow); i++)
    {
        fprintf(file, "    %s -> index_%ld [color=\"#f79642ff\", "
                                            "style=\"bold,dashed\", "
                                            "arrowhead=\"normal\"];",
                      Arrows[i].name, Arrows[i].to);
    }

    for (long i = 1; i < MAX_INDEX + 1; i++)
    {
        const char* next_color = (DATA(i) == POISON) ? "#0f5f13ff" : "#1114ff";
        const char* prev_color = (DATA(i) == POISON) ? "#87c58aff" : "#7bcfffff";

            fprintf(file, "    index_%ld:h -> index_%ld:h [dir=normal, color=\"%s\"];\n"
                          "    index_%ld:p -> index_%ld:n [dir=normal, color=\"%s\"];\n",
                           i, NEXT(i), next_color,
                           i, PREV(i), prev_color);
    }
}


ListErr_t WriteInHtmlFile(list_s* list,  const char* func, const char* file, int line)
{
    assert(list != NULL);
    assert(func != NULL);
    assert(file != NULL);

    DUMP_FILE = fopen("dump.html", "a");
    assert(DUMP_FILE != NULL);

    fprintf(DUMP_FILE, "    <pre><b>ListDump(%d) from %s(pos = %ld, value = %d) at %s:%d</b></pre>\n"
                       "    <pre><b>function return value: %ld",
                             COUNT_IMG, func, DUMP_POS, DUMP_DATA, file, line, DUMP_ELEM);
    PrintList(list);
    fprintf(DUMP_FILE, "    <img src=\"svg_dot/%ddump.svg\">\n",
                             COUNT_IMG++);

    fclose(DUMP_FILE);

    return LIST_OK;
}


void PrintList(list_s* list)
{
    assert(list != NULL);

    FILE* file = DUMP_FILE;
    assert(file != NULL);

    fprintf(file, "    <pre>___________________________________________\n"
                        "<u>|   index  |   data   |   next  |   prev  |</u>\n");

    fprintf(file, "<span style=\"background-color: #f79642ff;\">"
                  "| %8d | %8d | %8ld| %8ld|\n"
                  "</span>",
                  0, DATA(0), NEXT(0), PREV(0));

    for (int i = 1; i < MAX_INDEX + 1   ; i++)
    {
        const char* background_color = (DATA(i) == POISON) ? "palegreen" : "#81e6ffff";
        if(i == MAX_INDEX) fprintf(file, "<u>");
        fprintf(file, "<span style=\"background-color: %s;\">"
                      "| %8d | %8d | %8ld| %8ld|"
                      "</span>\n",
                      background_color, i, DATA(i), NEXT(i), PREV(i));
        if(i == MAX_INDEX) fprintf(file, "</u>");
    }
    fprintf(file, "</pre>\n");
}


int EndHTMLfile(list_s* list)
{
    DUMP_FILE = fopen("dump.html", "a");
    fprintf(DUMP_FILE, "</body>\n"
                  "</html>\n");

    return fclose(DUMP_FILE);
}
