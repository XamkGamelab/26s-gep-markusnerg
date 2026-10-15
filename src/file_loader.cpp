#include "file_loader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace file_loader
{
    bool ReadTextFile(const std::string& path, std::string& outContents)
    {
        std::ifstream stream(path, std::ios::in | std::ios::binary);

        if (!stream.is_open())
        {
            std::cerr << "file_loader: could not open " << path << std::endl;
            return false;
        }

        std::ostringstream buffer;
        buffer << stream.rdbuf();

        if (stream.bad())
        {
            std::cerr << "file_loader: failed while reading " << path << std::endl;
            return false;
        }

        outContents = buffer.str();
        return true;
    }
}
