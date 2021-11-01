#include "stm32f4xx.h"
#include "usart.h"
#include "remote_controller.h"
#include "delay.h"
#include "sys.h"
// 全局定义 STM32F411xE 或者 STM32F401xx
// 当前定义 STM32F411xE

// STM32F411 外部晶振25Mhz，考虑到USB使用，内部频率设置为96Mhz
// 需要100mhz,自行修改system_stm32f4xx.c

static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/

int main(void)
{

	RCC_ClockSecuritySystemCmd(ENABLE);
	SystemCoreClockUpdate();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart1_init(115200);
	uart2_init(500000);
	delay_init(84);
	
	
	while(1){
		
	}
  
	
}
