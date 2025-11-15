#ifndef CUBE_INITS_H
#define CUBE_INITS_H

#include "stm32g4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported handles (from Cube-generated init code) */
extern I2S_HandleTypeDef hi2s2;
extern DMA_HandleTypeDef hdma_spi2_rx;

/* Exported init functions */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_I2S2_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* CUBE_INITS_H */
