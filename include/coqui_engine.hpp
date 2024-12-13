#pragma once

#include <string>
#include <vector>
#include <memory>

class QProcess;

class CoquiEngine {
public:
    CoquiEngine();
    ~CoquiEngine();

    bool initialize(const std::string& modelPath);
    std::vector<float> synthesize(const std::string& text);
    bool isInitialized() const { return initialized; }

private:
    bool startPythonProcess();
    bool sendCommand(const std::string& command);
    std::string readResponse();
    std::vector<float> parseAudioData(const std::string& response);

    std::unique_ptr<QProcess> pythonProcess;
    bool initialized = false;
    std::string currentModelPath;
};
