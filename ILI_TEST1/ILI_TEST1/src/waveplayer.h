/*
 * waveplayer.h
 *
 *  Created on: 18 lip 2014
 *      Author: Pawe�
 */

#ifndef WAVEPLAYER_H_
#define WAVEPLAYER_H_

#include "main.h"

#define SAMPLING_FREQ 11025
//#define DMA_TIMER_VAL 1451 //for 22050
#define DMA_TIMER_VAL (1000.0/SAMPLING_FREQ)/31.25E-06 //for 22050

void DAC_PreConfig(void);

void convertBufferTo12bit(int16_t * buffer);

void playWav(uint8_t * name);

#endif /* WAVEPLAYER_H_ */
