#pragma once

#include <fstream>
#include <ios>

class File {
   public:
    static constexpr auto RD_ONLY = std::ios_base::in;
    static constexpr auto RW_FILE = std::ios_base::in | std::ios_base::out | std::ios_base::trunc;

   private:
    std::fstream file;
    std::string headers;

   public:
    explicit File(const std::string& file_name, std::ios_base::openmode mode = RD_ONLY);
    bool read_headers();
    bool read_line(std::string& line);
    bool back_to_start();

    template <typename CSVData>
    bool write_line(CSVData csv_data) {
        file << csv_data << '\n';
        return static_cast<bool>(file);
    }
};
