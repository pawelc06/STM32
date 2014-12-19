/**
  ******************************************************************************
  * @file    stm32l100c_discovery.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-July-2013
  * @brief   This file contains definitions for STM32L100C-Discovery's Leds, push-
  *          buttons hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L100C_DISCOVERY_H
#define __STM32L100C_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32L100C_DISCOVERY
  * @{
  */
      
/** @addtogroup STM32L100C_DISCOVERY_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32L100C_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4,
  LED6 = 5,
  B7 = 6,
  B8 = 7,
  B9 = 8,
  B10 = 9,
  B11 = 10,
  B12 = 11,
  D13 = 12


} Led_TypeDef;

typedef enum 
{
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;
  
/**
  * @}
  */ 

/** @defgroup STM32L100C_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** @addtogroup STM32L100C_DISCOVERY_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             13

#define LED1_PIN                         GPIO_Pin_1
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOA
  
#define LED2_PIN                         GPIO_Pin_2
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED3_PIN                         GPIO_Pin_3
#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED4_PIN                         GPIO_Pin_8
#define LED4_GPIO_PORT                   GPIOA
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED5_PIN                         GPIO_Pin_9
#define LED5_GPIO_PORT                   GPIOA
#define LED5_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED6_PIN                         GPIO_Pin_10
#define LED6_GPIO_PORT                   GPIOA
#define LED6_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define B7_PIN                         GPIO_Pin_11
#define B7_GPIO_PORT                   GPIOA
#define B7_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define B8_PIN                         GPIO_Pin_12
#define B8_GPIO_PORT                   GPIOA
#define B8_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define B9_PIN                         GPIO_Pin_13
#define B9_GPIO_PORT                   GPIOA
#define B9_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define B10_PIN                         GPIO_Pin_14
#define B10_GPIO_PORT                   GPIOA
#define B10_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define B11_PIN                         GPIO_Pin_15
#define B11_GPIO_PORT                   GPIOA
#define B11_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define B12_PIN                         GPIO_Pin_10
#define B12_GPIO_PORT                   GPIOC
#define B12_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define D13_PIN                         GPIO_Pin_11  //controlling ATTiny85/led band
#define D13_GPIO_PORT                   GPIOC
#define D13_GPIO_CLK                    RCC_AHBPeriph_GPIOC

/**
  * @}
  */ 

/** @addtogroup SSTM32L100C_DISCOVERY_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          1

/**
 * @brief USER push-button
 */
#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn 

/**
  * @}
  */ 
  
    
/** @defgroup STM32L100C_DISCOVERY_LOW_LEVEL_Exported_Macros
  * @{
  */  
/**
  * @}
  */ 

/** @defgroup STM32L100C_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L100C_DISCOVERY_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
