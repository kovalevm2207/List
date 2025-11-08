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

#ifdef DEBUG
    TEST_PRINT("#717171", "Распечатка ошибочного списка с проверкой на невалидный next индекс:")
    list.next[6] = 300;
    list.prev[7] = 5;

    int test_0 = LIST_OK;
    int* test_0_ptr = &test_0;

    TEST_PRINT("#717171", "+ Проверка на правильную последовательность заполненных элементов")
    InsertAfter(9, 1000, &list);

    TEST_PRINT("#717171", "Проверка срабатывания ошибок при использовании списка")
    TEST_PRINT("#717171", "Нулевые указатели на структуру list")

    ListDump(NULL, test_0_ptr);
    DeleteBefore(9, NULL);
    DeleteAfter(9, NULL);
    InsertAfter(9, 1000, NULL);
    InsertBefore(9, POISON, NULL);

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

    TEST_PRINT("#717171", "предупреждение на то, что в data пытаются положить значение равное POISON")
    InsertAfter(9, POISON, &list);

    TEST_PRINT("#717171", "Теперь проверим на обнаружение циклических зависимостей, на примере prev цикла:")
    list.prev[7] = 9;
    InsertAfter(9, POISON, &list);

    list.next[5] = 3;
    InsertAfter(9, POISON, &list);

    list.next[5] = 12;
    InsertAfter(9, POISON, &list);
#endif

    ListDtor(&list);
    return 0;
}
