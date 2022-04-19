#pragma once

#include <vector>

namespace native {
    namespace RecRoom {
        namespace Protobuf {
            struct OverridableBoolData;
            struct OverridableFloatData;
            struct OverridableIntData;
        }
    }
}

namespace overridable_fields {
    using namespace native::RecRoom::Protobuf;
    std::vector<unsigned char> get_bytes(OverridableBoolData* value);
    std::vector<unsigned char> get_bytes(OverridableFloatData* value);
    std::vector<unsigned char> get_bytes(OverridableIntData* value);
}