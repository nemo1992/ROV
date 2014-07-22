/*
 * My_UART.c
 *
 *  Created on: Jul 13, 2014
 *      Author: The Riddler
 */


#include "My_UART.h"
#include "My_DMA.h"

void Init_UART(void)
{

  // Enable the peripherals used by this example.
  //
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);


  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  ROM_IntMasterEnable();

  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  ROM_GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_DIR_MODE_OUT);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
  ROM_GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_DIR_MODE_OUT);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);
  ROM_GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_DIR_MODE_OUT);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3);


  //  ROM_GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_DIR_MODE_IN);
  //  ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);
  //  ROM_GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_DIR_MODE_IN);
  //  ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);

  //
  // Set GPIO A0 and A1 as UART pins.
  GPIOPinConfigure(GPIO_PC4_U1RX);
  GPIOPinConfigure(GPIO_PC5_U1TX);
  ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


  //
  // Configure the UART for 115,200, 8-N-1 operation.
  //
  ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,
                          (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                           UART_CONFIG_PAR_NONE));

  ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                          (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                           UART_CONFIG_PAR_NONE));
  //
  // Enable the UART interrupt.
  //
  ROM_IntEnable(INT_UART1);
  ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);


  ROM_IntEnable(INT_UART0);
  ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

  //
  // Prompt for text to be entered.
  //
  ROM_UARTDMAEnable(UART1_BASE,UART_DMA_TX);

  ROM_UARTDMAEnable(UART0_BASE,UART_DMA_TX);
  //UARTSend((unsigned char *)"\r\n2JEnter text: ", 16);
  // SysCtlDelay(53333);


}



void UART0IntHandler(void)
{
  unsigned long ulStatus;
  //unsigned long ulMode;

  //
  // Read the interrupt status of the UART.
  //
  ulStatus = ROM_UARTIntStatus(UART0_BASE, 1);

  //
  // Clear any pending status, even though there should be none since no UART
  // interrupts were enabled.  If UART error interrupts were enabled, then
  // those interrupts could occur here and should be handled.  Since uDMA is
  // used for both the RX and TX, then neither of those interrupts should be
  // enabled.
  //
  ROM_UARTIntClear(UART0_BASE, ulStatus);
  //
  // If the UART0 DMA TX channel is disabled, that means the TX DMA transfer
  // is done.
  //

}


void UART1IntHandler(void)
{
  unsigned long ulStatus;
  //unsigned long ulMode;

  //
  // Read the interrupt status of the UART.
  //
  ulStatus = ROM_UARTIntStatus(UART1_BASE, 1);

  //
  // Clear any pending status, even though there should be none since no UART
  // interrupts were enabled.  If UART error interrupts were enabled, then
  // those interrupts could occur here and should be handled.  Since uDMA is
  // used for both the RX and TX, then neither of those interrupts should be
  // enabled.
  //
  ROM_UARTIntClear(UART1_BASE, ulStatus);
  //
  // If the UART0 DMA TX channel is disabled, that means the TX DMA transfer
  // is done.
  //

}


void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
  //
  // Loop while there are more characters to send.
  //
  while(ulCount--)
  {
    //
    // Write the next character to the UART.
    //
    ROM_UARTCharPutNonBlocking(UART1_BASE, *pucBuffer++);
  }
}


void dec2char(int data_dec, char k)
{
  char array_char[4];
  if(data_dec<0)
  {
    data_dec = - data_dec;
  }
  if( data_dec < 10000 )
  {
    array_char[0] =  data_dec/1000;
    array_char[1] = (data_dec - array_char[0]*1000)/100;
    array_char[2] = (data_dec - array_char[0]*1000 - array_char[1]*100)/10;
    array_char[3] =  data_dec - array_char[0]*1000 - array_char[1]*100 - array_char[2]*10;
    array_char[0] += 0x30;
    array_char[1] += 0x30;
    array_char[2] += 0x30;
    array_char[3] += 0x30;

    char j;
    for(j=0;j<4;j++)
    {
      Uart_Send[6*k+1+j] = array_char[j];
    }
  }
}






void Display_Uart(void)//, unsigned char flag_acc )
{
  int s;
  for(s=0;s<12;s++)
  {
    if(Uart_Array[s]<0)
    {
      Uart_Send[6*s] = '-';
    }
    else
    {
      Uart_Send[6*s] = ' ';
    }
    if(s>0)
    {
      Uart_Send[6*s-1] = ' ';
    }
    dec2char(Uart_Array[s],s);
  }
  Uart_Send[71] = 0x0D;
  Uart_Send[72] = 0x0A;
}
