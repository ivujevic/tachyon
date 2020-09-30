#pragma once

namespace utils {
template <typename... Base>
struct Visitor : public Base... {
    using Base::operator()...;
};

template <typename... T>
Visitor(T...)->Visitor<T...>;
}
