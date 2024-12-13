#include "text_processor.hpp"
#include <algorithm>
#include <regex>
#include <sstream>

std::string TextProcessor::processText(const std::string& text) {
    // Remove markdown formatting
    std::string processed = text;
    
    // Remove headers
    processed = std::regex_replace(processed, std::regex(R"(^#+\s+)"), "");
    
    // Remove bold/italic
    processed = std::regex_replace(processed, std::regex(R"(\*\*|\*)"), "");
    processed = std::regex_replace(processed, std::regex(R"(__|\b_\b)"), "");
    
    // Remove links
    processed = std::regex_replace(processed, std::regex(R"(\[([^\]]+)\]\([^\)]+\))"), "$1");
    
    // Remove code blocks
    processed = std::regex_replace(processed, std::regex(R"(```[^`]*```)"), "");
    processed = std::regex_replace(processed, std::regex(R"(`[^`]*`)"), "");
    
    // Clean up whitespace
    processed = std::regex_replace(processed, std::regex(R"(\s+)"), " ");
    processed = std::regex_replace(processed, std::regex(R"(^\s+|\s+$)"), "");
    
    return processed;
}

std::vector<std::string> TextProcessor::splitIntoSegments(const std::string& text) {
    std::vector<std::string> segments;
    std::istringstream stream(text);
    std::string segment;
    std::string currentSegment;
    
    while (std::getline(stream, segment, '.')) {
        if (currentSegment.length() + segment.length() + 1 > MAX_SEGMENT_LENGTH) {
            if (!currentSegment.empty()) {
                segments.push_back(currentSegment);
            }
            currentSegment = segment + ".";
        } else {
            currentSegment += segment + ".";
        }
    }
    
    if (!currentSegment.empty()) {
        segments.push_back(currentSegment);
    }
    
    return segments;
}
