#pragma once
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::system_clock;

struct Timer {
  private:
    system_clock::time_point t1;

  public:
    Timer();
    int stop();
};
