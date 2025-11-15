// #include "arduino.h"
// #include "Clock_Config.h"
// src/main.cpp (top of file)
#include <Arduino.h>
#include <math.h>

// display libs (you already use these; keep them if used below)
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>


extern "C" {
  /* Cube wrapper: declares hi2s2, extern hdma_spi2_rx (if dma.c defines it),
     and the MX_* functions (SystemClock_Config, MX_GPIO_Init, MX_DMA_Init, MX_I2S2_Init) */
  // #include <stm32g4xx_hal_msp.c>
  #include "cube_inits.h"

  /* HAL core header */
  #include "stm32g4xx_hal.h"

  /* Optional: only include if you copied their sources into lib/CubeHAL */
  #include "stm32g4xx_it.h"
  #include "stm32g4xx_hal_msp.c"
}

// NOTE: ensure this matches the buffer size you used in Cube (samples, 16-bit)
#define AUDIO_BUF_SAMPLES 512
static int16_t audio_buf[AUDIO_BUF_SAMPLES];

extern I2S_HandleTypeDef hi2s2; // declared in i2s.h

volatile uint8_t audio_half_ready = 0;
volatile uint32_t audio_buf_count = 0;

extern "C" void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  (void)hi2s;
  audio_half_ready = 1;
  audio_buf_count++;
}

extern "C" void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
  (void)hi2s;
  audio_half_ready = 2;
  audio_buf_count++;
}

void setup() {
  Serial.begin(115200);
  while(!Serial) { delay(1); } // optional for native USB/CDC

  HAL_Init();
  SystemClock_Config();  // from Cube-generated system file
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2S2_Init();

  // If Cube generated UART init and you copied it, init it here (optional)
  // MX_USART2_UART_Init(); // only if you copied and need it

  delay(50);
  Serial.println("I2S DMA (Arduino) started");

  // Start circular DMA receive (AUDIO_BUF_SAMPLES 16-bit samples)
  if (HAL_I2S_Receive_DMA(&hi2s2, (uint16_t*)audio_buf, AUDIO_BUF_SAMPLES) != HAL_OK) {
    Serial.println("HAL_I2S_Receive_DMA failed");
    while (1) { delay(1000); } // stop here for debug
  }
}

extern "C" {
  #include "cube_inits.h"
}

void print_dma_status() {
  Serial.print("hi2s2.State=");
  Serial.print(HAL_I2S_GetState(&hi2s2));
  Serial.print(" hdma.Instance=");
  Serial.print((uint32_t)hdma_spi2_rx.Instance, HEX);
  Serial.print(" hdma.State=");
  Serial.print((int)hdma_spi2_rx.State);
#if defined(DMA1_Channel1)
  Serial.print(" CNDTR=");
  // Accessing CNDTR is platform specific; try this for G4:
  Serial.println(((DMA_Channel_TypeDef*)hdma_spi2_rx.Instance)->CNDTR);
#else
  Serial.println();
#endif
}

void loop() {
  static uint32_t printCounter = 0;

  if (audio_half_ready != 0) {
    uint8_t half = audio_half_ready;
    audio_half_ready = 0;

    int start = (half == 1) ? 0 : (AUDIO_BUF_SAMPLES / 2);
    int len = AUDIO_BUF_SAMPLES / 2;

    uint64_t acc = 0;
    for (int i = 0; i < len; ++i) {
      int16_t v = audio_buf[start + i];
      int32_t s = (int32_t)v;
      acc += (uint64_t)(s * s);
    }
    uint32_t rms = (len) ? (uint32_t)sqrt((double)acc / (double)len) : 0;

    printCounter++;
    if (printCounter >= 10) {
      printCounter = 0;
      Serial.print("buf#");
      Serial.print(audio_buf_count);
      Serial.print(" half=");
      Serial.print(half);
      Serial.print(" RMS=");
      Serial.println(rms);
    }
  }

  delay(10);
}
