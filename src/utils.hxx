#pragma once

#include <functional> 
#include <utility>    

/**
 * @brief Hashes a pair of values.
 * @tparam T1 The type of the first value in the pair.
 * @tparam T2 The type of the second value in the pair.
 */
struct PairHash {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& pair) const {
        size_t hash1 = std::hash<T1>{}(pair.first);
        size_t hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};
