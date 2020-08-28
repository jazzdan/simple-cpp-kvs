#include <map>
#include <string>

template <typename T>
class KeyValueStoreInterface {
 public:
  virtual T get(std::string key) = 0;
  virtual void put(std::string key, T value) = 0;
  virtual void remove(std::string key) = 0;
};

template <typename T>
class InMemoryKVS : KeyValueStoreInterface<T> {
 private:
  std::map<std::string, T> internalKeyValue;

 public:
  InMemoryKVS();
  virtual T get(std::string key);
  virtual void put(std::string key, T value);
  virtual void remove(std::string key);
};

template <typename T>
InMemoryKVS<T>::InMemoryKVS() {}

template <typename T>
T InMemoryKVS<T>::get(std::string key) {
  return this->internalKeyValue[key];
}

template <typename T>
void InMemoryKVS<T>::put(std::string key, T value) {
  this->internalKeyValue.insert(std::make_pair(key, value));
}

template <typename T>
void InMemoryKVS<T>::remove(std::string key) {
  this->internalKeyValue.erase(key);
}