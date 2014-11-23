/**
  ******************************************************************************
  * @file    stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-July-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "main.h"

volatile uint32_t mscounter;
volatile bool toggleFlag = false;
extern bool updated;
extern uint8_t mode; //0 - normal, 1 - hours, 2 - minutes, 3 seconds
extern uint16_t ssTogle;
volatile uint16_t sample;
volatile uint8_t i;
volatile uint8_t i2;
volatile uint8_t * wavPtr;
volatile uint8_t * wavPtrBegin;
volatile uint8_t timerproc_counter;

extern uint8_t numChannels;
extern uint16_t sampleRate;
extern uint8_t bitsPerSample;

FIL     plik;
UINT bytesToRead,bytesRead;

extern volatile bool canRead;

//uint8_t buffer[2][512];
#ifdef SAMPLE_WIDTH_16
	extern int16_t buffer[2][512];
#else
	#ifdef STEREO
		extern uint16_t buffer[2][512];
	#else
		extern uint8_t buffer[2][512];
	#endif
#endif

/** @addtogroup STM32L100C-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


void MsTick(void){
	//STM_EVAL_LEDToggle(LED3);
	mscounter++;
}

void SysTick_Handler(void)
{

	if(!((++timerproc_counter)%10)){
			//STM_EVAL_LEDToggle(LED3);
			disk_timerproc();
			timerproc_counter=0;

	}
	MsTick();
	TimingDelay_Decrement();
}

void DMA1_Channel2_IRQHandler(void){
	//DMA_InitTypeDef            DMA_InitStructure;

	if (DMA_GetITStatus(DMA1_IT_TC2) != RESET) {




		DMA_ClearITPendingBit(DMA1_IT_TC2);


		/*
		if (toggleFlag) {
			STM_EVAL_LEDOn(LED3);
		} else {
			STM_EVAL_LEDOff(LED3);
		}
		*/


		toggleFlag = !toggleFlag;

		//DMA1_Channel2->CMAR = (uint32_t)&buffer[0][(i++)%2];
		//DAC_DMACmd(DAC_Channel_1, DISABLE);
		//DAC_Cmd(DAC_Channel_1, DISABLE);

		/**********************************/
		DMA1_Channel2->CCR = 0x0;
		DMA1_Channel2->CNDTR = 0x200;

		//DMA1_Channel2->CNDTR = 0x400;


		    if(bitsPerSample == 16){
		    	/* 12 bit */
		    	DMA1_Channel2->CPAR = DAC_DHR12R1_Address;
		    }
		    else {
		    	/* 8- bit */
		    	if(numChannels == 1){ //mono
		    		DMA1_Channel2->CPAR = DAC_DHR8R1_Address;
		    	} else { //stereo 8 bit
		    		DMA1_Channel2->CPAR = DAC_DHR8RD_Address;
		    	}
		    }


		    //i = (i + 1)%2;



#ifdef WAV_FROM_INT_MEMORY
		DMA1_Channel2->CMAR = (uint32_t) wavPtr;
		wavPtr += SAMPLE_BUFFER_SIZE;
		if (wavPtr >= wavPtrBegin + 67392) {
			DAC_Cmd(DAC_Channel_1, DISABLE);
			DMA_Cmd(DMA1_Channel2, DISABLE);
			DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
		}
#else
		i ^= 0x01;
		DMA1_Channel2->CMAR = (uint32_t)&buffer[i][0];
#endif

		    //i ^= 0x01;

//#ifdef SAMPLE_WIDTH_16
		if(bitsPerSample == 16)
		    /* 12 bit */
		    DMA1_Channel2->CCR = 0x2593;
//#else
		else
		    /* 8-bit */
			if(numChannels == 1){
				DMA1_Channel2->CCR = 0x2093;
			} else { //stereo 2x8 bits = 16 bits
				DMA1_Channel2->CCR = 0x2593;
			}
//#endif
		/**********************************/

		    DMA1->IFCR = DMA1_IT_TC2;

		    canRead = true;



	}
}

void DMA1_Channel3_IRQHandler(void){

	if (DMA_GetITStatus(DMA1_IT_TC3) != RESET) {




		DMA_ClearITPendingBit(DMA1_IT_TC3);


		//STM_EVAL_LEDToggle(LED5);





		/**********************************/
		DMA1_Channel3->CCR = 0x0;

		/* number of channel data - 512 */
		DMA1_Channel3->CNDTR = 0x200;
		//DMA1_Channel3->CNDTR = 0x400;


		/* dual 8 bit */
		DMA1_Channel3->CPAR = DAC_DHR8RD_Address;


		i ^= 0x01;
		DMA1_Channel3->CMAR = (uint32_t)&buffer[i][0];




		    /* 2 x 8 bit stereo */
		    DMA1_Channel3->CCR = 0x2593;

		/**********************************/

		    DMA1->IFCR = DMA1_IT_TC3;

		    canRead = true;



	}


}



/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
