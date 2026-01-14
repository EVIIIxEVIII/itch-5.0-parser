#pragma once
#include <absl/container/btree_map.h>
#include <absl/container/flat_hash_map.h>
#include <cstring>
#include <sys/mman.h>
#include "order_book_shared.hpp"

// p50 = 52ns
// p95 = 170ns
// p99 = 568ns
// p999 = 1812ns

namespace OB {

template<Side S>
class MapLevels {
public:
    void add(Level level);
    void remove(Level level);
    Level best();

private:
    uint32_t best_idx = 0;
    absl::flat_hash_map<uint32_t, uint64_t> book;
};

template<Side S>
inline void MapLevels<S>::add(Level level) {
    book[level.price] += level.qty;

    if constexpr (S == Side::Bid) {
        if (level.price > best_idx) {
            best_idx = level.price;
        }
    } else {
        if (level.price < best_idx) {
            best_idx = level.price;
        }
    }
}

template<Side S>
inline void MapLevels<S>::remove(Level level) {
    auto it = book.find(level.price);
    UNEXPECTED(it == book.end(), "Remove didn't find a level");
    UNEXPECTED(level.qty > it->second, "Remove underflow");

    it->second -= level.qty;
    if (it->second == 0) {
        book.erase(it);

        if (level.price == best_idx && !book.empty()) {
            if constexpr (S == Side::Bid) {
                while (book.find(best_idx) == book.end()) {
                    best_idx--;
                }
            } else {
                while (book.find(best_idx) == book.end()) {
                    best_idx++;
                }
            }
        }
    }
}

template<Side S>
inline Level MapLevels<S>::best() {
    auto it = book.find(best_idx);

    if (it == book.end()) return {0, 0};
    return {best_idx, it->second};
}

}

