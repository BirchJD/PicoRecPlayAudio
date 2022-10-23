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


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/irq.h"
#include "PicoRecPlayAudio.h"


static unsigned char GpioKey = false;


int main(int argc, char* argv[])
{
   unsigned short AudioBuffer[AUDIO_BUFF_SIZE];

   Init();
  /***************************************/
 /* Ensure buffer size data is present. */
/***************************************/
   AudioBuffer[0] = (AUDIO_BUFF_SIZE & 0xFFFF);
   AudioBuffer[1] = (AUDIO_BUFF_SIZE >> 16);
   while (true)
   {
      sleep_ms(100);

  /*********************************/
 /* Display Pico LED when active. */
/*********************************/
      gpio_put(PICO_DEFAULT_LED_PIN, (GpioKey != false));

      switch (GpioKey)
      {
         case GPIO_KEY_RECORD:
            AudioCapture(AudioBuffer);
            GpioKey = false;
            break;
         case GPIO_KEY_PLAY:
  /*********************************************/
 /* Play audio from audio buffer to PWM pins. */
/*********************************************/
            WavPwmPlayAudio(AudioBuffer);
            while (WavPwmIsPlaying());
            GpioKey = false;
            break;
         case GPIO_KEY_DUMP:
   /********************************************/
  /* Send HEX data for a WAV file format from */
 /* the audio buffer to the debug output.    */
/********************************************/
            DumpWavFile(AudioBuffer);
            GpioKey = false;
            break;
      }
   };
}



void Init()
{
   stdio_init_all();

  /********************************/ 
 /* Configure Pico on board LED. */
/********************************/ 
   gpio_init(PICO_DEFAULT_LED_PIN);
   gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  /******************************/
 /* Configure Key Switch Pins. */
/******************************/
   gpio_pull_up(GPIO_KEY_RECORD);
   gpio_set_irq_enabled_with_callback(GPIO_KEY_RECORD, GPIO_IRQ_EDGE_FALL, true, &GPIO_CallBack);
   gpio_pull_up(GPIO_KEY_PLAY);
   gpio_set_irq_enabled_with_callback(GPIO_KEY_PLAY, GPIO_IRQ_EDGE_FALL, true, &GPIO_CallBack);
   gpio_pull_up(GPIO_KEY_DUMP);
   gpio_set_irq_enabled_with_callback(GPIO_KEY_DUMP, GPIO_IRQ_EDGE_FALL, true, &GPIO_CallBack);

  /******************/
 /* Configure ADC. */
/******************/
   adc_init();
   adc_gpio_init(GPIO_ADC_AUDIO_IN_LEFT);
   adc_gpio_init(GPIO_ADC_AUDIO_IN_RIGHT);

  /************************/
 /* Configure Audio PWM. */
/************************/
   WavPwmInit(GPIO_AUDIO_OUT_LEFT);
}



void GPIO_CallBack(uint GPIO, uint32_t Events)
{
   static unsigned int LastInt = 0;

  /**************************/
 /* Debounce button press. */
/**************************/
   if (!LastInt || time_us_32() - LastInt > 200000)
   {
      GpioKey = GPIO;
  /*********************************/
 /* Display Pico LED when active. */
/*********************************/
      gpio_put(PICO_DEFAULT_LED_PIN, 1);
   }
   LastInt = time_us_32();
}



void AudioCapture(unsigned short AudioBuffer[])
{
   unsigned long SampleCount = 0;
   unsigned long long SamplePeriod;

  /*********************************************/
 /* First two 16bit values are the data size. */
/*********************************************/
   SampleCount = 0;
   AudioBuffer[SampleCount++] = (AUDIO_BUFF_SIZE & 0xFFFF);
   AudioBuffer[SampleCount++] = (AUDIO_BUFF_SIZE >> 16);
  /**************************************************************/
 /* Fill audio buffer with values read from the A/D converter. */
/**************************************************************/
   SamplePeriod = time_us_64() + (1000000 / WAV_SAMPLE_RATE);
   while (SampleCount < AUDIO_BUFF_SIZE)
   {
  /***************************/
 /* Read values @ 22050 Hz. */
/***************************/
      while(time_us_64() < SamplePeriod);
      SamplePeriod = time_us_64() + (1000000 / WAV_SAMPLE_RATE);
  /****************************************************/
 /* Read the current audio level from A/D converter. */
/****************************************************/
      adc_select_input(ADC_PORT_AUDIO_IN_LEFT);
      AudioBuffer[SampleCount++] = WAV_PWM_COUNT * adc_read() / 4096;
      adc_select_input(ADC_PORT_AUDIO_IN_RIGHT);
      AudioBuffer[SampleCount++] = WAV_PWM_COUNT * adc_read() / 4096;
   };
}



void DumpWavFile(unsigned short AudioBuffer[])
{
   unsigned long Count;
   short Value;
   
   printf("\r\n");
   printf("----- WAV DUMP START -----\r\n");
  /*********************************/
 /* Write WAV file header record. */
/*********************************/
   for (Count = 0; Count < sizeof(WavHeader); ++Count)
   {
      if (!((Count + 1) % 32))
         printf("\r\n");
      printf("%02X", ((unsigned char*)&WavHeader)[Count]);
   }
   printf("\r\n");
  /**************************************/
 /* First two words are the data size. */
/**************************************/
   for (Count = 2; Count < AUDIO_BUFF_SIZE; ++Count)
   {
      if (!((Count + 1) % 16))
         printf("\r\n");
      Value = AudioBuffer[Count];
      Value = ((131072 * Value) / WAV_PWM_COUNT) - 32768;
      printf("%02hX%02hX", (Value & 0xFF), ((Value >> 8) & 0xFF));
   }
   printf("\r\n");
   printf("------ WAV DUMP END ------\r\n");
}
