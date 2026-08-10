#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

using Price =  long double;
using Time = long long;

constexpr Time ONE_HOUR_TIME_PERIOD = 3600;
constexpr Time ONE_DAY_TIME_PERIOD = 86400;
constexpr Time THIRTY_DAYS_TIME_PERIOD = 2592000;

constexpr const char* USD = "USD";

std::vector<std::pair<std::string, Time>> time_periods = {
    {"1h", ONE_HOUR_TIME_PERIOD}, {"1d", ONE_DAY_TIME_PERIOD}, {"30d", THIRTY_DAYS_TIME_PERIOD}};

struct TransactionData {
    std::string user_id;
    std::string currency;
    Time timestamp;
    Price delta;
    bool is_valid = false;
};

struct MarketTickData {
    std::string from_currency;
    std::string to_currency;
    Time timestamp;
    Price price;
    bool is_valid = false;
};

struct Bar {
    Time bar_start_ts;
    Price min_bal;
    Price max_bal;
    Price avg_bal;
};

struct UserData {
    Price current_balance;
    std::unordered_map<std::string, long long> ammount;
    std::array<Bar, 3> open_bar;
};
