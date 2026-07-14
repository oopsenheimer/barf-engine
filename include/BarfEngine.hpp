#pragma once

#include <map>
#include <string>

#include "CSVParser.hpp"
#include "File.hpp"

struct UserTracker {
    std::map<std::string, long double> token_balances;
    long double last_known_usd_balance;
};

void get_all_users(std::map<std::string, UserTracker>& all_users, File& user_data_file) {
    std::string user_data_row;
    while (user_data_file.read_line(user_data_row)) {
        auto user_data = UserData(user_data_row);
        if (user_data.is_valid && !all_users.contains(user_data.user_id)) {
            all_users[user_data.user_id] = UserTracker();
        }
    }
    user_data_file.back_to_start();
}

void process_user_data(std::map<std::string, UserTracker>& all_users, const UserData& user_data) {
    if (!user_data.is_valid) {
        return;
    }
    all_users[user_data.user_id].token_balances[user_data.currency] += user_data.delta;
}

void process_all_users(std::map<std::string, long double>& exchange_rate,
                       std::map<std::string, UserTracker>& all_users) {
    for (auto& [user_id, user_tracker] : all_users) {
        long double new_usd_balance{0.0};
        for (auto& [token, token_balance] : user_tracker.token_balances) {
            if (exchange_rate.contains(token)) {
                new_usd_balance += exchange_rate[token] * token_balance;
            }
        }
        user_tracker.last_known_usd_balance = new_usd_balance;
    }
}

void process_market_data(std::map<std::string, long double>& exchange_rate,
                         const MarketData& market_data) {
    auto currency = market_data.symbol.substr(0, 3);
    auto conversion_currency = market_data.symbol.substr(3);
    exchange_rate[currency] = market_data.price * exchange_rate[conversion_currency];
}