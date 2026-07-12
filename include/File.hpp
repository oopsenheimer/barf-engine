#pragma once

#include <fstream>

class File {
   private:
    std::fstream file;
    std::string headers;

   public:
    File(const std::string& file_name);
    bool read_headers();
    bool read_line(std::string& line);
    bool back_to_start();
};
