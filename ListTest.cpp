#include "List.h"

int main()
{
    list_s list = {};

    ListCtor(&list);

    InsertAfter(0, 200, &list);
    InsertBefore(1, 100, &list);
    InsertAfter(1, 300, &list);
    InsertAfter(3, 400, &list);
    InsertAfter(4, 500, &list);
    InsertAfter(5, 600, &list);
    InsertAfter(6, 700, &list);
    InsertAfter(7, 800, &list);
    InsertAfter(8, 900, &list);
    InsertAfter(9, 1000, &list);
    DeleteAfter(9, &list);

// Распечатка ошибочного списка:
    list.next[6 + 1] = 300 + 1;
    list.prev[7 + 1] = 5  + 1;

    int test_0 = LIST_OK;
    int* test_0_ptr = &test_0;

    ListDump(&list, test_0_ptr);

// Проверка срабатывания ошибок при использовании списка
    // Нулевые указатели на структуру list

    ListDump(NULL, test_0_ptr);
    DeleteBefore(9, NULL);
    DeleteAfter(9, NULL);
    InsertAfter(9, 1000, NULL);
    InsertBefore(9, 1000, NULL);

    long* test_1 = list.next;
    long* test_2 = list.prev;
    int* test_3 = list.data;

    list.next = NULL;
    InsertAfter(9, 1000, &list);

    list.prev = NULL;
    InsertAfter(9, 1000, &list);

    list.data = NULL;
    InsertAfter(9, 1000, &list);

    list.next = test_1;
    list.prev = test_2;
    list.data = test_3;

    ListDtor(&list);
    return 0;
}
