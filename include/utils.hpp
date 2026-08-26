#pragma once//redundante, pero bueno

#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>

#include <algorithm>

template <typename T>
bool vector_contains(const std::vector<T>& vector, const T& elem){
    if (std::find(vector.begin(), vector.end(), elem) != vector.end())
    {
        return true;
    }

    return false;
}

#endif