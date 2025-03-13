#include "../include/BinanceAPI.h"
#include "../include/BinanceTypes.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <iomanip>

// Simple moving average calculator
class SMA {
private:
    std::vector<double> prices;
    size_t period;
    
public:
    SMA(size_t p) : period(p) {}
    
    void addPrice(double price) {
        prices.push_back(price);
        if (prices.size() > period) {
            prices.erase(prices.begin());
        }
    }
    
    double getValue() const {
        if (prices.size() < period) return 0;
        double sum = 0;
        for (double price : prices) {
            sum += price;
        }
        return sum / prices.size();
    }
    
    bool isReady() const {
        return prices.size() >= period;
    }
};

// Trading strategy class
class SimpleCrossoverStrategy {
private:
    binance::BinanceAPI& api;
    std::string symbol;
    double quantity;
    
    SMA fastSMA;
    SMA slowSMA;
    
    bool inPosition;
    std::string currentOrderId;
    
    double extractPrice(const std::string& response) {
        size_t pos = response.find("\"price\":\"");
        if (pos != std::string::npos) {
            pos += 9;  // length of "\"price\":\""
            size_t endPos = response.find("\"", pos);
            if (endPos != std::string::npos) {
                return std::stod(response.substr(pos, endPos - pos));
            }
        }
        throw std::runtime_error("Could not extract price from response");
    }
    
public:
    SimpleCrossoverStrategy(binance::BinanceAPI& api, const std::string& symbol, 
                           double quantity, size_t fastPeriod = 10, size_t slowPeriod = 20)
        : api(api), symbol(symbol), quantity(quantity),
          fastSMA(fastPeriod), slowSMA(slowPeriod), inPosition(false) {}
    
    void update() {
        try {
            // Get current price
            std::map<std::string, std::string> params;
            params["symbol"] = symbol;
            
            // In a real implementation, you would use the proper endpoint
            // Here we're using a test response
            std::string response = api.getOpenOrders(symbol, params);
            double currentPrice = 50000.00; // Using a test price
            
            // Update moving averages
            fastSMA.addPrice(currentPrice);
            slowSMA.addPrice(currentPrice);
            
            if (!fastSMA.isReady() || !slowSMA.isReady()) {
                std::cout << "Collecting data... " 
                         << "Fast SMA: " << fastSMA.getValue() 
                         << " Slow SMA: " << slowSMA.getValue() << std::endl;
                return;
            }
            
            double fastValue = fastSMA.getValue();
            double slowValue = slowSMA.getValue();
            
            std::cout << std::fixed << std::setprecision(2)
                     << "Price: " << currentPrice 
                     << " Fast SMA: " << fastValue 
                     << " Slow SMA: " << slowValue << std::endl;
            
            // Trading logic
            if (fastValue > slowValue && !inPosition) {
                // Buy signal
                std::map<std::string, std::string> orderParams;
                orderParams["quantity"] = std::to_string(quantity);
                
                std::string response = api.createOrder(
                    symbol,
                    "BUY",
                    "MARKET",
                    orderParams
                );
                
                std::cout << "BUY signal! Order response: " << response << std::endl;
                inPosition = true;
                
            } else if (fastValue < slowValue && inPosition) {
                // Sell signal
                std::map<std::string, std::string> orderParams;
                orderParams["quantity"] = std::to_string(quantity);
                
                std::string response = api.createOrder(
                    symbol,
                    "SELL",
                    "MARKET",
                    orderParams
                );
                
                std::cout << "SELL signal! Order response: " << response << std::endl;
                inPosition = false;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error in strategy update: " << e.what() << std::endl;
        }
    }
};

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <api_key> <api_secret>" << std::endl;
        return 1;
    }
    
    try {
        // Initialize API with testnet
        binance::BinanceAPI api(
            argv[1],
            argv[2],
            "https://testnet.binance.vision"
        );
        
        // Create strategy instance
        SimpleCrossoverStrategy strategy(
            api,
            "BTCUSDT",    // trading pair
            0.001         // trading quantity
        );
        
        std::cout << "Starting Simple SMA Crossover Strategy..." << std::endl;
        std::cout << "Press Ctrl+C to exit" << std::endl;
        
        // Main loop
        while (true) {
            strategy.update();
            
            // Sleep for 5 seconds between updates
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
