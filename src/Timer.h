#pragma once
#include <chrono>
#include <iostream>

class Timer {
public:
    Timer() {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << (end - start_).count() << " ns elapsed\n";
    }

private:
    std::chrono::_V2::system_clock::time_point start_;
};