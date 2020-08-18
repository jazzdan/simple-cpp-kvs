#include <string>
#include <map>

template <typename T>
class KeyValueStore
{
private:
    std::map<std::string, T> internalKeyValue;

public:
    KeyValueStore();

    T get(std::string key);
    void put(std::string key, T value);
};

template class KeyValueStore<int>;