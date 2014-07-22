/*
 * My_DMA.c
 *
 *  Created on: Jul 13, 2014
 *      Author: The Riddler
 */


#include "My_DMA.h"

#pragma data_alignment=1024
unsigned char ucControlTable[1024];

void Init_DMA(void)
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
  ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UDMA);
  ROM_uDMAEnable();
  ROM_uDMAControlBaseSet(ucControlTable);

  ROM_uDMAChannelAttributeEnable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_USEBURST);
  ROM_uDMAChannelControlSet(UDMA_CHANNEL_UART1TX| UDMA_PRI_SELECT,
                            UDMA_SIZE_8 | UDMA_SRC_INC_8 |
                              UDMA_DST_INC_NONE| UDMA_ARB_4);
  ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX| UDMA_PRI_SELECT,
                             UDMA_MODE_BASIC,
                             (void *)&Uart_Send[0],
                             (void *)(UART1_BASE + UART_O_DR), Size_Send_DMA_1);
  ROM_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);


  ROM_uDMAChannelAttributeEnable(UDMA_CHANNEL_UART0TX, UDMA_ATTR_USEBURST);
  ROM_uDMAChannelControlSet(UDMA_CHANNEL_UART0TX| UDMA_PRI_SELECT,
                            UDMA_SIZE_8 | UDMA_SRC_INC_8 |
                              UDMA_DST_INC_NONE| UDMA_ARB_4);
  ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART0TX| UDMA_PRI_SELECT,
                             UDMA_MODE_BASIC,
                             (void *)&Uart_Send_0[0],
                             (void *)(UART0_BASE + UART_O_DR), Size_Send_DMA_0);
 // ROM_uDMAChannelEnable(UDMA_CHANNEL_UART0TX);


  ROM_IntEnable(INT_UDMAERR);

}

void DMA_Re_Enable_1(void)
{
  //  if(!ROM_uDMAChannelIsEnabled(UDMA_CHANNEL_UART1TX))
  //  {
  //
  // Start another DMA transfer to UART0 TX.
  //
  ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX| UDMA_PRI_SELECT,
                             UDMA_MODE_BASIC,
                             (void *)&Uart_Send[0],
                             (void *)(UART1_BASE + UART_O_DR), Size_Send_DMA_1);

  //
  // The uDMA TX channel must be re-enabled.
  //
  ROM_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
  //  }
}

void DMA_Re_Enable_0(void)
{
  //  if(!ROM_uDMAChannelIsEnabled(UDMA_CHANNEL_UART1TX))
  //  {
  //
  // Start another DMA transfer to UART0 TX.
  //
  ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART0TX| UDMA_PRI_SELECT,
                             UDMA_MODE_BASIC,
                             (void *)&Uart_Send_0[0],
                             (void *)(UART0_BASE + UART_O_DR), Size_Send_DMA_0);

  //
  // The uDMA TX channel must be re-enabled.
  //
  ROM_uDMAChannelEnable(UDMA_CHANNEL_UART0TX);
  //  }
}


void DMAIntHandler(void)
{
  unsigned long ulMode;

  //
  // Check for the primary control structure to indicate complete.
  //
  ulMode = ROM_uDMAChannelModeGet(UDMA_CHANNEL_SW);
  if(ulMode == UDMA_MODE_STOP)
  {
    //
    // Increment the count of completed transfers.
    //
    // g_ulMemXferCount++;

    //
    // Configure it for another transfer.
    //
    //    ROM_uDMAChannelTransferSet(UDMA_CHANNEL_UART0TX| UDMA_PRI_SELECT,
    //                               UDMA_MODE_BASIC,
    //                               (void *)0x20000080,
    //                               (void *)(UART0_BASE + UART_O_DR), Size_Send_DMA);
    //
    //
    // Initiate another transfer.
    //
    ROM_uDMAChannelEnable(UDMA_CHANNEL_SW);
    ROM_uDMAChannelRequest(UDMA_CHANNEL_SW);
  }

}



void uDMAErrorHandler(void)
{
  unsigned long ulStatus;

  //
  // Check for uDMA error bit
  //
  ulStatus = ROM_uDMAErrorStatusGet();

  //
  // If there is a uDMA error, then clear the error and increment
  // the error counter.
  //
  if(ulStatus)
  {
    ROM_uDMAErrorStatusClear();
    //g_uluDMAErrCount++;
  }
}
