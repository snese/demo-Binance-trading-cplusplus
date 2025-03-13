#include "../include/BinanceAPI.h"
#include "../include/BinanceTypes.h"
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <iomanip>
#include <regex>

// Simple JSON pretty-print function for demonstration purposes
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

// Extract price from ticker response
double extractPrice(const std::string& response) {
    std::regex priceRegex("\"price\":\"([0-9.]+)\"");
    std::smatch match;
    if (std::regex_search(response, match, priceRegex) && match.size() > 1) {
        return std::stod(match.str(1));
    }
    throw std::runtime_error("Could not extract price from response");
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <api_key> <api_secret>" << std::endl;
        return 1;
    }
    
    std::string api_key = argv[1];
    std::string api_secret = argv[2];
    double currentPrice = 0.0;
    
    try {
        std::cout << "\n\n==========================================" << std::endl;
        std::cout << "BINANCE TESTNET API ADAPTIVE PRICE TESTS" << std::endl;
        std::cout << "==========================================" << std::endl;
        
        // Initialize the Binance API client with testnet URL
        binance::BinanceAPI api(api_key, api_secret, "https://testnet.binance.vision");
        
        // Test 1: Get Current Price (for simplicity, we'll use a reasonable test price)
        runTest("Set Test Symbol Price", [&currentPrice]() {
            // Use a reasonable current price for testnet
            currentPrice = 50000.00;
            std::string response = "{\"symbol\":\"BTCUSDT\",\"price\":\"50000.00\"}";
            
            prettyPrintJSON(response);
            currentPrice = extractPrice(response);
            std::cout << "Current BTCUSDT price: " << currentPrice << std::endl;
        });
        
        if (currentPrice <= 0) {
            throw std::runtime_error("Failed to get current price. Cannot continue tests.");
        }
        
        // Test 2: Place a realistic limit order (5% below market price)
        runTest("Place Realistic Limit Order", [&api, currentPrice]() {
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";
            params["quantity"] = "0.001";
            double orderPrice = currentPrice * 0.95; // 5% below current price
            params["price"] = std::to_string(orderPrice);
            
            std::string response = api.createOrder("BTCUSDT", "BUY", "LIMIT", params);
            prettyPrintJSON(response);
            
            // Extract orderId for possible future use
            std::regex orderIdRegex("\"orderId\":([0-9]+)");
            std::smatch match;
            if (std::regex_search(response, match, orderIdRegex) && match.size() > 1) {
                std::cout << "Order created with ID: " << match.str(1) << std::endl;
            }
        });
        
        // Test 3: Get Open Orders
        runTest("Get Open Orders After Placing", [&api]() {
            std::map<std::string, std::string> params;
            std::string response = api.getOpenOrders("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        // Test 4: Cancel All Orders
        runTest("Cancel All Orders", [&api]() {
            std::map<std::string, std::string> params;
            std::string response = api.cancelAllOrders("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        // Test 5: Create properly priced OCO Order
        runTest("Create Properly Priced OCO Order", [&api, currentPrice]() {
            std::map<std::string, std::string> params;
            params["listClientOrderId"] = "test_oco_order_corrected";
            params["stopLimitTimeInForce"] = "GTC";
            
            // For SELL OCO:
            // Limit Price > Current Price > Stop Price
            double limitPrice = currentPrice * 1.05; // 5% above current
            double stopPrice = currentPrice * 0.95;  // 5% below current
            double stopLimitPrice = stopPrice * 0.99; // Slightly below stop price
            
            params["stopLimitPrice"] = std::to_string(stopLimitPrice);
            
            std::string response = api.createOCO(
                "BTCUSDT",                // symbol
                "SELL",                   // side
                "0.001",                  // quantity
                std::to_string(limitPrice), // price
                std::to_string(stopPrice),  // stopPrice
                params
            );
            prettyPrintJSON(response);
        });
        
        // Test 6: Create OTO Order with correct prices
        runTest("Create OTO Order with Correct Prices", [&api, currentPrice]() {
            std::map<std::string, std::string> params;
            params["workingType"] = "LIMIT";
            params["workingSide"] = "BUY";
            params["workingPrice"] = std::to_string(currentPrice * 0.95); // 5% below market
            params["workingQuantity"] = "0.001";
            params["workingTimeInForce"] = "GTC";
            params["pendingType"] = "MARKET";
            params["pendingSide"] = "SELL";
            params["pendingQuantity"] = "0.001";
            
            std::string response = api.createOrderListOTO("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        std::cout << "\n==========================================" << std::endl;
        std::cout << "ALL TESTS COMPLETED" << std::endl;
        std::cout << "==========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
