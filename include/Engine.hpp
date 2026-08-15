#pragma once

#include <array>
#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Data.hpp"
#include "Sink.hpp"
#include "Transformer.hpp"

class Engine {
   private:
    std::unordered_map<std::string, UserData> users;
    std::unordered_map<std::string, Amount> exchange_rate;
    std::unordered_map<std::string, std::unordered_set<std::string>> currency_users;
    std::array<std::optional<Sink>, NUM_PERIODS> output_file;

   public:
    Engine() { exchange_rate[USD] = 1.0; }
    ~Engine() {
        // flush any remaining bars
        for (int i = 0; i < NUM_PERIODS; ++i) {
            for (auto& [user_id, user_data] : users) {
                close_bar(PERIODS[i], user_data.open_bar[i]);
                flush_bar(user_id, get_closed_bar(user_data.open_bar[i], PERIODS[i]), i);
            }
        }
    }
    void process_transaction(const TransactionData& transaction_data) {
        auto& user_data = users[transaction_data.user_id];

        user_data.quantity[transaction_data.currency] += transaction_data.delta;
        currency_users[transaction_data.currency].insert(transaction_data.user_id);

        // update that user_id
        update_user_data(transaction_data.user_id, transaction_data.timestamp, user_data);
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
            update_user_data(user_id, market_data.timestamp, users[user_id]);
        }
    }

    void initilize_output_files(const std::array<const char*, NUM_PERIODS>& file_name) {
        for (int i = 0; i < NUM_PERIODS; ++i) {
            output_file[i] = Sink(file_name[i]);
        }
    }

   private:
    void update_user_data(const std::string& user_id, const Time& timestamp, UserData& user_data) {
        auto new_bal = get_user_bal(user_data.quantity);
        for (int i = 0; i < NUM_PERIODS; ++i) {
            update_user_bar(user_id, new_bal, timestamp, PERIODS[i], i, user_data.open_bar[i]);
        }
    }

    void update_user_bar(const std::string& user_id, const Amount& user_bal, const Time& timestamp,
                         const Time& period, const int& period_idx,
                         std::optional<OpenBar>& open_bar) {
        auto event_bar_start_ts = get_bar_start_timestamp(timestamp, period);
        auto event_bar_end_ts = get_bar_end_timestamp(timestamp, period);

        if (!open_bar.has_value()) {
            open_bar = OpenBar{event_bar_start_ts, timestamp, user_bal};
            return;
        }

        auto closed_bar_cnt = (timestamp - open_bar->bar_start_ts) / period;

        while (closed_bar_cnt > 0) {
            close_bar(period, open_bar);
            flush_bar(user_id, get_closed_bar(open_bar, period), period_idx);
            open_bar->bar_start_ts += period;
            open_bar->last_update_ts = open_bar->bar_start_ts;
            open_bar->sum_bal = 0;
            open_bar->min_bal = open_bar->last_bal;
            open_bar->max_bal = open_bar->last_bal;
            closed_bar_cnt--;
        }
        assert(event_bar_start_ts == open_bar->bar_start_ts);
        if (timestamp == event_bar_start_ts) {
            open_bar->min_bal = user_bal;
            open_bar->max_bal = user_bal;
        } else {
            open_bar->min_bal = std::min(user_bal, open_bar->min_bal);
            open_bar->max_bal = std::max(user_bal, open_bar->max_bal);
        }
        open_bar->sum_bal += open_bar->last_bal * (timestamp - open_bar->last_update_ts);
        open_bar->last_bal = user_bal;
        open_bar->last_update_ts = timestamp;
    }

    static void close_bar(const Time& period, std::optional<OpenBar>& open_bar) {
        open_bar->sum_bal +=
            open_bar->last_bal * (open_bar->bar_start_ts + period - open_bar->last_update_ts);
    }

    Amount get_user_bal(const decltype(UserData::quantity)& quantity) {
        Amount res = 0;
        for (const auto& [cur, cnt] : quantity) {
            res += exchange_rate.at(cur) * cnt;
        }
        return res;
    }

    static Time get_bar_start_timestamp(const Time& event_timestamp, const Time& period) {
        auto res = event_timestamp / period;
        return res * period;
    }

    static Time get_bar_end_timestamp(const Time& event_timestamp, const Time& period) {
        auto res = event_timestamp / period;
        res *= period;
        return res + period;
    }

    void flush_bar(const std::string& user_id, const Bar& closed_bar, const Time& period_idx) {
        if (output_file[period_idx].has_value()) {
            output_file[period_idx]->write_line(closed_bar_parser(user_id, closed_bar));
        }
    }

    static Bar get_closed_bar(const std::optional<OpenBar>& open_bar, const Time& period) {
        return Bar{.bar_start_ts = open_bar->bar_start_ts,
                   .min_bal = open_bar->min_bal,
                   .max_bal = open_bar->max_bal,
                   .avg_bal = open_bar->sum_bal / period};
    }
};