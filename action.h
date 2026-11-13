#pragma once
#include "types.h"
#include <vector>
#include <optional>
#include <string>

namespace Game {

struct Action {
    ActionType type;
    TargetNumber target;

    bool operator==(const Action& other) const = default;
    std::string toString() const;
};

}
