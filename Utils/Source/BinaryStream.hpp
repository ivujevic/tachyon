#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <bitset>

namespace utils {
class BinaryOutStream {
public:
    explicit BinaryOutStream(const char* path, unsigned long capacity = 1024 * 1024) : file_(fopen(path, "wb"), &fclose), capacity_(capacity) {

    }

    explicit BinaryOutStream(std::unique_ptr<FILE, decltype(&fclose)>&& f, unsigned long capacity = 1024 * 1024) : file_(std::move(f)), capacity_(capacity) {

    }

    BinaryOutStream& operator<<(const std::string& v) {
        writeArray(v, v.size());
        return *this;
    }

    template<typename T>
    BinaryOutStream& operator<<(const std::vector<T>& v) {
        writeArray(v, v.size());
        return *this;
    }

    template<typename A, typename B>
    BinaryOutStream& operator<<(const std::pair<A, B>& p) {
        *this << p.first << p.second;
        return *this;
    }

    template<typename K, typename V>
    BinaryOutStream& operator<<(const std::map<K, V>& v) {
        writeArray(v, v.size());
        return *this;
    }

    template<typename K, typename V>
    BinaryOutStream& operator<<(const std::unordered_map<K, V>& v) {
        writeArray(v, v.size());
        return *this;
    }

    template<typename T>
    BinaryOutStream& operator<<(const std::shared_ptr<T>& p) {
        *this << *p;
        return *this;
    }

    void write(const void* data, size_t n);

    template<typename T>
    std::enable_if_t<std::is_trivial_v<T> && !std::is_enum_v<T>, BinaryOutStream&> operator<<(const T& v) {
        write(v);
        return *this;
    }

    void flush();
    unsigned long position_ = 0;
private:
    template <typename T>
    void write(const T& v) {
        static_assert(std::is_trivial_v<T>, "non trivial type");
        write(&v, sizeof(v));
    }

    template <typename T>
    void writeArray(const T& value, size_t count) {
        write((unsigned int) count);
        for (const auto& it : value) *this << it;
    }

    std::unique_ptr<FILE, decltype(&fclose)> file_;
    std::vector<char> data_;
    unsigned long capacity_;
};

class BinaryInStream {
public:

    explicit BinaryInStream(const char* path) : file_(fopen(path, "rb"),  &fclose) {

    }

    template<typename T>
    T load() {
        static_assert(std::is_trivial_v<T>, "non trivial type");

        char out[sizeof(T)];
        auto readed = fread(out, sizeof(char), sizeof(T), file_.get());
        if (readed != sizeof(T)) {
            printf("Trivial type read error\n");
            std::terminate();
        }
        return *reinterpret_cast<const T*>(&out);
    }

    BinaryInStream& operator>>(std::bitset<40>& bitset) {
        unsigned long tmp;

        auto readed(fread(&tmp, sizeof(char), 5, file_.get()));
        if (readed != 5) {
            printf("Bitset read error\n");
            std::terminate();
        }
        bitset = std::bitset<40>(tmp);

        return *this;
    }

    template<typename K, typename V>
    BinaryInStream& operator>>(std::map<K, V>& map) {
        auto size(load<unsigned int>());
        while (size) {
            K key;
            V value;
            *this >> key >> value;
            map.emplace(std::move(key), std::move(value));
            size--;
        }

        return *this;
    }

    template<typename K, typename V>
    BinaryInStream& operator>>(std::unordered_map<K, V>& map) {
        auto size(load<unsigned int>());
        while (size) {
            K key;
            V value;
            *this >> key >> value;
            map.emplace(std::move(key), std::move(value));
            size--;
        }

        return *this;
    }

    template<typename T>
    BinaryInStream& operator>>(std::vector<T>& v) {
        auto size(load<unsigned int>());
        v.reserve(size);

        while(size) {
            T t;
            *this >> t;
            v.emplace_back(std::move(t));
            size--;
        }

        return *this;
    }
    BinaryInStream& operator>>(std::string& s) {
        auto size = load<unsigned int>();

        if (size != 0) {
            s.resize(size);
            auto readed(fread(s.data(), sizeof(char), size, file_.get()));
            if (readed != size) {
                printf("Read error\n");
                std::terminate();
            }
        }

        return *this;
    }

    template <typename T>
    typename std::enable_if_t<std::is_trivial_v<T> && !std::is_enum_v<T>, BinaryInStream&> operator>>(T& t) {
        t = load<T>();
        return *this;
    }

    void seekOn(long position, int type) {
        if (type != SEEK_END && type != SEEK_CUR && type != SEEK_SET) return;
        fseek(file_.get(), position, type);
    }

    unsigned int getPosition() {
        return ftell(file_.get());
    }

private:
    std::unique_ptr<FILE, decltype(&fclose)> file_;
};
}

