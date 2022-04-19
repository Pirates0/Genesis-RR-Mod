#pragma once
#include <string>

namespace native {namespace UnityEngine {struct AudioClip;}}

namespace audio {
    native::UnityEngine::AudioClip* clip_from_wav(const std::string &file_path, float &sound_duration);
}