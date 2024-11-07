module;
#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
export module REPL;
export import Parser;

export class REPL {
public:
    explicit REPL(Parser parser) : m_parser(std::move(parser)) {
        loop();
    }

    ~REPL() = default;

    void read(const std::string& input) {
        m_parser.parse_input(input);
        eval();
    }

    void eval() {
        m_parser.handle_tokens();
        const auto& results = m_parser.get_info();
        print(results);
    }

    static void print(const std::vector<std::string>& logs) {
        const auto now = std::chrono::system_clock::now();
        const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        tm time_info;

        if (localtime_s(&time_info, &now_time) == 0) {
            for (const auto& log : logs) {
                std::cout << std::put_time(&time_info, "%H:%M:%S") << " | " << log << "\n";
            }
        } else {
            std::cerr << "Failed to convert time\n";
        }
    }

    void loop() {
        while (true) {
            std::cout << "Konstruct> ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            }

            read(input);
            m_parser.cleanup();
        }
    }

private:
    Parser m_parser;
};
