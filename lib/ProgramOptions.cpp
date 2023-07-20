#include "ProgramOptions.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <stdexcept>

namespace WebServer {

    namespace {
        const std::string HELP_PARAM = "help";
        const std::string DEFAULT_CONFIG_FILEPATH = "/etc/webserver/config/application.json";
    }

    void ProgramOptions::parse(int argc, char* argv[]) {
        namespace po = boost::program_options;

        auto configOption = po::value<std::string>(&configFilepath)->default_value(DEFAULT_CONFIG_FILEPATH);

        po::options_description optionsDescription("Options");
        optionsDescription.add_options()("config,c", configOption, "A path to the config file.\n");

        po::variables_map optionValues;

        po::store(po::parse_command_line(argc, argv, optionsDescription), optionValues);
        po::notify(optionValues);
    }

    const std::string& ProgramOptions::getConfigFilepath() const {
        return configFilepath;
    }
};