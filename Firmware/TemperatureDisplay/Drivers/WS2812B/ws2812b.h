/*
 * WS2812 Led Driver

 Based on Phil's Lab Video: https://www.youtube.com/watch?v=MqbJTj0Cw6o
 Data Sheet: https://cdn.sparkfun.com/datasheets/BreakoutBoards/WS2812B.pdf
 */

#ifndef WS2812_RGB_LED_H
#define WS2812_RGB_LED_H

// Includes
#include "main.h"
// Include the hal library for your STM32 family
#include "stm32l4xx_hal_def.h"


// Defines
#define WS2812_NUM_LEDS 1
#define WS2812_TIM htim2
#define WS2812_TIM_CHANNEL TIM_CHANNEL_1

/* Set up ratios assuming 800 kHz PWM frequency and Data Transfer Time of 1.25 us */
#define WS2812_HI_VAL 64        // 0.8us / 1.25us * 100
#define WS2812_LOW_VAL 32       // 0.4us / 1.25us * 100
#define WS2812_RESET_PERIODS 40 // >50us / 1.25us
#define WS2812_LED_BITS 24      // G7 ... G0 R7 ... R0 B7 ... B0
#define WS2812_DMA_BUFF_LEN ((WS2812_NUM_LEDS * WS2812_LED_BITS) + WS2812_RESET_PERIODS)

// May need to change based on color order in datasheet 
typedef union
{
  struct
  {
    uint8_t b;
    uint8_t r;
    uint8_t g;
  } color;
  uint32_t rgb_data;
} ws2812Colors_t;

// Variables
extern TIM_HandleTypeDef WS2812_TIM;
extern ws2812Colors_t ws2812_color_data[WS2812_NUM_LEDS];
extern uint8_t ws2812_dma_buffer[WS2812_DMA_BUFF_LEN];
extern volatile uint8_t ws2812_dma_complete_flag;

// Functions

HAL_StatusTypeDef WS2812_Init(void);
HAL_StatusTypeDef WS2812_Update(void);
void WS2812_SetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void WS2812_Callback(void);

#endif /* WS2812_RGB_LED_H */