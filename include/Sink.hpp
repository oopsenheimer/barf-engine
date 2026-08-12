#pragma once

#include <fstream>
#include <ios>
#include <stdexcept>

class Sink {
    std::ofstream file;

   public:
    Sink(const Sink& other) = delete;
    Sink& operator=(const Sink& other) = delete;

    Sink(Sink&& other) noexcept = default;
    Sink& operator=(Sink&& other) noexcept = default;

    Sink(const char* file_name) {
        file.open(file_name, std::ios_base::trunc | std::ios_base ::out);
        if (!file.is_open()) {
            throw std::runtime_error("[-] ERROR OPENING SINK FILE");
        }
    }

    void write_line(const char* line) {
        file << line << '\n';
    }
};