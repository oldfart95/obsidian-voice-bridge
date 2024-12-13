#!/usr/bin/env python3

import json
import sys
import traceback
from TTS.utils.manage import ModelManager
from TTS.utils.synthesizer import Synthesizer
import numpy as np

class TTSServer:
    def __init__(self):
        self.synthesizer = None
        self.model_path = None

    def initialize(self, model_path):
        try:
            # Initialize the model manager
            manager = ModelManager()

            # If model_path is not provided, use the default model
            if not model_path:
                model_path = manager.download_model("tts_models/en/ljspeech/tacotron2-DDC")

            # Initialize the synthesizer
            self.synthesizer = Synthesizer(
                model_path=model_path,
                config_path=None,  # Let TTS find the config
                speakers_file_path=None,
                language_ids_file_path=None,
                vocoder_path=None,
                vocoder_config_path=None,
                encoder_path=None,
                encoder_config_path=None,
                use_cuda=False
            )
            self.model_path = model_path
            return {"status": "success", "message": f"Model initialized from {model_path}"}
        except Exception as e:
            return {"status": "error", "message": f"Failed to initialize model: {str(e)}\n{traceback.format_exc()}"}

    def synthesize(self, text):
        try:
            if not self.synthesizer:
                return {"status": "error", "message": "Synthesizer not initialized"}

            # Generate speech
            wav = self.synthesizer.tts(text)
            
            # Convert to list for JSON serialization
            wav_list = wav.tolist()
            
            return {"status": "success", "audio": wav_list}
        except Exception as e:
            return {"status": "error", "message": f"Synthesis failed: {str(e)}\n{traceback.format_exc()}"}

def main():
    server = TTSServer()
    
    while True:
        try:
            # Read command from stdin
            line = sys.stdin.readline()
            if not line:
                break
                
            # Parse command
            try:
                cmd = json.loads(line)
            except json.JSONDecodeError as e:
                response = {"status": "error", "message": f"Invalid JSON: {str(e)}"}
                print(json.dumps(response))
                continue
            
            # Process command
            if cmd["command"] == "init":
                response = server.initialize(cmd.get("model_path"))
            elif cmd["command"] == "synthesize":
                response = server.synthesize(cmd["text"])
            elif cmd["command"] == "exit":
                break
            else:
                response = {"status": "error", "message": f"Unknown command: {cmd['command']}"}
            
            # Send response
            print(json.dumps(response))
            sys.stdout.flush()
            
        except Exception as e:
            response = {"status": "error", "message": f"Server error: {str(e)}\n{traceback.format_exc()}"}
            print(json.dumps(response))
            sys.stdout.flush()

if __name__ == "__main__":
    main()
