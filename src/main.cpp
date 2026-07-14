#include <map>
#include <string>

#include "BarfEngine.hpp"
#include "CSVParser.hpp"
#include "File.hpp"

constexpr std::string USER_DATA_FILE = "user_data.csv";
constexpr std::string MARKET_DATA_FILE = "market_data.csv";

std::map<std::string, UserTracker> all_users;
std::map<std::string, long double> exchange_rate;

void setup_market() {
    exchange_rate["USD"] = 1;
}

int main() {
    setup_market();
    File market_data_file(MARKET_DATA_FILE);
    File user_data_file(USER_DATA_FILE);

    get_all_users(all_users, user_data_file);

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
        
        if (m_ts <= u_ts) {
            process_market_data(exchange_rate, market_data);
            process_all_users(exchange_rate, all_users);
            market_data_file.read_line(market_data_row);
            market_data = MarketData(market_data_row);
        } else {
            process_user_data(all_users, user_data);
            user_data_file.read_line(user_data_row);
            user_data = UserData(user_data_row);
        }
    }
}