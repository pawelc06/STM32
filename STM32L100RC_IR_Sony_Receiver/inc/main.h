/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-July-2013
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "stm32l100c_discovery.h"
#include "stdbool.h"
#include "../FatFs_0.10/src/ff.h"
#include "ir_decode.h"
#include "lcd_pcd8544_lib.h"
#include <stdint.h>

#define USE_PETIT_FAT 0

/* Private define ------------------------------------------------------------*/
#define DAC_DHR12R2_Address      0x40007414

/* 8-bit */
#define DAC_DHR8R1_Address       0x40007410

/* channel1 12-bit left aligned data holding register */
#define DAC_DHR12L1_Address       0x4000740C

/* DAC channel1 12-bit right-aligned data holding register */
#define DAC_DHR12R1_Address       0x40007408

/* -------------------------- DUAL registers ---------------------------------------- */

/*  Dual DAC 12-bit right-aligned data holding register */
#define DAC_DHR12RD_Address       0x40007420

/*  DUAL DAC 12-bit left aligned data holding register */
#define DAC_DHR12LD_Address       0x40007424

/*  DUAL DAC 8-bit right aligned data holding register */
#define DAC_DHR8RD_Address       0x40007428

#define STEREO 1
//#define SAMPLE_WIDTH_16 1
//#define WAV_FROM_INT_MEMORY 1
#define SAMPLE_BUFFER_SIZE 512

IR_Frame_TypeDef IR_FRAME;




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
static FATFS g_sFatFs; //obiekt FATFs



/* Exported types ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
