#!/bin/bash

echo "Creating build directory..."
mkdir -p build

# Common compiler flags
CXXFLAGS="-std=c++17 -Wall -Wextra -I include"

# Check if we're on macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS typically installs OpenSSL via Homebrew in /usr/local/opt/openssl
    if [ -d "/usr/local/opt/openssl" ]; then
        OPENSSL_PATH="/usr/local/opt/openssl"
    elif [ -d "/opt/homebrew/opt/openssl@3" ]; then
        OPENSSL_PATH="/opt/homebrew/opt/openssl@3"
    else
        echo "Error: OpenSSL not found. Please install it with: brew install openssl"
        exit 1
    fi
    
    CXXFLAGS="$CXXFLAGS -I$OPENSSL_PATH/include"
    LDFLAGS="-L$OPENSSL_PATH/lib -lcurl -lssl -lcrypto"
else
    # Linux and other systems
    LDFLAGS="-lcurl -lssl -lcrypto"
fi

# Compile source files to object files
echo "Compiling BinanceAPI.cpp..."
g++ $CXXFLAGS -c src/BinanceAPI.cpp -o build/BinanceAPI.o
g++ $CXXFLAGS -c src/BinanceAuth.cpp -o build/BinanceAuth.o
g++ $CXXFLAGS -c src/BinanceTypes.cpp -o build/BinanceTypes.o
g++ $CXXFLAGS -c src/HttpClient.cpp -o build/HttpClient.o

# Create archive/static library
echo "Creating static library..."
ar rcs build/libbinance_api.a build/BinanceAPI.o build/BinanceAuth.o build/BinanceTypes.o build/HttpClient.o

# Compile and link main example
echo "Building binance_example executable..."
g++ $CXXFLAGS src/main.cpp -o build/binance_example build/libbinance_api.a $LDFLAGS

# Compile and link test executables
echo "Building testnet_test executable..."
g++ $CXXFLAGS src/testnet_test.cpp -o build/testnet_test build/libbinance_api.a $LDFLAGS

echo "Building adaptive_test executable..."
g++ $CXXFLAGS src/adaptive_test.cpp -o build/adaptive_test build/libbinance_api.a $LDFLAGS

echo "Building strategy_example executable..."
g++ $CXXFLAGS src/strategy_example.cpp -o build/strategy_example build/libbinance_api.a $LDFLAGS

echo "Building utils_test executable..."
g++ $CXXFLAGS src/utils_test.cpp -o build/utils_test build/libbinance_api.a $LDFLAGS

echo "Build completed. Executables are in the build/ directory."
echo ""
echo "Available test executables:"
echo "1. Basic example:"
echo "   ./build/binance_example \"YOUR_API_KEY\" \"YOUR_API_SECRET\""
echo ""
echo "2. Comprehensive API tests:"
echo "   ./build/testnet_test \"YOUR_API_KEY\" \"YOUR_API_SECRET\""
echo ""
echo "3. Adaptive price tests:"
echo "   ./build/adaptive_test \"YOUR_API_KEY\" \"YOUR_API_SECRET\""
echo ""
echo "4. Trading strategy example:"
echo "   ./build/strategy_example \"YOUR_API_KEY\" \"YOUR_API_SECRET\""
echo ""
echo "5. Utils and price calculation tests:"
echo "   ./build/utils_test \"YOUR_API_KEY\" \"YOUR_API_SECRET\""
echo ""
echo "Note: All executables support both mainnet and testnet."
echo "When testing, always use testnet first!"
