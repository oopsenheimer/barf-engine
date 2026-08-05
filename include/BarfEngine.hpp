#pragma once

#include <map>
#include <set>
#include <string>
#include <unordered_map>

#include "CSVParser.hpp"
#include "File.hpp"

// tracker per timestamp
struct UserTracker {
    std::map<std::string, long double> token_balances;
    long double max_bal = 0;
    long double min_bal = 0;
    long double sum_bal = 0;
    long double last_bal = 0;
    long long last_ts = 0;
};

class Barf {
   private:
    std::set<std::string> all_users;
    std::unordered_map<std::string, long double> exchange_rate;

    std::unordered_map<std::string,
                       std::map<long long, std::unordered_map<std::string, UserTracker>>>
        bars;

    std::unordered_map<std::string, long long> time_periods;

   public:
    Barf() {
        time_periods = {
            {"1h", 3600},
            {"1d", 86400},
            {"30d", 2592000},
        };
    }
    void setup(File& _user_data_file) {
        exchange_rate["USD"] = 1;
        get_all_users(_user_data_file);
    }
    void process_market_data(const MarketData& market_data) {
        if (market_data.to_currency == "USD") {
            exchange_rate[market_data.from_currency] = market_data.price;
        }
        process_valid_users(market_data.timestamp);
    }
    void process_user_data(const UserData& user_data) {
        for (auto& [period, length] : time_periods) {
            auto timestamp = get_start_timestamp(user_data.timestamp, length);
            if (bars[period][timestamp].contains(user_data.user_id)) {
                update_user_data(period, timestamp, user_data);
            } else {
                insert_user_data(period, timestamp, user_data);
            }
        }
    }

   private:
    void update_user_data(const std::string& period, const long long& start_timestamp,
                          const UserData& user_data){
        auto& user_tracker = bars[period][start_timestamp][user_data.user_id];
        user_tracker.token_balances[user_data.currency] += user_data.delta;

        process_user_balance(user_tracker, user_data.timestamp);
    }

    void insert_user_data(const std::string& period, const long long& start_timestamp,
                          const UserData& user_data) {
        auto balance = user_data.delta * exchange_rate[user_data.currency];
        bars[period][start_timestamp][user_data.user_id] =
            UserTracker{.token_balances{{user_data.currency, user_data.delta}}};

        process_user_balance(bars[period][start_timestamp][user_data.user_id], user_data.timestamp);
    }

    static long long get_start_timestamp(const long long& timestamp, const long long& length) {
        return ((timestamp / length) * length);
    }
    void process_valid_users(const long long& m_ts) {
        // for now process all the users
    }
    void get_all_users(File& _user_data_file) {
        std::string user_data_row;
        while (_user_data_file.read_line(user_data_row)) {
            auto user_data = UserData(user_data_row);
            if (user_data.is_valid) {
                all_users.insert(user_data.user_id);
            }
        }
        _user_data_file.back_to_start();
    }

    void process_user_balance(UserTracker& user_tracker, const long long& u_ts) {
        auto new_bal = get_balance(user_tracker.token_balances);
        user_tracker.max_bal = std::max(new_bal, user_tracker.max_bal);
        user_tracker.min_bal = std::max(new_bal, user_tracker.min_bal);
        user_tracker.sum_bal += user_tracker.last_bal * (u_ts - user_tracker.last_ts);
        user_tracker.last_bal = new_bal;
        user_tracker.last_ts = u_ts;
    }

    long double get_balance(const std::map<std::string, long double>& token_balances) {
        long double res = 0.0;
        for (const auto& [cur, val] : token_balances) {
            res += exchange_rate[cur] * val;
        }
        return res;
    }
};
