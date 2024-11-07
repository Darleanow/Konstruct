module;
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
export module Parser;

export enum class ParserModes {
    CLM_MODE,
    REPL_MODE,
};

enum class CommandTypes {
    CREATE,
    EDIT,
    DELETE,
    UNKNOWN
};

struct CMD_TYPE {
    CommandTypes type = CommandTypes::UNKNOWN;
    std::string value;
};

struct Token {
    CMD_TYPE command;
    std::unordered_map<std::string, std::string> args;
};

std::vector<std::string> str_split(const std::string &input, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    tokens.push_back(input.substr(start));
    return tokens;
}

export class Parser {
public:
    explicit Parser(const ParserModes parser_mode) : m_parser_mode(parser_mode) {
        m_delimiter = m_parser_mode == ParserModes::CLM_MODE ? "--" : " ";
    }

    void parse_input(const std::string &input) {
        if (const auto &raw_tokens = str_split(input, m_delimiter);
            m_parser_mode == ParserModes::REPL_MODE && !raw_tokens.empty()) {
            parse_repl_command(raw_tokens);
        } else {
            for (const auto &tok: raw_tokens) {
                parse_token(tok);
            }
        }
    }

    void handle_tokens() {
        for (const auto &[command, args]: m_tokens) {
            std::string log = "Handling command: " + command.value + " with parameters:";
            for (const auto &[key, value]: args) {
                log.append(" ").append(key).append("=").append(value);
            }
            m_logs.push_back(log);
        }
    }

    [[nodiscard]] std::vector<std::string> get_info() const {
        return m_logs;
    }

    void cleanup() {
        m_tokens.clear();
    }

private:
    void parse_repl_command(const std::vector<std::string> &raw_tokens) {
        if (raw_tokens.empty()) return;

        Token token;

        token.command = parse_command(raw_tokens[0]);

        if (raw_tokens.size() > 1) {
            const auto &subcommand = raw_tokens[1];
            token.command = parse_subcommand(token.command, subcommand);


            std::string args_string;
            for (size_t i = 2; i < raw_tokens.size(); ++i) {
                args_string += raw_tokens[i] + " ";
            }
            token.args = parse_arguments(args_string);
        }

        m_tokens.push_back(token);
    }

    void parse_token(const std::string &tok) {
        Token token;
        token.command = parse_command(tok);

        if (!token.command.value.empty()) {
            const auto &arg_string = extract_arguments(tok, token.command.value);
            token.args = parse_arguments(arg_string);
        }

        m_tokens.push_back(token);
    }

    static CMD_TYPE parse_command(const std::string &tok) {
        CMD_TYPE cmd;
        if (tok == "proj") {
            cmd.type = CommandTypes::UNKNOWN;
            cmd.value = "proj";
        } else {
            cmd.type = CommandTypes::UNKNOWN;
            cmd.value = "UNKNOWN";
        }
        return cmd;
    }

    static CMD_TYPE parse_subcommand(const CMD_TYPE &parent_cmd, const std::string &subcommand) {
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

    static std::string extract_arguments(const std::string &input, const std::string &command) {
        if (const size_t pos = input.find(command); pos != std::string::npos) {
            return input.substr(pos + command.size() + 1);
        }
        return "";
    }

    static std::unordered_map<std::string, std::string> parse_arguments(const std::string &arg_string) {
        std::unordered_map<std::string, std::string> args;

        for (const auto &token: str_split(arg_string, " ")) {
            if (token.empty()) continue;

            if (const auto key_value = str_split(token, "="); key_value.size() == 2) {
                args[key_value[0]] = key_value[1];
            }
        }
        return args;
    }

    std::string m_delimiter;
    ParserModes m_parser_mode;
    std::vector<Token> m_tokens;
    std::vector<std::string> m_logs;
};
