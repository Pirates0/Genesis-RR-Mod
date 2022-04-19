#include <intrin.h>
#include <overridable_fields.hpp>
#include <native.hpp>

using namespace native::RecRoom::Protobuf;

std::vector<unsigned char> overridable_fields::get_bytes(OverridableBoolData* value) {
    auto result = std::vector<unsigned char>();
    if(!value) return result;

    if(value->overrides) {
        result.insert(result.end(), {0x08, 0x01});
    }
    if(value->innervalue) {
        result.insert(result.end(), {0x10, 0x01});
    }

    return result;
}

std::vector<unsigned char> overridable_fields::get_bytes(OverridableFloatData* value) {
    auto result = std::vector<unsigned char>();
    if(!value) return result;

    if(value->overrides) {
        result.insert(result.end(), {0x08, 0x01});
    }
    if(value->innervalue) {
        auto ptr = (unsigned char*)&value->innervalue;
        result.insert(result.end(), {0x15, ptr[0], ptr[1], ptr[2], ptr[3]});
    }

    return result;
}

std::vector<unsigned char> overridable_fields::get_bytes(OverridableIntData* value) {
    auto result = std::vector<unsigned char>();
    if(!value) return result;

    if(value->overrides) {
        result.insert(result.end(), {0x08, 0x01});
    }
    if(value->innervalue) {
        result.push_back(0x10); //tag for integral type

        unsigned int innervalue = value->innervalue;
        //this code is apparently not used, so only unsigned ints are expected
        // if(innervalue < 0) {
        //     innervalue *= -1;
        //     if(innervalue % 2 == 0) innervalue++;
        // }
        // else if(innervalue > 0) {
        //     innervalue *= 2;
        // }

        //set the 'next byte' bit on the bytes which will be followed by another byte  
        while(innervalue > 0x7f) {
            result.push_back( ((unsigned char)(innervalue & 0x7f)) | 0x80);
            innervalue >>= 7;
        }
        //and the last byte does not have the 'next byte' bit
        result.push_back( ((unsigned char)(innervalue & 0x7f)) );
    }

    return result;
}