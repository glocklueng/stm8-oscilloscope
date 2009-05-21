/**
 ******************************************************************************
 * @file key.c
 * @brief  ��״̬����д�İ���ɨ��
 * @author ifree64 - stm8 �ֳ�ʾ����Դ��Ŀ��
 * @version V0.1.0
 * @date 03/25/2009
 ******************************************************************************
 *
 * free for use
 *
 ******************************************************************************
 */
#include "key.h"

enum KEY_ScanState{
	KSCAN_START = 0,        /* �޼�����, ����ɨ���ʼ״̬ */
	KSCAN_PRESS,            /* �����£��ж϶���, >0.1s */
	KSCAN_LONGPRESS,        /* �Ƕ������ж��Ƿ񳤰�����>0.8s */
	KSCAN_PDLF,             /* �ж��������õ�һ�γ�������1s ��ʼ���� */
	KSCAN_LIANFA            /* ������ÿ0.2s����һ���������� */
};

struct KEY_StateFsm{
	enum KEY_ScanState KPressNext;       /* �����£���һ��״̬ */ 
	enum KEY_ScanState NoKeyNext;        /* ��̧����һ��״̬ */
	u8 KPressTime;	               /* �������������ʱ��, 10ms�ı��� */
	u8 KPressType;	     	       /* ��������������������������� */	
};

/* ��֤״̬���ֵ���������е�λ����ͬ  */
struct KEY_StateFsm const KEY_StateFsmTable[] = {
	/* kpress_next,     nokey_next,     time,   type   */
	{ KSCAN_PRESS,      KSCAN_START,    0,      KTYPE_NO},      /* KSCAN_START: 0 */
	{ KSCAN_LONGPRESS,  KSCAN_START,    5,      KTYPE_NORMAL},  /* KSCAN_PRESS: 1 */
	{ KSCAN_PDLF,       KSCAN_START,    80,     KTYPE_LONG},    /* KSCAN_LONGPRESS: 2 */
	{ KSCAN_LIANFA,     KSCAN_START,    100,    KTYPE_NO},      /* KSCAN_PDLF: 3 */
	{ KSCAN_LIANFA,     KSCAN_START,    20,     KTYPE_LIANFA}   /* KSCAN_LIANFA: 4*/
};

void KEY_Init(void)
{
	GPIO_Init(KEY_ROW_PORT, KEY_ROW_PINALL, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(KEY_COLUMN_PORT, KEY_COLUMN_PINALL , GPIO_MODE_OUT_PP_LOW_FAST ); 
}

/* 
   ������������ȡ����״̬,  ���ݲ�ͬ������·�ͱ�ŷ�ʽ�޸ı�����
   ��ڲ�������
   ���ڲ������������
	 
	 ROW : PD7 PD6 PD5
	 COLUMN : PG5 PG4 PG3 PG2
   */
static u8 KEY_Read(void)
{    
  u8 KCode = 0;
	u8 row = 0, column = 0;
	u8 t;
	GPIO_WriteLow(KEY_COLUMN_PORT, KEY_COLUMN_PINALL);	 
	t = (~GPIO_ReadInputData(KEY_ROW_PORT))&KEY_ROW_PINALL;
	if(t)
		{
			// �м����£��ҵ���
			u8 t2 = ~KEY_COLUMN1_PIN;
			while(++column <= KEY_COLUMN_NUM)
			{
				KEY_COLUMN_PORT->ODR |= KEY_COLUMN_PINALL; 							// ���������ø�
				KEY_COLUMN_PORT->ODR &= t2;								 							// ɨ�����õ�
				row = (~GPIO_ReadInputData(KEY_ROW_PORT))&KEY_ROW_PINALL;	// ������
				if( row == t)
					break;
				else
					t2 >>= 1;
			}
			KCode = 3*(column-1);
			if( row == KEY_ROW1_PIN ) KCode += 1;
			else if(row == KEY_ROW2_PIN) KCode += 2;
			else KCode += 3;
		}
	return KCode;
}

/*
   ʹ��˵����Ϊ�˵õ��ú����ṩ�Ĺ��ܣ�����ÿ10ms����1��
   ��������������ɨ�裬����״̬������ã����ص�ǰ����״̬
   ��ڲ�������
   ���ڲ�����������źͰ������࣬��ʽ����
   D7 D6 D5 D4 D3 D2 D1 D0
   M1 M0 K5 K4 K3 K2 K1 K0
M1:M0 ��ʾ�������ͣ� 01 ��ͨ  10 ������  11 ��������
K5:K0 ������� 1~63, �ܹ�����֧��2^6 - 1 = 63������    
*/
u8 KEY_Scan(void)
{
	static enum KEY_ScanState KScan_CurrState = KSCAN_START;
	static u8 OldKey;
	static u8 KTimeCounter = 0;

	u8 rKeyCode = 0;		// 0 for No key press
	u8 CurrKey = KEY_Read();

	if( (CurrKey != 0) && (CurrKey == OldKey) )
	{
		if(++KTimeCounter >= KEY_StateFsmTable[KScan_CurrState].KPressTime ) 
		{
			KTimeCounter = 0;
			rKeyCode = KEY_StateFsmTable[KScan_CurrState].KPressType | CurrKey;
			KScan_CurrState = KEY_StateFsmTable[KScan_CurrState].KPressNext;
		}
	}	
	else
	{
		//   �������¼�
		KScan_CurrState = KEY_StateFsmTable[KScan_CurrState].NoKeyNext;
		OldKey = CurrKey;
	}
	return rKeyCode;
}

/******************* (C) COPYRIGHT 2009 STM8DSO TEAM *****END OF FILE****/
