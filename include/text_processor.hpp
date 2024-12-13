#pragma once

#include <string>
#include <vector>

class TextProcessor {
public:
    TextProcessor() = default;
    ~TextProcessor() = default;

    // Process text (clean and normalize)
    std::string processText(const std::string& text);

    // Split text into manageable segments
    std::vector<std::string> splitIntoSegments(const std::string& text);

private:
    // Constants for text processing
    static constexpr size_t MAX_SEGMENT_LENGTH = 1000;
};
