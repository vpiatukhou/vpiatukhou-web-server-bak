#include "ApplicationConfig.h"

#include <boost/foreach.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <filesystem>
#include <string>

namespace WebServer {

    namespace {
        using namespace std::literals::string_literals;
        namespace pt = boost::property_tree;
        namespace fs = std::filesystem;

        const char PATH_DELIMITER = '/';
        const auto EMPTY = ""s;

        const auto SERVER_PORT_PROPERTY = "serverPort"s;
        const auto SERVER_NAME_PROPERTY = "serverName"s;

        const auto SSL_ENABLED_PROPERTY = "ssl.enabled"s;
        const auto SSL_CERTIFICATE_FILEPATH_PROPERTY = "ssl.certificateFilepath"s;
        const auto SSL_PRIVATE_KEY_FILEPATH_PROPERTY = "ssl.privateKeyFilepath"s;
        const auto SSL_DH_FILEPATH_PROPERTY = "ssl.dhFilepath"s;
        const auto SSL_PASSWORD_FILEPATH_PROPERTY = "ssl.password"s;

        const auto STATIC_RESOURCE_BASE_DIR_PROPERTY = "staticResourceMapping.baseDir"s;
        const auto ERROR_PAGE_MAPPING_PROPERTY = "staticResourceMapping.errorPageMapping"s;
        const auto ERROR_PAGE_MAPPING_STATUS_PROPERTY = "status"s;
        const auto ERROR_PAGE_MAPPING_PAGE_PROPERTY = "page"s;

        const auto MEDIA_TYPE_MAPPING_PROPERTY = "mediaTypeMapping"s;
        const auto MEDIA_TYPE_MAPPING_FILE_NAME_PROPERTY = "fileNameRegex"s;
        const auto MEDIA_TYPE_MAPPING_TYPE_PROPERTY = "mediaType"s;

        const Port DEFAULT_PORT = 8080;
        const auto DEFAULT_SERVER_NAME = "Web Server"s;
        const bool DEFAULT_SSL_ENABLED = false;

        const auto DEFAULT_STATIC_RESOURCE_DIR = "/var/www/webserver/"s;

        void readErrorPageMapping(const pt::iptree& properties, const fs::path& baseDir,
            std::unordered_map<HttpStatus, std::filesystem::path>& errorPageMapping) {

            if (auto mappingNode = properties.get_child_optional(ERROR_PAGE_MAPPING_PROPERTY)) {
                BOOST_FOREACH(pt::iptree::value_type const& value, *mappingNode) {
                    auto status = value.second.get<HttpStatus>(ERROR_PAGE_MAPPING_STATUS_PROPERTY);
                    fs::path pagePath = value.second.get<std::string>(ERROR_PAGE_MAPPING_PAGE_PROPERTY);

                    if (pagePath.is_absolute()) {
                        //remove the leading '/', if someone accidentally makes the path absolute in the config
                        pagePath = fs::relative(pagePath, pagePath.root_path());
                    }

                    pagePath = baseDir / fs::path(pagePath);
                    pagePath = pagePath.lexically_normal();

                    errorPageMapping[status] = pagePath;
                }
            }
        }

        void readMediaTypeMapping(const pt::iptree& properties, const fs::path& baseDir, 
            std::vector<MediaTypeMapping>& mediaTypeMapping) {

            if (auto mappingNode = properties.get_child_optional(MEDIA_TYPE_MAPPING_PROPERTY)) {
                BOOST_FOREACH(pt::iptree::value_type const& value, *mappingNode) {
                    MediaTypeMapping mapping {
                        value.second.get<std::string>(MEDIA_TYPE_MAPPING_FILE_NAME_PROPERTY),
                        value.second.get<std::string>(MEDIA_TYPE_MAPPING_TYPE_PROPERTY)
                    };
                    mediaTypeMapping.push_back(mapping);
                }
            }
        }
    }

    ApplicationConfig::ApplicationConfig(const std::string& configFilepath_) {
        if (fs::is_regular_file(configFilepath_)) {
            BOOST_LOG_TRIVIAL(info) << "Reading the config '" << configFilepath_ << "'.";

            pt::iptree properties;
            pt::read_json(configFilepath_, properties);

            port = properties.get<Port>(SERVER_PORT_PROPERTY, DEFAULT_PORT);
            serverName = properties.get<std::string>(SERVER_NAME_PROPERTY, DEFAULT_SERVER_NAME);

            sslEnabled = properties.get<bool>(SSL_ENABLED_PROPERTY, DEFAULT_SSL_ENABLED);
            sslCertificatePath = properties.get<std::string>(SSL_CERTIFICATE_FILEPATH_PROPERTY, EMPTY);
            sslPrivateKeyPath = properties.get<std::string>(SSL_PRIVATE_KEY_FILEPATH_PROPERTY, EMPTY);
            sslDhFilepath = properties.get<std::string>(SSL_DH_FILEPATH_PROPERTY, EMPTY);
            sslPassword = properties.get<std::string>(SSL_PASSWORD_FILEPATH_PROPERTY, EMPTY);

            staticResouceBaseDir = properties.get<std::string>(STATIC_RESOURCE_BASE_DIR_PROPERTY, DEFAULT_STATIC_RESOURCE_DIR);
            staticResouceBaseDir += PATH_DELIMITER;
            staticResouceBaseDir = fs::path(staticResouceBaseDir).lexically_normal();

            readErrorPageMapping(properties, staticResouceBaseDir, errorPageMapping);
            readMediaTypeMapping(properties, staticResouceBaseDir, mediaTypeMapping);
        } else {
            BOOST_LOG_TRIVIAL(info) << "The application config '" << configFilepath_ 
                << "' was not found. Default values will be used.";;

            port = DEFAULT_PORT;
            serverName = DEFAULT_SERVER_NAME;
            sslEnabled = DEFAULT_SSL_ENABLED;
            staticResouceBaseDir = DEFAULT_STATIC_RESOURCE_DIR;
        }
    }

    Port ApplicationConfig::getServerPort() const {
        return port;
    }

    const std::string& ApplicationConfig::getServerName() const {
        return serverName;
    }

    bool ApplicationConfig::isSslEnabled() const {
        return sslEnabled;
    }

    const std::filesystem::path& ApplicationConfig::getSslCertificatePath() const {
        return sslCertificatePath;
    }

    const std::string& ApplicationConfig::getSslPassword() const {
        return sslPassword;
    }

    const std::filesystem::path& ApplicationConfig::getSslPrivateKeyPath() const {
        return sslPrivateKeyPath;
    }

    const std::filesystem::path& ApplicationConfig::getSslDhFilepath() const {
        return sslDhFilepath;
    }

    const std::filesystem::path& ApplicationConfig::getStaticResouceBaseDir() const {
        return staticResouceBaseDir;
    }

    const std::unordered_map<unsigned int, std::filesystem::path>& ApplicationConfig::getErrorPageMapping() const {
        return errorPageMapping;
    }

    const std::vector<MediaTypeMapping>& ApplicationConfig::getMediaTypeMapping() const {
        return mediaTypeMapping;
    }
}
