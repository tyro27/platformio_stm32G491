#ifndef STM32_I2S_PINS_HEADER
#define STM32_I2S_PINS_HEADER

// Tell the library that pin mappings are provided
#define STM_I2S_PINS

// I2S2 pin mapping (adjust if you later use different peripheral)
// These macro names follow the library's expected naming convention used in its stm32-config-i2s.h
#define I2S2_WS_PORT GPIOB
#define I2S2_WS_PIN  GPIO_PIN_12

#define I2S2_CK_PORT GPIOB
#define I2S2_CK_PIN  GPIO_PIN_13

#define I2S2_SD_PORT GPIOB
#define I2S2_SD_PIN  GPIO_PIN_15

#endif // STM32_I2S_PINS_HEADER