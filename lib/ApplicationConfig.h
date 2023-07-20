#pragma once

#include "Port.h"

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

namespace WebServer {

    /**
     * An HTTP status. This alias is used only in config. In other places we use http:status enum from Boost.
     */
    using HttpStatus = unsigned int;

    /**
     * Mapping a filename pattern to a media type.
     */
    struct MediaTypeMapping {
        std::string filenameRegExp;
        std::string mediaType;
    };

    /**
     * Holds the configuration of the application.
     *
     * ATTENTION: the SSL password is CACHED in memory.
     */
    class ApplicationConfig {
    public:
        ApplicationConfig(const std::string& configFilepath_);
        ApplicationConfig(const ApplicationConfig&) = delete;
        ApplicationConfig(ApplicationConfig&&) = delete;
        ~ApplicationConfig() = default;

        ApplicationConfig& operator=(const ApplicationConfig&) = delete;
        ApplicationConfig& operator=(ApplicationConfig&&) = delete;

        /**
         * @return the port, the server is listening on.
         */
        Port getServerPort() const;

        /**
         * @return the value which is returned in "Server" header in a HTTP response.
         */
        const std::string& getServerName() const;

        /**
         * @return TRUE if SSL is enabled.
         */
        bool isSslEnabled() const;

        /**
         * @return a path to the SSL certificate file. The path is normalized (please see https://en.cppreference.com/w/cpp/filesystem/path).
         */
        const std::filesystem::path& getSslCertificatePath() const;

        /**
         * @return a path to the private key file. The path is normalized (please see https://en.cppreference.com/w/cpp/filesystem/path).
         */
        const std::filesystem::path& getSslPrivateKeyPath() const;

        /**
         * @return a path to the DH file. The path is normalized (please see https://en.cppreference.com/w/cpp/filesystem/path).
         */
        const std::filesystem::path& getSslDhFilepath() const;

        /**
         * @return the certificate's pass phrase.
         *         ATTENTION: the password is CACHED in memory.
         */
        const std::string& getSslPassword() const;

        /**
         * @return a path to the directory in which static web resources are stored.
         *         A web client can request any file in this directory.
         *         The path is always ends with a directory delimiter ('/' on UNIX).
         *         The path is normalized (please see https://en.cppreference.com/w/cpp/filesystem/path).
         */
        const std::filesystem::path& getStaticResouceBaseDir() const;

        /**
         * @return a path to the page which will be displayed if the server returns 401 response.
         *         The pass already contains getStaticResouceBaseDir().
         */
        const std::filesystem::path& getForbiddenPage() const;

        /**
         * @return a path to the page which will be displayed if the server returns 404 response.
         *         The pass already contains getStaticResouceBaseDir().
         */
        const std::filesystem::path& getNotFoundPage() const;

        /**
         * @return a path to the page which will be displayed if the server returns 405 response.
         *         The pass already contains getStaticResouceBaseDir().
         */
        const std::filesystem::path& getMethodNotAllowedPage() const;

        /**
         * @return mapping between HTTP status codes and error pages.
         */
        const std::unordered_map<unsigned int, std::filesystem::path>& getErrorPageMapping() const;

        /**
         * @return mapping between media types and filenames.
         */
        const std::vector<MediaTypeMapping>& getMediaTypeMapping() const;

    private:
        Port port = 0;
        std::string serverName;

        bool sslEnabled = false;
        std::filesystem::path sslCertificatePath;
        std::filesystem::path sslPrivateKeyPath;
        std::string sslPassword;
        std::filesystem::path sslDhFilepath;

        std::filesystem::path staticResouceBaseDir;

        std::unordered_map<HttpStatus, std::filesystem::path> errorPageMapping;
        std::vector<MediaTypeMapping> mediaTypeMapping;
    };

    using ApplicationConfigPtr = std::shared_ptr<ApplicationConfig>;

}
