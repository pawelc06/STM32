/*
 * waveplayer.c
 *
 *  Created on: 18 lip 2014
 *      Author: Paweł
 */

//#include "main.h"
#include "waveplayer.h"
#include "string.h"
//#include "clock.h"

extern bool updated;
extern uint8_t * wavPtr;
extern uint8_t * wavPtrBegin;

volatile uint8_t numChannels;
volatile uint16_t sampleRate;
volatile uint8_t bitsPerSample;

#ifdef SAMPLE_WIDTH_16
	int16_t buffer[2][512];
#else
	#ifdef STEREO
		uint16_t buffer[2][512];
	#else
		uint8_t buffer[2][512];
	#endif
#endif

extern FIL plik;
//extern UINT bytesToRead, bytesRead;
volatile bool canRead;
extern uint8_t i;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
DAC_InitTypeDef DAC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

/**
 * @brief  PrecConfiguration: configure PA4 and PA5 in analog,
 *                           enable DAC clock, enable DMA1 clock
 * @param  None
 * @retval None
 */
void DAC_PreConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* DMA1 clock enable (to be used with DAC) */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* GPIOA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Configure PA.04 (DAC_OUT1), PA.05 (DAC_OUT2) as analog */
	//GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	if(numChannels == 1){
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	}	else {
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	}

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void convertBufferTo12bit(int16_t * buffer) {
	uint16_t i;
	for (i = 0; i < SAMPLE_BUFFER_SIZE; i++) {
		buffer[i] = (buffer[i] + 32768) >> 4;
	}
}


void playStereoSine12b(){

	const uint16_t Sine12bit2[32] = {
	                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
	                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
	                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

	uint32_t Idx1 = 0;

	numChannels = 2;
	/* Preconfiguration before using DAC----------------------------------------*/
		DAC_PreConfig();

		/* TIM6 Configuration ------------------------------------------------------*/
		/* TIM6 Periph clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* TIM2 Configuration */
	  /* Time base configuration */
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	  //TIM_TimeBaseStructure.TIM_Period = 0x19;
	  TIM_TimeBaseStructure.TIM_Period = 0xFF;
	  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	  /* TIM2 TRGO selection */
	  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	  /* DAC channel1 Configuration */
	  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	  /* DAC channel2 Configuration */
	  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	  /* Fill Sine32bit table */
	  for (Idx1 = 0; Idx1 < 32; Idx1++)
	  {
	    DualSine12bit[Idx1] = (Sine12bit2[Idx1] << 16) + (Sine12bit2[Idx1]);
	  }


	  /* DMA1 channel4 configuration */
	  DMA_DeInit(DMA1_Channel3);

	  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	  DMA_InitStructure.DMA_BufferSize = 32;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;


	  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	  /* Enable DMA1 Channel4 */
	  DMA_Cmd(DMA1_Channel3, ENABLE);


	  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	     automatically connected to the DAC converter. */
	  DAC_Cmd(DAC_Channel_1, ENABLE);
	  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
	     automatically connected to the DAC converter. */
	  DAC_Cmd(DAC_Channel_2, ENABLE);

	  /* Enable DMA for DAC Channel2 */
	  DAC_DMACmd(DAC_Channel_2, ENABLE);

	  /* TIM2 enable counter */
	  TIM_Cmd(TIM6, ENABLE);

	  while (1)
	  {
	  }
}

void playStereoSaw8b(){

	uint16_t Saw8bit2[512];

	uint16_t Idx1 = 0;

	numChannels = 2;
	/* Preconfiguration before using DAC----------------------------------------*/
		DAC_PreConfig();

		/* TIM6 Configuration ------------------------------------------------------*/
		/* TIM6 Periph clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* TIM2 Configuration */
	  /* Time base configuration */
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	  //TIM_TimeBaseStructure.TIM_Period = 0x19;
	  TIM_TimeBaseStructure.TIM_Period = 0xFF;
	  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	  /* TIM2 TRGO selection */
	  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	  /* DAC channel1 Configuration */
	  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	  /* DAC channel2 Configuration */
	  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	  /* Fill Sine32bit table */
	  for (Idx1 = 0; Idx1 < 510; Idx1++)
	  {
		  Saw8bit2[Idx1] = ((Idx1%256) << 8) + (Idx1%256);

	  }


	  /* DMA1 channel4 configuration */
	  DMA_DeInit(DMA1_Channel3);

	  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8RD_Address;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Saw8bit2;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	  DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;


	  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	  /* Enable DMA1 Channel4 */
	  DMA_Cmd(DMA1_Channel3, ENABLE);

	  uint16_t a = DMA1_Channel3->CCR;

	  LCDN_Write_Dec(a);

	  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	     automatically connected to the DAC converter. */
	  DAC_Cmd(DAC_Channel_1, ENABLE);
	  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
	     automatically connected to the DAC converter. */
	  DAC_Cmd(DAC_Channel_2, ENABLE);

	  /* Enable DMA for DAC Channel2 */
	  DAC_DMACmd(DAC_Channel_2, ENABLE);

	  /* TIM2 enable counter */
	  TIM_Cmd(TIM6, ENABLE);

	  while (1)
	  {
	  }
}

void playStereoWav8b(uint8_t * name){

	FRESULT fresult;

		UINT bytesToRead, bytesRead;

		NVIC_InitTypeDef nvicStructure;


		fresult = f_open(&plik, name, FA_READ);

			bytesToRead = f_size(&plik);

			fresult = f_lseek(&plik, 22);

			f_read(&plik, &numChannels, 2, &bytesRead);
			f_read(&plik, &sampleRate, 2, &bytesRead);

			fresult = f_lseek(&plik, 34);

			f_read(&plik, &bitsPerSample, 2, &bytesRead);

			LCDN_WriteXY("numCh:",0,1);
			  	LCDN_Write_Dec(numChannels);


			  	LCDN_WriteXY("SR:",0,2);
			  	LCDN_Write_Dec(sampleRate);


			  	LCDN_WriteXY("bps:",0,3);
			  	LCDN_Write_Dec(bitsPerSample);


			fresult = f_lseek(&plik, 44);
			  	//fresult = f_lseek(&plik, 0);


		/* Preconfiguration before using DAC----------------------------------------*/
		DAC_PreConfig();

		/* TIM6 Configuration ------------------------------------------------------*/
		/* TIM6 Periph clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);


		/* TIM6 Configuration */
		  TIM_DeInit(TIM6);

		  TIM_SetAutoreload(TIM6, ((1000.0/sampleRate)/31.25E-06));

		  /* TIM6 TRGO selection */
		  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

		  /* Enable TIM6 update interrupt */
		  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);





		/* TIM6 enable counter */
		TIM_Cmd(TIM6, ENABLE);



	  /* DAC channel1 Configuration */
	  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	  /* DAC channel2 Configuration */
	  DAC_Init(DAC_Channel_2, &DAC_InitStructure);




	  /* DMA1 channel4 configuration */
	  DMA_DeInit(DMA1_Channel3);

	  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8RD_Address;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &buffer[0][0];
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	  DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;



	  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

	  /* Enable DMA1 Channel3 */
	  DMA_Cmd(DMA1_Channel3, ENABLE);


	  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	     automatically connected to the DAC converter. */
	  DAC_Cmd(DAC_Channel_1, ENABLE);
	  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
	     automatically connected to the DAC converter. */
	  DAC_Cmd(DAC_Channel_2, ENABLE);

	  /* Enable DMA for DAC Channel2 */
	  DAC_DMACmd(DAC_Channel_2, ENABLE);

	  /* TIM2 enable counter */
	  TIM_Cmd(TIM6, ENABLE);

	  /* Enable the DMA1_Channel2 Interrupt */
	  	nvicStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	  	nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
	  	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	  	nvicStructure.NVIC_IRQChannelCmd = ENABLE;




	  	fresult = f_read(&plik, &buffer[0][0], SAMPLE_BUFFER_SIZE * 2, &bytesRead);
	  	fresult = f_read(&plik, &buffer[1][0], SAMPLE_BUFFER_SIZE * 2, &bytesRead);

	  	NVIC_Init(&nvicStructure);

	  	while (1) {
	  			//IR_Decode(&IR_FRAME);
	  			if (canRead == true) {


	  					//STM_EVAL_LEDOn(LED5);
	  					f_read(&plik, &buffer[i ^ 0x01][0], SAMPLE_BUFFER_SIZE * 2,	&bytesRead);
	  					//STM_EVAL_LEDOff(LED5);


	  					canRead = false;
	  					if (bytesRead < SAMPLE_BUFFER_SIZE * 2)
	  						break;



	  			}

	  		}

	  		DAC_Cmd(DAC_Channel_1, DISABLE);
	  		DAC_Cmd(DAC_Channel_2, DISABLE);

	  		//DMA_Cmd(DMA1_Channel2, DISABLE);
	  		DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);
	  		fresult = f_close(&plik);
}

void playWav(uint8_t * name) {

	FRESULT fresult;

	UINT bytesToRead, bytesRead;

	NVIC_InitTypeDef nvicStructure;


	fresult = f_open(&plik, name, FA_READ);

		bytesToRead = f_size(&plik);

		fresult = f_lseek(&plik, 22);

		f_read(&plik, &numChannels, 2, &bytesRead);
		f_read(&plik, &sampleRate, 2, &bytesRead);

		fresult = f_lseek(&plik, 34);

		f_read(&plik, &bitsPerSample, 2, &bytesRead);

		LCDN_WriteXY("numCh:",0,1);
		  	LCDN_Write_Dec(numChannels);


		  	LCDN_WriteXY("SR:",0,2);
		  	LCDN_Write_Dec(sampleRate);


		  	LCDN_WriteXY("bps:",0,3);
		  	LCDN_Write_Dec(bitsPerSample);


		//fresult = f_lseek(&plik, 44);
		  	fresult = f_lseek(&plik, 0);


	/* Preconfiguration before using DAC----------------------------------------*/
	DAC_PreConfig();

	/* TIM6 Configuration ------------------------------------------------------*/
	/* TIM6 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);


	/* TIM6 Configuration */
	  TIM_DeInit(TIM6);

	  TIM_SetAutoreload(TIM6, ((1000.0/sampleRate)/31.25E-06));


	  /* TIM6 TRGO selection */
	  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	  /* Enable TIM6 update interrupt */
	  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);




	/****************************/

	DAC_DeInit();


	if(numChannels == 2){
			DMA_DeInit(DMA1_Channel3); //stereo
	} else {
		DMA_DeInit(DMA1_Channel2);    //mono
	}


	if(bitsPerSample == 16){ //16-bit
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
	}	else { //8 bit
		if(numChannels == 1){ //mono
			DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
		} else { //stereo 8 bit
			DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8RD_Address;
		}
	}


	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &buffer[0][0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;


	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	if(bitsPerSample == 16){
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
	}

	else {
		if(numChannels == 1){ //mono 8 bit
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
		} else { //stereo 8 bit
			DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		}
	}


	//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	if(numChannels == 1){ //mono 8 bit
		DMA_Init(DMA1_Channel2, &DMA_InitStructure);
		DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	} else {
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);
		DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
	}





	/* Enable the DMA1_Channel2 Interrupt */
	if(numChannels == 2){
		nvicStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn; //stereo
		nvicStructure.NVIC_IRQChannelPreemptionPriority = 2;
		nvicStructure.NVIC_IRQChannelSubPriority = 0;
		nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	} else {
		nvicStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn; //mono
		nvicStructure.NVIC_IRQChannelPreemptionPriority = 2;
		nvicStructure.NVIC_IRQChannelSubPriority = 0;
		nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	}



	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);

	//uint16_t a = DAC->CR;



	if(numChannels == 2){
		/* Enable DMA1 Channel4 */
		DAC_DMACmd(DAC_Channel_2, ENABLE);

	}

	//fresult = f_open(&plik,"z.wav", FA_READ);
	//fresult = f_open(&plik, "im16.wav", FA_READ);

	//fresult = f_lseek(&plik, 0);
//#ifdef SAMPLE_WIDTH_16
	if(bitsPerSample == 16){
		fresult = f_read(&plik, &buffer[0][0], SAMPLE_BUFFER_SIZE * 2, &bytesRead);
		convertBufferTo12bit(&buffer[0][0]);

		fresult = f_read(&plik, &buffer[1][0], SAMPLE_BUFFER_SIZE * 2, &bytesRead);
		convertBufferTo12bit(&buffer[1][0]);
	} else {
//#else
		if(numChannels == 1){ //mono 8 bit
			fresult = f_read(&plik, &buffer[0][0], SAMPLE_BUFFER_SIZE , &bytesRead);
			fresult = f_read(&plik, &buffer[1][0], SAMPLE_BUFFER_SIZE , &bytesRead);
		} else { // 8 bit stereo
			fresult = f_read(&plik, &buffer[0][0], SAMPLE_BUFFER_SIZE * 2 , &bytesRead);
			fresult = f_read(&plik, &buffer[1][0], SAMPLE_BUFFER_SIZE * 2 , &bytesRead);
		}
	}
//#endif

	//fresult = f_read(&plik, &buffer[1][0], 512 , &bytesRead);

	NVIC_Init(&nvicStructure);




	if(numChannels == 2) //stereo 8 bit
		DMA_Cmd(DMA1_Channel3, ENABLE);
	else DMA_Cmd(DMA1_Channel2, ENABLE);

	/* DAC channel1 Configuration */

		DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);

if(numChannels == 2 ){
	/* DAC channel2 Configuration */

			DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

		DAC_Init(DAC_Channel_2, &DAC_InitStructure);

		DAC_Cmd(DAC_Channel_2, ENABLE);
}

/* TIM6 enable counter */
	TIM_Cmd(TIM6, ENABLE);

	canRead = false;
	i = 0;

	while (1) {
		IR_Decode(&IR_FRAME);
		if (canRead == true) {
			if(bitsPerSample == 16){

				//STM_EVAL_LEDOn(LED5);
				f_read(&plik, &buffer[i ^ 0x01][0], SAMPLE_BUFFER_SIZE * 2,	&bytesRead);
				//STM_EVAL_LEDOff(LED5);
				convertBufferTo12bit(&buffer[i ^ 0x01][0]);

				canRead = false;
				if (bytesRead < SAMPLE_BUFFER_SIZE * 2)
					break;
			} else {

				if(numChannels == 2 ){


					//STM_EVAL_LEDOn(LED5);
					f_read(&plik, &buffer[i ^ 0x01][0], SAMPLE_BUFFER_SIZE * 2 , &bytesRead);
					//STM_EVAL_LEDOff(LED5);
					canRead = false;

					if (bytesRead < SAMPLE_BUFFER_SIZE*2)
									break;
				} else {
					//STM_EVAL_LEDOn(LED5);
					f_read(&plik, &buffer[i ^ 0x01][0], SAMPLE_BUFFER_SIZE , &bytesRead);
					//STM_EVAL_LEDOff(LED5);
					canRead = false;

					if (bytesRead < SAMPLE_BUFFER_SIZE)
									break;
				}

			}


		}

	}

	DAC_Cmd(DAC_Channel_1, DISABLE);
	DMA_Cmd(DMA1_Channel2, DISABLE);

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);
	fresult = f_close(&plik);

}

void playWavFromMemory(uint8_t * memBuffer) {

	FRESULT fresult;

	UINT bytesToRead, bytesRead;

	NVIC_InitTypeDef nvicStructure;
	uint8_t * mPtr = memBuffer;

	/* Preconfiguration before using DAC----------------------------------------*/
	DAC_PreConfig();

	/* TIM6 Configuration ------------------------------------------------------*/
	/* TIM6 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);


	/* TIM6 Configuration */
	  TIM_DeInit(TIM6);

	  TIM_SetAutoreload(TIM6, ((1000.0/sampleRate)/31.25E-06));

	  /* TIM6 TRGO selection */
	  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	  /* Enable TIM6 update interrupt */
	  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);



	/* TIM6 enable counter */
	TIM_Cmd(TIM6, ENABLE);

	/****************************/
	/* Triangle Wave generator -----------------------------------------*/
	DAC_DeInit();

	/* Noise Wave generator --------------------------------------------*/
	/* DAC channel1 Configuration */

	//DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	DMA_DeInit(DMA1_Channel2);
#ifdef SAMPLE_WIDTH_16
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
#else
	if(numChannels == 1){
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
	} else {
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8RD_Address;
	}
#endif
	//DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
	//DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &buffer[0][0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	//DMA_InitStructure.DMA_BufferSize = 32;
	DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
#ifdef SAMPLE_WIDTH_16
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
#else
	if(numChannels == 1){
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	} else {
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	}
#endif

	//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	/* Enable the DMA1_Channel3 Interrupt */
	nvicStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
	nvicStructure.NVIC_IRQChannelSubPriority = 3;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;

	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);


	//fresult = f_open(&plik, name, FA_READ);

	bytesToRead = f_size(&plik);

	//fresult = f_lseek(&plik, 44 + 22050);
	//fresult = f_lseek(&plik, 0);
//#ifdef SAMPLE_WIDTH_16
	if(bitsPerSample == 16){
		memcpy(&buffer[0][0],mPtr,SAMPLE_BUFFER_SIZE*2);
		convertBufferTo12bit(&buffer[0][0]);

		mPtr = mPtr + SAMPLE_BUFFER_SIZE*2;

		//fresult = f_read(&plik, &buffer[1][0], SAMPLE_BUFFER_SIZE * 2, &bytesRead);
		memcpy(&buffer[1][0],mPtr,SAMPLE_BUFFER_SIZE*2);
		convertBufferTo12bit(&buffer[1][0]);

		mPtr = mPtr + SAMPLE_BUFFER_SIZE*2;
	} else {
//#else
	//fresult = f_read(&plik, &buffer[0][0], SAMPLE_BUFFER_SIZE , &bytesRead);
	//fresult = f_read(&plik, &buffer[1][0], SAMPLE_BUFFER_SIZE , &bytesRead);
		if(numChannels == 1){
			memcpy(&buffer[0][0],mPtr,SAMPLE_BUFFER_SIZE);
			mPtr = mPtr + SAMPLE_BUFFER_SIZE;
			memcpy(&buffer[1][0],mPtr,SAMPLE_BUFFER_SIZE);
			mPtr = mPtr + SAMPLE_BUFFER_SIZE;
		} else { // stereo 8 bit
			memcpy(&buffer[0][0],mPtr,SAMPLE_BUFFER_SIZE);
			mPtr = mPtr + SAMPLE_BUFFER_SIZE;
			memcpy(&buffer[1][0],mPtr,SAMPLE_BUFFER_SIZE);
			mPtr = mPtr + SAMPLE_BUFFER_SIZE;
		}
	}
//#endif

	//fresult = f_read(&plik, &buffer[1][0], 512 , &bytesRead);

	NVIC_Init(&nvicStructure);

	DMA_Cmd(DMA1_Channel2, ENABLE);

	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);

	canRead = false;
	i = 0;

	while (1) {


		if (canRead == true) {
#ifdef SAMPLE_WIDTH_16
			//f_read(&plik, &buffer[i ^ 0x01][0], SAMPLE_BUFFER_SIZE * 2,&bytesRead);
			memcpy(&buffer[i ^ 0x01][0],mPtr,SAMPLE_BUFFER_SIZE*2);
			convertBufferTo12bit(&buffer[i ^ 0x01][0]);

			canRead = false;
			if (bytesRead < SAMPLE_BUFFER_SIZE * 2)
				break;
#else
			//f_read(&plik, &buffer[i ^ 0x01][0], SAMPLE_BUFFER_SIZE , &bytesRead);
			memcpy(&buffer[i ^ 0x01][0],mPtr,SAMPLE_BUFFER_SIZE);
			mPtr = mPtr + SAMPLE_BUFFER_SIZE;
			canRead = false;
			if (mPtr >= memBuffer+67392)
			break;
#endif

		}

	}

	DAC_Cmd(DAC_Channel_1, DISABLE);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
	//fresult = f_close(&plik);

}

void playWavFromIntMemory(const uint8_t * memBuffer) {



	UINT bytesToRead, bytesRead;

	NVIC_InitTypeDef nvicStructure;

	wavPtrBegin = memBuffer;
	wavPtr = wavPtrBegin;

	/* Preconfiguration before using DAC----------------------------------------*/
	DAC_PreConfig();

	/* TIM6 Configuration ------------------------------------------------------*/
	/* TIM6 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);


	/* TIM6 Configuration */
	  TIM_DeInit(TIM6);

	  TIM_SetAutoreload(TIM6, ((1000.0/sampleRate)/31.25E-06));

	  /* TIM6 TRGO selection */
	  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	  /* Enable TIM6 update interrupt */
	  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	//TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);



	/* TIM6 enable counter */
	TIM_Cmd(TIM6, ENABLE);

	/****************************/

	DAC_DeInit();




	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	DMA_DeInit(DMA1_Channel2);
#ifdef SAMPLE_WIDTH_16
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
#else
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
#endif

	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) wavPtr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	DMA_InitStructure.DMA_BufferSize = SAMPLE_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
#ifdef SAMPLE_WIDTH_16
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
#else
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
#endif


	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

	/* Enable the DMA1_Channel2 Interrupt */
	nvicStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 8;
	nvicStructure.NVIC_IRQChannelSubPriority = 3;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;

	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);




	NVIC_Init(&nvicStructure);

	DMA_Cmd(DMA1_Channel2, ENABLE);

	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);








}
