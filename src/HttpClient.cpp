#include "../include/HttpClient.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace binance {

// Callback function to write HTTP response data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch(std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
}

// Implementation for the HttpClient class using the PIMPL idiom
class HttpClient::Impl {
public:
    Impl() : curl(nullptr) {}

    ~Impl() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }

    bool init() {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        return (curl != nullptr);
    }

    std::string get(const std::string& url, const std::map<std::string, std::string>& headers) {
        return request("GET", url, "", headers);
    }

    std::string post(const std::string& url, const std::string& data, 
                    const std::map<std::string, std::string>& headers) {
        return request("POST", url, data, headers);
    }

    std::string del(const std::string& url, const std::map<std::string, std::string>& headers) {
        return request("DELETE", url, "", headers);
    }

private:
    CURL* curl;
    
    std::string request(const std::string& method, const std::string& url, 
                       const std::string& data, 
                       const std::map<std::string, std::string>& headers) {
        if (!curl) {
            throw std::runtime_error("CURL not initialized");
        }
        
        curl_easy_reset(curl);
        
        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set response callback
        std::string responseString;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

        // Set method and data if needed
        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            if (!data.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
            }
        } else if (method == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }

        // Set headers
        struct curl_slist* headersList = NULL;
        for (const auto& header : headers) {
            std::string headerLine = header.first + ": " + header.second;
            headersList = curl_slist_append(headersList, headerLine.c_str());
        }

        // Always set application/json content type if not specified
        if (headers.find("Content-Type") == headers.end()) {
            headersList = curl_slist_append(headersList, "Content-Type: application/json");
        }

        if (headersList) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersList);
        }

        // Set timeouts
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);  // 30 seconds timeout
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);  // 10 seconds connect timeout

        // Set SSL options
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        // Set verbose mode for debugging (comment out in production)
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Clean up headers
        if (headersList) {
            curl_slist_free_all(headersList);
        }

        // Check for errors
        if (res != CURLE_OK) {
            std::stringstream ss;
            ss << "CURL error: " << curl_easy_strerror(res);
            throw std::runtime_error(ss.str());
        }

        // Get response code
        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        // Check for HTTP error
        if (httpCode >= 400) {
            std::stringstream ss;
            ss << "HTTP error " << httpCode << ": " << responseString;
            throw std::runtime_error(ss.str());
        }

        return responseString;
    }
};

// HttpClient implementation
HttpClient::HttpClient() : pImpl(new Impl()) {}

HttpClient::~HttpClient() = default;

bool HttpClient::init() {
    return pImpl->init();
}

std::string HttpClient::get(const std::string& url, const std::map<std::string, std::string>& headers) {
    return pImpl->get(url, headers);
}

std::string HttpClient::post(const std::string& url, const std::string& data, 
                           const std::map<std::string, std::string>& headers) {
    return pImpl->post(url, data, headers);
}

std::string HttpClient::del(const std::string& url, const std::map<std::string, std::string>& headers) {
    return pImpl->del(url, headers);
}

} // namespace binance
