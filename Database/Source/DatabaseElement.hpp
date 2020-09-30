#pragma once

#include <string>

namespace tachyon {

struct DatabaseElement {

    DatabaseElement(unsigned long id, std::string name, std::string sequence) : id_(id), name_(std::move(name)), sequence_(std::move(sequence)) {

    }

    const unsigned long id_;
    const std::string name_;
    const std::string sequence_;
};
}
