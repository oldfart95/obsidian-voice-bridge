"""Test suite for the TTS bridge."""

import os
import sys
import unittest
from pathlib import Path

import numpy as np
from scipy.io import wavfile

# Add project root to Python path
project_root = Path(__file__).parent.parent
sys.path.insert(0, str(project_root))

from test_config import (
    TEST_RESOURCES,
    TEST_TEXT,
    TEST_OUTPUT,
    TEST_MARKDOWN,
    COQUI_MODEL
)

try:
    import obsidian_tts_bridge
except ImportError:
    print("Error: obsidian_tts_bridge module not found.")
    print("Please build the C++ bridge first using CMake.")
    sys.exit(1)

class TestTTSBridge(unittest.TestCase):
    """Test cases for the TTS bridge."""
    
    @classmethod
    def setUpClass(cls):
        """Set up test environment."""
        # Create test resources
        TEST_RESOURCES.mkdir(parents=True, exist_ok=True)
        
        # Create test input file
        with open(TEST_TEXT, 'w') as f:
            f.write(TEST_MARKDOWN)
        
        # Initialize TTS bridge
        cls.bridge = obsidian_tts_bridge.TTSBridge()
        
        # Initialize with Coqui model
        success = cls.bridge.initialize(COQUI_MODEL)
        if not success:
            raise RuntimeError("Failed to initialize TTS bridge with model")
    
    def setUp(self):
        """Set up each test."""
        # Remove any existing test output
        if TEST_OUTPUT.exists():
            TEST_OUTPUT.unlink()
    
    def test_initialization(self):
        """Test that the bridge initializes properly."""
        self.assertTrue(self.bridge.is_initialized())
        self.assertFalse(self.bridge.is_processing())
    
    def test_text_to_speech(self):
        """Test basic text-to-speech conversion."""
        test_text = "This is a simple test of the text-to-speech system."
        
        # Convert text to speech
        success = self.bridge.text_to_speech(test_text, str(TEST_OUTPUT))
        self.assertTrue(success)
        
        # Verify output file exists and has content
        self.assertTrue(TEST_OUTPUT.exists())
        self.assertGreater(TEST_OUTPUT.stat().st_size, 0)
    
    def test_markdown_processing(self):
        """Test processing of Markdown text."""
        # Read test markdown file
        with open(TEST_TEXT, 'r') as f:
            markdown_text = f.read()
        
        # Convert to speech
        success = self.bridge.text_to_speech(markdown_text, str(TEST_OUTPUT))
        self.assertTrue(success)
        
        # Verify output
        self.assertTrue(TEST_OUTPUT.exists())
        self.assertGreater(TEST_OUTPUT.stat().st_size, 0)
    
    def test_voice_settings(self):
        """Test voice configuration."""
        test_text = "Testing voice configuration."
        
        # Test different speeds
        self.bridge.set_speed(1.5)
        success1 = self.bridge.text_to_speech(test_text, str(TEST_OUTPUT))
        size1 = TEST_OUTPUT.stat().st_size
        
        self.bridge.set_speed(0.8)
        success2 = self.bridge.text_to_speech(test_text, str(TEST_OUTPUT))
        size2 = TEST_OUTPUT.stat().st_size
        
        self.assertTrue(success1 and success2)
        self.assertNotEqual(size1, size2)
    
    def test_batch_processing(self):
        """Test batch processing of multiple files."""
        # Create multiple test files
        test_files = []
        for i in range(3):
            test_file = TEST_RESOURCES / f"test_{i}.md"
            with open(test_file, 'w') as f:
                f.write(f"Test file {i}\n\nThis is test content.")
            test_files.append(str(test_file))
        
        # Process batch
        output_dir = str(TEST_RESOURCES / "batch_output")
        success = self.bridge.process_batch(test_files, output_dir)
        
        # Verify outputs
        self.assertTrue(success)
        for i in range(3):
            output_file = Path(output_dir) / f"test_{i}.mp3"
            self.assertTrue(output_file.exists())
            self.assertGreater(output_file.stat().st_size, 0)
    
    def test_error_handling(self):
        """Test error handling."""
        # Test with empty text
        with self.assertRaises(Exception):
            self.bridge.text_to_speech("", str(TEST_OUTPUT))
        
        # Test with invalid output path
        with self.assertRaises(Exception):
            self.bridge.text_to_speech("Test text", "/invalid/path/output.mp3")
    
    @classmethod
    def tearDownClass(cls):
        """Clean up test environment."""
        # Clean up test files
        if TEST_RESOURCES.exists():
            for file in TEST_RESOURCES.glob("*"):
                try:
                    file.unlink()
                except:
                    pass
            TEST_RESOURCES.rmdir()

if __name__ == '__main__':
    unittest.main()
