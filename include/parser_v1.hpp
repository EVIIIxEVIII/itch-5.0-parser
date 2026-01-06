#pragma once

#include <cstddef>
#include <stdint.h>
#include <cstring>
#include <string>
#include <stdexcept>
#include <array>

namespace ITCHv1 {

constexpr std::array<uint8_t, 22> message_type_chars = {
    'S','R','H','Y','L','V','W','K','J','h',
    'A','F','E','C','X','D','U',
    'P','Q','B','I','O'
};

enum class MessageType {
    SYSTEM_EVENT                = 'S',
    STOCK_DIRECTORY             = 'R',
    STOCK_TRADING_ACTION        = 'H',
    REG_SHO                     = 'Y',
    MARKET_PARTICIPANT_POSITION = 'L',
    MWCB_DECLINE_LEVEL_MESSAGE  = 'V',
    MWCB_STATUS_MESSAGE         = 'W',
    IPO_QUOTING_PERIOD_UPD      = 'K',
    LULD_AUCTION_COLLAR         = 'J',
    OPERATIONAL_HALT            = 'h',

    ADD_ORDER_NO_MPID = 'A',
    ADD_ORDER_MPID    = 'F',

    ORDER_EXECUTED       = 'E',
    ORDER_EXECUTED_PRICE = 'C',
    ORDER_CANCEL         = 'X',
    ORDER_DELETE         = 'D',
    ORDER_REPLACE        = 'U',

    NON_CROSS_TRADE_MSG = 'P',
    CROSS_TRADE_MSG     = 'Q',
    BROKEN_TRADE_MSG    = 'B',

    NOII_MSG = 'I',

    DIRECT_LISTING_CAPITAL_RAISE = 'O',
};

// X('A', ADD_ORDER_NO_MPID,            AddOrderNoMpid,            add_order_no_mpid,             likely) \
// X('X', ORDER_CANCEL,                 OrderCancel,               order_cancel,                  likely) \
// X('D', ORDER_DELETE,                 OrderDelete,               order_delete,                  likely) \
// X('U', ORDER_REPLACE,                OrderReplace,              order_replace,                 likely) \

#define ITCH_MESSAGE_LIST(X) \
    X('S', SYSTEM_EVENT,                 SystemEvent,               system_event,                  unlikely) \
    X('R', STOCK_DIRECTORY,              StockDirectory,            stock_directory,               unlikely) \
    X('H', STOCK_TRADING_ACTION,         TradingAction,             trading_action,                unlikely) \
    X('Y', REG_SHO,                      RegSho,                    reg_sho,                       unlikely) \
    X('L', MARKET_PARTICIPANT_POSITION,  MarketParticipantPos,      market_participant_pos,        unlikely) \
    X('V', MWCB_DECLINE_LEVEL_MESSAGE,   MwcbDeclineLevel,          mwcb_decline_level,            unlikely) \
    X('W', MWCB_STATUS_MESSAGE,          MwcbStatus,                mwcb_status,                   unlikely) \
    X('K', IPO_QUOTING_PERIOD_UPD,       IpoQuotationPeriodUpd,     ipo_quotation_period_upd,      unlikely) \
    X('J', LULD_AUCTION_COLLAR,          LuldAuctionCollar,         luld_auction_collar,           unlikely) \
    X('h', OPERATIONAL_HALT,             OperationalHalt,           operational_halt,              unlikely) \
    X('F', ADD_ORDER_MPID,               AddOrderMpid,              add_order_mpid,                likely) \
    X('E', ORDER_EXECUTED,               OrderExecuted,             order_executed,                likely) \
    X('C', ORDER_EXECUTED_PRICE,         OrderExecutedPrice,        order_executed_price,          likely) \
    X('P', NON_CROSS_TRADE_MSG,          TradeMessageNonCross,      trade_msg_non_cross,           unlikely) \
    X('Q', CROSS_TRADE_MSG,              TradeMessageCross,         trade_msg_cross,               unlikely) \
    X('B', BROKEN_TRADE_MSG,             BrokenTrade,               broken_trade,                  unlikely) \
    X('I', NOII_MSG,                     Noii,                      noii,                          unlikely) \
    X('O', DIRECT_LISTING_CAPITAL_RAISE, DirectListingCapitalRaise, direct_listing_capital_raise,  unlikely)

#define ITCH_MESSAGE_LIST_REDUCED_SET(X) \
    X('R', STOCK_DIRECTORY,              StockDirectory,              stock_directory) \
    X('A', ADD_ORDER_NO_MPID,            AddOrderNoMpid,              add_order_no_mpid) \
    X('F', ADD_ORDER_MPID,               AddOrderMpid,                add_order_mpid) \
    X('E', ORDER_EXECUTED,               OrderExecuted,               order_executed) \
    X('C', ORDER_EXECUTED_PRICE,         OrderExecutedPrice,          order_executed_price) \
    X('X', ORDER_CANCEL,                 OrderCancel,                 order_cancel) \
    X('D', ORDER_DELETE,                 OrderDelete,                 order_delete) \
    X('U', ORDER_REPLACE,                OrderReplace,                order_replace) \

struct SystemEvent {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     event_code;
};

struct StockDirectory {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char  stock[8];
    char  market_category;
    char  financial_status_indicator;
    uint32_t round_lot_size;
    char round_lots_only;
    char issue_classification;
    char issue_sub_type[2];
    char authenticity;
    char short_sale_threshold_indicator;
    char ipo_flag;
    char luld_reference_price_tier;
    char etp_flag;
    uint32_t etp_leverage_factor;
    char inverse_indicator;
};

struct TradingAction {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     stock[8];
    char     trading_state;
    char     reserved;
    char     reason[4];
};

struct RegSho {
    uint16_t locate_code;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     stock[8];
    char     reg_sho_action;
};

struct MarketParticipantPos {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     mpid[4];
    char     stock[8];
    char     primary_market_maker;
    char     market_maker_mode;
    char     market_participant_state;
};

struct MwcbDeclineLevel {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t level1;
    uint64_t level2;
    uint64_t level3;
};

struct MwcbStatus {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     breached_level;
};

struct IpoQuotationPeriodUpd {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     stock[8];
    uint32_t ipo_quotation_release_time;
    char     ipo_quatation_release_qualifier;
    uint32_t ipo_price;
};

struct LuldAuctionCollar {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     stock[8];
    uint32_t auction_collar_reference_price;
    uint32_t upper_auction_collar_price;
    uint32_t lower_auction_collar_price;
    uint32_t auction_collar_extension;
};

struct OperationalHalt {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char     stock[8];
    char     market_code;
    char     operational_halt_action;
};

struct AddOrderNoMpid {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char     buy_sell;
    uint32_t shares;
    char     stock[8];
    uint32_t price;
};

struct AddOrderMpid {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char     buy_sell;
    uint32_t shares;
    char     stock[8];
    uint32_t price;
    char     attribution[4];
};

struct OrderExecuted {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t executed_shares;
    uint64_t match_number;
};

struct OrderExecutedPrice {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t executed_shares;
    uint64_t match_number;
    char     printable;
    uint32_t execution_price;
};

struct OrderCancel {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t cancelled_shares;
};

struct OrderDelete {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
};

struct OrderReplace {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint64_t new_reference_number;
    uint32_t shares;
    uint32_t price;
};

struct TradeMessageNonCross {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char     buy_sell;
    uint32_t shares;
    char     stock[8];
    uint32_t price;
    uint64_t match_number;
};

struct TradeMessageCross {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint32_t shares;
    char     stock[8];
    uint32_t cross_price;
    uint64_t match_number;
    char     cross_type;
};

struct BrokenTrade {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t match_number;
};

struct Noii {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;

    uint64_t paired_shares;
    uint64_t imbalance_shares;
    char     imbalance_direction;

    char     stock[8];
    uint32_t far_price;
    uint32_t near_price;
    uint32_t current_reference_price;
    char     cross_type;
    char     price_variation_indicator;
};

struct DirectListingCapitalRaise {
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;

    char     stock[8];
    char     open_eligibility_status;
    uint32_t minimum_allowable_price;
    uint32_t maximum_allowable_price;
    uint32_t near_execution_price;
    uint64_t near_execution_time;
    uint32_t lower_price_range_collar;
    uint32_t upper_price_range_collar;
};

struct Message {
    MessageType type;
    size_t      size;
    union {
        SystemEvent system_event;
        StockDirectory stock_directory;
        TradingAction trading_action;
        RegSho reg_sho;
        MarketParticipantPos market_participant_pos;
        MwcbDeclineLevel mwcb_decline_level;
        MwcbStatus mwcb_status;
        IpoQuotationPeriodUpd ipo_quotation_period_upd;
        LuldAuctionCollar luld_auction_collar;
        OperationalHalt operational_halt;
        AddOrderNoMpid add_order_no_mpid;
        AddOrderMpid add_order_mpid;
        OrderExecuted order_executed;
        OrderExecutedPrice order_executed_price;
        OrderCancel order_cancel;
        OrderDelete order_delete;
        OrderReplace order_replace;
        TradeMessageNonCross trade_msg_non_cross;
        TradeMessageCross trade_msg_cross;
        BrokenTrade broken_trade;
        Noii noii;
        DirectListingCapitalRaise direct_listing_capital_raise;
    };
};

class ItchParser {
public:
    template <typename Handler>
    void parse(std::byte const *  src, size_t len, Handler& handler);

    template <typename SpecificHandler>
    void parse_specific(std::byte const * src, size_t len, SpecificHandler& handler);
    template <typename SpecificHandler>
    void parse_specific_reduced(std::byte const * src, size_t len, SpecificHandler& handler);
};

inline uint64_t load_be64(const std::byte* p) {
    return (uint64_t(p[0]) << 56) |
           (uint64_t(p[1]) << 48) |
           (uint64_t(p[2]) << 40) |
           (uint64_t(p[3]) << 32) |
           (uint64_t(p[4]) << 24) |
           (uint64_t(p[5]) << 16) |
           (uint64_t(p[6]) << 8)  |
           uint64_t(p[7]);
}

inline uint32_t load_be32(const std::byte* p) {
    return (uint32_t(p[0]) << 24) |
           (uint32_t(p[1]) << 16) |
           (uint32_t(p[2]) << 8)  |
           uint32_t(p[3]);
}

inline uint16_t load_be16(const std::byte* p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}

inline uint64_t load_be48(const std::byte* p) {
    uint32_t hi = load_be32(p);
    uint16_t lo = load_be16(p);
    return (uint64_t(hi) << 16) | lo;
}

template<typename SpecificHandler>
inline void parseSystemEvent(std::byte const* src, SpecificHandler& handler) {
    SystemEvent sysEvent;

    sysEvent.stock_locate = load_be16(src);
    src += 2;
    sysEvent.tracking_number = load_be16(src);
    src += 2;
    sysEvent.timestamp = load_be48(src);
    src += 6;
    sysEvent.event_code = static_cast<char>(src[0]);
    src += 1;

    handler.handle_system_event(sysEvent);
}

template<typename SpecificHandler>
inline void parseStockDirectory(std::byte const * src, SpecificHandler& handler) {
    StockDirectory stockDir;
    stockDir.stock_locate = load_be16(src);
    src += 2; // offsets and this additions compile to the same thing :(
    stockDir.tracking_number = load_be16(src);
    src += 2;
    stockDir.timestamp = load_be48(src);
    src += 6;
    std::memcpy(stockDir.stock, src, 8);
    src += 8;
    stockDir.market_category = static_cast<char>(src[0]);
    src += 1;
    stockDir.financial_status_indicator = static_cast<char>(src[0]);
    src += 1;
    stockDir.round_lot_size = load_be32(src);
    src += 4;
    stockDir.round_lots_only = static_cast<char>(src[0]);
    src += 1;
    stockDir.issue_classification = static_cast<char>(src[0]);
    src += 1;
    std::memcpy(stockDir.issue_sub_type, src, 2);
    src += 2;
    stockDir.authenticity = static_cast<char>(src[0]);
    src += 1;
    stockDir.short_sale_threshold_indicator = static_cast<char>(src[0]);
    src += 1;
    stockDir.ipo_flag = static_cast<char>(src[0]);
    src += 1;
    stockDir.luld_reference_price_tier = static_cast<char>(src[0]);
    src += 1;
    stockDir.etp_flag = static_cast<char>(src[0]);
    src += 1;
    stockDir.etp_leverage_factor = load_be32(src);
    src += 4;
    stockDir.inverse_indicator = static_cast<char>(src[0]);
    src += 1;

    handler.handle_stock_directory(stockDir);
}

template<typename SpecificHandler>
inline void parseTradingAction(std::byte const* src, SpecificHandler& handler) {
    TradingAction tradingAction;

    tradingAction.stock_locate = load_be16(src);
    src += 2;
    tradingAction.tracking_number = load_be16(src);
    src += 2;
    tradingAction.timestamp = load_be48(src);
    src += 6;
    std::memcpy(tradingAction.stock, src, 8);
    src += 8;
    tradingAction.trading_state = static_cast<char>(src[0]);
    src += 1;
    tradingAction.reserved = static_cast<char>(src[0]);
    src += 1;
    std::memcpy(tradingAction.reason, src, 4);
    src += 4;

    handler.handle_trading_action(tradingAction);
}

template<typename SpecificHandler>
inline void parseRegSho(std::byte const* src, SpecificHandler& handler) {
    RegSho regSho;

    regSho.locate_code = load_be16(src);
    src += 2;
    regSho.tracking_number = load_be16(src);
    src += 2;
    regSho.timestamp = load_be48(src);
    src += 6;
    std::memcpy(regSho.stock, src, 8);
    src += 8;
    regSho.reg_sho_action = static_cast<char>(src[0]);
    src += 1;

    handler.handle_reg_sho(regSho);
}

template<typename SpecificHandler>
inline void parseMarketParticipantPos(std::byte const* src, SpecificHandler& handler) {
    MarketParticipantPos marketPartPos;

    marketPartPos.stock_locate = load_be16(src);
    src += 2;
    marketPartPos.tracking_number = load_be16(src);
    src += 2;
    marketPartPos.timestamp = load_be48(src);
    src += 6;
    std::memcpy(marketPartPos.mpid, src, 4);
    src += 4;
    std::memcpy(marketPartPos.stock, src, 8);
    src += 8;
    marketPartPos.primary_market_maker = static_cast<char>(src[0]);
    src += 1;
    marketPartPos.market_maker_mode = static_cast<char>(src[0]);
    src += 1;
    marketPartPos.market_participant_state = static_cast<char>(src[0]);
    src += 1;

    handler.handle_market_participant_pos(marketPartPos);
}

template<typename SpecificHandler>
inline void parseMwcbDeclineLevel(std::byte const* src, SpecificHandler& handler) {
    MwcbDeclineLevel mwcbDeclineLevel;

    mwcbDeclineLevel.stock_locate = load_be16(src);
    src += 2;
    mwcbDeclineLevel.tracking_number = load_be16(src);
    src += 2;
    mwcbDeclineLevel.timestamp = load_be48(src);
    src += 6;

    mwcbDeclineLevel.level1 = load_be64(src);
    src += 8;
    mwcbDeclineLevel.level2 = load_be64(src);
    src += 8;
    mwcbDeclineLevel.level3 = load_be64(src);
    src += 8;

    handler.handle_mwcb_decline_level(mwcbDeclineLevel);
}

template<typename SpecificHandler>
inline void parseMwcbStatus(std::byte const* src, SpecificHandler& handler) {
    MwcbStatus mwcbStatus;

    mwcbStatus.stock_locate = load_be16(src);
    src += 2;
    mwcbStatus.tracking_number = load_be16(src);
    src += 2;
    mwcbStatus.timestamp = load_be48(src);
    src += 6;
    mwcbStatus.breached_level = static_cast<char>(src[0]);
    src += 1;

    handler.handle_mwcb_status(mwcbStatus);
}

template<typename SpecificHandler>
inline void parseIpoQuotationPeriodUpd(std::byte const* src, SpecificHandler& handler) {
    IpoQuotationPeriodUpd ipoQuotingPerUpd;

    ipoQuotingPerUpd.stock_locate = load_be16(src);
    src += 2;
    ipoQuotingPerUpd.tracking_number = load_be16(src);
    src += 2;
    ipoQuotingPerUpd.timestamp = load_be48(src);
    src += 6;
    std::memcpy(ipoQuotingPerUpd.stock, src, 8);
    src += 8;
    ipoQuotingPerUpd.ipo_quotation_release_time = load_be32(src);
    src += 4;
    ipoQuotingPerUpd.ipo_quatation_release_qualifier = static_cast<char>(src[0]);
    src += 1;
    ipoQuotingPerUpd.ipo_price = load_be32(src);
    src += 4;

    handler.handle_ipo_quotation_period_upd(ipoQuotingPerUpd);
}

template<typename SpecificHandler>
inline void parseLuldAuctionCollar(std::byte const* src, SpecificHandler& handler) {
    LuldAuctionCollar luldActionCollar;

    luldActionCollar.stock_locate = load_be16(src);
    src += 2;
    luldActionCollar.tracking_number = load_be16(src);
    src += 2;
    luldActionCollar.timestamp = load_be48(src);
    src += 6;
    std::memcpy(luldActionCollar.stock, src, 8);
    src += 8;
    luldActionCollar.auction_collar_reference_price = load_be32(src);
    src += 4;
    luldActionCollar.upper_auction_collar_price = load_be32(src);
    src += 4;
    luldActionCollar.lower_auction_collar_price = load_be32(src);
    src += 4;
    luldActionCollar.auction_collar_extension = load_be32(src);
    src += 4;

    handler.handle_luld_auction_collar(luldActionCollar);
}

template<typename SpecificHandler>
inline void parseOperationalHalt(std::byte const* src, SpecificHandler& handler) {
    OperationalHalt operationalHalt;

    operationalHalt.stock_locate = load_be16(src);
    src += 2;
    operationalHalt.tracking_number = load_be16(src);
    src += 2;
    operationalHalt.timestamp = load_be48(src);
    src += 6;
    std::memcpy(operationalHalt.stock, src, 8);
    src += 8;
    operationalHalt.market_code = static_cast<char>(src[0]);
    src += 1;
    operationalHalt.operational_halt_action = static_cast<char>(src[0]);
    src += 1;

    handler.handle_operational_halt(operationalHalt);
}

template<typename SpecificHandler>
inline void parseAddOrderNoMpid(std::byte const * src, SpecificHandler& handler) {
    AddOrderNoMpid addOrderNoMpid;

    addOrderNoMpid.stock_locate = load_be16(src);
    src += 2;
    addOrderNoMpid.tracking_number = load_be16(src);
    src += 2;
    addOrderNoMpid.timestamp = load_be48(src);
    src += 6;
    addOrderNoMpid.order_reference_number = load_be64(src);
    src += 8;
    addOrderNoMpid.buy_sell = static_cast<char>(src[0]);
    src += 1;
    addOrderNoMpid.shares = load_be32(src);
    src += 4;
    std::memcpy(addOrderNoMpid.stock, src, 8);
    src += 8;
    addOrderNoMpid.price = load_be32(src);
    src += 4;

    handler.handle_add_order_no_mpid(addOrderNoMpid);
}

template<typename SpecificHandler>
inline void parseAddOrderMpid(std::byte const * src, SpecificHandler& handler) {
    AddOrderMpid addOrderMpid;

    addOrderMpid.stock_locate = load_be16(src);
    src += 2;
    addOrderMpid.tracking_number = load_be16(src);
    src += 2;
    addOrderMpid.timestamp = load_be48(src);
    src += 6;
    addOrderMpid.order_reference_number = load_be64(src);
    src += 8;
    addOrderMpid.buy_sell = static_cast<char>(src[0]);
    src += 1;
    addOrderMpid.shares = load_be32(src);
    src += 4;
    std::memcpy(addOrderMpid.stock, src, 8);
    src += 8;
    addOrderMpid.price = load_be32(src);
    src += 4;
    std::memcpy(addOrderMpid.attribution, src, 4);
    src += 4;

    handler.handle_add_order_mpid(addOrderMpid);
}

template<typename SpecificHandler>
inline void parseOrderExecuted(std::byte const * src, SpecificHandler& handler) {
    OrderExecuted orderExecuted;

    orderExecuted.stock_locate = load_be16(src);
    src += 2;
    orderExecuted.tracking_number = load_be16(src);
    src += 2;
    orderExecuted.timestamp = load_be48(src);
    src += 6;
    orderExecuted.order_reference_number = load_be64(src);
    src += 8;
    orderExecuted.executed_shares = load_be32(src);
    src += 4;
    orderExecuted.match_number = load_be64(src);
    src += 8;

    handler.handle_order_executed(orderExecuted);
}

template<typename SpecificHandler>
inline void parseOrderExecutedPrice(std::byte const * src, SpecificHandler& handler) {
    OrderExecutedPrice orderExecutedPrice;

    orderExecutedPrice.stock_locate = load_be16(src);
    src += 2;
    orderExecutedPrice.tracking_number = load_be16(src);
    src += 2;
    orderExecutedPrice.timestamp = load_be48(src);
    src += 6;
    orderExecutedPrice.order_reference_number = load_be64(src);
    src += 8;
    orderExecutedPrice.executed_shares = load_be32(src);
    src += 4;
    orderExecutedPrice.match_number = load_be64(src);
    src += 8;
    orderExecutedPrice.printable = static_cast<char>(src[0]);
    src += 1;
    orderExecutedPrice.execution_price = load_be32(src);
    src += 4;

    handler.handle_order_executed_price(orderExecutedPrice);
}

template<typename SpecificHandler>
inline void parseOrderCancel(std::byte const * src, SpecificHandler& handler) {
    OrderCancel orderCancel;

    orderCancel.stock_locate = load_be16(src);
    src += 2;
    orderCancel.tracking_number = load_be16(src);
    src += 2;
    orderCancel.timestamp = load_be48(src);
    src += 6;
    orderCancel.order_reference_number = load_be64(src);
    src += 8;
    orderCancel.cancelled_shares = load_be32(src);
    src += 4;

    handler.handle_order_cancel(orderCancel);
}

template<typename SpecificHandler>
inline void parseOrderDelete(std::byte const * src, SpecificHandler& handler) {
    OrderDelete orderDelete;

    orderDelete.stock_locate = load_be16(src);
    src += 2;
    orderDelete.tracking_number = load_be16(src);
    src += 2;
    orderDelete.timestamp = load_be48(src);
    src += 6;
    orderDelete.order_reference_number = load_be64(src);
    src += 8;

    handler.handle_order_delete(orderDelete);
}

template<typename SpecificHandler>
inline void parseOrderReplace(std::byte const * src, SpecificHandler& handler) {
    OrderReplace orderReplace;

    orderReplace.stock_locate = load_be16(src);
    src += 2;
    orderReplace.tracking_number = load_be16(src);
    src += 2;
    orderReplace.timestamp = load_be48(src);
    src += 6;
    orderReplace.order_reference_number = load_be64(src);
    src += 8;
    orderReplace.new_reference_number = load_be64(src);
    src += 8;
    orderReplace.shares = load_be32(src);
    src += 4;
    orderReplace.price = load_be32(src);
    src += 4;

    handler.handle_order_replace(orderReplace);
}

template<typename SpecificHandler>
inline void parseTradeMessageNonCross(std::byte const* src, SpecificHandler& handler) {
    TradeMessageNonCross tradeMessageNonCross;

    tradeMessageNonCross.stock_locate = load_be16(src);
    src += 2;
    tradeMessageNonCross.tracking_number = load_be16(src);
    src += 2;
    tradeMessageNonCross.timestamp = load_be48(src);
    src += 6;
    tradeMessageNonCross.order_reference_number = load_be64(src);
    src += 8;
    tradeMessageNonCross.buy_sell = static_cast<char>(src[0]);
    src += 1;
    tradeMessageNonCross.shares = load_be32(src);
    src += 4;
    std::memcpy(tradeMessageNonCross.stock, src, 8);
    src += 8;
    tradeMessageNonCross.price = load_be32(src);
    src += 4;
    tradeMessageNonCross.match_number = load_be64(src);
    src += 8;

    handler.handle_trade_msg_non_cross(tradeMessageNonCross);
}

template<typename SpecificHandler>
inline void parseTradeMessageCross(std::byte const* src, SpecificHandler& handler) {
    TradeMessageCross tradeMessageCross;

    tradeMessageCross.stock_locate = load_be16(src);
    src += 2;
    tradeMessageCross.tracking_number = load_be16(src);
    src += 2;
    tradeMessageCross.timestamp = load_be48(src);
    src += 6;
    tradeMessageCross.shares = load_be32(src);
    src += 4;
    std::memcpy(tradeMessageCross.stock, src, 8);
    src += 8;
    tradeMessageCross.cross_price = load_be32(src);
    src += 4;
    tradeMessageCross.match_number = load_be64(src);
    src += 8;
    tradeMessageCross.cross_type = static_cast<char>(src[0]);
    src += 1;

    handler.handle_trade_msg_cross(tradeMessageCross);
}

template<typename SpecificHandler>
inline void parseBrokenTrade(std::byte const* src, SpecificHandler& handler) {
    BrokenTrade brokenTrade;

    brokenTrade.stock_locate = load_be16(src);
    src += 2;
    brokenTrade.tracking_number = load_be16(src);
    src += 2;
    brokenTrade.timestamp = load_be48(src);
    src += 6;
    brokenTrade.match_number = load_be64(src);
    src += 8;

    handler.handle_broken_trade(brokenTrade);
}

template<typename SpecificHandler>
inline void parseNoii(std::byte const* src, SpecificHandler& handler) {
    Noii noii;

    noii.stock_locate = load_be16(src);
    src += 2;
    noii.tracking_number = load_be16(src);
    src += 2;
    noii.timestamp = load_be48(src);
    src += 6;
    noii.paired_shares = load_be64(src);
    src += 8;
    noii.imbalance_shares = load_be64(src);
    src += 8;
    noii.imbalance_direction = static_cast<char>(src[0]);
    src += 1;
    std::memcpy(noii.stock, src, 8);
    src += 8;
    noii.far_price = load_be32(src);
    src += 4;
    noii.near_price = load_be32(src);
    src += 4;
    noii.current_reference_price = load_be32(src);
    src += 4;
    noii.cross_type = static_cast<char>(src[0]);
    src += 1;
    noii.price_variation_indicator = static_cast<char>(src[0]);
    src += 1;

    handler.handle_noii(noii);
}

template<typename SpecificHandler>
inline void parseDirectListingCapitalRaise(std::byte const* src, SpecificHandler& handler) {
    DirectListingCapitalRaise directListingCapitalRaise;

    directListingCapitalRaise.stock_locate = load_be16(src);
    src += 2;
    directListingCapitalRaise.tracking_number = load_be16(src);
    src += 2;
    directListingCapitalRaise.timestamp = load_be48(src);
    src += 6;
    std::memcpy(directListingCapitalRaise.stock, src, 8);
    src += 8;
    directListingCapitalRaise.open_eligibility_status = static_cast<char>(src[0]);
    src += 1;
    directListingCapitalRaise.minimum_allowable_price = load_be32(src);
    src += 4;
    directListingCapitalRaise.maximum_allowable_price = load_be32(src);
    src += 4;
    directListingCapitalRaise.near_execution_price = load_be32(src);
    src += 4;
    directListingCapitalRaise.near_execution_time = load_be64(src);
    src += 8;
    directListingCapitalRaise.lower_price_range_collar = load_be32(src);
    src += 4;
    directListingCapitalRaise.upper_price_range_collar = load_be32(src);
    src += 4;

    handler.handle_direct_listing_capital_raise(directListingCapitalRaise);
}

template<typename SpecificHandler>
using ParseFn = void(*)(std::byte const *, SpecificHandler&);

template<typename SpecificHandler>
[[gnu::noinline, gnu::cold]] static void bad_type(std::byte const * _, SpecificHandler& __) {
    throw std::runtime_error("Unknown message type");
}

consteval bool is_valid_message_type(uint8_t c) {
    for (uint8_t v : message_type_chars) {
        if (v == c) return true;
    }
    return false;
}

template<typename SpecificHandler>
inline void ignore_message(std::byte const * src, SpecificHandler& dst) {};

template<typename SpecificHandler>
consteval std::array<ParseFn<SpecificHandler>, 256> make_dispatch() {
    std::array<ParseFn<SpecificHandler>, 256> table{};

    for (int i = 0; i < table.size(); ++i)  {
        if (is_valid_message_type(i)) {
            table[i] = &ignore_message<SpecificHandler>;
        } else {
            table[i] = &bad_type<SpecificHandler>;
        }
    }

    #define X(CHAR, ENUM, TYPE, FIELD) \
        table[static_cast<uint8_t>(CHAR)] = &parse##TYPE<SpecificHandler>;

    ITCH_MESSAGE_LIST_REDUCED_SET(X)

    #undef X

    return table;
}

template<typename SpecificHandler>
consteval std::array<ParseFn<SpecificHandler>, 256> make_high_prio_dispatch() {
    std::array<ParseFn<SpecificHandler>, 256> table{};

    for (int i = 0; i < table.size(); ++i)  {
        if (is_valid_message_type(i)) {
            table[i] = &ignore_message<SpecificHandler>;
        } else {
            table[i] = &bad_type<SpecificHandler>;
        }
    }

    table[static_cast<uint8_t>('D')] = &parseOrderDelete<SpecificHandler>;
    table[static_cast<uint8_t>('A')] = &parseAddOrderNoMpid<SpecificHandler>;
    table[static_cast<uint8_t>('X')] = &parseOrderCancel<SpecificHandler>;
    table[static_cast<uint8_t>('U')] = &parseOrderReplace<SpecificHandler>;

    return table;
}


template<typename SpecificHandler>
alignas(64) inline constexpr auto dispatch = make_dispatch<SpecificHandler>();

template<typename SpecificHandler>
alignas(64) inline constexpr auto high_prio_dispatch = make_high_prio_dispatch<SpecificHandler>();

template<typename SpecificHandler>
void ItchParser::parse_specific(std::byte const * src, size_t len, SpecificHandler& handler) {
    std::byte const * end = src + len;

    while (end - src >= 3) {
        uint16_t size = load_be16(src);
        src += 2;
        if (end - src < size) {
            break;
        }

        auto raw_type = char(src[0]);
        MessageType type = static_cast<MessageType>(raw_type);
        src += 1;

        handler.handle_before();

        //high_prio_dispatch<SpecificHandler>[raw_type](src, handler);
        dispatch<SpecificHandler>[raw_type](src, handler);

        handler.handle_after();

        src += size - 1;
    }
}

}
