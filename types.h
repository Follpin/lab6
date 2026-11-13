#pragma once
#include <cstdint>
#include <string>

namespace Game {

using Number = uint64_t;
using PlayerId = int;

enum class ActionType {
    AddOne = 1,
    MultiplyTwo = 2,
    DivideThree = 3
};

enum class TargetNumber {
    A = 0,
    B = 1
};

std::string actionTypeToString(ActionType type);
std::string targetToString(TargetNumber target);

}
