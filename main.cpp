#include <iostream>
#include <vector>
#include <string>

import REPLHandler;
import CLIHandler;
import ParseOption;

#define REPL_EXIT_SUCCESS 1
#define CLI_EXIT_SUCCESS 0

int main(const int argc, char **argv) {
    const std::vector<std::string> args(argv + 1, argv + argc);

    const ParseOption option_parser(args);

    if (option_parser.getOption("it") == "true") {
        std::cout << "Entering REPL mode. Type 'exit' to quit.\n";
        REPLHandler repl_handler;
        std::string input;
        while (true) {
            std::cout << "REPL> ";
            std::getline(std::cin, input);
            if (input == "exit") break;
            repl_handler.parse_input(input);
            repl_handler.execute_commands();
        }
        return REPL_EXIT_SUCCESS;
    }

    const std::string lang = option_parser.getOption("lang");
    const std::string standard = option_parser.getOption("standard");
    const std::vector<std::string> modules = option_parser.getOptionList("modules");

    CLIHandler cli_handler;
    cli_handler.parse_input(lang + " " + standard);
    for (const auto &module: modules) {
        cli_handler.parse_input(module);
    }
    cli_handler.execute_commands();

    std::cout << "Language: " << lang << "\n";
    std::cout << "Standard: " << standard << "\n";
    std::cout << "Modules:\n";
    for (const auto &module: modules) {
        std::cout << "  - " << module << "\n";
    }

    std::cout << "\nAll options:\n";
    option_parser.printOptions();

    std::cout << "Press enter to continue. . .";

    std::string enter_press_holder;
    std::getline(std::cin, enter_press_holder);

    return CLI_EXIT_SUCCESS;
}
