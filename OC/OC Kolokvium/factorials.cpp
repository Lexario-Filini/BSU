#include "factorials.h"
#include <iostream>

namespace core {

    std::vector<std::uint64_t> first_n_factorials(std::uint64_t n) {
        if (!is_natural(n)) {
            throw ValidationError("n must be a natural number (> 0)");
        }

        std::vector<std::uint64_t> result;
        result.reserve(static_cast<size_t>(n));

        std::uint64_t acc = 1;
        for (std::uint64_t i = 1; i <= n; ++i) {
            if (i != 0 && acc > std::numeric_limits<std::uint64_t>::max() / i) {
                throw OverflowError("factorial overflow at i=" + std::to_string(i));
            }
            acc *= i;
            result.push_back(acc);
        }

        return result;
    }

} 
