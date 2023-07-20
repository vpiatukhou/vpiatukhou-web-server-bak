#pragma once

#include <filesystem>
#include <fstream>

#include <memory>

namespace WebServer {

    /**
     * Reads static resources (HTML, CSS etc.) from the filesystem.
     */
    class StaticResourceReader {
    public:

        /**
         * Reads the given file into the given string.
         * 
         * @param filepath  - the file to be read
         * @param out       - the file content is written to this string
         * @return TRUE if the file has been succesfully read.
         *         FALSE - otherwise (e.g. the filepath refers to a directory instead of file).
         */
        bool readResourceFromFile(const std::filesystem::path& filepath, std::string& out);

    };

    using StaticResourceReaderPtr = std::shared_ptr<StaticResourceReader>;

}
