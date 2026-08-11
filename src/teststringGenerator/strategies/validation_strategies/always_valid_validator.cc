#include "teststringGenerator/strategies/validation_strategies/always_valid_validator.hh"

namespace ga {

bool AlwaysValidValidator::isValid(const TestString& sequence, const Spec& spec) const {
    (void)sequence;
    (void)spec;
    return true;
}

}  // namespace ga
