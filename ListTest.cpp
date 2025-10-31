#include "List.h"

int main()
{
    list_s list = {};

    ListCtor(&list);

    InsertAfter(0, 10, &list);

    InsertAfter(1, 20, &list);

    InsertAfter(2, 30, &list);

    InsertAfter(1, 40, &list);

    InsertAfter(2, 50, &list);

    ListDtor(&list);
    return 0;
}
