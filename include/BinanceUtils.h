#ifndef BINANCE_UTILS_H
#define BINANCE_UTILS_H

#include <string>
#include <map>

namespace binance {

struct OCOPrices {
    double limitPrice;
    double stopPrice;
    double stopLimitPrice;
};

// Calculate valid prices for OCO orders based on current market price
OCOPrices calculateOCOPrices(double currentPrice, bool isSell, double percentage = 0.02) {
    OCOPrices prices;
    if (isSell) {
        // For SELL OCO: Limit Price must be higher than Stop Price
        // And Stop Limit Price must be lower than Stop Price
        prices.stopPrice = currentPrice * (1 - percentage);        // 2% below market
        prices.limitPrice = currentPrice * (1 + percentage);       // 2% above market
        prices.stopLimitPrice = prices.stopPrice * 0.999;         // Just slightly below stop price
    } else {
        // For BUY OCO: Stop Price must be higher than Limit Price
        // And Stop Limit Price must be higher than Stop Price
        prices.limitPrice = currentPrice * (1 - percentage);       // 2% below market
        prices.stopPrice = currentPrice * (1 + percentage);        // 2% above market
        prices.stopLimitPrice = prices.stopPrice * 1.001;         // Just slightly above stop price
    }

    // Ensure proper price formatting
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.2f", prices.limitPrice);
    prices.limitPrice = std::stod(buffer);
    snprintf(buffer, sizeof(buffer), "%.2f", prices.stopPrice);
    prices.stopPrice = std::stod(buffer);
    snprintf(buffer, sizeof(buffer), "%.2f", prices.stopLimitPrice);
    prices.stopLimitPrice = std::stod(buffer);

    return prices;
}

// Format price according to symbol's requirements
std::string formatPrice(double price, int decimals = 2) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.*f", decimals, price);
    return std::string(buffer);
}

// Helper to create OCO order parameters
std::map<std::string, std::string> createOCOParams(const OCOPrices& prices, 
                                                  const std::string& quantity,
                                                  const std::string& timeInForce = "GTC") {
    std::map<std::string, std::string> params;
    params["stopLimitTimeInForce"] = timeInForce;
    params["stopLimitPrice"] = formatPrice(prices.stopLimitPrice);
    params["quantity"] = quantity;
    
    // Add validation
    if (prices.limitPrice <= 0 || prices.stopPrice <= 0 || prices.stopLimitPrice <= 0) {
        throw std::invalid_argument("Invalid price values for OCO order");
    }
    return params;
}

// Get price precision for a symbol (default 2 decimals)
int getPricePrecision(const std::string& symbol) {
    // Common trading pairs and their decimal places
    static const std::map<std::string, int> precisions = {
        {"BTCUSDT", 2},
        {"ETHUSDT", 2},
        {"BNBUSDT", 2},
        {"BTCBUSD", 2},
        {"ETHBUSD", 2}
    };
    
    auto it = precisions.find(symbol);
    return it != precisions.end() ? it->second : 2;
}

} // namespace binance

#endif // BINANCE_UTILS_H
