#include <iostream>
#include "kvslib.h"

int main()
{
    InMemoryKVS<int> x = InMemoryKVS<int>();
    x.put("hello", 5);

    std::cout << x.get("hello") << std::endl;

    x.remove("hello");

    std::cout << x.get("hello") << std::endl;

    InMemoryKVS<std::string> y = InMemoryKVS<std::string>();
    std::string str("world");
    y.put("hello", str);
    std::cout << y.get("foo") << std::endl;
    return 0;
}