#pragma once

#include <string>
#include <sstream>
#include "utils.h"

namespace toolkit{

class variant : public std::string {
public:
    template<typename T>
    variant(const T &t) : 
        std::string(std::to_string(t)) {
    }

    template<size_t N>
    variant(const char(&s)[N]) : 
        std::string(s, N) {
    }

    variant(const char *cstr) : 
        std::string(cstr) {
    }

    variant(const std::string &other = std::string()) : 
        std::string(other){
    }

    // 类型转换函数

    template<typename T>
    operator T() const {
        return as<T>();
    }
    // 非类类型
    template<typename T>
    typename std::enable_if<!std::is_class<T>::value, T>::type as() const {
        return as_default<T>();
    }
    // 类类型
    template<typename T>
    typename std::enable_if<std::is_class<T>::value, T>::type as() const {
        return T((const std::string &)*this);
    }


    // 比较
    template<typename T>
    bool operator==(const T &t) const {
        return 0 == this->compare(variant(t));
    }

    bool operator==(const char *t) const {
        return 0 == this->compare(t);
    }

private:

    // 默认转换函数
    template <typename T>
    T as_default() const {
        T t;
        std::stringstream ss;
        return ss << *this && ss >> t ? t : T();
    }
};

// 特化版本
template <>
bool variant::as<bool>() const;

template <>
uint8_t variant::as<uint8_t>() const;

}
