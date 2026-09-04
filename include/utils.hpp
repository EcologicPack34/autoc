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

template <typename Key, typename Value>
Value unorderedmap_get_or_default(const std::unordered_map<Key,Value>& map, const Key& key, const Value& default_value){
    auto elem {map.find(key)};
    return (elem != map.end()) ? elem->second : default_value;
}

#endif