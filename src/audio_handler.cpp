#include "audio_handler.hpp"
#include <QFile>
#include <QAudioFormat>
#include <QtEndian>
#include <QMediaPlayer>

AudioHandler::AudioHandler() {
    // Get the default output device
    outputDevice = QMediaDevices::defaultAudioOutput();
    
    // Create audio output for the default device
    audioOutput = std::make_unique<QAudioOutput>(outputDevice);
    audioBuffer = std::make_unique<QBuffer>();
}

AudioHandler::~AudioHandler() = default;

bool AudioHandler::saveAudioData(const std::vector<float>& audioData, const std::string& outputPath) {
    QFile file(QString::fromStdString(outputPath));
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    // Convert float samples to 16-bit PCM
    QByteArray rawData;
    rawData.reserve(static_cast<int>(audioData.size() * sizeof(qint16)));
    
    for (float sample : audioData) {
        qint16 pcmSample = static_cast<qint16>(sample * 32767.0f);
        qint16 littleEndian = qToLittleEndian(pcmSample);
        rawData.append(reinterpret_cast<const char*>(&littleEndian), sizeof(qint16));
    }

    qint64 bytesWritten = file.write(rawData);
    file.close();

    return bytesWritten == rawData.size();
}

bool AudioHandler::playAudio(const std::vector<float>& audioData) {
    if (!audioBuffer->isOpen()) {
        audioBuffer->open(QIODevice::ReadWrite);
    }
    
    // Convert float samples to 16-bit PCM
    QByteArray rawData;
    rawData.reserve(static_cast<int>(audioData.size() * sizeof(qint16)));
    
    for (float sample : audioData) {
        qint16 pcmSample = static_cast<qint16>(sample * 32767.0f);
        qint16 littleEndian = qToLittleEndian(pcmSample);
        rawData.append(reinterpret_cast<const char*>(&littleEndian), sizeof(qint16));
    }

    audioBuffer->buffer() = rawData;
    audioBuffer->seek(0);

    // Create a media player to play the audio
    auto player = std::make_unique<QMediaPlayer>();
    player->setAudioOutput(audioOutput.get());
    player->setSourceDevice(audioBuffer.get(), QUrl("audio/pcm"));
    player->play();

    return true;
}
