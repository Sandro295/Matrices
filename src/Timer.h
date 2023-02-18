#pragma once
#include <chrono>
#include <iostream>

class Timer {
public:
    Timer() {
        start_ = std::chrono::high_resolution_clock::now();
    }
    Timer(const std::string& func_name) : func_name_(func_name) {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << func_name_ << (func_name_!="" ? ": " : "") <<
            (end - start_).count() << " ns elapsed\n";
    }

private:
    std::chrono::_V2::system_clock::time_point start_;
    const std::string func_name_;
};