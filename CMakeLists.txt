cmake_minimum_required(VERSION 3.10)
project(BinanceTradingAPI VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
include(CTest)

# Find required packages
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories(${CURL_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

# Add include directories
include_directories(include)

# Source files
set(SOURCES
    src/BinanceAPI.cpp
    src/BinanceAuth.cpp
    src/BinanceTypes.cpp
    src/HttpClient.cpp
)

# Create library
add_library(binance_api STATIC ${SOURCES})
target_link_libraries(binance_api 
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
)

# Add warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(binance_api PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(binance_api PRIVATE /W4)
endif()

# Function to add an executable with common settings
function(add_binance_executable name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} binance_api)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic)
    elseif(MSVC)
        target_compile_options(${name} PRIVATE /W4)
    endif()
endfunction()

# Create executables
add_binance_executable(binance_example src/main.cpp)
add_binance_executable(testnet_test src/testnet_test.cpp)
add_binance_executable(adaptive_test src/adaptive_test.cpp)
add_binance_executable(strategy_example src/strategy_example.cpp)

# Install targets
install(TARGETS binance_api
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    ${CMAKE_SOURCE_DIR}/include/BinanceAPI.h
    ${CMAKE_SOURCE_DIR}/include/BinanceAuth.h
    ${CMAKE_SOURCE_DIR}/include/BinanceTypes.h
    ${CMAKE_SOURCE_DIR}/include/HttpClient.h
    DESTINATION include/binance
)

# Add examples to install
install(TARGETS
    binance_example
    testnet_test
    adaptive_test
    strategy_example
    RUNTIME DESTINATION bin
)

# Generate and install pkg-config file
configure_file(
    ${CMAKE_SOURCE_DIR}/binance-trading-api.pc.in
    ${CMAKE_BINARY_DIR}/binance-trading-api.pc
    @ONLY
)
install(FILES
    ${CMAKE_BINARY_DIR}/binance-trading-api.pc
    DESTINATION lib/pkgconfig
)
