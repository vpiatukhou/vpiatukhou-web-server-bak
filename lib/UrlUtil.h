#pragma once

#include <filesystem>

namespace WebServer {

    bool checkIfPathStartsWithBase(const std::filesystem::path& filepath, const std::filesystem::path& base);

}