module;
#include <string>
#include <filesystem>
#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include <iostream>

export module ConfHandler;

export class ConfHandler {
public:
    explicit ConfHandler(std::string language) : m_config_path(std::filesystem::current_path()),
                                                 m_language(std::move(language)) {
        m_config_path.append(language);

        if (!exists(m_config_path)) {
            throw std::runtime_error("The language chosen doesn't exists !");
        }

        for (const auto &filepath: std::filesystem::recursive_directory_iterator(
                 m_config_path, std::filesystem::directory_options::skip_permission_denied)) {
            if (filepath.path().filename().compare("config.yaml")) {
                std::cout << "Found config file: " << filepath.path().filename() << ". Do you want to use it ? Y/n" <<
                        std::endl;
                char response;
                std::cin >> response;
                if (response != 'Y' && response != 'y') {
                    manual_config_processing();
                }
            }
        }

        std::cout <<
                "Nothing was found with the name 'config.yaml' under the default directory, do you want to input the path yourself ? Y/n";
        char response;
        std::cin >> response;

        if (response != 'Y' && response != 'y') {
            manual_config_processing();
        }
        std::string custom_path;

        std::cout << "Enter the path: ";
        getline(std::cin, custom_path);

        if (exists(std::filesystem::path(custom_path))) {
            std::cout << "File found, proceeding. . .\n";
            m_config_path = std::filesystem::path(custom_path);
            process_yaml_file();
        } else {
            std::cout << "Provided path doesn't exists.";
            if (exists(std::filesystem::path(custom_path).parent_path())) {
                std::cout << "However, found the following files under the parent directory: ";
                // TODO(enzo): List files here and allow selection
            }
        }
    }

    ~ConfHandler() = default;

    static void manual_config_processing() {
        std::cout << "Success !";
    }

    static void process_yaml_file() {
        throw std::runtime_error("Not Implemented");
    }

private:
    std::filesystem::path m_config_path;
    std::string m_language;
};
