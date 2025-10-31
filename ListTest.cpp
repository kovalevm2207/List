#include "List.h"

int main()
{
    list_s list = {};

    ListCtor(&list);

    InsertAfter(0, 100, &list);
    InsertAfter(1, 200, &list);
    InsertAfter(2, 300, &list);
    InsertAfter(3, 400, &list);
    InsertAfter(4, 500, &list);
    InsertAfter(5, 600, &list);
    InsertAfter(6, 700, &list);
    InsertAfter(7, 800, &list);
    InsertAfter(8, 900, &list);
    InsertAfter(9, 1000, &list);

    list.next[6 + 1] = 300 + 1;
    list.prev[7 + 1] = 5  + 1;

    ListDump(&list);

    ListDtor(&list);
    return 0;
}
