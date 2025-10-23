#pragma once
#include <memory>
#include <vector>
#include <string>
#include "errors.h"

namespace core {

    template <typename T>
    struct List {
        struct Node {
            T value;
            std::unique_ptr<Node> next;
            explicit Node(T v) : value(std::move(v)), next(nullptr) {}
        };

        std::unique_ptr<Node> head;

        static List from_vector(const std::vector<T>& v) {
            List lst;
            for (auto it = v.rbegin(); it != v.rend(); ++it) {
                auto n = std::make_unique<Node>(*it);
                n->next = std::move(lst.head);
                lst.head = std::move(n);
            }
            return lst;
        }

        std::vector<T> to_vector() const {
            std::vector<T> out;
            const Node* cur = head.get();
            while (cur) {
                out.push_back(cur->value);
                cur = cur->next.get();
            }
            return out;
        }

        static std::unique_ptr<Node> reverse_recursive(std::unique_ptr<Node> node) {
            if (!node || !node->next) {
                return node; 
            }
            auto new_head = reverse_recursive(std::move(node->next));
            Node* tail = new_head.get();
            while (tail->next) tail = tail->next.get();
            tail->next = std::move(node);
            tail->next->next.reset(); 
            return new_head;
        }

        static std::pair<std::unique_ptr<Node>, Node*> reverse_recursive_linear(std::unique_ptr<Node> node) {
            if (!node) return { nullptr, nullptr };
            if (!node->next) {
                Node* tail = node.get();
                return { std::move(node), tail };
            }
            auto [new_head, tail] = reverse_recursive_linear(std::move(node->next));
            tail->next = std::move(node);
            tail->next->next.reset();
            tail = tail->next.get();
            return { std::move(new_head), tail };
        }

        void reverse() {
            auto [new_head, _] = reverse_recursive_linear(std::move(head));
            head = std::move(new_head);
        }
    };

} 
