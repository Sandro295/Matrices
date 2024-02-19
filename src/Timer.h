#pragma once
#include <chrono>
#include <iostream>
#include <string_view>

class Timer {
    using time = std::chrono::high_resolution_clock;
public:
    Timer() {
        start_ = time::now();
    }

    Timer(std::string_view func_name) : func_name_(func_name) {
        start_ = time::now();
    }

    void start() {
        start_ = time::now();
    }

    void stop() {
        end_ = time::now();
    }

    auto getTimeMs() {
        const auto duration = end_ - start_;
        return duration.count() / 1'000'000.;
    }

    void PrintTime() {
        const auto duration = end_ - start_;
        std::cout << func_name_ << (func_name_ != "" ? ": " : "") <<
            duration.count() / 1'000'000. << " ms elapsed\n";
        already_done = true;
    }

    ~Timer() {
        if (already_done) {
            return;
        }
        stop();
        PrintTime();
    }

private:
    bool already_done{false};
    std::chrono::_V2::system_clock::time_point start_;
    std::chrono::_V2::system_clock::time_point end_;
    const std::string_view func_name_;
};
