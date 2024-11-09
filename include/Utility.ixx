module;
#include <string>
#include <vector>
#include <sstream>

export module Utility;

export std::vector<std::string> split_string(const std::string &input, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.emplace_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }

    tokens.emplace_back(input.substr(start));
    return tokens;
}

export std::string join_tokens(std::vector<std::string>::const_iterator begin,
                               const std::vector<std::string>::const_iterator& end, const std::string &delimiter) {
    std::ostringstream oss;
    if (begin != end) {
        oss << *begin++;
    }
    while (begin != end) {
        oss << delimiter << *begin++;
    }
    return oss.str();
}
