#pragma once

#include <fstream>
#include <ios>
#include <stdexcept>
#include <string>

class Source {
    std::ifstream file;

   public:
    Source(const Source& other) = delete;
    Source& operator=(const Source&) = delete;

    Source(Source&& other) noexcept = default;
    Source& operator=(Source&& other) noexcept = default;


    Source(const char* file_name) {
        file.open(file_name, std::ios_base::in);
        if (!file.is_open()) {
            throw std::runtime_error("[-] ERROR OPENING SOURCE FILE");
        }
    };

    std::string read_line() {
        std::string res;
        std::getline(file, res);
        return res;
    }
};