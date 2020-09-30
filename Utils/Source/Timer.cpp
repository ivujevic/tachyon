
#include "Timer.hpp"

namespace utils {
Timer::Timer() : start_(Timer::now()){

}

void Timer::start() {
    start_ = Timer::now();
}

Timer::rep Timer::end() const {
    return duration(start_, Timer::now());
}

Timer::time_point_t Timer::now() {
    return Timer::clock::now();
}

Timer::rep Timer::duration(const Timer::time_point_t& start, const Timer::time_point_t& end) {
    return std::chrono::duration_cast<std::chrono::duration<Timer::rep, std::milli>>(end - start).count();

}
}
