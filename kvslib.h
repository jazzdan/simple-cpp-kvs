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
    void remove(std::string key);
};

template class KeyValueStore<int>;
template class KeyValueStore<std::string>;