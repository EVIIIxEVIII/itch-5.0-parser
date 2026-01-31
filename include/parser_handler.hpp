#pragma once

#include <benchmark/benchmark.h>
#include <cstdint>
#include <absl/container/flat_hash_map.h>
#include <emmintrin.h>
#include <x86intrin.h>
#include "itch_parser.hpp"
#include "levels/vector_levels_b_search_split.hpp"
#include "order_book.hpp"
#include "order_book_shared.hpp"
#include "spsc_queue.hpp"

class ParserHandler {
public:
    ParserHandler(SPSCQueue<OB::BestLvlChange>& strategy_queue)
        : strategy_queue{strategy_queue}
    {}

    void handle(const ITCH::StockDirectory&);
    void handle(const ITCH::AddOrderNoMpid&);
    void handle(const ITCH::AddOrderMpid&);
    void handle(const ITCH::OrderExecuted&);
    void handle(const ITCH::OrderExecutedPrice&);
    void handle(const ITCH::OrderCancel&);
    void handle(const ITCH::OrderDelete&);
    void handle(const ITCH::OrderReplace&);
    void handle(const ITCH::SystemEvent&);


    void handle_after();
    void handle_before();
    void reset();

    absl::flat_hash_map<uint64_t, uint64_t> latency_distribution;
    bool last_message = false;
    uint64_t total_messages = 0;

private:
    void publish_ob_change(const std::optional<OB::BestLvlChange>& best_lvl_change);
    void touch();

    OB::OrderBook<OB::VectorLevelBSearchSplit> order_book;

    bool touched = false;
    unsigned aux_start, aux_end;

    SPSCQueue<OB::BestLvlChange>& strategy_queue;

    uint64_t t0;
    uint16_t target_stock_locate = -1;
};

inline void ParserHandler::handle_before() {
    #ifndef PERF
    touched = false;
    t0 = __rdtscp(&aux_start);
    #endif
}

inline void ParserHandler::handle_after() {
    #ifndef PERF
    uint64_t t1 = __rdtscp(&aux_end);
    auto cycles = t1 - t0;

    if (touched) {
        latency_distribution[cycles]++;
    }
    #endif
}

inline void ParserHandler::publish_ob_change(const std::optional<OB::BestLvlChange>& best_lvl_change) {
    //if (best_lvl_change.has_value()) {
    //    bool res = strategy_queue.try_push(best_lvl_change.value());
    //    OB::UNEXPECTED(!res, "Failed to push to strategy queue!");
    //}
}

inline void ParserHandler::touch() {
    touched = true;
    total_messages++;
}

inline void ParserHandler::handle(const ITCH::SystemEvent& msg) {
    if (msg.event_code == 'C') {
        last_message = true;
    }

    touch();
}

inline void ParserHandler::handle(const ITCH::StockDirectory& msg) {
    if (std::string_view(msg.stock, 8) != "NVDA    ") return;
    target_stock_locate = msg.stock_locate;
    touch();
}

inline void ParserHandler::handle(const ITCH::AddOrderNoMpid& msg) {
    if (msg.stock_locate != target_stock_locate) return;

    auto ob_change = order_book.add_order(msg.order_reference_number, static_cast<OB::Side>(msg.buy_sell), msg.shares, msg.price);
    publish_ob_change(ob_change);
    touch();
}

inline void ParserHandler::handle(const ITCH::AddOrderMpid& msg) {
    if (msg.stock_locate != target_stock_locate) return;
    auto ob_change = order_book.add_order(msg.order_reference_number, static_cast<OB::Side>(msg.buy_sell), msg.shares, msg.price);
    publish_ob_change(ob_change);
    touch();
}

inline void ParserHandler::handle(const ITCH::OrderExecuted& msg) {
    if (msg.stock_locate != target_stock_locate) return;
    auto ob_change = order_book.execute_order(msg.order_reference_number, msg.executed_shares);
    publish_ob_change(ob_change);
    touch();
}

inline void ParserHandler::handle(const ITCH::OrderExecutedPrice& msg) {
    if (msg.stock_locate != target_stock_locate) return;
    auto ob_change = order_book.execute_order(msg.order_reference_number, msg.executed_shares);
    publish_ob_change(ob_change);
    touch();
}

inline void ParserHandler::handle(const ITCH::OrderCancel& msg) {
    if (msg.stock_locate != target_stock_locate) return;
    auto ob_change = order_book.cancel_order(msg.order_reference_number, msg.cancelled_shares);
    publish_ob_change(ob_change);
    touch();
}

inline void ParserHandler::handle(const ITCH::OrderDelete& msg) {
    if (msg.stock_locate != target_stock_locate) return;
    auto ob_change = order_book.delete_order(msg.order_reference_number);
    publish_ob_change(ob_change);
    touch();
}

inline void ParserHandler::handle(const ITCH::OrderReplace& msg) {
    if (msg.stock_locate != target_stock_locate) return;
    auto ob_change = order_book.replace_order(msg.order_reference_number, msg.new_reference_number, msg.shares, msg.price);
    publish_ob_change(ob_change);
    touch();
}
