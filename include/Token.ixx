module;
#include <string>
#include <unordered_map>

export module Token;

export enum class CommandTypes {
    CREATE,
    EDIT,
    DELETE,
    UNKNOWN
};

export struct CMD_TYPE {
    CommandTypes type = CommandTypes::UNKNOWN;
    std::string value;
};

export struct Token {
    CMD_TYPE command;
    std::unordered_map<std::string, std::string> args;
};
