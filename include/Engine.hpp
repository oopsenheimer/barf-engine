#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Data.hpp"

class Engine {
    std::unordered_map<std::string, Price> exchange_rate;
    
    std::unordered_map<std::string, std::unordered_set<std::string>> currency_users;

   public:
    std::unordered_map<std::string, UserData> users;
    Engine() { exchange_rate[USD] = 1.0; }
    ~Engine() {
        // flush any remaining bars
    }
    void process_transaction(const TransactionData& transaction_data) {

        users[transaction_data.user_id].ammount[transaction_data.currency] +=
            transaction_data.delta;
        currency_users[transaction_data.currency].insert(transaction_data.user_id);

        // update that user_id
        update_user_data(transaction_data.user_id, transaction_data.timestamp);
    }

    void process_market_tick(const MarketTickData& market_data) {
        if (market_data.to_currency != USD) {
            throw std::runtime_error("[-] MARKET DATA CONVERSION NOT ALLOWED");
        }

        // update market data
        exchange_rate[market_data.from_currency] =
            market_data.price * exchange_rate[market_data.to_currency];

        for (const std::string& user_id : currency_users[market_data.from_currency]) {
            // update those user_id
            update_user_data(user_id, market_data.timestamp);
        }
    }

   private:
    void update_user_data(const std::string& user_id, const Time& timestamp) {
        
    }
};