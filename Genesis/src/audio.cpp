#define NOMINMAX

#include <fstream>
#include <vector>
#include <Windows.h>
#include <thread>
#include <limits>
#include <native.hpp>
#include <audio.hpp>

native::UnityEngine::AudioClip* audio::clip_from_wav(const std::string &file_path, float &sound_duration) {
    using namespace native::UnityEngine;

    auto audio_file = std::ifstream(file_path, std::ios::binary);
    if(!audio_file.is_open()) return nullptr;

    audio_file.seekg(0);

    uint32_t header;
    audio_file.read((char*)&header, sizeof(header));
    if(header != 0x46464952) return nullptr; //RIFF

    audio_file.seekg(8);

    audio_file.read((char*)&header, sizeof(header));
    if(header != 0x45564157) return nullptr; //WAVE

    audio_file.read((char*)&header, sizeof(header));
    if(header != 0x20746D66) return nullptr; //fmt 

    audio_file.read((char*)&header, sizeof(header));
    if(header != 0x10) return nullptr; //size of subchunk, 16 for PCM

    audio_file.read((char*)&header, sizeof(header));
    if((header & 0x0000FFFF) != 1) return nullptr; //first 2 bytes (seen in the file) are audio format (1 for PCM), other two are the number of channels
    int channel_count = header >> 16;

    int sample_rate;
    audio_file.read((char*)&sample_rate, sizeof(sample_rate));

    audio_file.seekg(34);
    unsigned short bits_per_sample;
    audio_file.read((char*)&bits_per_sample, sizeof(bits_per_sample));
    if(bits_per_sample % 8 != 0) return nullptr; //because it would mess with the way the samples will be converted to floats

    audio_file.read((char*)&header, sizeof(header));
    if(header != 0x61746164) return nullptr; //data

    audio_file.read((char*)&header, sizeof(header)); //contains the length of the data section

    int sample_count = header / (bits_per_sample / 8);

    sound_duration = (float)sample_count / sample_rate;

    auto clip = AudioClip::Create(il2cpp_string_new(XS("\"We are always one step ahead of our hackers.\" -Shawn 22/08/2019")), sample_count / channel_count, channel_count, sample_rate, false, nullptr);

    //convert the data in the file to float samples, storing the result in a native array
    auto sample_array = il2cpp_array_new(il2cpp_class_from_name(il2cpp_get_corlib(), XS("System"), XS("Single")), sample_count); //it is rare for the function to fail after this point

    if(bits_per_sample / 8 == 1) { //todo: template function
        int8_t current;
        auto destination = (float*)(sample_array->vector);
        int array_index = 0;
        for(uint32_t i = 0; i < header; i++) {
            audio_file.read((char*)&current, sizeof(current));
            destination[array_index++] = (float)current / std::numeric_limits<int8_t>::max();
        }
    }
    else if(bits_per_sample / 8 == 2) {
        int16_t current;
        auto destination = (float*)(sample_array->vector);
        int array_index = 0;
        for(uint32_t i = 0; i < header; i += 2) {
            audio_file.read((char*)&current, sizeof(current));
            destination[array_index++] = (float)current / std::numeric_limits<int16_t>::max();
        }
    }
    else if(bits_per_sample / 8 == 4) {
        int32_t current;
        auto destination = (float*)(sample_array->vector);
        int array_index = 0;
        for(uint32_t i = 0; i < header; i += 4) {
            audio_file.read((char*)&current, sizeof(current));
            destination[array_index++] = (float)current / std::numeric_limits<int32_t>::max();
        }
    }
    else return nullptr;

    AudioClip::SetData(clip, sample_array, 0, nullptr);

    return clip;
}