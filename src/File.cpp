#include "File.hpp"

#include <ios>
#include <stdexcept>

File::File(const std::string& file_name, std::ios_base::openmode mode) {
    file.open(file_name, mode);
    if (!file.is_open()) {
        throw std::runtime_error("[-] ERROR OPENING FILE");
    }

    if (!read_headers()) {
        throw std::runtime_error("[-] ERROR READING FILE");
    }
}

bool File::read_headers() { return static_cast<bool>(std::getline(file, headers)); }

bool File::read_line(std::string& line) { return static_cast<bool>(std::getline(file, line)); }

bool File::back_to_start() {
    file.clear();
    file.seekg(0);
    return static_cast<bool>(file);
}