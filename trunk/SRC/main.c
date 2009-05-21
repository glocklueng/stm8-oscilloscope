/**
  ******************************************************************************
  * @file stmdso\main.c
  * @brief This file contains the firmware main function.
  * @author ifree64  leexiaofeng@gmail.com
  * @version V0.1
  * @date 04/11/2009
  ******************************************************************************
  */

#include "stm8s.h"
#include "main.h"
#include "key.h"
#include "keyparse.h"
#include "dataprocess.h"
#include "lcd_19264.h"
#include "wavedraw.h"
#include "adc.h"

volatile u8 TriggerVoltage;         

volatile u8 flag_10ms_ok;
@near u8 ADC_Buffer[ADC_BUFFER_SIZE];
volatile u16 ADC_Index;
volatile enum STMDSO_State SystemState;
volatile enum ADC_State ADCState;

// ����0��û���ҵ�; ����Ӳ��������ɾ���ú���
u16 GetTriggerPostion(u16 start, u8 tigvol)
{
        while( start < ADC_BUFFER_SIZE )
        {
            if( ADC_Buffer[start] < tigvol && ADC_Buffer[start+1] >= tigvol )
                return start;
            else
                start++;
        }
        return 0;
}

void CLK_Init(void)
{    
    /* Configure clock prescaler */
    CLK->CKDIVR = 0x00;          /* To be programmed with different value */
                                 /* Fmaster, Fmaster/1, ...               */
    /* To select external source by automtic switch */
    CLK->SWCR |= 0x02;            /* Enable switch */
    CLK->SWR   = 0xB4;            /* Select HSE */
		
    while (!(CLK->SWCR & 0x08));  /* Wait for switch done */
		
		/* verify the external clock is selected (optional) */
    if (CLK->CMSR != 0xB4)        
		    while(1);
    
    CLK->CSSR |= 0x01; 
}

void TIM4_Init(void)
{
    TIM4_DeInit();
    
    // 24000000/128/1000 = 187.5
    
    /* Time base configuration */ 
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, FMASTER/128/1000);    // 1ms�ж�
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}

void TriggerInterruptInit(void)
{
    // to do...
}

void main(void)
{
    u16 trigpos;
    u8 keycode;
    
    SystemState = AutoRunMode;
    // ���ڻ���ʹ���������
	  ADCState = Triggered;       // ADCState = WaitTrigger;

    CLK_Init();                 // ��ʱ�ӳ�ʼ��
    TIM4_Init();                // TIM4 ���ڲ���ϵͳ������Ҫ�Ķ�ʱ�ź�
    KEY_Init();                 // ����������ʼ��
    KeyParse_Init();            // ��������ģ���ʼ��
    LCD_Init();                 // LCD������ʼ��
    WDraw_Init();               // ������ʾģ���ʼ��
    TriggerInterruptInit();     // �ⲿ�����жϳ�ʼ��
    ADC_Init();                 // ADC�����̿�ģ���ʼ��
    DProc_Init();               // ���ݴ���ģ���ʼ��
  

    enableInterrupts();

    /* Infinite loop */
    while (1)
    {
    
				if(flag_10ms_ok)
        {
            flag_10ms_ok = 0;
            keycode = KEY_Scan();
						switch(GET_KTYPE(keycode))
            {
                case KTYPE_NORMAL:
                    KeyParse(GET_KCODE(keycode));
                default:
                    break;
            }
        }
        switch(SystemState)
        {
            // to do
            case AutoRunMode:
            case ManualMode:
                //  ��������
                if( ADCState == ADC_Buffer_Full )
                {
                    trigpos = GetTriggerPostion(0, 64);
                    WDraw_DisplayUpdate(&ADC_Buffer[trigpos]);
                    ADCState = Triggered;
                    ADC_Index = 0;
                }
                break;
            default:
                break;
        }
		
		}
  
}

void DSO_ModeChange()
{
    switch(SystemState){
        case StopMode:
            SystemState = AutoRunMode;
            break;
        case AutoRunMode:
            SystemState = ManualMode;
            break;
        case ManualMode:
            SystemState = StopMode;
            break;
        default:
            break;
    }
    
}
void DSO_RunStop(void)
{
    if(ADCState != STOP)
    {
        ADCState = STOP;
        SystemState = StopMode;
    }
    else
    {
        SystemState = AutoRunMode;
        ADCState = WaitTrigger;
    }
}
            
            
/**
  * @brief Reports the name of the source file and the source line number where
  * the assert error has occurred.
  * User can add his own implementation to report the file name and line number.
  * ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
  * @retval void None
  * @par Required preconditions:
  * None
  * @par Called functions:
  * None
  */
#ifdef FULL_ASSERT
void assert_failed(u8 *file, u16 line)
#else
void assert_failed(void)
#endif
{
  /* Add your own code to manage an assert error */
  /* Infinite loop */
  while (1)
  {
  }
}

/******************* (C) COPYRIGHT 2009 STM8DSO TEAM *****END OF FILE****/
