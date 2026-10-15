#pragma once

#include <string>

namespace file_loader
{
    bool ReadTextFile(const std::string& path, std::string& outContents);
}
