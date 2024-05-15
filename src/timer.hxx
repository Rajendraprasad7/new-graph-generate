#pragma once
#include <chrono>

template<typename Function, typename... Args>
auto measureTime(Function&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    std::invoke(std::forward<Function>(func), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
