module;

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

export module ParseOption;

std::vector<std::string> split(const std::string &str, const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

export class ParseOption {
public:
    explicit ParseOption(const std::vector<std::string> &args) {
        for (const auto &arg: args) {
            handle_parse_arg(arg);
        }
    }

    void add_option(const std::string &arg) {
        handle_parse_arg(arg);
    }

    void handle_parse_arg(const std::string &arg) {
        if (arg.find("--") == 0) {
            if (const auto equalPos = arg.find('='); equalPos != std::string::npos) {
                const std::string key = arg.substr(2, equalPos - 2);
                const std::string value = arg.substr(equalPos + 1);
                m_options[key] = value;
            }
        }
    }

    [[nodiscard]] std::string getOption(const std::string &key) const {
        const auto it = m_options.find(key);
        return it != m_options.end() ? it->second : "";
    }


    [[nodiscard]] std::vector<std::string> getOptionList(const std::string &key, const char delimiter = ',') const {
        if (const std::string value = getOption(key); !value.empty()) {
            return split(value, delimiter);
        }
        return {};
    }

    std::vector<std::string> execute() {
        std::vector<std::string> logs;

        for (const auto &[key, value]: m_options) {
            std::string log = static_cast<std::string>("Handling command ").append(key).append(" with value ").
                    append(value);

            logs.push_back(log);
        }

        return logs;
    }


    void printOptions() const {
        for (const auto &[key, value]: m_options) {
            std::cout << "--" << key << " = " << value << "\n";
        }
    }

private:
    std::unordered_map<std::string, std::string> m_options;
};
