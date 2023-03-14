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

    void PrintTime() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = end - start_;
        std::cout << func_name_ << (func_name_ != "" ? ": " : "") <<
            duration.count() / 1'000'000. << " ms elapsed\n";
    }

    ~Timer() {
        PrintTime();
    }

private:
    std::chrono::_V2::system_clock::time_point start_;
    const std::string func_name_;
};
