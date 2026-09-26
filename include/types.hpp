#ifndef _TYPES_H_
#define _TYPES_H_

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>

using string = std::string;
using stringview = std::string_view;

template<typename K, typename V>
using umap = std::unordered_map<K,V>;

template<typename T>
using optional = std::optional<T>;

template <typename T>
using vec = std::vector<T>;

#endif