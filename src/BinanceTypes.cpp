#include "../include/BinanceTypes.h"
#include <sstream>
#include <stdexcept>
#include <iomanip>

namespace binance {

// Enum to string conversion functions
std::string toString(OrderSide side) {
    switch (side) {
        case OrderSide::BUY: return "BUY";
        case OrderSide::SELL: return "SELL";
        default: throw std::invalid_argument("Invalid OrderSide value");
    }
}

std::string toString(OrderType type) {
    switch (type) {
        case OrderType::LIMIT: return "LIMIT";
        case OrderType::MARKET: return "MARKET";
        case OrderType::STOP_LOSS: return "STOP_LOSS";
        case OrderType::STOP_LOSS_LIMIT: return "STOP_LOSS_LIMIT";
        case OrderType::TAKE_PROFIT: return "TAKE_PROFIT";
        case OrderType::TAKE_PROFIT_LIMIT: return "TAKE_PROFIT_LIMIT";
        case OrderType::LIMIT_MAKER: return "LIMIT_MAKER";
        default: throw std::invalid_argument("Invalid OrderType value");
    }
}

std::string toString(TimeInForce tif) {
    switch (tif) {
        case TimeInForce::GTC: return "GTC";
        case TimeInForce::IOC: return "IOC";
        case TimeInForce::FOK: return "FOK";
        default: throw std::invalid_argument("Invalid TimeInForce value");
    }
}

std::string toString(OrderResponseType respType) {
    switch (respType) {
        case OrderResponseType::ACK: return "ACK";
        case OrderResponseType::RESULT: return "RESULT";
        case OrderResponseType::FULL: return "FULL";
        default: throw std::invalid_argument("Invalid OrderResponseType value");
    }
}

std::string toString(SelfTradePreventionMode mode) {
    switch (mode) {
        case SelfTradePreventionMode::NONE: return "NONE";
        case SelfTradePreventionMode::EXPIRE_TAKER: return "EXPIRE_TAKER";
        case SelfTradePreventionMode::EXPIRE_MAKER: return "EXPIRE_MAKER";
        case SelfTradePreventionMode::EXPIRE_BOTH: return "EXPIRE_BOTH";
        default: throw std::invalid_argument("Invalid SelfTradePreventionMode value");
    }
}

std::string toString(CancelReplaceMode mode) {
    switch (mode) {
        case CancelReplaceMode::STOP_ON_FAILURE: return "STOP_ON_FAILURE";
        case CancelReplaceMode::ALLOW_FAILURE: return "ALLOW_FAILURE";
        default: throw std::invalid_argument("Invalid CancelReplaceMode value");
    }
}

std::string toString(OrderStatus status) {
    switch (status) {
        case OrderStatus::NEW: return "NEW";
        case OrderStatus::PARTIALLY_FILLED: return "PARTIALLY_FILLED";
        case OrderStatus::FILLED: return "FILLED";
        case OrderStatus::CANCELED: return "CANCELED";
        case OrderStatus::PENDING_CANCEL: return "PENDING_CANCEL";
        case OrderStatus::REJECTED: return "REJECTED";
        case OrderStatus::EXPIRED: return "EXPIRED";
        default: throw std::invalid_argument("Invalid OrderStatus value");
    }
}

std::string toString(ContingencyType type) {
    switch (type) {
        case ContingencyType::OCO: return "OCO";
        case ContingencyType::OTO: return "OTO";
        case ContingencyType::OTOCO: return "OTOCO";
        default: throw std::invalid_argument("Invalid ContingencyType value");
    }
}

// String to enum conversion functions
OrderSide orderSideFromString(const std::string& str) {
    if (str == "BUY") return OrderSide::BUY;
    if (str == "SELL") return OrderSide::SELL;
    throw std::invalid_argument("Invalid order side string: " + str);
}

OrderType orderTypeFromString(const std::string& str) {
    if (str == "LIMIT") return OrderType::LIMIT;
    if (str == "MARKET") return OrderType::MARKET;
    if (str == "STOP_LOSS") return OrderType::STOP_LOSS;
    if (str == "STOP_LOSS_LIMIT") return OrderType::STOP_LOSS_LIMIT;
    if (str == "TAKE_PROFIT") return OrderType::TAKE_PROFIT;
    if (str == "TAKE_PROFIT_LIMIT") return OrderType::TAKE_PROFIT_LIMIT;
    if (str == "LIMIT_MAKER") return OrderType::LIMIT_MAKER;
    throw std::invalid_argument("Invalid order type string: " + str);
}

TimeInForce timeInForceFromString(const std::string& str) {
    if (str == "GTC") return TimeInForce::GTC;
    if (str == "IOC") return TimeInForce::IOC;
    if (str == "FOK") return TimeInForce::FOK;
    throw std::invalid_argument("Invalid time in force string: " + str);
}

OrderResponseType orderResponseTypeFromString(const std::string& str) {
    if (str == "ACK") return OrderResponseType::ACK;
    if (str == "RESULT") return OrderResponseType::RESULT;
    if (str == "FULL") return OrderResponseType::FULL;
    throw std::invalid_argument("Invalid order response type string: " + str);
}

SelfTradePreventionMode selfTradePreventionModeFromString(const std::string& str) {
    if (str == "NONE") return SelfTradePreventionMode::NONE;
    if (str == "EXPIRE_TAKER") return SelfTradePreventionMode::EXPIRE_TAKER;
    if (str == "EXPIRE_MAKER") return SelfTradePreventionMode::EXPIRE_MAKER;
    if (str == "EXPIRE_BOTH") return SelfTradePreventionMode::EXPIRE_BOTH;
    throw std::invalid_argument("Invalid self trade prevention mode string: " + str);
}

CancelReplaceMode cancelReplaceModeFromString(const std::string& str) {
    if (str == "STOP_ON_FAILURE") return CancelReplaceMode::STOP_ON_FAILURE;
    if (str == "ALLOW_FAILURE") return CancelReplaceMode::ALLOW_FAILURE;
    throw std::invalid_argument("Invalid cancel replace mode string: " + str);
}

OrderStatus orderStatusFromString(const std::string& str) {
    if (str == "NEW") return OrderStatus::NEW;
    if (str == "PARTIALLY_FILLED") return OrderStatus::PARTIALLY_FILLED;
    if (str == "FILLED") return OrderStatus::FILLED;
    if (str == "CANCELED") return OrderStatus::CANCELED;
    if (str == "PENDING_CANCEL") return OrderStatus::PENDING_CANCEL;
    if (str == "REJECTED") return OrderStatus::REJECTED;
    if (str == "EXPIRED") return OrderStatus::EXPIRED;
    throw std::invalid_argument("Invalid order status string: " + str);
}

ContingencyType contingencyTypeFromString(const std::string& str) {
    if (str == "OCO") return ContingencyType::OCO;
    if (str == "OTO") return ContingencyType::OTO;
    if (str == "OTOCO") return ContingencyType::OTOCO;
    throw std::invalid_argument("Invalid contingency type string: " + str);
}

// Utility function to convert parameters to query string
std::string paramsToQueryString(const std::map<std::string, std::string>& params) {
    if (params.empty()) {
        return "";
    }
    
    std::stringstream ss;
    bool first = true;
    for (const auto& param : params) {
        if (!first) {
            ss << "&";
        }
        ss << param.first << "=" << param.second;
        first = false;
    }
    return ss.str();
}

// Convert struct types to parameter maps
std::map<std::string, std::string> toParamMap(const OrderParams& params) {
    std::map<std::string, std::string> result;
    
    result["symbol"] = params.symbol;
    result["side"] = toString(params.side);
    result["type"] = toString(params.type);
    
    if (params.timeInForce) {
        result["timeInForce"] = toString(*params.timeInForce);
    }
    
    if (params.quantity) {
        result["quantity"] = *params.quantity;
    }
    
    if (params.quoteOrderQty) {
        result["quoteOrderQty"] = *params.quoteOrderQty;
    }
    
    if (params.price) {
        result["price"] = *params.price;
    }
    
    if (params.newClientOrderId) {
        result["newClientOrderId"] = *params.newClientOrderId;
    }
    
    if (params.strategyId) {
        result["strategyId"] = *params.strategyId;
    }
    
    if (params.strategyType) {
        result["strategyType"] = std::to_string(*params.strategyType);
    }
    
    if (params.stopPrice) {
        result["stopPrice"] = *params.stopPrice;
    }
    
    if (params.trailingDelta) {
        result["trailingDelta"] = std::to_string(*params.trailingDelta);
    }
    
    if (params.icebergQty) {
        result["icebergQty"] = *params.icebergQty;
    }
    
    if (params.newOrderRespType) {
        result["newOrderRespType"] = toString(*params.newOrderRespType);
    }
    
    if (params.selfTradePreventionMode) {
        result["selfTradePreventionMode"] = toString(*params.selfTradePreventionMode);
    }
    
    return result;
}

std::map<std::string, std::string> toParamMap(const OCOOrderParams& params) {
    std::map<std::string, std::string> result;
    
    result["symbol"] = params.symbol;
    result["side"] = toString(params.side);
    result["quantity"] = params.quantity;
    result["price"] = params.price;
    result["stopPrice"] = params.stopPrice;
    
    if (params.listClientOrderId) {
        result["listClientOrderId"] = *params.listClientOrderId;
    }
    
    if (params.limitClientOrderId) {
        result["limitClientOrderId"] = *params.limitClientOrderId;
    }
    
    if (params.limitStrategyId) {
        result["limitStrategyId"] = *params.limitStrategyId;
    }
    
    if (params.limitStrategyType) {
        result["limitStrategyType"] = std::to_string(*params.limitStrategyType);
    }
    
    if (params.limitIcebergQty) {
        result["limitIcebergQty"] = *params.limitIcebergQty;
    }
    
    if (params.trailingDelta) {
        result["trailingDelta"] = std::to_string(*params.trailingDelta);
    }
    
    if (params.stopClientOrderId) {
        result["stopClientOrderId"] = *params.stopClientOrderId;
    }
    
    if (params.stopStrategyId) {
        result["stopStrategyId"] = *params.stopStrategyId;
    }
    
    if (params.stopStrategyType) {
        result["stopStrategyType"] = std::to_string(*params.stopStrategyType);
    }
    
    if (params.stopLimitPrice) {
        result["stopLimitPrice"] = *params.stopLimitPrice;
    }
    
    if (params.stopIcebergQty) {
        result["stopIcebergQty"] = *params.stopIcebergQty;
    }
    
    if (params.stopLimitTimeInForce) {
        result["stopLimitTimeInForce"] = toString(*params.stopLimitTimeInForce);
    }
    
    if (params.newOrderRespType) {
        result["newOrderRespType"] = toString(*params.newOrderRespType);
    }
    
    if (params.selfTradePreventionMode) {
        result["selfTradePreventionMode"] = toString(*params.selfTradePreventionMode);
    }
    
    return result;
}

std::map<std::string, std::string> toParamMap(const OTOOrderParams& params) {
    std::map<std::string, std::string> result;
    
    result["symbol"] = params.symbol;
    
    if (params.listClientOrderId) {
        result["listClientOrderId"] = *params.listClientOrderId;
    }
    
    if (params.newOrderRespType) {
        result["newOrderRespType"] = toString(*params.newOrderRespType);
    }
    
    if (params.selfTradePreventionMode) {
        result["selfTradePreventionMode"] = toString(*params.selfTradePreventionMode);
    }
    
    // Working order parameters
    result["workingType"] = toString(params.workingType);
    result["workingSide"] = toString(params.workingSide);
    
    if (params.workingClientOrderId) {
        result["workingClientOrderId"] = *params.workingClientOrderId;
    }
    
    result["workingPrice"] = params.workingPrice;
    result["workingQuantity"] = params.workingQuantity;
    
    if (params.workingIcebergQty) {
        result["workingIcebergQty"] = *params.workingIcebergQty;
    }
    
    if (params.workingTimeInForce) {
        result["workingTimeInForce"] = toString(*params.workingTimeInForce);
    }
    
    if (params.workingStrategyId) {
        result["workingStrategyId"] = *params.workingStrategyId;
    }
    
    if (params.workingStrategyType) {
        result["workingStrategyType"] = std::to_string(*params.workingStrategyType);
    }
    
    // Pending order parameters
    result["pendingType"] = toString(params.pendingType);
    result["pendingSide"] = toString(params.pendingSide);
    
    if (params.pendingClientOrderId) {
        result["pendingClientOrderId"] = *params.pendingClientOrderId;
    }
    
    if (params.pendingPrice) {
        result["pendingPrice"] = *params.pendingPrice;
    }
    
    if (params.pendingStopPrice) {
        result["pendingStopPrice"] = *params.pendingStopPrice;
    }
    
    if (params.pendingTrailingDelta) {
        result["pendingTrailingDelta"] = *params.pendingTrailingDelta;
    }
    
    result["pendingQuantity"] = params.pendingQuantity;
    
    if (params.pendingIcebergQty) {
        result["pendingIcebergQty"] = *params.pendingIcebergQty;
    }
    
    if (params.pendingTimeInForce) {
        result["pendingTimeInForce"] = toString(*params.pendingTimeInForce);
    }
    
    if (params.pendingStrategyId) {
        result["pendingStrategyId"] = *params.pendingStrategyId;
    }
    
    if (params.pendingStrategyType) {
        result["pendingStrategyType"] = std::to_string(*params.pendingStrategyType);
    }
    
    return result;
}

std::map<std::string, std::string> toParamMap(const OTOCOOrderParams& params) {
    std::map<std::string, std::string> result;
    
    result["symbol"] = params.symbol;
    
    if (params.listClientOrderId) {
        result["listClientOrderId"] = *params.listClientOrderId;
    }
    
    if (params.newOrderRespType) {
        result["newOrderRespType"] = toString(*params.newOrderRespType);
    }
    
    if (params.selfTradePreventionMode) {
        result["selfTradePreventionMode"] = toString(*params.selfTradePreventionMode);
    }
    
    // Working order parameters
    result["workingType"] = toString(params.workingType);
    result["workingSide"] = toString(params.workingSide);
    
    if (params.workingClientOrderId) {
        result["workingClientOrderId"] = *params.workingClientOrderId;
    }
    
    result["workingPrice"] = params.workingPrice;
    result["workingQuantity"] = params.workingQuantity;
    
    if (params.workingIcebergQty) {
        result["workingIcebergQty"] = *params.workingIcebergQty;
    }
    
    if (params.workingTimeInForce) {
        result["workingTimeInForce"] = toString(*params.workingTimeInForce);
    }
    
    if (params.workingStrategyId) {
        result["workingStrategyId"] = *params.workingStrategyId;
    }
    
    if (params.workingStrategyType) {
        result["workingStrategyType"] = std::to_string(*params.workingStrategyType);
    }
    
    // Pending parameters
    result["pendingSide"] = toString(params.pendingSide);
    result["pendingQuantity"] = params.pendingQuantity;
    
    // Pending above order parameters
    result["pendingAboveType"] = toString(params.pendingAboveType);
    
    if (params.pendingAboveClientOrderId) {
        result["pendingAboveClientOrderId"] = *params.pendingAboveClientOrderId;
    }
    
    if (params.pendingAbovePrice) {
        result["pendingAbovePrice"] = *params.pendingAbovePrice;
    }
    
    if (params.pendingAboveStopPrice) {
        result["pendingAboveStopPrice"] = *params.pendingAboveStopPrice;
    }
    
    if (params.pendingAboveTrailingDelta) {
        result["pendingAboveTrailingDelta"] = *params.pendingAboveTrailingDelta;
    }
    
    if (params.pendingAboveIcebergQty) {
        result["pendingAboveIcebergQty"] = *params.pendingAboveIcebergQty;
    }
    
    if (params.pendingAboveTimeInForce) {
        result["pendingAboveTimeInForce"] = toString(*params.pendingAboveTimeInForce);
    }
    
    if (params.pendingAboveStrategyId) {
        result["pendingAboveStrategyId"] = *params.pendingAboveStrategyId;
    }
    
    if (params.pendingAboveStrategyType) {
        result["pendingAboveStrategyType"] = std::to_string(*params.pendingAboveStrategyType);
    }
    
    // Pending below order parameters
    result["pendingBelowType"] = toString(params.pendingBelowType);
    
    if (params.pendingBelowClientOrderId) {
        result["pendingBelowClientOrderId"] = *params.pendingBelowClientOrderId;
    }
    
    if (params.pendingBelowPrice) {
        result["pendingBelowPrice"] = *params.pendingBelowPrice;
    }
    
    if (params.pendingBelowStopPrice) {
        result["pendingBelowStopPrice"] = *params.pendingBelowStopPrice;
    }
    
    if (params.pendingBelowTrailingDelta) {
        result["pendingBelowTrailingDelta"] = *params.pendingBelowTrailingDelta;
    }
    
    if (params.pendingBelowIcebergQty) {
        result["pendingBelowIcebergQty"] = *params.pendingBelowIcebergQty;
    }
    
    if (params.pendingBelowTimeInForce) {
        result["pendingBelowTimeInForce"] = toString(*params.pendingBelowTimeInForce);
    }
    
    if (params.pendingBelowStrategyId) {
        result["pendingBelowStrategyId"] = *params.pendingBelowStrategyId;
    }
    
    if (params.pendingBelowStrategyType) {
        result["pendingBelowStrategyType"] = std::to_string(*params.pendingBelowStrategyType);
    }
    
    return result;
}

} // namespace binance
