#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	while (1)
	{
		if((UARTCharsAvail(UART0_BASE))&&(UARTCharGet(UART0_BASE)=='.'))
		{
			/*
			char array[6];
		array[0]=-100&0x00FF;
		array[1]=-100/256;
		array[2]=99&0x00FF;
		array[3]=99&0xFF00;
		array[4]=1000&0x00FF;
		array[5]=(unsigned char)(1000/256);
		*/
			float fl = 19.999;
			char array[6];
			int val = (int)fl;
			if(val < 0)
			{
				array[0]= 255;
				val = -val;
			}
			else
			{
				array[0]=0;
				val = val;
			}

			array[1]=val&0x00FF;
			array[2]=(unsigned char)(val/256);

/*
		int temp = array[0],temp1,temp2,temp3;
		if((temp/10)!=0)
		{
			temp1=temp/10;
			array[0]=temp%10+0x30;
			if((temp1/10)!=0)
			{
				temp2=temp1/10;
				array[1]=temp%10;
			}
			if((temp2/10)!=0)
			{
				temp3=temp2/10;
				array[2]=temp%10;
			}
			else
			{
				array[2]=temp2;
			}
		}
*/
			UARTCharPut(UART0_BASE,array[0]);
			UARTCharPut(UART0_BASE,',');
			UARTCharPut(UART0_BASE,array[1]);
			UARTCharPut(UART0_BASE,',');
			UARTCharPut(UART0_BASE,array[2]);
			UARTCharPut(UART0_BASE,',');
			UARTCharPut(UART0_BASE,array[3]);
			UARTCharPut(UART0_BASE,',');
			UARTCharPut(UART0_BASE,array[4]);
			UARTCharPut(UART0_BASE,',');
			UARTCharPut(UART0_BASE,array[5]);
		}
		//if (UARTCharsAvail(UART0_BASE)) UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
	}
}
