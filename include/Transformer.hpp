#pragma once

#include <sstream>
#include <string>

#include "Data.hpp"

TransactionData transaction_data_parser(const std::string& data_line) {
    if (data_line.empty()) {
        return {};
    }
    std::stringstream data_line_stream(data_line);

    TransactionData res;
    std::string user_id;
    std::string currency;
    std::string timestamp_str;
    std::string delta_str;

    std::getline(data_line_stream, user_id, ',');
    std::getline(data_line_stream, currency, ',');
    std::getline(data_line_stream, timestamp_str, ',');
    std::getline(data_line_stream, delta_str, ',');

    res.user_id = user_id;
    res.currency = currency;
    res.timestamp = std::stoll(timestamp_str);
    res.delta = std::stold(delta_str);
    res.is_valid = true;
    return res;
}

MarketTickData market_tick_parser(const std::string& data_line) {
    if (data_line.empty()) {
        return {};
    }
    std::stringstream data_line_stream(data_line);

    MarketTickData res;
    std::string symbol;
    std::string price_str;
    std::string timestamp_str;

    std::getline(data_line_stream, symbol, ',');
    std::getline(data_line_stream, price_str, ',');
    std::getline(data_line_stream, timestamp_str, ',');

    res.from_currency = symbol.substr(0, 3);
    res.to_currency = symbol.substr(3);
    res.price = std::stold(price_str);
    res.timestamp = std::stoll(timestamp_str);
    res.is_valid = true;
    return res;
}

std::string get_output_header() {
    return OUTPUT_HEADER;
}

std::string closed_bar_parser(const std::string& user_id, const Bar& closed_bar) {
    return user_id + ',' + std::to_string(closed_bar.min_bal) + ',' +
           std::to_string(closed_bar.max_bal) + ',' + std::to_string(closed_bar.avg_bal) + ',' +
           std::to_string(closed_bar.bar_start_ts);
}