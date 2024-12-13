#pragma once

#include "text_processor.hpp"
#include "audio_handler.hpp"
#include "coqui_engine.hpp"
#include <memory>
#include <string>
#include <vector>

class TTSBridge {
public:
    TTSBridge();
    ~TTSBridge();

    bool initialize(const std::string& modelPath);
    bool textToSpeech(const std::string& text, const std::string& outputPath);
    bool playText(const std::string& text);

private:
    std::unique_ptr<TextProcessor> textProcessor;
    std::unique_ptr<AudioHandler> audioHandler;
    std::unique_ptr<CoquiEngine> coquiEngine;
};
