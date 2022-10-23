// PicoRecPlayAudio - Raspberry Pi Pico Audio Record/Playbak/WAV File
// Copyright (C) 2022 Jason Birch
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef __PICO_REC_PLAY_AUDIO_H
#define __PICO_REC_PLAY_AUDIO_H


#include "WavPwmAudio.h"


#define BUFF_SIZE                      128

// Audio buffer 2 channels for 3 seconds @ 22050 Hz Samples/Second.
#define AUDIO_CHANNELS                 2
#define AUDIO_PERIOD                   3
#define AUDIO_BUFF_SIZE                (AUDIO_CHANNELS * WAV_SAMPLE_RATE * AUDIO_PERIOD)

#define APP_VERSION                    "PicoRecPlayAudio V1.00 (C) 2022-10-21 Jason Birch"
#define APP_TERM80_INFO                "Audio Recording & Playback"

#define ADC_PORT_AUDIO_IN_LEFT         0
#define ADC_PORT_AUDIO_IN_RIGHT        1

#define GPIO_KEY_RECORD                7
#define GPIO_KEY_PLAY                  11
#define GPIO_KEY_DUMP                  15
#define GPIO_AUDIO_OUT_LEFT            16
#define GPIO_AUDIO_OUT_RIGHT           17
#define GPIO_ADC_AUDIO_IN_LEFT         26
#define GPIO_ADC_AUDIO_IN_RIGHT        27

  /**************************/
 /* WAV file data offsets. */
/**************************/
#define WAV_ARG_CHUNK_ID         0  // "RIFF"
#define WAV_ARG_CHUNK_SIZE       4  // FileSize - 8
#define WAV_ARG_FORMAT           8  // "WAVE"
#define WAV_ARG_SUB_CHUNK_ID     12 // "fmt "
#define WAV_ARG_SUB_CHUNK_SIZE   16 // 0x00000010
#define WAV_ARG_AUDIO_FORMAT     20 // 0x0001
#define WAV_ARG_NUM_CHANNELS     22 // 2
#define WAV_ARG_SAMPLE_RATE      24 // 22050
#define WAV_ARG_BYTE_RATE        28 // 44100
#define WAV_ARG_BLOCK_ALIGN      32 // 0x0002
#define WAV_ARG_BITS_PER_SAMPLE  34 // 16
#define WAV_ARG_SUB_CHUNK2_ID    36 // "data"
#define WAV_ARG_SUB_CHUNK2_SIZE  40 // FileSize - 44
#define WAV_ARG_DATA             44 // 16 bit Sample Data (Left/Right)

typedef struct
{
   unsigned char ChunkID[4];
   unsigned int ChunkSize;
   unsigned char Format[4];
   unsigned char SubChunkID[4];
   unsigned int SubChunkSize;
   unsigned short AudioFormat;
   unsigned short Channels;
   unsigned int SampleRate;
   unsigned int ByteRate;
   unsigned short BlockAlign;
   unsigned short BitsPerSample;
   unsigned char SubChunk2ID[4];
   unsigned int SubChunk2Size;
} WavHeaderType;

static WavHeaderType WavHeader =
{
   "RIFF",                                // WAV_ARG_CHUNK_ID
   sizeof(short)*(AUDIO_BUFF_SIZE-2) + 36,// WAV_ARG_CHUNK_SIZE
   "WAVE",                                // WAV_ARG_FORMAT
   "fmt ",                                // WAV_ARG_SUB_CHUNK_ID
   0x00000010,                            // WAV_ARG_SUB_CHUNK_SIZE
   0x0001,                                // WAV_ARG_AUDIO_FORMAT
   2,                                     // WAV_ARG_NUM_CHANNELS
   22050,                                 // WAV_ARG_SAMPLE_RATE
   44100,                                 // WAV_ARG_BYTE_RATE
   0x0002,                                // WAV_ARG_BLOCK_ALIGN
   16,                                    // WAV_ARG_BITS_PER_SAMPLE
   "data",                                // WAV_ARG_SUB_CHUNK2_ID
   sizeof(short)*(AUDIO_BUFF_SIZE-2),     // WAV_ARG_SUB_CHUNK2_SIZE
};


void Init();
void GPIO_CallBack(uint GPIO, uint32_t Events);
void AudioCapture(unsigned short AudioBuffer[]);
void DumpWavFile(unsigned short AudioBuffer[]);


#endif
