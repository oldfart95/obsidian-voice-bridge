#include "tts_bridge.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <string>

namespace py = pybind11;

TTSBridge::TTSBridge() {
    textProcessor = std::make_unique<TextProcessor>();
    audioHandler = std::make_unique<AudioHandler>();
    coquiEngine = std::make_unique<CoquiEngine>();
}

TTSBridge::~TTSBridge() = default;

bool TTSBridge::initialize(const std::string& modelPath) {
    return coquiEngine->initialize(modelPath);
}

bool TTSBridge::textToSpeech(const std::string& text, const std::string& outputPath) {
    try {
        // Clean and process the text
        auto processedText = textProcessor->processText(text);
        
        // Split text into segments if needed
        auto textSegments = textProcessor->splitIntoSegments(processedText);
        
        // Convert each segment to speech
        std::vector<float> audioData;
        for (const auto& segment : textSegments) {
            auto segmentAudio = coquiEngine->synthesize(segment);
            audioData.insert(audioData.end(), segmentAudio.begin(), segmentAudio.end());
        }
        
        // Save the audio to file
        return audioHandler->saveAudioData(audioData, outputPath);
        
    } catch (const std::exception& e) {
        // Log error and return false
        return false;
    }
    
    return true;
}

bool TTSBridge::playText(const std::string& text) {
    try {
        // Clean and process the text
        auto processedText = textProcessor->processText(text);
        
        // Convert text to speech
        auto audioData = coquiEngine->synthesize(processedText);
        
        // Play the audio
        return audioHandler->playAudio(audioData);
        
    } catch (const std::exception& e) {
        // Log error and return false
        return false;
    }
}

// Python bindings
PYBIND11_MODULE(obsidian_tts_bridge, m) {
    py::class_<TTSBridge>(m, "TTSBridge")
        .def(py::init<>())
        .def("initialize", &TTSBridge::initialize)
        .def("text_to_speech", &TTSBridge::textToSpeech)
        .def("play_text", &TTSBridge::playText);
}
