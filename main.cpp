#include <iostream>
#include "kvslib.h"

int main()
{
    KeyValueStore<int> x = KeyValueStore<int>();
    x.put("hello", 5);

    std::cout << x.get("hello") << std::endl;
    return 0;
}