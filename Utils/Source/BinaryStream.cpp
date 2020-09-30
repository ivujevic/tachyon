#include "BinaryStream.hpp"

namespace utils {
void BinaryOutStream::write(const void* data, size_t n) {

    if (data_.size() + n > capacity_) flush();

    data_.insert(data_.end(), static_cast<const char*>(data), static_cast<const char*>(data) + n);
    position_ += n;
}

void BinaryOutStream::flush() {
    if (data_.empty()) return;
    fwrite(data_.data(), sizeof(char), data_.size(), file_.get());
    data_.clear();

}
}
