#pragma once

#include <ostream>
#include <sstream>

struct UserData {
    std::string user_id;
    std::string currency;
    long long timestamp;
    long double delta;
    bool is_valid = false;

    UserData() = default;

    UserData(const std::string& user_data_row) {
        if (user_data_row.empty()) {
            return;
        }
        std::stringstream data_stream(user_data_row);
        std::string timestamp_str;
        std::string delta_str;
        std::getline(data_stream, user_id, ',');
        std::getline(data_stream, currency, ',');
        std::getline(data_stream, timestamp_str, ',');
        std::getline(data_stream, delta_str, ',');
        timestamp = std::stoll(timestamp_str);
        delta = std::stold(delta_str);
        is_valid = true;
    }
};

struct MarketData {
    std::string from_currency;
    std::string to_currency;
    long long timestamp;
    long double price;
    bool is_valid = false;

    MarketData() = default;

    MarketData(const std::string& market_data_row) {
        if (market_data_row.empty()) {
            return;
        }
        std::stringstream data_stream(market_data_row);
        std::string symbol;
        std::string timestamp_str;
        std::string price_str;
        std::getline(data_stream, symbol, ',');
        std::getline(data_stream, timestamp_str, ',');
        std::getline(data_stream, price_str, ',');
        from_currency = symbol.substr(0, 3);
        to_currency = symbol.substr(3);
        timestamp = std::stoll(timestamp_str);
        price = std::stold(price_str);
        is_valid = true;
    }
};