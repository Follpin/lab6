#include "input_validator.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <format>
#include <limits>

namespace Game {

std::pair<bool, std::string> InputValidator::validateAndParseNumber(const std::string& input, Number& outNumber) {
    if (input.empty()) {
        return { false, "Input string is empty." };
    }

    size_t first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return { false, "Input string contains only whitespace." };
    }
    size_t last = input.find_last_not_of(" \t\r\n");
    std::string trimmed = input.substr(first, (last - first + 1));

    if (trimmed[0] == '-') {
        return { false, "Number must be positive (received negative value: " + trimmed + ")." };
    }
    if (trimmed[0] == '+') {
        trimmed = trimmed.substr(1);
    }
    if (trimmed.empty()) {
        return { false, "Invalid number format (plus sign only)." };
    }

    for (char c : trimmed) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            if (c == '.' || c == ',') {
                return { false, "Integer expected, not a floating-point number (" + trimmed + ")." };
            }
            return { false, "String contains invalid non-numeric characters: '" + std::string(1, c) + "'." };
        }
    }

    try {
        size_t idx = 0;
        unsigned long long val = std::stoull(trimmed, &idx);
        if (idx != trimmed.size()) {
            return { false, "Could not parse the number completely." };
        }
        if (val == 0) {
            return { false, "Number must be strictly positive (> 0)." };
        }

        constexpr unsigned long long kMaxInitial = 1'000'000'000ULL;
        if (val > kMaxInitial) {
            return { false, std::format("Number is too large (> {}). Please choose a smaller initial number.", kMaxInitial) };
        }

        outNumber = static_cast<Number>(val);
        return { true, "" };
    } catch (const std::out_of_range&) {
        return { false, "Number is out of range for unsigned long long." };
    } catch (const std::exception& e) {
        return { false, std::string("Parse error: ") + e.what() };
    }
}

Number InputValidator::promptNumber(const std::string& promptText, Number defaultValue) {
    while (true) {
        std::cout << std::format("{} [default: {}]: ", promptText, defaultValue);
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cout << "\nEnd-of-file (EOF) reached. Using default value: " << defaultValue << "\n";
            return defaultValue;
        }

        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            std::cout << std::format("-> Using default value: {}\n", defaultValue);
            return defaultValue;
        }

        Number result = 0;
        auto [ok, err] = validateAndParseNumber(line, result);
        if (ok) {
            return result;
        } else {
            std::cout << "ERROR: " << err << " Please try again.\n";
        }
    }
}

}
