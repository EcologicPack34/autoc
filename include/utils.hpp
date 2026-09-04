#pragma once//redundante, pero bueno

#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>

#include <algorithm>
#include <unordered_map>

template <typename T>
bool vector_contains(const std::vector<T>& vector, const T& elem){
    if (std::find(vector.begin(), vector.end(), elem) != vector.end())
    {
        return true;
    }

    return false;
}

//Note: by returning references in case of default value only lvalue references are accepted, as an rvalue would be destroyed after calling the function and thus causing undefined behaviour
template <typename Key, typename Value>
const Value& unorderedmap_get_or_default(const std::unordered_map<Key,Value>& map, const Key& key, const Value& default_value){
    auto elem {map.find(key)};
    return (elem != map.end()) ? elem->second : default_value;
}

template <typename Key, typename Value>
const Value* unorderedmap_try_get(const std::unordered_map<Key, Value>& map, const Key& key){
    auto elem {map.find(key)};

    return (elem != map.end()) ? &elem->second : nullptr;
}


#endif