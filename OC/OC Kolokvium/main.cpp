#include <iostream>
#include <vector>
#include <string>
#include "errors.h"
#include "factorials.h"
#include "dedup.h"
#include "linked_list.h"

static void print_vector_ull(const std::vector<std::uint64_t>& v, const char* label) {
    std::cerr << label << " size=" << v.size() << "\n";
    std::cout << label << ": ";
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";
}

template <typename T>
static void print_vector(const std::vector<T>& v, const char* label) {
    std::cerr << label << " size=" << v.size() << "\n";
    std::cout << label << ": ";
    for (const auto& x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    try {
        std::uint64_t n;
        std::cout << "Enter n (natural number for factorials): ";
        if (!(std::cin >> n)) {
            std::cerr << "[ERROR] Invalid input for n\n";
            return 1;
        }

        auto facts = core::first_n_factorials(n);
        print_vector_ull(facts, "Factorials");

        std::cout << "Enter the number of elements in the array: ";
        size_t m;
        if (!(std::cin >> m)) {
            std::cerr << "[ERROR] Invalid input for array size\n";
            return 1;
        }

        std::vector<int> data;
        data.reserve(m);
        std::cout << "Enter " << m << " integers: ";
        for (size_t i = 0; i < m; ++i) {
            int val;
            if (!(std::cin >> val)) {
                std::cerr << "[ERROR] Invalid input for array element\n";
                return 1;
            }
            data.push_back(val);
        }

        auto unique_data = core::dedup_preserve_order(data);
        print_vector(unique_data, "Array without duplicates");

        std::cout << "Enter the number of words for the list: ";
        size_t k;
        if (!(std::cin >> k)) {
            std::cerr << "[ERROR] Invalid input for list size\n";
            return 1;
        }

        std::vector<std::string> words;
        words.reserve(k);
        std::cout << "Enter " << k << " words: ";
        for (size_t i = 0; i < k; ++i) {
            std::string s;
            if (!(std::cin >> s)) {
                std::cerr << "[ERROR] Invalid input for word\n";
                return 1;
            }
            words.push_back(s);
        }

        auto lst = core::List<std::string>::from_vector(words);
        std::cout << "List before reverse: ";
        for (auto& w : lst.to_vector()) std::cout << w << " ";
        std::cout << "\n";

        lst.reverse();

        std::cout << "List after reverse:  ";
        for (auto& w : lst.to_vector()) std::cout << w << " ";
        std::cout << "\n";

        return 0;
    }
    catch (const core::ValidationError& e) {
        std::cerr << "[ERROR] ValidationError: " << e.what() << "\n";
        return 1;
    }
    catch (const core::OverflowError& e) {
        std::cerr << "[ERROR] OverflowError: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Unhandled exception: " << e.what() << "\n";
        return 1;
    }
}
