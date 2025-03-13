#include "../include/BinanceAuth.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstring>
#include <array>

namespace {
    // SHA-256 Constants
    constexpr std::array<uint32_t, 64> K = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    // Helper functions for SHA-256
    constexpr uint32_t rotr(uint32_t x, uint32_t n) {
        return (x >> n) | (x << (32 - n));
    }

    constexpr uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (~x & z);
    }

    constexpr uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    constexpr uint32_t sigma0(uint32_t x) {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }

    constexpr uint32_t sigma1(uint32_t x) {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }

    constexpr uint32_t gamma0(uint32_t x) {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }

    constexpr uint32_t gamma1(uint32_t x) {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }

    // Simple SHA-256 implementation
    std::array<uint8_t, 32> sha256(const void* data, size_t len) {
        uint32_t h0 = 0x6a09e667;
        uint32_t h1 = 0xbb67ae85;
        uint32_t h2 = 0x3c6ef372;
        uint32_t h3 = 0xa54ff53a;
        uint32_t h4 = 0x510e527f;
        uint32_t h5 = 0x9b05688c;
        uint32_t h6 = 0x1f83d9ab;
        uint32_t h7 = 0x5be0cd19;

        const uint8_t* msg = static_cast<const uint8_t*>(data);
        
        // Pre-processing: padding the message
        size_t bits_len = len * 8;
        size_t new_len = ((len + 8) / 64 + 1) * 64;  // Align to 64 bytes (512 bits)
        uint8_t* padded = new uint8_t[new_len];
        
        std::memcpy(padded, msg, len);
        padded[len] = 0x80;  // Append 1 bit followed by zeros
        for (size_t i = len + 1; i < new_len - 8; ++i) {
            padded[i] = 0;
        }
        
        // Append the original length in bits as a 64-bit big-endian integer
        for (int i = 0; i < 8; ++i) {
            padded[new_len - 8 + i] = (bits_len >> (56 - i * 8)) & 0xFF;
        }

        // Process the message in 512-bit chunks
        for (size_t chunk = 0; chunk < new_len; chunk += 64) {
            uint32_t w[64];
            
            // Prepare the message schedule
            for (int i = 0; i < 16; ++i) {
                w[i] = static_cast<uint32_t>(padded[chunk + i * 4]) << 24 |
                       static_cast<uint32_t>(padded[chunk + i * 4 + 1]) << 16 |
                       static_cast<uint32_t>(padded[chunk + i * 4 + 2]) << 8 |
                       static_cast<uint32_t>(padded[chunk + i * 4 + 3]);
            }
            
            for (int i = 16; i < 64; ++i) {
                w[i] = gamma1(w[i-2]) + w[i-7] + gamma0(w[i-15]) + w[i-16];
            }
            
            // Initialize working variables
            uint32_t a = h0;
            uint32_t b = h1;
            uint32_t c = h2;
            uint32_t d = h3;
            uint32_t e = h4;
            uint32_t f = h5;
            uint32_t g = h6;
            uint32_t h = h7;
            
            // Compression function main loop
            for (int i = 0; i < 64; ++i) {
                uint32_t temp1 = h + sigma1(e) + ch(e, f, g) + K[i] + w[i];
                uint32_t temp2 = sigma0(a) + maj(a, b, c);
                
                h = g;
                g = f;
                f = e;
                e = d + temp1;
                d = c;
                c = b;
                b = a;
                a = temp1 + temp2;
            }
            
            // Update hash values
            h0 += a;
            h1 += b;
            h2 += c;
            h3 += d;
            h4 += e;
            h5 += f;
            h6 += g;
            h7 += h;
        }
        
        delete[] padded;
        
        // Produce the final hash value
        std::array<uint8_t, 32> hash;
        int idx = 0;
        
        for (uint32_t h : {h0, h1, h2, h3, h4, h5, h6, h7}) {
            hash[idx++] = (h >> 24) & 0xFF;
            hash[idx++] = (h >> 16) & 0xFF;
            hash[idx++] = (h >> 8) & 0xFF;
            hash[idx++] = h & 0xFF;
        }
        
        return hash;
    }

    // HMAC-SHA256 implementation
    std::array<uint8_t, 32> hmacSha256(const void* key, size_t key_len, const void* data, size_t data_len) {
        constexpr size_t block_size = 64;  // SHA-256 block size is 64 bytes
        
        // If key is longer than block size, hash it
        uint8_t key_block[block_size];
        if (key_len > block_size) {
            auto hashed_key = sha256(key, key_len);
            std::memcpy(key_block, hashed_key.data(), 32);
            std::memset(key_block + 32, 0, block_size - 32);
        } else {
            std::memcpy(key_block, key, key_len);
            std::memset(key_block + key_len, 0, block_size - key_len);
        }
        
        // XOR key with inner and outer pads
        uint8_t inner_key[block_size];
        uint8_t outer_key[block_size];
        
        for (size_t i = 0; i < block_size; ++i) {
            inner_key[i] = key_block[i] ^ 0x36;
            outer_key[i] = key_block[i] ^ 0x5C;
        }
        
        // Inner hash: H(inner_key || data)
        uint8_t* inner_data = new uint8_t[block_size + data_len];
        std::memcpy(inner_data, inner_key, block_size);
        std::memcpy(inner_data + block_size, data, data_len);
        auto inner_hash = sha256(inner_data, block_size + data_len);
        delete[] inner_data;
        
        // Outer hash: H(outer_key || inner_hash)
        uint8_t outer_data[block_size + 32];
        std::memcpy(outer_data, outer_key, block_size);
        std::memcpy(outer_data + block_size, inner_hash.data(), 32);
        auto outer_hash = sha256(outer_data, block_size + 32);
        
        return outer_hash;
    }
    
    // Convert byte array to hex string
    std::string bytesToHex(const std::array<uint8_t, 32>& bytes) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (auto byte : bytes) {
            ss << std::setw(2) << static_cast<int>(byte);
        }
        return ss.str();
    }
}

namespace binance {

BinanceAuth::BinanceAuth(const std::string& api_key, const std::string& api_secret)
    : api_key_(api_key), api_secret_(api_secret) {
}

const std::string& BinanceAuth::getApiKey() const {
    return api_key_;
}

std::string BinanceAuth::generateSignature(const std::map<std::string, std::string>& params) const {
    // Build query string from parameters
    std::string query_string;
    for (const auto& param : params) {
        if (!query_string.empty()) {
            query_string += "&";
        }
        query_string += param.first + "=" + param.second;
    }
    
    // Generate HMAC SHA256 signature
    auto digest = hmacSha256(api_secret_.data(), api_secret_.size(),
                           query_string.data(), query_string.size());
    
    // Convert to hex string
    return bytesToHex(digest);
}

void BinanceAuth::signRequest(std::map<std::string, std::string>& params) const {
    // Add timestamp if not already present
    if (params.find("timestamp") == params.end()) {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
        params["timestamp"] = std::to_string(timestamp);
    }
    
    // Generate signature and add to params
    std::string signature = generateSignature(params);
    params["signature"] = signature;
}

std::map<std::string, std::string> BinanceAuth::createHeaders() const {
    std::map<std::string, std::string> headers;
    headers["X-MBX-APIKEY"] = api_key_;
    return headers;
}

} // namespace binance
