#pragma once
#include <vector>
#include <algorithm>
#include <optional>
#include "order_book_shared.hpp"

namespace OB {

template<Side S>
class VectorLevelBSearchSplit {
public:
    VectorLevelBSearchSplit() {
        prices.reserve(8000);
        qtys.reserve(8000);
    }

    std::optional<BestLvlChange> remove(Level level);
    std::optional<BestLvlChange> add(Level level);

    Level best() const;

    std::vector<uint32_t> prices;
    std::vector<uint64_t> qtys;
};

template<Side S>
inline Level VectorLevelBSearchSplit<S>::best() const {
    if (!prices.empty()) {
        size_t i = prices.size() - 1;
        return { qtys[i], prices[i] };
    } else {
        return {0, 0};
    }
}

template<Side S>
inline std::optional<BestLvlChange> VectorLevelBSearchSplit<S>::remove(Level level) {
    auto it = std::lower_bound(
        prices.begin(), prices.end(), level.price,
        [](uint32_t lhs, uint32_t price) {
            if constexpr (S == Side::Bid) {
                return lhs < price;
            } else {
                return lhs > price;
            }
        }
    );
    UNEXPECTED(it == prices.end() || *it != level.price, "Remove didn't find a level");

    size_t idx = it - prices.begin();
    UNEXPECTED(level.qty > qtys[idx], "Remove underflow");

    auto qty_bef = qtys.back();
    auto price_bef = prices.back();

    qtys[idx] -= level.qty;
    if (qtys[idx] == 0) {
        prices.erase(prices.begin() + idx);
        qtys.erase(qtys.begin() + idx);
    }

    if (prices.empty()) {
        return std::nullopt;
    }

    auto qty_af = qtys.back();
    auto price_af = prices.back();

    if (qty_bef != qty_af || price_bef != price_af) {
        return std::optional{BestLvlChange {
            .qty = qty_af,
            .price = price_af,
            .side = S
        }};
    } else {
        return std::nullopt;
    }
}

template<Side S>
inline std::optional<BestLvlChange> VectorLevelBSearchSplit<S>::add(Level level) {
    auto it = std::lower_bound(
        prices.begin(), prices.end(), level.price,
        [](uint32_t lhs, uint32_t price) {
            if constexpr (S == Side::Bid) {
                return lhs < price;
            } else {
                return lhs > price;
            }
        }
    );

    size_t idx = it - prices.begin();

    uint64_t qty_bef = 0;
    uint32_t price_bef = 0;

    if (!prices.empty()) {
        qty_bef = qtys.back();
        price_bef = prices.back();
    }

    if (it != prices.end() && *it == level.price) {
        qtys[idx] += level.qty;
    } else {
        prices.insert(it, level.price);
        qtys.insert(qtys.begin() + idx, level.qty);
    }

    auto qty_af = qtys.back();
    auto price_af = prices.back();

    if (qty_bef != qty_af || price_bef != price_af) {
        return std::optional{BestLvlChange {
            .qty = qty_af,
            .price = price_af,
            .side = S
        }};
    } else {
        return std::nullopt;
    }
}

}
