#include <iostream>
#include <vector>
#include <string>
import ParseOption;
import REPL;


#define REPL_EXIT_SUCESS 1;
#define CLM_EXIT_SUCESS 0;

int main(const int argc, char **argv) {
    const std::vector<std::string> args(argv + 1, argv + argc);

    const ParseOption legacy_parser(args);

    if (legacy_parser.getOption("it") == "true") {
        std::cout << "Entering REPL mode. Type 'exit' to quit.\n";
        Parser parser(ParserModes::REPL_MODE);
        REPL repl(parser);
        return REPL_EXIT_SUCESS;
    }

    const std::string lang = legacy_parser.getOption("lang");
    const std::string standard = legacy_parser.getOption("standard");
    const std::vector<std::string> modules = legacy_parser.getOptionList("modules");

    std::cout << "Language: " << lang << "\n";
    std::cout << "Standard: " << standard << "\n";
    std::cout << "Modules:\n";
    for (const auto &module: modules) {
        std::cout << "  - " << module << "\n";
    }

    std::cout << "\nAll options:\n";
    legacy_parser.printOptions();

    std::cout << "Press enter to continue. . .";

    std::string enter_press_holder;
    std::getline(std::cin, enter_press_holder);


    return CLM_EXIT_SUCESS;
}
