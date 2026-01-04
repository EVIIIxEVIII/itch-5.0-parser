#pragma once

#include <cstdint>
#include <chrono>
#include <absl/container/flat_hash_map.h>
#include "array_level.hpp"
#include "heap_level.hpp"
#include "parser_v1.hpp"
#include "vector_level.hpp"
#include "vector_level_split_array.hpp"
#include "array_level_binary_search.hpp"
#include "order_book.hpp"

struct OrderBookHandlerSingle {
    uint16_t target_stock_locate = -1;

    void handle_system_event(const ITCHv1::SystemEvent&) {}
    void handle_trading_action(const ITCHv1::TradingAction&) {}
    void handle_reg_sho(const ITCHv1::RegSho&) {}
    void handle_market_participant_pos(const ITCHv1::MarketParticipantPos&) {}
    void handle_mwcb_decline_level(const ITCHv1::MwcbDeclineLevel&) {}
    void handle_mwcb_status(const ITCHv1::MwcbStatus&) {}
    void handle_ipo_quotation_period_upd(const ITCHv1::IpoQuotationPeriodUpd&) {}
    void handle_luld_auction_collar(const ITCHv1::LuldAuctionCollar&) {}
    void handle_operational_halt(const ITCHv1::OperationalHalt&) {}
    void handle_trade_msg_non_cross(const ITCHv1::TradeMessageNonCross&) {}
    void handle_trade_msg_cross(const ITCHv1::TradeMessageCross&) {}
    void handle_broken_trade(const ITCHv1::BrokenTrade&) {}
    void handle_noii(const ITCHv1::Noii&) {}
    void handle_direct_listing_capital_raise(const ITCHv1::DirectListingCapitalRaise&) {}

    void handle_stock_directory(const ITCHv1::StockDirectory&);
    void handle_add_order_no_mpid(const ITCHv1::AddOrderNoMpid&);
    void handle_add_order_mpid(const ITCHv1::AddOrderMpid&);
    void handle_order_executed(const ITCHv1::OrderExecuted&);
    void handle_order_executed_price(const ITCHv1::OrderExecutedPrice&);
    void handle_order_cancel(const ITCHv1::OrderCancel&);
    void handle_order_delete(const ITCHv1::OrderDelete&);
    void handle_order_replace(const ITCHv1::OrderReplace&);

    void handle_after();
    void handle_before();

    using clock = std::chrono::high_resolution_clock;
    OB::OrderBook<OB::HeapLevels> order_book;

    bool touched = false;
    std::chrono::time_point<clock> start;
    absl::flat_hash_map<uint64_t, uint64_t> latency_distribution;
};

inline void OrderBookHandlerSingle::handle_before() {
    touched = false;
    start = clock::now();
}

inline void OrderBookHandlerSingle::handle_after() {
    auto end = clock::now();
    auto latency = std::chrono::round<std::chrono::nanoseconds>(end - start).count();
    if (touched) {
        latency_distribution[latency]++;
    }
}

inline void OrderBookHandlerSingle::handle_stock_directory(const ITCHv1::StockDirectory& msg) {
    if (std::string_view(msg.stock, 8) == "NVDA    ") {
        target_stock_locate = msg.stock_locate;
    }
}

inline void OrderBookHandlerSingle::handle_add_order_no_mpid(const ITCHv1::AddOrderNoMpid& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.add_order(msg.order_reference_number, static_cast<OB::Side>(msg.buy_sell), msg.shares, msg.price);
        touched = true;
    }
}

inline void OrderBookHandlerSingle::handle_add_order_mpid(const ITCHv1::AddOrderMpid& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.add_order(msg.order_reference_number, static_cast<OB::Side>(msg.buy_sell), msg.shares, msg.price);
        touched = true;
    }
}

inline void OrderBookHandlerSingle::handle_order_executed(const ITCHv1::OrderExecuted& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.execute_order(msg.order_reference_number, msg.executed_shares);
        touched = true;
    }
}

inline void OrderBookHandlerSingle::handle_order_executed_price(const ITCHv1::OrderExecutedPrice& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.execute_order(msg.order_reference_number, msg.executed_shares);
        touched = true;
    }
}

inline void OrderBookHandlerSingle::handle_order_cancel(const ITCHv1::OrderCancel& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.cancel_order(msg.order_reference_number, msg.cancelled_shares);
        touched = true;
    }
}

inline void OrderBookHandlerSingle::handle_order_delete(const ITCHv1::OrderDelete& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.delete_order(msg.order_reference_number);
        touched = true;
    }
}

inline void OrderBookHandlerSingle::handle_order_replace(const ITCHv1::OrderReplace& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.replace_order(msg.order_reference_number, msg.new_reference_number, msg.shares, msg.price);
        touched = true;
    }
}
