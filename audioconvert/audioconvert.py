#!/usr/bin/env python3
import argparse
import wave
import numpy as np
from pydub import AudioSegment

def convert_wav_to_raw_array(input_filename, output_filename):
    # Load and convert audio using pydub
    audio = AudioSegment.from_wav(input_filename)

    # Convert to mono, 44.1 kHz, 16-bit
    audio = audio.set_channels(1)
    audio = audio.set_frame_rate(44100)
    audio = audio.set_sample_width(2)

    # Normalize audio (to 0 dBFS)
    change_in_dBFS = 0.0 - audio.max_dBFS
    audio = audio.apply_gain(change_in_dBFS)

    # Get raw audio bytes
    raw_data = audio.raw_data
    byte_array = np.frombuffer(raw_data, dtype=np.uint8)

    # Write to C-style header file
    with open(output_filename, 'w') as f:
        f.write('const unsigned char out_raw[] __attribute__((aligned((2)))) = {\n')
        for i, byte in enumerate(byte_array):
            if i % 12 == 0:
                f.write('    ')
            f.write(f'0x{byte:02x}')
            if i != len(byte_array) - 1:
                f.write(', ')
            if (i + 1) % 12 == 0:
                f.write('\n')
        if len(byte_array) % 12 != 0:
            f.write('\n')
        f.write('};\n')

def main():
    parser = argparse.ArgumentParser(description='Convert and normalize WAV to C-style 16-bit mono 44.1kHz raw array.')
    parser.add_argument('input', help='Input WAV filename')
    parser.add_argument('-o', '--output', default='out.h', help='Output header filename (default: out.h)')

    args = parser.parse_args()
    convert_wav_to_raw_array(args.input, args.output)

if __name__ == '__main__':
    main()
