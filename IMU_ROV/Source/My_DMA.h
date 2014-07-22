/*
 * My_DMA.h
 *
 *  Created on: Jul 13, 2014
 *      Author: The Riddler
 */

#ifndef MY_DMA_H_
#define MY_DMA_H_

#include "My_Init.h"



extern void Init_DMA(void);
extern void DMA_Re_Enable_1(void);
extern void DMA_Re_Enable_0(void);
extern void DMAIntHandler(void);
extern void uDMAErrorHandler(void);

#endif /* MY_DMA_H_ */
