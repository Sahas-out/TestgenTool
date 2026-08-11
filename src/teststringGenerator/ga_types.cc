#include "teststringGenerator/ga_types.hh"

namespace ga {

Alphabet blockNames(const Spec& spec) {
    Alphabet names;
    names.reserve(spec.blocks.size());
    for (const auto& block : spec.blocks) {
        names.push_back(block->name);
    }
    return names;
}

}  // namespace ga
