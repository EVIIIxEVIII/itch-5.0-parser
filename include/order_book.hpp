#pragma once
#include <cstdint>
#include <absl/container/flat_hash_map.h>
#include <optional>
#include "order_book_shared.hpp"

namespace OB {

template<template<Side> typename Levels>
class OrderBook {
public:
    std::optional<BestLvlChange> add_order(uint64_t order_id, Side side, uint32_t qty, uint32_t price);
    std::optional<BestLvlChange> cancel_order(uint64_t order_id, uint32_t qty);
    std::optional<BestLvlChange> execute_order(uint64_t order_id, uint32_t qty);
    std::optional<BestLvlChange> replace_order(uint64_t order_id, uint64_t new_order_id, uint32_t qty, uint32_t price);
    std::optional<BestLvlChange> delete_order(uint64_t order_id);

    Level best_bid();
    Level best_ask();

    absl::flat_hash_map<uint64_t, Order> orders_map;
    Levels<Side::Bid> bid_levels;
    Levels<Side::Ask> ask_levels;
};

template<template<Side> typename Levels>
Level OrderBook<Levels>::best_bid() {
    return bid_levels.best();
}

template<template<Side> typename Levels>
Level OrderBook<Levels>::best_ask() {
    return ask_levels.best();
}

template<template<Side> typename Levels>
std::optional<BestLvlChange> OrderBook<Levels>::add_order(uint64_t order_id, Side side, uint32_t qty, uint32_t price) {
    Order order;
    order.qty = qty;
    order.side = side;
    order.price = price;
    orders_map.insert({order_id, order});

    std::optional<BestLvlChange> best_lvl_change;

    if (side == Side::Bid) {
        best_lvl_change = bid_levels.add({qty, price});
    } else {
        best_lvl_change = ask_levels.add({qty, price});
    }

    return best_lvl_change;
}

template<template<Side> typename Levels>
std::optional<BestLvlChange> OrderBook<Levels>::cancel_order(uint64_t order_id, uint32_t qty) {
    UNEXPECTED(!orders_map.contains(order_id), "Cancel order did not find an order");
    Order& order = orders_map.at(order_id);
    UNEXPECTED(order.qty < qty, "Partial cancel order volume greater than order volume");

    std::optional<BestLvlChange> best_lvl_change;

    if (order.side == Side::Bid) {
        best_lvl_change = bid_levels.remove({qty, order.price});
    } else {
        best_lvl_change = ask_levels.remove({qty, order.price});
    }

    order.qty -= qty;
    if (order.qty == 0) {
        orders_map.erase(order_id);
    }

    return best_lvl_change;
}

template<template<Side> typename Levels>
std::optional<BestLvlChange> OrderBook<Levels>::execute_order(uint64_t order_id, uint32_t qty) {
    UNEXPECTED(!orders_map.contains(order_id), "Execute order did not find an order");
    Order& order = orders_map.at(order_id);
    UNEXPECTED(order.qty < qty, "Partial execute order volume greater than order volume");

    std::optional<BestLvlChange> best_lvl_change;

    if (order.side == Side::Bid) {
        best_lvl_change = bid_levels.remove({qty, order.price});
    } else {
        best_lvl_change = ask_levels.remove({qty, order.price});
    }

    order.qty -= qty;
    if (order.qty == 0) {
        orders_map.erase(order_id);
    }

    return best_lvl_change;
}

template<template<Side> typename Levels>
std::optional<BestLvlChange> OrderBook<Levels>::replace_order(uint64_t order_id, uint64_t new_order_id, uint32_t qty, uint32_t price) {
    UNEXPECTED(!orders_map.contains(order_id), "Replace order did not find an order");
    Order& old_order = orders_map.at(order_id);

    Order new_order;
    new_order.side = old_order.side;
    new_order.price = price;
    new_order.qty = qty;

    std::optional<BestLvlChange> op1;
    std::optional<BestLvlChange> op2;

    if (new_order.side == Side::Bid) {
        op1 = bid_levels.remove({old_order.qty, old_order.price});
        op2 = bid_levels.add({qty, price});
    } else {
        op1 = ask_levels.remove({old_order.qty, old_order.price});
        op2 = ask_levels.add({qty, price});
    }

    orders_map.insert({new_order_id, new_order}); // old_order cannot be used after this point
    orders_map.erase(order_id);

    if (op2 != std::nullopt) {
        return op2;
    } else if (op1 != std::nullopt) {
        return op1;
    }
    return std::nullopt;
}

template<template<Side> typename Levels>
std::optional<BestLvlChange> OrderBook<Levels>::delete_order(uint64_t order_id) {
    UNEXPECTED(!orders_map.contains(order_id), "Delete order did not find an order");
    Order& order = orders_map.at(order_id);

    std::optional<BestLvlChange> best_lvl_change;

    if (order.side == Side::Bid) {
        best_lvl_change = bid_levels.remove({order.qty, order.price});
    } else {
        best_lvl_change = ask_levels.remove({order.qty, order.price});
    }

    orders_map.erase(order_id);
    return best_lvl_change;
}

}

