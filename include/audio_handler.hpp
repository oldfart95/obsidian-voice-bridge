#pragma once

#include <string>
#include <memory>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioDevice>
#include <QtMultimedia/QMediaDevices>
#include <QtCore/QBuffer>

class AudioHandler {
public:
    AudioHandler();
    ~AudioHandler();

    bool saveAudioData(const std::vector<float>& audioData, const std::string& outputPath);
    bool playAudio(const std::vector<float>& audioData);

private:
    std::unique_ptr<QAudioOutput> audioOutput;
    std::unique_ptr<QBuffer> audioBuffer;
    QAudioDevice outputDevice;
};
