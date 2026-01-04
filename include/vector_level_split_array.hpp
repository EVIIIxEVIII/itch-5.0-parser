#pragma once
#include <vector>
#include "order_book_shared.hpp"

namespace OB {

template<Side S>
class VectorLevelBSearch {
public:
    VectorLevelBSearch() {
        levels.reserve(3000);
    }

    void remove(Level level);
    void add(Level level);
    Level best() const;

    std::vector<Level> levels;
};

template<Side S>
inline Level VectorLevelBSearch<S>::best() const {
    return levels.back();
}

template<Side S>
inline void VectorLevelBSearch<S>::remove(Level level) {
    auto it = std::lower_bound(
        levels.begin(), levels.end(), level.price,
        [](const Level& lhs, uint32_t price) {
            if constexpr (S == Side::Bid) {
                return lhs.price < price;
            } else {
                return lhs.price > price;
            }
        }
    );

    UNEXPECTED(it == levels.end() || it->price != level.price, "Remove didn't find a level");
    UNEXPECTED(level.qty > it->qty, "Remove lead to an underflow for level");

    it->qty -= level.qty;
    if (it->qty == 0) {
        levels.erase(it);
    }
}

template<Side S>
inline void VectorLevelBSearch<S>::add(Level level) {
    auto it = std::lower_bound(
        levels.begin(), levels.end(), level.price,
        [](const Level& lhs, uint32_t price) {
            if constexpr (S == Side::Bid) {
                return lhs.price < price;
            } else {
                return lhs.price > price;
            }
        }
    );

    if (it != levels.end() && it->price == level.price) {
        it->qty += level.qty;
    } else {
        levels.insert(it, level);
    }
}

}
