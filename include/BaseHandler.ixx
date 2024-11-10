module;

#include <functional>
#include <iostream>

export module BaseHandler;
import Token;
import Utility;


export template<typename Derived>
class BaseHandler {
public:
    using CommandHandler = std::function<void(const Token &)>;

    explicit BaseHandler(const std::string &delimiter) : m_delimiter(delimiter) {
        register_common_commands();
    }

    void parse_input(const std::string &input) {
        const auto tokens = split_string(input, m_delimiter);
        if (tokens.empty()) {
            log_error("Input is empty.");
            return;
        }
        static_cast<Derived *>(this)->parse_tokens(tokens);
    }

    void execute_commands() {
        for (const auto &token: m_parsed_tokens) {
            if (auto it = m_command_handlers.find(token.command.value); it != m_command_handlers.end()) {
                it->second(token);
            } else {
                log_error("No handler found for command: " + token.command.value);
            }
        }
    }

protected:
    void add_command_handler(const std::string &command, CommandHandler handler) {
        m_command_handlers[command] = std::move(handler);
    }

    [[nodiscard]] static CMD_TYPE identify_command(const std::string &token_str) {
        static const std::unordered_map<std::string, CommandTypes> command_lookup = {
            {"proj", CommandTypes::UNKNOWN},
            {"create", CommandTypes::CREATE},
            {"edit", CommandTypes::EDIT},
            {"delete", CommandTypes::DELETE}
        };

        CMD_TYPE cmd;

        if (const auto &it = command_lookup.find(token_str); it != command_lookup.end()) {
            cmd.type = it->second;
            cmd.value = token_str;
        } else {
            cmd.type = CommandTypes::UNKNOWN;
            cmd.value = "UNKNOWN";
        }
        return cmd;
    }

    void register_common_commands() {
        static_cast<Derived *>(this)->register_command_handlers();
    }

    static void log_error(const std::string &message) {
        std::cerr << "Error: " << message << std::endl;
    }

    std::string m_delimiter;
    std::vector<Token> m_parsed_tokens;
    std::unordered_map<std::string, CommandHandler> m_command_handlers;
};
