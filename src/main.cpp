#include "../include/BinanceAPI.h"
#include "../include/BinanceTypes.h"
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <chrono>
#include <thread>

// Simple JSON pretty-print function for demonstration purposes
void prettyPrintJSON(const std::string& json) {
    int indent = 0;
    bool inQuotes = false;
    
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
    // Check for API key and secret as command-line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <api_key> <api_secret>" << std::endl;
        return 1;
    }
    
    std::string api_key = argv[1];
    std::string api_secret = argv[2];
    
    try {
        // Initialize the Binance API client with testnet URL
        binance::BinanceAPI api(api_key, api_secret, "https://testnet.binance.vision");
        
        std::cout << "Binance Trading API Testnet Example" << std::endl;
        std::cout << "=================================" << std::endl << std::endl;
        
        // 1. Test connectivity and get server time
        std::cout << "Testing connectivity..." << std::endl;
        try {
            // We'll test a simple query to see if our authentication works
            std::map<std::string, std::string> params;
            std::string response = api.getOpenOrders("BTCUSDT", params);
            std::cout << "API connection successful!" << std::endl;
            std::cout << "Open orders for BTCUSDT:" << std::endl;
            prettyPrintJSON(response);
        } catch (const std::exception& e) {
            std::cerr << "Error testing connectivity: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
        
        // 2. Test order creation (testOrder endpoint)
        std::cout << "Testing order creation (test mode)..." << std::endl;
        try {
            // Create a test limit order
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";  // Good Till Canceled
            params["quantity"] = "0.001";   // Order quantity
            params["price"] = "50000";      // Order price
            
            std::string response = api.testOrder("BTCUSDT", "BUY", "LIMIT", params);
            std::cout << "Order test successful!" << std::endl;
            prettyPrintJSON(response);
        } catch (const std::exception& e) {
            std::cerr << "Error testing order: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
        
        // 3. Create and test a market order
        std::cout << "Testing market order creation (test mode)..." << std::endl;
        try {
            std::map<std::string, std::string> params;
            params["quantity"] = "0.001";  // Order quantity
            
            std::string response = api.testOrder("BTCUSDT", "BUY", "MARKET", params);
            std::cout << "Market order test successful!" << std::endl;
            prettyPrintJSON(response);
        } catch (const std::exception& e) {
            std::cerr << "Error testing market order: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;

        // 4. Test SOR order creation
        std::cout << "Testing SOR order creation (test mode)..." << std::endl;
        try {
            std::map<std::string, std::string> params;
            params["timeInForce"] = "GTC";
            params["quantity"] = "0.001";
            params["price"] = "50000";
            params["computeCommissionRates"] = "true";
            
            std::string response = api.testSOROrder("BTCUSDT", "BUY", "LIMIT", params);
            std::cout << "SOR order test successful!" << std::endl;
            prettyPrintJSON(response);
        } catch (const std::exception& e) {
            std::cerr << "Error testing SOR order: " << e.what() << std::endl;
        }
        
        std::cout << std::endl;

        // 5. Test OCO order creation
        std::cout << "Testing OCO order creation (test mode)..." << std::endl;
        try {
            // Note: This uses a real endpoint, but we'll catch and handle any errors
            std::map<std::string, std::string> params;
            params["listClientOrderId"] = "test_oco_order";
            params["stopLimitTimeInForce"] = "GTC";
            params["stopLimitPrice"] = "51000";
            
            std::string response = api.createOCO(
                "BTCUSDT", "SELL", "0.001", "60000", "50000", params
            );
            std::cout << "OCO order creation response:" << std::endl;
            prettyPrintJSON(response);
        } catch (const std::exception& e) {
            std::cerr << "Error creating OCO order (expected in test mode): " << e.what() << std::endl;
        }
        
        std::cout << std::endl;
        std::cout << "Demonstration complete!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
