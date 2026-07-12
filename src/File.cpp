#include "File.hpp"

#include <stdexcept>

File::File(const std::string& file_name)
    : file(file_name) {
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