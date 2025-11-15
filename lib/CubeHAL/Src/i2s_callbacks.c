#include "stm32g4xx_hal.h"
#include "cube_inits.h" // prototypes + extern hdma/hi2s declarations if you have

/* If you have a user LED on PA5 change pins accordingly */
#define DEBUG_LED_PORT GPIOA
#define DEBUG_LED_PIN  GPIO_PIN_5

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  (void)hi2s;
  /* minimal ISR-safe toggle */
  DEBUG_LED_PORT->ODR ^= DEBUG_LED_PIN;  // direct register toggle (very short)
  // if you prefer HAL call (not ideal in ISR, but many do):
  // HAL_GPIO_TogglePin(DEBUG_LED_PORT, DEBUG_LED_PIN);
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
  (void)hi2s;
  DEBUG_LED_PORT->ODR ^= DEBUG_LED_PIN;
}
