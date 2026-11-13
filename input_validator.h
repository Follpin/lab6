#pragma once
#include "types.h"
#include <string>
#include <optional>
#include <utility>

namespace Game {

class InputValidator {
public:
    static std::pair<bool, std::string> validateAndParseNumber(const std::string& input, Number& outNumber);
    static Number promptNumber(const std::string& promptText, Number defaultValue);
};

}
