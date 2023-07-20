#include "StaticResourceReader.h"

namespace WebServer {

    namespace {
        namespace fs = std::filesystem;

        const char END_OF_STRING = '\0';
    }

    bool StaticResourceReader::readResourceFromFile(const fs::path& filepath, std::string& out) {
        if (fs::is_regular_file(filepath)) {
            if (std::ifstream input{filepath, std::ios::binary | std::ios::ate}) {
                auto fileSize = input.tellg();
                out.resize(fileSize, END_OF_STRING);
                input.seekg(0);
                if (input.read(out.data(), fileSize)) {
                    return true;
                }
            }
        }
        return false;
    }

}
