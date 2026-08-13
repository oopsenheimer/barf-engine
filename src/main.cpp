#include <limits>

#include "Data.hpp"
#include "Engine.hpp"
#include "Transformer.hpp"
#include "Source.hpp"

constexpr const char* MARKET_DATA_FILENAME = "market_data.csv";
constexpr const char* USER_DATA_FILENAME = "user_data.csv";

constexpr const char* H1_BARS_OUTPUT_FILENAME = "bars-1h.csv";
constexpr const char* D1_BARS_OUTPUT_FILENAME = "bars-1d.csv";
constexpr const char* D30_BARS_OUTPUT_FILENAME = "bars-30d.csv";

int main() {
    Source market_data_file(MARKET_DATA_FILENAME);
    Source user_data_file(USER_DATA_FILENAME);

    // get headers out
    auto market_data_h = market_data_file.read_line();
    auto user_data_h = user_data_file.read_line();

    auto market_tick = market_tick_parser(market_data_file.read_line());
    auto transaction = transaction_data_parser(user_data_file.read_line());

    Engine barf;
    barf.initilize_output_files(
        {H1_BARS_OUTPUT_FILENAME, D1_BARS_OUTPUT_FILENAME, D30_BARS_OUTPUT_FILENAME});

    while (market_tick.is_valid || transaction.is_valid) {
        Time m_ts = market_tick.is_valid ? market_tick.timestamp : std::numeric_limits<Time>::max();
        Time t_ts = transaction.is_valid ? transaction.timestamp : std::numeric_limits<Time>::max();

        if (m_ts <= t_ts) {
            barf.process_market_tick(market_tick);
            market_tick = market_tick_parser(market_data_file.read_line());
        } else {
            barf.process_transaction(transaction);
            transaction = transaction_data_parser(user_data_file.read_line());
        }
    }
}