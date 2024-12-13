#include "coqui_engine.hpp"
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QCoreApplication>
#include <stdexcept>

namespace py = pybind11;

CoquiEngine::CoquiEngine() : initialized(false) {
    startPythonProcess();
}

CoquiEngine::~CoquiEngine() {
    if (pythonProcess) {
        // Send exit command
        QJsonObject cmd;
        cmd["command"] = "exit";
        QJsonDocument doc(cmd);
        sendCommand(doc.toJson().toStdString());
        
        pythonProcess->waitForFinished(1000);
        pythonProcess->close();
    }
}

bool CoquiEngine::initialize(const std::string& modelPath) {
    if (initialized) {
        return true;
    }

    currentModelPath = modelPath;
    
    // Create init command
    QJsonObject cmd;
    cmd["command"] = "init";
    cmd["model_path"] = QString::fromStdString(modelPath);
    
    QJsonDocument doc(cmd);
    if (!sendCommand(doc.toJson().toStdString())) {
        return false;
    }

    // Parse response
    auto response = readResponse();
    auto json = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
    if (json.object()["status"].toString() != "success") {
        qWarning() << "Failed to initialize TTS:" << json.object()["message"].toString();
        return false;
    }

    initialized = true;
    return true;
}

std::vector<float> CoquiEngine::synthesize(const std::string& text) {
    if (!initialized) {
        throw std::runtime_error("CoquiEngine not initialized");
    }

    // Create synthesis command
    QJsonObject cmd;
    cmd["command"] = "synthesize";
    cmd["text"] = QString::fromStdString(text);
    
    QJsonDocument doc(cmd);
    if (!sendCommand(doc.toJson().toStdString())) {
        qWarning() << "Failed to send synthesis command";
        return std::vector<float>();
    }

    // Parse response
    auto response = readResponse();
    auto json = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
    if (json.object()["status"].toString() != "success") {
        qWarning() << "Synthesis failed:" << json.object()["message"].toString();
        return std::vector<float>();
    }

    return parseAudioData(response);
}

void CoquiEngine::setVoice(const std::string& voice) {
    currentVoice = voice;
}

void CoquiEngine::setSpeed(float speed) {
    currentSpeed = std::max(0.5f, std::min(2.0f, speed));
}

void CoquiEngine::setPitch(float pitch) {
    currentPitch = std::max(0.5f, std::min(2.0f, pitch));
}

bool CoquiEngine::loadModel(const std::string& path) {
    unloadModel();
    return initialize(path);
}

void CoquiEngine::unloadModel() {
    if (initialized) {
        initialized = false;
    }
}

void CoquiEngine::initPythonProcess() {
    // No need to initialize Python process with QProcess
}

void CoquiEngine::terminatePythonProcess() {
    // No need to terminate Python process with QProcess
}

bool CoquiEngine::startPythonProcess() {
    pythonProcess = std::make_unique<QProcess>();
    pythonProcess->setProcessChannelMode(QProcess::MergedChannels);
    
    // First try to find tts_server.py in the install directory
    QString exePath = QCoreApplication::applicationDirPath();
    QStringList scriptPaths = {
        QDir(exePath).filePath("../src/tts_server.py"),
        QDir(exePath).filePath("src/tts_server.py"),
        QDir::current().filePath("src/tts_server.py")
    };
    
    QString scriptPath;
    for (const auto& path : scriptPaths) {
        if (QFile::exists(path)) {
            scriptPath = path;
            break;
        }
    }
    
    if (scriptPath.isEmpty()) {
        qWarning() << "Could not find tts_server.py in any of the search paths";
        return false;
    }
    
    // Start Python script
    qInfo() << "Starting TTS server:" << scriptPath;
    pythonProcess->start("python3", QStringList() << scriptPath);
    
    if (!pythonProcess->waitForStarted(5000)) {
        qWarning() << "Failed to start TTS server:" << pythonProcess->errorString();
        return false;
    }
    
    return true;
}

bool CoquiEngine::sendCommand(const std::string& command) {
    if (!pythonProcess) {
        qWarning() << "Python process not initialized";
        return false;
    }
    
    qint64 bytesWritten;
    if (!pythonProcess->write(QString::fromStdString(command).toUtf8() + "\n", bytesWritten)) {
        qWarning() << "Failed to write command to Python process";
        return false;
    }
    
    if (!pythonProcess->waitForBytesWritten(5000)) {
        qWarning() << "Timeout waiting for command to be written";
        return false;
    }
    
    return true;
}

std::string CoquiEngine::readResponse() {
    if (!pythonProcess) {
        qWarning() << "Python process not initialized";
        return "";
    }
    
    if (!pythonProcess->waitForReadyRead(5000)) {
        qWarning() << "Timeout waiting for response from Python process";
        return "";
    }
    
    return pythonProcess->readAll().toStdString();
}

std::vector<float> CoquiEngine::parseAudioData(const std::string& response) {
    auto json = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
    if (json.object()["status"].toString() != "success") {
        qWarning() << "Error in audio data response:" << json.object()["message"].toString();
        return std::vector<float>();
    }

    auto audioArray = json.object()["audio"].toArray();
    std::vector<float> audio;
    audio.reserve(audioArray.size());
    
    for (const auto& value : audioArray) {
        audio.push_back(value.toDouble());
    }
    
    return audio;
}
