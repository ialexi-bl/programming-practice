#include "benchmark.hpp"

Timer::Timer() : t1(std::chrono::high_resolution_clock::now())
{
}

int Timer::stop()
{
    system_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    return duration_cast<milliseconds>(t2 - t1).count();
}