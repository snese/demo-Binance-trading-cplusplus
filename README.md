# Binance Trading API C++ Library

*A personal demo project by HC*

A modern C++ library for interacting with the Binance Spot Trading API, featuring comprehensive order types and real-time market data integration. This project showcases advanced C++ programming concepts and best practices in financial API integration.

## Features

- Complete Binance Spot API coverage
- Real-time market price data
- Advanced order types support (OCO, OTO, OTOCO)
- Smart order routing (SOR)
- Automatic price calculations
- Extensive test coverage

## Order Types

- Market Orders
- Limit Orders
- Stop Loss Orders
- One-Cancels-the-Other (OCO) Orders
- One-Triggers-the-Other (OTO) Orders
- One-Triggers-OCO (OTOCO) Orders

## Prerequisites

- C++17 or higher
- OpenSSL
- CMake 3.10 or higher

## Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/binance-trading-api.git
cd binance-trading-api

# Build the library
./build.sh
```

## Quick Start

```cpp
#include "BinanceAPI.h"
#include "BinanceUtils.h"
#include <iostream>

int main() {
    // Initialize API with your credentials
    binance::BinanceAPI api(
        "your-api-key",
        "your-api-secret",
        "https://testnet.binance.vision"  // Use testnet for testing
    );

    try {
        // Get current market price
        std::string priceData = api.getSymbolPriceTicker("BTCUSDT", {});
        std::cout << "Current market data: " << priceData << std::endl;

        // Place a limit order
        std::map<std::string, std::string> params;
        params["timeInForce"] = "GTC";
        params["quantity"] = "0.001";
        params["price"] = "50000";

        std::string response = api.createOrder(
            "BTCUSDT",
            "BUY",
            "LIMIT",
            params
        );

        std::cout << "Order placed: " << response << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

## Creating OCO Orders

One-Cancels-the-Other (OCO) orders follow specific price relationship rules:

### SELL OCO
- Limit Price > Current Market Price > Stop Price
- Stop Limit Price < Stop Price

### BUY OCO
- Stop Price > Current Market Price > Limit Price
- Stop Limit Price > Stop Price

Example:

```cpp
// Get current market price
std::string priceData = api.getSymbolPriceTicker("BTCUSDT", {});

// Calculate OCO prices using utility functions
binance::OCOPrices prices = binance::calculateOCOPrices(currentPrice, true); // true for SELL

// Create OCO order
std::map<std::string, std::string> params;
params["stopLimitTimeInForce"] = "GTC";
params["stopLimitPrice"] = binance::formatPrice(prices.stopLimitPrice);
params["quantity"] = "0.001";

std::string response = api.createOCO(
    "BTCUSDT",
    "SELL",
    "0.001",
    binance::formatPrice(prices.limitPrice),
    binance::formatPrice(prices.stopPrice),
    params
);
```

## Testing

The library includes comprehensive test suites:

```bash
# Build and run tests
./build.sh
cd build

# Run specific test executables
./utils_test "YOUR_API_KEY" "YOUR_API_SECRET"      # Test utilities and price calculations
./testnet_test "YOUR_API_KEY" "YOUR_API_SECRET"    # Test API functionality
./adaptive_test "YOUR_API_KEY" "YOUR_API_SECRET"   # Test adaptive price features
```

## Error Handling

The library uses standard C++ exceptions for error handling. All API calls should be wrapped in try-catch blocks:

```cpp
try {
    // API calls here
} catch (const std::invalid_argument& e) {
    // Handle invalid parameters
} catch (const std::runtime_error& e) {
    // Handle API errors
} catch (const std::exception& e) {
    // Handle other errors
}
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.
