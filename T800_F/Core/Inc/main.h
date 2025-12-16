/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void OLED_Display(void);
void KEY_Scan(void);
/* USER CODE END EFP */

/* USER CODE BEGIN PV */
extern uint8_t page;
extern uint8_t num;
extern uint8_t last_num;
extern uint8_t auto_count_enabled;
extern uint8_t last_auto_enabled;
extern uint8_t auto_num;
extern uint8_t last_auto_num;
extern uint8_t auto_seconds;
extern uint8_t last_auto_seconds;
extern uint8_t auto_count_start_time;
extern uint8_t display_initialized[3];

void SystemClock_Config(void);
void KEY_Scan(void);
void OLED_Display(void);
void OLED_Display_Page1(void);
void OLED_Display_Page2(void);
void OLED_Display_Page3(void);
void OLED_ClearArea(uint8_t Line, uint8_t Column, uint8_t length);
void OLED_ShowNumWithClear(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
/* USER CODE END PV */


/* Private defines -----------------------------------------------------------*/
#define led_test_Pin GPIO_PIN_13
#define led_test_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_2
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_3
#define KEY2_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_5
#define KEY4_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOB
#define sound_datainput_Pin GPIO_PIN_12
#define sound_datainput_GPIO_Port GPIOG
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
