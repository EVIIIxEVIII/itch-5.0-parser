#pragma once

#include <cstdint>
#include <chrono>
#include <absl/container/flat_hash_map.h>
#include <x86intrin.h>
#include "array_level.hpp"
#include "heap_level.hpp"
#include "hybrid_vector_map.hpp"
#include "parser_v1.hpp"
#include "vector_level.hpp"
#include "vector_level_split_array.hpp"
#include "array_level_binary_search.hpp"
#include "order_book.hpp"
#include "heap_levels_custom_map.hpp"

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
    void reset();

    OB::OrderBook<OB::HeapLevels> order_book;

    bool touched = false;
    absl::flat_hash_map<uint64_t, uint64_t> latency_distribution;

    uint64_t total_messages = 0;
    unsigned aux_start, aux_end;

    uint64_t t0;

    uint32_t last_price = 0;
    std::vector<uint32_t> prices;


    OrderBookHandlerSingle() {
        prices.reserve(20'000);
    }
};

inline void OrderBookHandlerSingle::handle_before() {
    touched = false;
    t0 = __rdtscp(&aux_start);
}

inline void OrderBookHandlerSingle::handle_after() {
    uint32_t best_bid = order_book.best_bid().price;
    if (last_price != best_bid) {
        prices.push_back(best_bid);
        last_price = best_bid;
    }

    uint64_t t1 = __rdtscp(&aux_end);
    auto cycles = t1 - t0;

    if (touched && aux_end == aux_start) {
        latency_distribution[cycles]++;
    }
}

inline void OrderBookHandlerSingle::handle_stock_directory(const ITCHv1::StockDirectory& msg) {
    if (std::string_view(msg.stock, 8) == "NVDA    ") {
        target_stock_locate = msg.stock_locate;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_add_order_no_mpid(const ITCHv1::AddOrderNoMpid& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.add_order(msg.order_reference_number, static_cast<OB::Side>(msg.buy_sell), msg.shares, msg.price);
        touched = true;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_add_order_mpid(const ITCHv1::AddOrderMpid& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.add_order(msg.order_reference_number, static_cast<OB::Side>(msg.buy_sell), msg.shares, msg.price);
        touched = true;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_order_executed(const ITCHv1::OrderExecuted& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.execute_order(msg.order_reference_number, msg.executed_shares);
        touched = true;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_order_executed_price(const ITCHv1::OrderExecutedPrice& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.execute_order(msg.order_reference_number, msg.executed_shares);
        touched = true;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_order_cancel(const ITCHv1::OrderCancel& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.cancel_order(msg.order_reference_number, msg.cancelled_shares);
        touched = true;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_order_delete(const ITCHv1::OrderDelete& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.delete_order(msg.order_reference_number);
        touched = true;
        total_messages++;
    }
}

inline void OrderBookHandlerSingle::handle_order_replace(const ITCHv1::OrderReplace& msg) {
    if (msg.stock_locate == target_stock_locate) {
        order_book.replace_order(msg.order_reference_number, msg.new_reference_number, msg.shares, msg.price);
        touched = true;
        total_messages++;
    }
}
