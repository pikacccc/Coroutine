#include <iostream>

#include "hash_table.h"

int main()
{
    hash_table<std::string, int> table;

    table.insert_or_update("1", 1);
    table.insert_or_update("2", 1);
    table.insert_or_update("3", 1);

    int a;
    table.try_remove("2", a);
    std::cout << a;
}
