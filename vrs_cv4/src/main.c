/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 6.0.0   2016-10-11

The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"


/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */
void adc_init();
void gpio_init();
void Delay(__IO uint32_t nTime);
int getPressedButton();

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{

	/**
	*  IMPORTANT NOTE!
	*  See the <system_*.c> file and how/if the SystemInit() function updates
	*  SCB->VTOR register. Sometimes the symbol VECT_TAB_SRAM needs to be defined
	*  when building the project if code has been located to RAM and interrupts
	*  are used. Otherwise the interrupt table located in flash will be used.
	*  E.g.  SCB->VTOR = 0x20000000;
	*/

	/**
	*  At this stage the microcontroller clock setting is already configured,
	*  this is done through SystemInit() function which is called from startup
	*  file (startup_stm32l1xx_hd.s) before to branch to application main.
	*  To reconfigure the default setting of SystemInit() function, refer to
	*  system_stm32l1xx.c file
	*/

	/* TODO - Add your application code here */

	adc_init();

//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	gpio_init();

	/* Infinite loop */

	int delays[] = {2,4,9,16,25};
	int mode = 4;
	int lastButtonPressed = 4;
	while (1)
	{
		GPIOA->ODR ^= (uint32_t)(0b01<<5);

		for (int i=0; i<delays[mode]; i++) {

			int buttonPressed = getPressedButton();
			if (lastButtonPressed != 4 && buttonPressed == 4) {
				mode = lastButtonPressed;
			}
			lastButtonPressed = buttonPressed;
			Delay(10);
		}
	}
	return 0;
}

void Delay(__IO uint32_t nTime)
{
	for (int i=0; i<1000*nTime;i++);
//  TimingDelay = nTime;

//  while(TimingDelay != 0);
}

int getPressedButton() {
	int buttonValues[] = {2500, 3000, 3550, 3800};

	ADC_SoftwareStartConv(ADC1);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	int AD_value=ADC_GetConversionValue(ADC1);

	int buttonPressed = 0;
	for (buttonPressed = 0; buttonPressed < 4; buttonPressed++) {
		if (buttonValues[buttonPressed] > AD_value)
			break;
	}

	return buttonPressed;
}

void gpio_init() {
	GPIOA->MODER &= ~((uint32_t)0b11<<(2*5));
	GPIOA->MODER |= (uint32_t)0b01<<(2*5); //Out
	GPIOA->OTYPER &= ~((uint32_t)0b01<<5); //Push-Pull
	GPIOA->PUPDR &= ~((uint32_t)0b11<<(2*5));
	GPIOA->PUPDR |= (uint32_t)0b01<<(2*5); //Pull UP
	GPIOA->OSPEEDR |= (uint32_t)0b11<<(2*5); //Very high
}

void adc_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//Opravi� a upravi�
	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ; //tu nastavit pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_16Cycles); //tu nastavit pin
	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
	}
	/* Start ADC Software Conversion */
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */


  /* Infinite loop */
	while (1)
	{
	}
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
