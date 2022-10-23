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


#ifndef __WAV_PWM_AUDIO_H
#define __WAV_PWM_AUDIO_H


#define WAV_SAMPLE_RATE          22050
#define WAV_PWM_COUNT            (125000000 / WAV_SAMPLE_RATE)


void WavPwmInit(unsigned char GpioPinChannelA);
unsigned char WavPwmIsPlaying();
void WavPwmStopAudio();
unsigned char WavPwmPlayAudio(const unsigned short WavPwmData[]);


#endif

