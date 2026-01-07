#pragma once

#include <cstdint>
#include <absl/container/flat_hash_map.h>
#include <x86intrin.h>
#include "heap_level.hpp"
#include "itch_parser.hpp"
#include "order_book.hpp"

struct BenchmarkParsing {
    uint16_t target_stock_locate = -1;

inline void handle(const ITCH::AddOrderNoMpid& msg);
    inline void handle(const ITCH::OrderCancel& msg);
    inline void handle(const ITCH::OrderDelete& msg);
    inline void handle(const ITCH::OrderReplace& msg);

    inline void handle(const ITCH::SystemEvent& msg);
    inline void handle(const ITCH::StockDirectory& msg);
    inline void handle(const ITCH::TradingAction& msg);
    inline void handle(const ITCH::RegSho& msg);
    inline void handle(const ITCH::MarketParticipantPos& msg);
    inline void handle(const ITCH::MwcbDeclineLevel& msg);
    inline void handle(const ITCH::MwcbStatus& msg);
    inline void handle(const ITCH::IpoQuotationPeriodUpd& msg);
    inline void handle(const ITCH::LuldAuctionCollar& msg);
    inline void handle(const ITCH::OperationalHalt& msg);

    inline void handle(const ITCH::AddOrderMpid& msg);
    inline void handle(const ITCH::OrderExecuted& msg);
    inline void handle(const ITCH::OrderExecutedPrice& msg);

    inline void handle(const ITCH::TradeMessageNonCross& msg);
    inline void handle(const ITCH::TradeMessageCross& msg);
    inline void handle(const ITCH::BrokenTrade& msg);
    inline void handle(const ITCH::Noii& msg);
    inline void handle(const ITCH::DirectListingCapitalRaise& msg);

    void handle_after();
    void handle_before();
    void reset();

    OB::OrderBook<OB::HeapLevels> order_book;
    absl::flat_hash_map<uint64_t, uint64_t> latency_distribution;

    uint64_t total_messages = 0;
    unsigned aux_start, aux_end;

    uint64_t t0;
};

inline void BenchmarkParsing::handle_before() {
    t0 = __rdtscp(&aux_start);
}

inline void BenchmarkParsing::handle_after() {
    uint64_t t1 = __rdtscp(&aux_end);
    auto cycles = t1 - t0;
    latency_distribution[cycles]++;
}

inline void BenchmarkParsing::handle(const ITCH::AddOrderNoMpid& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::OrderCancel& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::OrderDelete& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::OrderReplace& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::SystemEvent& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::StockDirectory& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::TradingAction& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::RegSho& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::MarketParticipantPos& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::MwcbDeclineLevel& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::MwcbStatus& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::IpoQuotationPeriodUpd& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::LuldAuctionCollar& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::OperationalHalt& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::AddOrderMpid& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::OrderExecuted& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::OrderExecutedPrice& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::TradeMessageNonCross& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::TradeMessageCross& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::BrokenTrade& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::Noii& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

inline void BenchmarkParsing::handle(const ITCH::DirectListingCapitalRaise& msg) {
    total_messages++;
    asm volatile("" : : "r,m"(msg));
}

