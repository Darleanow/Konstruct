module;
#include <string>
#include <unordered_map>
#include <iostream>

export module CLIHandler;
import BaseHandler;
import Token;
import Utility;


export class CLIHandler : public BaseHandler<CLIHandler> {
public:
    CLIHandler() : BaseHandler("--") {
    }

    void register_command_handlers() {
        add_command_handler("proj create", [this](const Token &token) { handle_create_command(token); });
        add_command_handler("proj edit", [this](const Token &token) { handle_edit_command(token); });
        add_command_handler("proj delete", [this](const Token &token) { handle_delete_command(token); });
        add_command_handler("UNKNOWN", [this](const Token &token) { handle_unknown_command(token); });
    }

    void parse_tokens(const std::vector<std::string> &tokens) {
        for (const auto &token_str: tokens) {
            Token token;
            token.command = identify_command(token_str);

            if (!token.command.value.empty()) {
                const auto arg_string = extract_arguments(token_str, token.command.value);
                token.args = parse_argument_string(arg_string);
            }

            m_parsed_tokens.push_back(token);
        }
    }

private:
    static std::string extract_arguments(const std::string &input, const std::string &command) {
        if (const size_t pos = input.find(command); pos != std::string::npos) {
            return input.substr(pos + command.size() + 1);
        }
        return "";
    }

    static std::unordered_map<std::string, std::string> parse_argument_string(const std::string &arg_string) {
        std::unordered_map<std::string, std::string> args;
        for (const auto &token: split_string(arg_string, " ")) {
            if (token.empty()) continue;

            if (const auto key_value = split_string(token, "="); key_value.size() == 2) {
                args[key_value[0]] = key_value[1];
            } else {
                log_error("Invalid argument format: " + token);
            }
        }
        return args;
    }

    static void handle_create_command(const Token &token) {
        std::cout << "CLI: Executing 'create' with args: ";
        for (const auto &[key, value]: token.args) {
            std::cout << key << "=" << value << " ";
        }
        std::cout << std::endl;
    }

    static void handle_edit_command(const Token &token) {
        std::cout << "CLI: Executing 'edit' with args: ";
        for (const auto &[key, value]: token.args) {
            std::cout << key << "=" << value << " ";
        }
        std::cout << std::endl;
    }

    static void handle_delete_command(const Token &token) {
        std::cout << "CLI: Executing 'delete' with args: ";
        for (const auto &[key, value]: token.args) {
            std::cout << key << "=" << value << " ";
        }
        std::cout << std::endl;
    }

    static void handle_unknown_command(const Token &token) {
        std::cout << "CLI: Unknown command: " << token.command.value << std::endl;
    }
};
