#ifndef BINANCE_AUTH_H
#define BINANCE_AUTH_H

#include <string>
#include <map>

namespace binance {

/**
 * @class BinanceAuth
 * @brief Handles authentication and signatures for Binance API requests
 */
class BinanceAuth {
public:
    /**
     * @brief Constructor
     * @param api_key The Binance API key
     * @param api_secret The Binance API secret
     */
    BinanceAuth(const std::string& api_key, const std::string& api_secret);
    
    /**
     * @brief Get the API key
     * @return The API key
     */
    const std::string& getApiKey() const;
    
    /**
     * @brief Generate signature for a request
     * @param params Map of request parameters
     * @return HMAC SHA256 signature
     */
    std::string generateSignature(const std::map<std::string, std::string>& params) const;
    
    /**
     * @brief Add timestamp and signature to parameters
     * @param params Map of parameters to modify
     */
    void signRequest(std::map<std::string, std::string>& params) const;
    
    /**
     * @brief Create standard headers for Binance API requests
     * @return Map of HTTP headers
     */
    std::map<std::string, std::string> createHeaders() const;

private:
    std::string api_key_;
    std::string api_secret_;
};

} // namespace binance

#endif // BINANCE_AUTH_H
