#include "../include/BinanceAPI.h"
#include "../include/HttpClient.h"
#include "../include/BinanceAuth.h"
#include "../include/BinanceTypes.h"
#include <string>
#include <map>
#include <sstream>
#include <stdexcept>

namespace binance {

// Implementation class using the PIMPL idiom
class BinanceAPI::Impl {
public:
    Impl(const std::string& api_key, const std::string& api_secret, const std::string& base_url)
        : auth(api_key, api_secret), base_url(base_url) {
        
        httpClient.init();
    }

    ~Impl() = default;

    std::string createOrder(const std::string& symbol, const std::string& side, const std::string& type, 
                          const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["type"] = type;
        
        return sendSignedRequest("POST", "/api/v3/order", requestParams);
    }

    std::string testOrder(const std::string& symbol, const std::string& side, const std::string& type, 
                        const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["type"] = type;
        
        return sendSignedRequest("POST", "/api/v3/order/test", requestParams);
    }

    std::string queryOrder(const std::string& symbol, const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("GET", "/api/v3/order", requestParams);
    }

    std::string cancelOrder(const std::string& symbol, const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("DELETE", "/api/v3/order", requestParams);
    }

    std::string cancelAllOrders(const std::string& symbol, const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("DELETE", "/api/v3/openOrders", requestParams);
    }

    std::string cancelReplaceOrder(const std::string& symbol, const std::string& side, 
                                 const std::string& type, const std::string& cancelReplaceMode,
                                 const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["type"] = type;
        requestParams["cancelReplaceMode"] = cancelReplaceMode;
        
        return sendSignedRequest("POST", "/api/v3/order/cancelReplace", requestParams);
    }

    std::string getOpenOrders(const std::string& symbol = "", const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        if (!symbol.empty()) {
            requestParams["symbol"] = symbol;
        }
        
        return sendSignedRequest("GET", "/api/v3/openOrders", requestParams);
    }

    std::string getAllOrders(const std::string& symbol, const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("GET", "/api/v3/allOrders", requestParams);
    }

    std::string createOCO(const std::string& symbol, const std::string& side, const std::string& quantity,
                        const std::string& price, const std::string& stopPrice,
                        const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["quantity"] = quantity;
        requestParams["price"] = price;
        requestParams["stopPrice"] = stopPrice;
        
        return sendSignedRequest("POST", "/api/v3/order/oco", requestParams);
    }

    std::string createOrderListOCO(const std::string& symbol, const std::string& side, 
                                 const std::string& quantity, 
                                 const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["quantity"] = quantity;
        
        return sendSignedRequest("POST", "/api/v3/orderList/oco", requestParams);
    }

    std::string createOrderListOTO(const std::string& symbol,
                                 const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("POST", "/api/v3/orderList/oto", requestParams);
    }

    std::string createOrderListOTOCO(const std::string& symbol,
                                   const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("POST", "/api/v3/orderList/otoco", requestParams);
    }

    std::string cancelOrderList(const std::string& symbol,
                              const std::map<std::string, std::string>& params) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        
        return sendSignedRequest("DELETE", "/api/v3/orderList", requestParams);
    }

    std::string queryOrderList(const std::map<std::string, std::string>& params) {
        return sendSignedRequest("GET", "/api/v3/orderList", params);
    }

    std::string getAllOrderLists(const std::map<std::string, std::string>& params = {}) {
        return sendSignedRequest("GET", "/api/v3/allOrderList", params);
    }

    std::string getOpenOrderLists(const std::map<std::string, std::string>& params = {}) {
        return sendSignedRequest("GET", "/api/v3/openOrderList", params);
    }

    std::string createSOROrder(const std::string& symbol, const std::string& side, 
                             const std::string& type, const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["type"] = type;
        
        return sendSignedRequest("POST", "/api/v3/sor/order", requestParams);
    }

    std::string testSOROrder(const std::string& symbol, const std::string& side, 
                           const std::string& type, const std::map<std::string, std::string>& params = {}) {
        std::map<std::string, std::string> requestParams = params;
        requestParams["symbol"] = symbol;
        requestParams["side"] = side;
        requestParams["type"] = type;
        
        return sendSignedRequest("POST", "/api/v3/sor/order/test", requestParams);
    }

    std::string sendPublicRequest(const std::string& endpoint, [[maybe_unused]] const std::string& method,
                                 const std::map<std::string, std::string>& params) {
        std::string url = base_url + endpoint;
        std::string queryString = paramsToQueryString(params);
        
        if (!queryString.empty()) {
            url += "?" + queryString;
        }
        
        std::map<std::string, std::string> headers;
        return httpClient.get(url, headers);
    }

private:
    BinanceAuth auth;
    HttpClient httpClient;
    std::string base_url;

    std::string paramsToQueryString(const std::map<std::string, std::string>& params) {
        if (params.empty()) {
            return "";
        }
        
        std::stringstream ss;
        bool first = true;
        for (const auto& pair : params) {
            if (!first) {
                ss << "&";
            }
            ss << pair.first << "=" << pair.second;
            first = false;
        }
        return ss.str();
    }

    std::string sendSignedRequest(const std::string& method, const std::string& endpoint, 
                                std::map<std::string, std::string> params) {
        // Add timestamp and signature
        auth.signRequest(params);
        
        // Create URL with query string for GET requests or DELETE requests with params
        std::string url = base_url + endpoint;
        std::string queryString = paramsToQueryString(params);
        
        // Set headers
        std::map<std::string, std::string> headers = auth.createHeaders();
        
        std::string response;
        if (method == "GET") {
            if (!queryString.empty()) {
                url += "?" + queryString;
            }
            response = httpClient.get(url, headers);
        } else if (method == "POST") {
            response = httpClient.post(url, queryString, headers);
        } else if (method == "DELETE") {
            if (!queryString.empty()) {
                url += "?" + queryString;
            }
            response = httpClient.del(url, headers);
        } else {
            throw std::invalid_argument("Unsupported HTTP method: " + method);
        }
        
        return response;
    }
};

// BinanceAPI implementation

BinanceAPI::BinanceAPI(const std::string& api_key, const std::string& api_secret, const std::string& base_url)
    : pImpl(new Impl(api_key, api_secret, base_url)) {
}

BinanceAPI::~BinanceAPI() = default;

std::string BinanceAPI::createOrder(const std::string& symbol, const std::string& side, 
                                   const std::string& type, const std::map<std::string, std::string>& params) {
    return pImpl->createOrder(symbol, side, type, params);
}

std::string BinanceAPI::testOrder(const std::string& symbol, const std::string& side, 
                                 const std::string& type, const std::map<std::string, std::string>& params) {
    return pImpl->testOrder(symbol, side, type, params);
}

std::string BinanceAPI::queryOrder(const std::string& symbol, const std::map<std::string, std::string>& params) {
    return pImpl->queryOrder(symbol, params);
}

std::string BinanceAPI::cancelOrder(const std::string& symbol, const std::map<std::string, std::string>& params) {
    return pImpl->cancelOrder(symbol, params);
}

std::string BinanceAPI::cancelAllOrders(const std::string& symbol, const std::map<std::string, std::string>& params) {
    return pImpl->cancelAllOrders(symbol, params);
}

std::string BinanceAPI::cancelReplaceOrder(const std::string& symbol, const std::string& side, 
                                          const std::string& type, const std::string& cancelReplaceMode,
                                          const std::map<std::string, std::string>& params) {
    return pImpl->cancelReplaceOrder(symbol, side, type, cancelReplaceMode, params);
}

std::string BinanceAPI::getOpenOrders(const std::string& symbol, const std::map<std::string, std::string>& params) {
    return pImpl->getOpenOrders(symbol, params);
}

std::string BinanceAPI::getAllOrders(const std::string& symbol, const std::map<std::string, std::string>& params) {
    return pImpl->getAllOrders(symbol, params);
}

std::string BinanceAPI::createOCO(const std::string& symbol, const std::string& side, const std::string& quantity,
                                 const std::string& price, const std::string& stopPrice,
                                 const std::map<std::string, std::string>& params) {
    return pImpl->createOCO(symbol, side, quantity, price, stopPrice, params);
}

std::string BinanceAPI::createOrderListOCO(const std::string& symbol, const std::string& side, 
                                          const std::string& quantity, 
                                          const std::map<std::string, std::string>& params) {
    return pImpl->createOrderListOCO(symbol, side, quantity, params);
}

std::string BinanceAPI::createOrderListOTO(const std::string& symbol,
                                          const std::map<std::string, std::string>& params) {
    return pImpl->createOrderListOTO(symbol, params);
}

std::string BinanceAPI::createOrderListOTOCO(const std::string& symbol,
                                           const std::map<std::string, std::string>& params) {
    return pImpl->createOrderListOTOCO(symbol, params);
}

std::string BinanceAPI::cancelOrderList(const std::string& symbol,
                                       const std::map<std::string, std::string>& params) {
    return pImpl->cancelOrderList(symbol, params);
}

std::string BinanceAPI::queryOrderList(const std::map<std::string, std::string>& params) {
    return pImpl->queryOrderList(params);
}

std::string BinanceAPI::getAllOrderLists(const std::map<std::string, std::string>& params) {
    return pImpl->getAllOrderLists(params);
}

std::string BinanceAPI::getOpenOrderLists(const std::map<std::string, std::string>& params) {
    return pImpl->getOpenOrderLists(params);
}

std::string BinanceAPI::createSOROrder(const std::string& symbol, const std::string& side, 
                                      const std::string& type, const std::map<std::string, std::string>& params) {
    return pImpl->createSOROrder(symbol, side, type, params);
}

std::string BinanceAPI::testSOROrder(const std::string& symbol, const std::string& side, 
                                    const std::string& type, const std::map<std::string, std::string>& params) {
    return pImpl->testSOROrder(symbol, side, type, params);
}

std::string BinanceAPI::getSymbolPriceTicker(const std::string& symbol, const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string> queryParams = params;
    if (!symbol.empty()) {
        queryParams["symbol"] = symbol;
    }
    return pImpl->sendPublicRequest("/api/v3/ticker/price", "GET", queryParams);
}

} // namespace binance
