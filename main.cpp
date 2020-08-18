#include <iostream>
#include "kvslib.h"

int main()
{
    KeyValueStore<int> x = KeyValueStore<int>();
    x.put("hello", 5);

    std::cout << x.get("hello") << std::endl;

    x.remove("hello");

    std::cout << x.get("hello") << std::endl;

    KeyValueStore<std::string> y = KeyValueStore<std::string>();
    std::string str("world");
    y.put("hello", str);
    std::cout << y.get("foo").length() << std::endl;
    return 0;
}