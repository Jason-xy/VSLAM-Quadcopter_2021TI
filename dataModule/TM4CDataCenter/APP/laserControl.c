#include "laserControl.h"

void Laser_Init(void){

//    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
//                       SYSCTL_OSC_MAIN);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinTypeGPIOOutput(GPIOA_BASE, GPIO_PIN_2);
		GPIOPinWrite(GPIOA_BASE, GPIO_PIN_2, 0xFF);
}
void Laser_Off(void){
	GPIOPinWrite(GPIOA_BASE, GPIO_PIN_2, 0);
}

void Laser_On(void){
	GPIOPinWrite(GPIOA_BASE, GPIO_PIN_2, 0xFF);
}


