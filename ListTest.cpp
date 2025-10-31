#include "List.h"

int main()
{
    list_s list = {};

    ListCtor(&list);

    InsertAfter(0, 10, &list);

    InsertAfter(1, 20, &list);

    InsertBefore(1, 15, &list);

    ListDtor(&list);
    return 0;
}
