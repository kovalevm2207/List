#include "List.h"

int main()
{
    list_s list = {};

    ListCtor(&list);

    InsertAfter(0, 10, &list);

    InsertBefore(1, 5, &list);

    DeleteAfter(2, &list);

    ListDtor(&list);
    return 0;
}
