#pragma once
#include <unordered_set>
#include <vector>
#include <list>
#include <deque>
#include <type_traits>
#include "errors.h"

namespace core {

    template <typename Container>
    auto dedup_preserve_order(const Container& input) {
        using T = typename Container::value_type;

        std::unordered_set<T> seen;
        std::vector<T> unique;
        unique.reserve(input.size());

        for (const auto& el : input) {
            if (seen.find(el) == seen.end()) {
                seen.insert(el);
                unique.push_back(el);
            }
        }

        if constexpr (std::is_same_v<Container, std::vector<T>>) {
            return unique; 
        }
        else if constexpr (std::is_same_v<Container, std::list<T>>) {
            return std::list<T>(unique.begin(), unique.end());
        }
        else if constexpr (std::is_same_v<Container, std::deque<T>>) {
            return std::deque<T>(unique.begin(), unique.end());
        }
        else {
            return unique;
        }
    }

} 
