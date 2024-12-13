"""Test configuration for the TTS bridge."""

import os
from pathlib import Path

# Project root directory
PROJECT_ROOT = Path(__file__).parent.parent

# Test resources directory
TEST_RESOURCES = PROJECT_ROOT / "tests" / "resources"

# Create resources directory if it doesn't exist
TEST_RESOURCES.mkdir(parents=True, exist_ok=True)

# Test files
TEST_TEXT = TEST_RESOURCES / "test_input.md"
TEST_OUTPUT = TEST_RESOURCES / "test_output.mp3"

# Model configuration
COQUI_MODEL = "tts_models/en/ljspeech/tacotron2-DDC"  # Default English model

# Test text content
TEST_MARKDOWN = """# Test Document

## Section 1
This is a test of the TTS bridge. It should handle **bold text** and *italic text* properly.

## Section 2
- List item 1
- List item 2

### Subsection
[This link](https://example.com) should be read without the URL.

> This is a blockquote that should be read normally.

## Section 3
1. Numbered item
2. Another numbered item

Final paragraph with some `code` that should be cleaned up."""
