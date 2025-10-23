#pragma once
#include <stdexcept>
#include <string>
#include <limits>
#include <type_traits>

namespace core {

    struct ValidationError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct OverflowError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    template <typename T>
    constexpr bool is_natural(T value) {
        if constexpr (std::is_signed_v<T>)
            return value > 0;
        else
            return value != 0;
    }

} 