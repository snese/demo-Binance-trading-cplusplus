/**
 * @file BinanceAPI.h
 * @author HC
 * @brief Main Binance API interface header - Personal Demo Project
 * @version 1.0
 * @date 2025-03-13
 * 
 * @copyright Copyright (c) 2025 HC
 */

#ifndef BINANCE_API_H
#define BINANCE_API_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>

namespace binance {

/**
 * @class BinanceAPI
 * @brief Main interface for interacting with Binance Spot API
 */
class BinanceAPI {
public:
    /**
     * @brief Constructor
     * @param api_key The Binance API key
     * @param api_secret The Binance API secret
     * @param base_url Base URL for the Binance API (default: https://api.binance.com)
     */
    BinanceAPI(const std::string& api_key, const std::string& api_secret, 
               const std::string& base_url = "https://api.binance.com");
    
    /**
     * @brief Destructor
     */
    ~BinanceAPI();

    /**
     * @brief Creates a new order
     * @param symbol Trading pair symbol (e.g., "BTCUSDT")
     * @param side "BUY" or "SELL"
     * @param type Order type (e.g., "LIMIT", "MARKET")
     * @param params Additional parameters map
     * @return JSON string containing the response
     */
    std::string createOrder(const std::string& symbol, const std::string& side, 
                           const std::string& type, const std::map<std::string, std::string>& params = {});

    /**
     * @brief Test new order creation
     * @param symbol Trading pair symbol
     * @param side "BUY" or "SELL"
     * @param type Order type
     * @param params Additional parameters map
     * @return JSON string containing the response
     */
    std::string testOrder(const std::string& symbol, const std::string& side, 
                         const std::string& type, const std::map<std::string, std::string>& params = {});

    /**
     * @brief Query order status
     * @param symbol Trading pair symbol
     * @param params Additional parameters (must include orderId or origClientOrderId)
     * @return JSON string containing the response
     */
    std::string queryOrder(const std::string& symbol, const std::map<std::string, std::string>& params);

    /**
     * @brief Cancel an active order
     * @param symbol Trading pair symbol
     * @param params Additional parameters (must include orderId or origClientOrderId)
     * @return JSON string containing the response
     */
    std::string cancelOrder(const std::string& symbol, const std::map<std::string, std::string>& params);

    /**
     * @brief Cancel all open orders on a symbol
     * @param symbol Trading pair symbol
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string cancelAllOrders(const std::string& symbol, const std::map<std::string, std::string>& params = {});

    /**
     * @brief Cancel an existing order and send a new order
     * @param symbol Trading pair symbol
     * @param side "BUY" or "SELL"
     * @param type Order type
     * @param cancelReplaceMode "STOP_ON_FAILURE" or "ALLOW_FAILURE"
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string cancelReplaceOrder(const std::string& symbol, const std::string& side, 
                                  const std::string& type, const std::string& cancelReplaceMode,
                                  const std::map<std::string, std::string>& params);

    /**
     * @brief Get current open orders
     * @param symbol Trading pair symbol (optional)
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string getOpenOrders(const std::string& symbol = "", const std::map<std::string, std::string>& params = {});

    /**
     * @brief Get all orders (active, canceled, or filled)
     * @param symbol Trading pair symbol
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string getAllOrders(const std::string& symbol, const std::map<std::string, std::string>& params = {});

    /**
     * @brief Create a new OCO (One-Cancels-the-Other) order
     * @param symbol Trading pair symbol
     * @param side "BUY" or "SELL"
     * @param quantity Order quantity
     * @param price Price for the limit order
     * @param stopPrice Stop price for the stop-loss/stop-loss-limit order
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string createOCO(const std::string& symbol, const std::string& side, const std::string& quantity,
                         const std::string& price, const std::string& stopPrice,
                         const std::map<std::string, std::string>& params = {});
                         
    /**
     * @brief Create a new order list - OCO (One-Cancels-the-Other)
     * @param symbol Trading pair symbol
     * @param side "BUY" or "SELL"
     * @param quantity Order quantity
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string createOrderListOCO(const std::string& symbol, const std::string& side, 
                                  const std::string& quantity, 
                                  const std::map<std::string, std::string>& params);

    /**
     * @brief Create a new order list - OTO (One-Triggers-the-Other)
     * @param symbol Trading pair symbol
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string createOrderListOTO(const std::string& symbol,
                                  const std::map<std::string, std::string>& params);

    /**
     * @brief Create a new order list - OTOCO (One-Triggers-One-Cancels-the-Other)
     * @param symbol Trading pair symbol
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string createOrderListOTOCO(const std::string& symbol,
                                    const std::map<std::string, std::string>& params);

    /**
     * @brief Cancel an order list
     * @param symbol Trading pair symbol
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string cancelOrderList(const std::string& symbol,
                               const std::map<std::string, std::string>& params);

    /**
     * @brief Query an order list
     * @param params Parameter map (must include orderListId or listClientOrderId)
     * @return JSON string containing the response
     */
    std::string queryOrderList(const std::map<std::string, std::string>& params);

    /**
     * @brief Query all order lists
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string getAllOrderLists(const std::map<std::string, std::string>& params = {});

    /**
     * @brief Query open order lists
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string getOpenOrderLists(const std::map<std::string, std::string>& params = {});

    /**
     * @brief Create a new order using smart order routing (SOR)
     * @param symbol Trading pair symbol
     * @param side "BUY" or "SELL"
     * @param type Order type
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string createSOROrder(const std::string& symbol, const std::string& side, 
                              const std::string& type, const std::map<std::string, std::string>& params = {});

    /**
     * @brief Test new SOR order creation
     * @param symbol Trading pair symbol
     * @param side "BUY" or "SELL"
     * @param type Order type
     * @param params Additional parameters
     * @return JSON string containing the response
     */
    std::string testSOROrder(const std::string& symbol, const std::string& side, 
                            const std::string& type, const std::map<std::string, std::string>& params = {});
    
    /**
     * @brief Get symbol price ticker
     * @param symbol Trading pair symbol
     * @param params Additional parameters
     * @return JSON string containing the response with current price
     */
    std::string getSymbolPriceTicker(const std::string& symbol, const std::map<std::string, std::string>& params = {});

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace binance

#endif // BINANCE_API_H
