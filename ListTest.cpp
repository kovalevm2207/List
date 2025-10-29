#include "List.h"

int main()
{
    list_s list = {};
    ListCtor(&list);
    ListDump(list);
    ListDtor(&list);
    return 0;
}
