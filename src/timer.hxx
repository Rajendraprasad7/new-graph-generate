#pragma once
#include <chrono>

/**
 * @brief Measures the time taken by a function to execute.
 * @tparam Function The type of the function to measure the time of.
 * @tparam Args The types of the arguments to the function.
 * @param func The function to measure the time of.
 * @param args The arguments to the function.
 * @return The time taken by the function to execute in milliseconds.
 */
template<typename Function, typename... Args>
auto measureTime(Function&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    std::invoke(std::forward<Function>(func), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
