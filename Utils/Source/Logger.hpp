#pragma once

#include <string_view>
#include <cstdio>

namespace logg {
struct Logger {


    template<class... Args>
    void log(std::string_view, Args&&...) {
        return;
    }

};

extern Logger& ERROR;
extern Logger& INFO;

template <class... Args>
void info(std::string_view fmt, Args&&...) {
    printf("%.*s", (int) fmt.size(), fmt.data());
//    INFO.log(fmt, std::forward<Args>(args)...);
}


template <class... Args>
void error(std::string_view fmt, Args&&... args) {
    ERROR.log(fmt, std::forward<Args>(args)...);
}

}
