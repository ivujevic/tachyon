#pragma once

#include <chrono>

namespace utils {
struct Timer {

    using rep = float;
    using clock = std::chrono::high_resolution_clock;
    using time_point_t = std::chrono::time_point<clock>;

    Timer();

    void start();
    rep end() const;

    static time_point_t now();
    static rep duration(const time_point_t& start, const time_point_t& end);

private:
    time_point_t start_;
};
}

