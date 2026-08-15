#pragma once

#include <array>
#include <optional>
#include <string>
#include <unordered_map>

using Amount = long double;
using Time = long long;

constexpr int NUM_PERIODS = 3;

constexpr Time ONE_HOUR_TIME_PERIOD = 3600;
constexpr Time ONE_DAY_TIME_PERIOD = 86400;
constexpr Time THIRTY_DAYS_TIME_PERIOD = 2592000;

constexpr const char* USD = "USD";
constexpr const char* OUTPUT_HEADER =
    "user_id,minimum_balance,maximum_balance,average_balance,start_timestamp";

constexpr std::array<Time, NUM_PERIODS> PERIODS = {ONE_HOUR_TIME_PERIOD, ONE_DAY_TIME_PERIOD,
                                         THIRTY_DAYS_TIME_PERIOD};

struct TransactionData {
    std::string user_id;
    std::string currency;
    Time timestamp;
    Amount delta;
    bool is_valid = false;
};

struct MarketTickData {
    std::string from_currency;
    std::string to_currency;
    Time timestamp;
    Amount price;
    bool is_valid = false;
};

struct Bar {
    Time bar_start_ts;
    Amount min_bal, max_bal, avg_bal;
};

struct OpenBar {
    Time bar_start_ts;
    Amount min_bal;
    Amount max_bal;
    Amount sum_bal{};
    Amount last_bal;
    Time last_update_ts;

    OpenBar(Time bar_start_ts, Time update_ts, Amount bal)
        : bar_start_ts(bar_start_ts),
          min_bal(bal),
          max_bal(bal),
          last_bal(bal),
          last_update_ts(update_ts) {}
};

struct UserData {
    std::unordered_map<std::string, Amount> quantity;
    std::array<std::optional<OpenBar>, NUM_PERIODS> open_bar;
};
