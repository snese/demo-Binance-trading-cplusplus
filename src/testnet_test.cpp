#include "../include/BinanceAPI.h"
#include "../include/BinanceTypes.h"
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <iomanip>

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

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <api_key> <api_secret>" << std::endl;
        return 1;
    }
    
    std::string api_key = argv[1];
    std::string api_secret = argv[2];
    
    try {
        std::cout << "\n\n=======================================" << std::endl;
        std::cout << "BINANCE TESTNET API COMPREHENSIVE TESTS" << std::endl;
        std::cout << "=======================================" << std::endl;
        
        // Initialize the Binance API client with testnet URL
        binance::BinanceAPI api(api_key, api_secret, "https://testnet.binance.vision");
        
        // Test 1: Check Server Time (Public Endpoint)
        runTest("Server Time (Public Endpoint)", [&api]() {
            std::map<std::string, std::string> emptyParams;
            std::string response = api.getOpenOrders("BTCUSDT", emptyParams);
            prettyPrintJSON(response);
        });
        
        // Test 2: Get Account Information (Authenticated Endpoint)
        runTest("Account Information", [&api]() {
            std::map<std::string, std::string> params;
            std::string response = api.getAllOrders("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        // Test 3: Test Order (Limit Order)
        runTest("Test Limit Order", [&api]() {
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";
            params["quantity"] = "0.001";
            params["price"] = "50000";
            
            std::string response = api.testOrder("BTCUSDT", "BUY", "LIMIT", params);
            prettyPrintJSON(response);
        });
        
        // Test 4: Test Order (Market Order)
        runTest("Test Market Order", [&api]() {
            std::map<std::string, std::string> params;
            params["quantity"] = "0.001";
            
            std::string response = api.testOrder("BTCUSDT", "BUY", "MARKET", params);
            prettyPrintJSON(response);
        });
        
        // Test 5: Place a real limit order (low price, won't execute)
        runTest("Place Limit Order", [&api]() {
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";
            params["quantity"] = "0.001";
            params["price"] = "10000";  // Very low price, won't execute
            
            std::string response = api.createOrder("BTCUSDT", "BUY", "LIMIT", params);
            prettyPrintJSON(response);
        });
        
        // Test 6: Get Open Orders
        runTest("Get Open Orders", [&api]() {
            std::map<std::string, std::string> params;
            std::string response = api.getOpenOrders("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        // Test 7: Cancel All Orders
        runTest("Cancel All Orders", [&api]() {
            std::map<std::string, std::string> params;
            std::string response = api.cancelAllOrders("BTCUSDT", params);
            prettyPrintJSON(response);
        });
        
        // Test 8: Test SOR Order
        runTest("Test SOR Order", [&api]() {
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";
            params["quantity"] = "0.001";
            params["price"] = "50000";
            params["computeCommissionRates"] = "true";
            
            std::string response = api.testSOROrder("BTCUSDT", "BUY", "LIMIT", params);
            prettyPrintJSON(response);
        });
        
        // Test 9: Create OCO Order
        runTest("Create OCO Order", [&api]() {
            std::map<std::string, std::string> params;
            params["listClientOrderId"] = "test_oco_order";
            params["stopLimitTimeInForce"] = "GTC";
            params["stopLimitPrice"] = "51000";
            
            std::string response = api.createOCO(
                "BTCUSDT", "SELL", "0.001", "60000", "50000", params
            );
            prettyPrintJSON(response);
        });
        
        // Test 10: Advanced Order Types - OTO
        runTest("Create OTO Order", [&api]() {
            std::map<std::string, std::string> params;
            params["workingType"] = "LIMIT";
            params["workingSide"] = "BUY";
            params["workingPrice"] = "20000";
            params["workingQuantity"] = "0.001";
            params["workingTimeInForce"] = "GTC";
            params["pendingType"] = "MARKET";
            params["pendingSide"] = "SELL";
            params["pendingQuantity"] = "0.001";
            
            std::string response = api.createOrderListOTO("BTCUSDT", params);
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
