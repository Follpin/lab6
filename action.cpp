#include "action.h"
#include <format>

namespace Game {

std::string actionTypeToString(ActionType type) {
    switch (type) {
        case ActionType::AddOne:
            return "+1";
        case ActionType::MultiplyTwo:
            return "*2";
        case ActionType::DivideThree:
            return "/3";
    }
    return "Unknown";
}

std::string targetToString(TargetNumber target) {
    switch (target) {
        case TargetNumber::A:
            return "number a";
        case TargetNumber::B:
            return "number b";
    }
    return "Unknown";
}

std::string Action::toString() const {
    return std::format("{} to {}", actionTypeToString(type), targetToString(target));
}

}
