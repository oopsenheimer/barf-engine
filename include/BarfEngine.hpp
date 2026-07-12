#pragma once

#include "CSVParser.hpp"
#include "File.hpp"

#include <set>
#include <string>

void get_all_users(std::set<std::string>& all_users, File& user_data_file) {
    std::string user_data_row;
    while (user_data_file.read_line(user_data_row)) {
        auto user_data = UserData(user_data_row);
        if (user_data.is_valid) {
            all_users.insert(user_data.user_id);
        }
    }
    user_data_file.back_to_start();
}

void process_user_data(const UserData& user_data) {}

void process_market_data(const MarketData& market_data) {}