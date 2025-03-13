#include "../include/BinanceAPI.h"
#include "../include/BinanceUtils.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <stdexcept>

void printTestResult(const std::string& testName, bool success) {
    std::cout << std::left << std::setw(50) << testName << " : " 
              << (success ? "PASSED" : "FAILED") << std::endl;
}

void runTest(const std::string& testName, std::function<void()> testFunction) {
    std::cout << "\n=== Running test: " << testName << " ===" << std::endl;
    try {
        testFunction();
        printTestResult(testName, true);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        printTestResult(testName, false);
    }
}

void prettyPrintJSON(const std::string& json) {
    int indent = 0;
    bool inQuotes = false;
    
    std::cout << "JSON Response: ";
    if (json.empty()) {
        std::cout << "<empty>" << std::endl;
        return;
    }
    
    std::cout << std::endl;
    
    for (char c : json) {
        if (c == '"' && (indent == 0 || json[indent-1] != '\\')) {
            inQuotes = !inQuotes;
            std::cout << c;
        } else if (!inQuotes && (c == '{' || c == '[')) {
            std::cout << c << std::endl;
            indent += 2;
            for (int i = 0; i < indent; ++i) std::cout << ' ';
        } else if (!inQuotes && (c == '}' || c == ']')) {
            std::cout << std::endl;
            indent -= 2;
            for (int i = 0; i < indent; ++i) std::cout << ' ';
            std::cout << c;
        } else if (!inQuotes && c == ',') {
            std::cout << c << std::endl;
            for (int i = 0; i < indent; ++i) std::cout << ' ';
        } else if (!inQuotes && c == ':') {
            std::cout << c << ' ';
        } else {
            std::cout << c;
        }
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <api_key> <api_secret>" << std::endl;
        return 1;
    }
    
    try {
        std::cout << "\n\n=======================================" << std::endl;
        std::cout << "BINANCE UTILS AND PRICE TESTING" << std::endl;
        std::cout << "=======================================" << std::endl;
        
        // Initialize API with testnet
        binance::BinanceAPI api(
            argv[1],
            argv[2],
            "https://testnet.binance.vision"
        );

        // Get current price from ticker
        std::string tickerResponse = api.getSymbolPriceTicker("BTCUSDT", {});
        std::cout << "Current market data: " << tickerResponse << std::endl;
        
        // Parse price from response - simple JSON parsing
        size_t pricePos = tickerResponse.find("price");
        if (pricePos == std::string::npos) {
            throw std::runtime_error("Could not find price in ticker response");
        }
        size_t startQuote = tickerResponse.find('"', pricePos + 7);
        size_t endQuote = tickerResponse.find('"', startQuote + 1);
        std::string priceStr = tickerResponse.substr(startQuote + 1, endQuote - startQuote - 1);
        double testPrice = std::stod(priceStr);
        
        std::cout << "Using current market price: " << testPrice << std::endl;
        
        // Test 1: Calculate OCO Prices for SELL
        runTest("Calculate SELL OCO Prices", [testPrice]() {
            binance::OCOPrices prices = binance::calculateOCOPrices(testPrice, true);
            std::cout << "SELL OCO Prices:" << std::endl;
            std::cout << "  Limit Price: " << prices.limitPrice << std::endl;
            std::cout << "  Stop Price: " << prices.stopPrice << std::endl;
            std::cout << "  Stop Limit Price: " << prices.stopLimitPrice << std::endl;
            
            // Verify price relationships
            if (!(prices.limitPrice > testPrice && testPrice > prices.stopPrice)) {
                throw std::runtime_error("Invalid SELL OCO price relationships");
            }
        });
        
        // Test 2: Calculate OCO Prices for BUY
        runTest("Calculate BUY OCO Prices", [testPrice]() {
            binance::OCOPrices prices = binance::calculateOCOPrices(testPrice, false);
            std::cout << "BUY OCO Prices:" << std::endl;
            std::cout << "  Limit Price: " << prices.limitPrice << std::endl;
            std::cout << "  Stop Price: " << prices.stopPrice << std::endl;
            std::cout << "  Stop Limit Price: " << prices.stopLimitPrice << std::endl;
            
            // Verify price relationships
            if (!(prices.stopPrice > testPrice && testPrice > prices.limitPrice)) {
                throw std::runtime_error("Invalid BUY OCO price relationships");
            }
        });
        
        // Test 3: Place OCO SELL Order with calculated prices
        runTest("Place OCO SELL Order", [&api, testPrice]() {
            try {
                // For SELL OCO on BTCUSDT:
                // 1. Limit Price must be above market price
                // 2. Stop Price must be below market price
                // 3. Stop Limit Price must be slightly below Stop Price
                binance::OCOPrices prices = binance::calculateOCOPrices(testPrice, true, 0.03); // Using 3% for wider spread
                
                std::map<std::string, std::string> params;
                params["stopLimitTimeInForce"] = "GTC";
                params["stopLimitPrice"] = binance::formatPrice(prices.stopLimitPrice, 2);
                params["quantity"] = "0.001";
                
                std::cout << "\nAttempting OCO SELL order with:" << std::endl
                         << "Symbol: BTCUSDT" << std::endl
                         << "Current Price: " << binance::formatPrice(testPrice) << std::endl
                         << "Limit Price: " << binance::formatPrice(prices.limitPrice) << std::endl
                         << "Stop Price: " << binance::formatPrice(prices.stopPrice) << std::endl
                         << "Stop Limit Price: " << binance::formatPrice(prices.stopLimitPrice) << std::endl
                         << "Quantity: 0.001" << std::endl;
                
                // Verify price relationships
                if (prices.limitPrice <= testPrice || testPrice <= prices.stopPrice || 
                    prices.stopPrice <= prices.stopLimitPrice) {
                    throw std::invalid_argument("Invalid price relationships for SELL OCO");
                }
                
                std::string response = api.createOCO(
                    "BTCUSDT",
                    "SELL",
                    "0.001",
                    binance::formatPrice(prices.limitPrice),
                    binance::formatPrice(prices.stopPrice),
                    params
                );
                
                prettyPrintJSON(response);
                
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid parameters: " << e.what() << std::endl;
                throw;
            } catch (const std::exception& e) {
                std::cerr << "Error placing OCO order: " << e.what() << std::endl;
                throw;
            }
        });
        
        // Test 4: Place Limit Order with price check
        runTest("Place Valid Limit Order", [&api, testPrice]() {
            double orderPrice = testPrice * 0.95; // 5% below market
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";
            params["quantity"] = "0.001";
            params["price"] = binance::formatPrice(orderPrice);
            
            std::string response = api.createOrder(
                "BTCUSDT",
                "BUY",
                "LIMIT",
                params
            );
            
            prettyPrintJSON(response);
        });
        
        // Test 5: Clean up orders
        runTest("Cancel All Orders", [&api]() {
            std::map<std::string, std::string> params;
            std::string response = api.cancelAllOrders("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        std::cout << "\n=======================================" << std::endl;
        std::cout << "ALL TESTS COMPLETED" << std::endl;
        std::cout << "=======================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
