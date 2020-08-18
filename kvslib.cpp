#include "kvslib.h"

using namespace std;

template <typename T>
KeyValueStore<T>::KeyValueStore()
{
}

template <typename T>
T KeyValueStore<T>::get(string key)
{
    return this->internalKeyValue[key];
}

template <typename T>
void KeyValueStore<T>::put(string key, T value)
{
    this->internalKeyValue.insert(std::make_pair(key, value));
}

template <typename T>
void KeyValueStore<T>::remove(string key)
{
    this->internalKeyValue.erase(key);
}