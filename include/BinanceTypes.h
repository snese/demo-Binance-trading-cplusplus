#ifndef BINANCE_TYPES_H
#define BINANCE_TYPES_H

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <variant>

namespace binance {

/**
 * @enum OrderSide
 * @brief Specifies whether an order is buying or selling
 */
enum class OrderSide {
    BUY,
    SELL
};

/**
 * @enum OrderType
 * @brief Types of orders that can be placed
 */
enum class OrderType {
    LIMIT,
    MARKET,
    STOP_LOSS,
    STOP_LOSS_LIMIT,
    TAKE_PROFIT,
    TAKE_PROFIT_LIMIT,
    LIMIT_MAKER
};

/**
 * @enum TimeInForce
 * @brief How long an order will remain active before it is executed or expires
 */
enum class TimeInForce {
    GTC,  // Good Till Canceled
    IOC,  // Immediate or Cancel
    FOK   // Fill or Kill
};

/**
 * @enum OrderResponseType
 * @brief Specifies the format of the order response
 */
enum class OrderResponseType {
    ACK,     // Only returns the orderId
    RESULT,  // Returns order information without fills
    FULL     // Returns full order information with fills
};

/**
 * @enum SelfTradePreventionMode
 * @brief Specifies how to prevent self-trading
 */
enum class SelfTradePreventionMode {
    NONE,
    EXPIRE_TAKER,
    EXPIRE_MAKER,
    EXPIRE_BOTH
};

/**
 * @enum CancelReplaceMode
 * @brief Specifies behavior for cancel-replace operation
 */
enum class CancelReplaceMode {
    STOP_ON_FAILURE,
    ALLOW_FAILURE
};

/**
 * @enum OrderStatus
 * @brief Status of an order
 */
enum class OrderStatus {
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELED,
    PENDING_CANCEL,
    REJECTED,
    EXPIRED
};

/**
 * @enum ContingencyType
 * @brief Type of order list contingency
 */
enum class ContingencyType {
    OCO,  // One Cancels Other
    OTO,  // One Triggers Other
    OTOCO // One Triggers OCO
};

/**
 * @enum ListStatusType
 * @brief Status type of an order list
 */
enum class ListStatusType {
    EXEC_STARTED,
    ALL_DONE,
    REJECT
};

/**
 * @enum ListOrderStatus
 * @brief Order status of an order list
 */
enum class ListOrderStatus {
    EXECUTING,
    ALL_DONE,
    REJECT
};

/**
 * @struct OrderParams
 * @brief Parameters for creating an order
 */
struct OrderParams {
    std::string symbol;
    OrderSide side;
    OrderType type;
    std::optional<TimeInForce> timeInForce;
    std::optional<std::string> quantity;
    std::optional<std::string> quoteOrderQty;
    std::optional<std::string> price;
    std::optional<std::string> newClientOrderId;
    std::optional<std::string> strategyId;
    std::optional<int> strategyType;
    std::optional<std::string> stopPrice;
    std::optional<long> trailingDelta;
    std::optional<std::string> icebergQty;
    std::optional<OrderResponseType> newOrderRespType;
    std::optional<SelfTradePreventionMode> selfTradePreventionMode;
};

/**
 * @struct OCOOrderParams
 * @brief Parameters for creating an OCO order
 */
struct OCOOrderParams {
    std::string symbol;
    OrderSide side;
    std::string quantity;
    std::string price;
    std::string stopPrice;
    std::optional<std::string> listClientOrderId;
    std::optional<std::string> limitClientOrderId;
    std::optional<std::string> limitStrategyId;
    std::optional<int> limitStrategyType;
    std::optional<std::string> limitIcebergQty;
    std::optional<long> trailingDelta;
    std::optional<std::string> stopClientOrderId;
    std::optional<std::string> stopStrategyId;
    std::optional<int> stopStrategyType;
    std::optional<std::string> stopLimitPrice;
    std::optional<std::string> stopIcebergQty;
    std::optional<TimeInForce> stopLimitTimeInForce;
    std::optional<OrderResponseType> newOrderRespType;
    std::optional<SelfTradePreventionMode> selfTradePreventionMode;
};

/**
 * @struct OTOOrderParams
 * @brief Parameters for creating an OTO order
 */
struct OTOOrderParams {
    std::string symbol;
    std::optional<std::string> listClientOrderId;
    std::optional<OrderResponseType> newOrderRespType;
    std::optional<SelfTradePreventionMode> selfTradePreventionMode;

    // Working order parameters
    OrderType workingType;
    OrderSide workingSide;
    std::optional<std::string> workingClientOrderId;
    std::string workingPrice;
    std::string workingQuantity;
    std::optional<std::string> workingIcebergQty;
    std::optional<TimeInForce> workingTimeInForce;
    std::optional<std::string> workingStrategyId;
    std::optional<int> workingStrategyType;

    // Pending order parameters
    OrderType pendingType;
    OrderSide pendingSide;
    std::optional<std::string> pendingClientOrderId;
    std::optional<std::string> pendingPrice;
    std::optional<std::string> pendingStopPrice;
    std::optional<std::string> pendingTrailingDelta;
    std::string pendingQuantity;
    std::optional<std::string> pendingIcebergQty;
    std::optional<TimeInForce> pendingTimeInForce;
    std::optional<std::string> pendingStrategyId;
    std::optional<int> pendingStrategyType;
};

/**
 * @struct OTOCOOrderParams
 * @brief Parameters for creating an OTOCO order
 */
struct OTOCOOrderParams {
    std::string symbol;
    std::optional<std::string> listClientOrderId;
    std::optional<OrderResponseType> newOrderRespType;
    std::optional<SelfTradePreventionMode> selfTradePreventionMode;

    // Working order parameters
    OrderType workingType;
    OrderSide workingSide;
    std::optional<std::string> workingClientOrderId;
    std::string workingPrice;
    std::string workingQuantity;
    std::optional<std::string> workingIcebergQty;
    std::optional<TimeInForce> workingTimeInForce;
    std::optional<std::string> workingStrategyId;
    std::optional<int> workingStrategyType;

    // Pending parameters
    OrderSide pendingSide;
    std::string pendingQuantity;

    // Pending above order parameters
    OrderType pendingAboveType;
    std::optional<std::string> pendingAboveClientOrderId;
    std::optional<std::string> pendingAbovePrice;
    std::optional<std::string> pendingAboveStopPrice;
    std::optional<std::string> pendingAboveTrailingDelta;
    std::optional<std::string> pendingAboveIcebergQty;
    std::optional<TimeInForce> pendingAboveTimeInForce;
    std::optional<std::string> pendingAboveStrategyId;
    std::optional<int> pendingAboveStrategyType;

    // Pending below order parameters
    OrderType pendingBelowType;
    std::optional<std::string> pendingBelowClientOrderId;
    std::optional<std::string> pendingBelowPrice;
    std::optional<std::string> pendingBelowStopPrice;
    std::optional<std::string> pendingBelowTrailingDelta;
    std::optional<std::string> pendingBelowIcebergQty;
    std::optional<TimeInForce> pendingBelowTimeInForce;
    std::optional<std::string> pendingBelowStrategyId;
    std::optional<int> pendingBelowStrategyType;
};

/**
 * @struct OrderFill
 * @brief Information about fills in an order
 */
struct OrderFill {
    std::string price;
    std::string qty;
    std::string commission;
    std::string commissionAsset;
    std::string tradeId;
};

/**
 * @struct OrderInfo
 * @brief Information about an order
 */
struct OrderInfo {
    std::string symbol;
    long orderId;
    long orderListId;
    std::string clientOrderId;
    long transactTime;
    std::string price;
    std::string origQty;
    std::string executedQty;
    std::string origQuoteOrderQty;
    std::string cummulativeQuoteQty;
    OrderStatus status;
    TimeInForce timeInForce;
    OrderType type;
    OrderSide side;
    std::optional<std::string> stopPrice;
    std::optional<std::string> icebergQty;
    long time;
    long updateTime;
    bool isWorking;
    long workingTime;
    SelfTradePreventionMode selfTradePreventionMode;
    std::optional<bool> usedSor;
    std::optional<std::string> workingFloor;
    std::optional<long> preventedMatchId;
    std::optional<std::string> preventedQuantity;
    std::optional<long> strategyId;
    std::optional<int> strategyType;
    std::optional<long> trailingDelta;
    std::optional<long> trailingTime;
    std::vector<OrderFill> fills;
};

/**
 * @struct OrderItem
 * @brief Brief information about an order in an order list
 */
struct OrderItem {
    std::string symbol;
    long orderId;
    std::string clientOrderId;
};

/**
 * @struct OrderListInfo
 * @brief Information about an order list
 */
struct OrderListInfo {
    long orderListId;
    ContingencyType contingencyType;
    ListStatusType listStatusType;
    ListOrderStatus listOrderStatus;
    std::string listClientOrderId;
    long transactionTime;
    std::string symbol;
    std::vector<OrderItem> orders;
    std::vector<OrderInfo> orderReports;
};

/**
 * @struct CancelReplaceResult
 * @brief Result of a cancel-replace operation
 */
struct CancelReplaceResult {
    bool cancelResult;
    bool newOrderResult;
    std::variant<OrderInfo, std::string> cancelResponse; // Either OrderInfo or error string
    std::variant<OrderInfo, std::string> newOrderResponse; // Either OrderInfo or error string
};

/**
 * @struct CommissionRates
 * @brief Commission rates for orders
 */
struct CommissionRates {
    std::string maker;
    std::string taker;
};

/**
 * @struct Discount
 * @brief Discount information for BNB fees
 */
struct Discount {
    bool enabledForAccount;
    bool enabledForSymbol;
    std::string discountAsset;
    std::string discount;
};

/**
 * @struct TestOrderResult
 * @brief Result of a test order with commission rates
 */
struct TestOrderResult {
    std::optional<CommissionRates> standardCommissionForOrder;
    std::optional<CommissionRates> taxCommissionForOrder;
    std::optional<Discount> discount;
};

// Helper functions to convert enums to/from strings
std::string toString(OrderSide side);
std::string toString(OrderType type);
std::string toString(TimeInForce tif);
std::string toString(OrderResponseType respType);
std::string toString(SelfTradePreventionMode mode);
std::string toString(CancelReplaceMode mode);
std::string toString(OrderStatus status);
std::string toString(ContingencyType type);

OrderSide orderSideFromString(const std::string& str);
OrderType orderTypeFromString(const std::string& str);
TimeInForce timeInForceFromString(const std::string& str);
OrderResponseType orderResponseTypeFromString(const std::string& str);
SelfTradePreventionMode selfTradePreventionModeFromString(const std::string& str);
CancelReplaceMode cancelReplaceModeFromString(const std::string& str);
OrderStatus orderStatusFromString(const std::string& str);
ContingencyType contingencyTypeFromString(const std::string& str);

// Utility function to convert parameters to query string
std::string paramsToQueryString(const std::map<std::string, std::string>& params);

// Convert struct types to parameter maps
std::map<std::string, std::string> toParamMap(const OrderParams& params);
std::map<std::string, std::string> toParamMap(const OCOOrderParams& params);
std::map<std::string, std::string> toParamMap(const OTOOrderParams& params);
std::map<std::string, std::string> toParamMap(const OTOCOOrderParams& params);

} // namespace binance

#endif // BINANCE_TYPES_H
