module;
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

export module REPLHandler;
import BaseHandler;
import Utility;
import Token;

export class REPLHandler : public BaseHandler<REPLHandler> {
public:
    REPLHandler() : BaseHandler(" ") {
    }

    void register_command_handlers() {
        add_command_handler("proj create", [this](const Token &token) { handle_create_command(token); });
        add_command_handler("proj edit", [this](const Token &token) { handle_edit_command(token); });
        add_command_handler("proj delete", [this](const Token &token) { handle_delete_command(token); });
        add_command_handler("UNKNOWN", [this](const Token &token) { handle_unknown_command(token); });
    }

    void parse_tokens(const std::vector<std::string> &tokens) {
        if (tokens.empty()) return;

        Token token;
        token.command = identify_command(tokens[0]);

        if (tokens.size() > 1) {
            const auto &subcommand = tokens[1];
            token.command = identify_subcommand(token.command, subcommand);
            const std::string args_string = join_tokens(tokens.begin() + 2, tokens.end(), " ");
            token.args = parse_argument_string(args_string);
        }

        m_parsed_tokens.push_back(token);
    }

private:
    static CMD_TYPE identify_subcommand(const CMD_TYPE &parent_cmd, const std::string &subcommand) {
        CMD_TYPE cmd = parent_cmd;
        if (cmd.value == "proj") {
            if (subcommand == "create") {
                cmd.type = CommandTypes::CREATE;
                cmd.value = "proj create";
            } else if (subcommand == "edit") {
                cmd.type = CommandTypes::EDIT;
                cmd.value = "proj edit";
            } else if (subcommand == "delete") {
                cmd.type = CommandTypes::DELETE;
                cmd.value = "proj delete";
            } else {
                cmd.type = CommandTypes::UNKNOWN;
                cmd.value = "proj unknown";
            }
        }
        return cmd;
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
        std::cout << "Konstruct: Executing 'create' with args: ";
        for (const auto &[key, value]: token.args) {
            std::cout << key << "=" << value << " ";
        }
        std::cout << std::endl;
    }

    static void handle_edit_command(const Token &token) {
        std::cout << "Konstruct: Executing 'edit' with args: ";
        for (const auto &[key, value]: token.args) {
            std::cout << key << "=" << value << " ";
        }
        std::cout << std::endl;
    }

    static void handle_delete_command(const Token &token) {
        std::cout << "Konstruct: Executing 'delete' with args: ";
        for (const auto &[key, value]: token.args) {
            std::cout << key << "=" << value << " ";
        }
        std::cout << std::endl;
    }

    static void handle_unknown_command(const Token &token) {
        std::cout << "Konstruct: Unknown command: " << token.command.value << std::endl;
    }
};
