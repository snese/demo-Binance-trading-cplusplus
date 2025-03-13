#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <map>
#include <functional>
#include <memory>

namespace binance {

/**
 * @class HttpClient
 * @brief HTTP client for making RESTful API requests
 */
class HttpClient {
public:
    /**
     * @brief Constructor
     */
    HttpClient();
    
    /**
     * @brief Destructor
     */
    ~HttpClient();
    
    /**
     * @brief Initializes the HTTP client
     * @return True if initialization succeeds, false otherwise
     */
    bool init();
    
    /**
     * @brief Perform HTTP GET request
     * @param url The URL to request
     * @param headers Map of HTTP headers
     * @return Response string
     */
    std::string get(const std::string& url, const std::map<std::string, std::string>& headers = {});
    
    /**
     * @brief Perform HTTP POST request
     * @param url The URL to request
     * @param data The POST data string
     * @param headers Map of HTTP headers
     * @return Response string
     */
    std::string post(const std::string& url, const std::string& data = "", 
                    const std::map<std::string, std::string>& headers = {});
    
    /**
     * @brief Perform HTTP DELETE request
     * @param url The URL to request
     * @param headers Map of HTTP headers
     * @return Response string
     */
    std::string del(const std::string& url, const std::map<std::string, std::string>& headers = {});

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace binance

#endif // HTTP_CLIENT_H
