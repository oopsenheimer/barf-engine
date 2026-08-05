#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "BarfEngine.hpp"
#include "CSVParser.hpp"
#include "File.hpp"

constexpr std::string USER_DATA_FILE = "user_data.csv";
constexpr std::string MARKET_DATA_FILE = "market_data.csv";
constexpr std::string BARS_1H_FILE = "bars-1h.csv";
constexpr std::string BARS_1D_FILE = "bars-1d.csv";
constexpr std::string BARS_30D_FILE = "bars-30d.csv";

File market_data_file(MARKET_DATA_FILE);
File user_data_file(USER_DATA_FILE);
File bars_1h_file(BARS_1H_FILE, File::RW_FILE);
File bars_1d_file(BARS_1D_FILE, File::RW_FILE);
File bars_30d_file(BARS_30D_FILE, File::RW_FILE);

long long global_start_ts = -1;
long long global_end_ts = -1;
long long global_last_ts = -1;

int main() {

    std::string user_data_row;
    user_data_file.read_line(user_data_row);
    auto user_data = UserData(user_data_row);

    std::string market_data_row;
    market_data_file.read_line(market_data_row);
    auto market_data = MarketData(market_data_row);

    while (user_data.is_valid || market_data.is_valid) {
        long long m_ts =
            market_data.is_valid ? market_data.timestamp : std::numeric_limits<long long>::max();
        long long u_ts =
            user_data.is_valid ? user_data.timestamp : std::numeric_limits<long long>::max();
        global_last_ts = std::min(m_ts, u_ts);
        if (m_ts <= u_ts) {
            // process the market data -> update the exchange rates
            
            // process the users having that currency and update the values

            market_data_file.read_line(market_data_row);
            market_data = MarketData(market_data_row);
        } else {
            user_data_file.read_line(user_data_row);
            user_data = UserData(user_data_row);
        }
    }
}