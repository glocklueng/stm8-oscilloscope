/**
  ******************************************************************************
  * @file key.h
  * @brief This file contains functions prototypes.
  * @author ifree64 - stm8 �ֳ�ʾ����Դ��Ŀ��
  * @version V0.1.0
  * @date 03/25/2009
  ******************************************************************************
  */

#ifndef _KEY_H_
#define	_KEY_H_

#include "stm8s.h"

/* *******************************************************************************
 * keycode��ʽ:   D7 D6 D5 D4 D3 D2 D1 D0
 *                M1 M0 K5 K4 K3 K2 K1 K0
 *  M1:M0 ��ʾ�������ͣ� 01 ��ͨ  10 ������  11 ��������
 *  K5:K0 ������� �ܹ�����֧��2^6 - 1 = 63������
 * *******************************************************************************/
#define KTYPE_NO        0x00
#define KTYPE_NORMAL    (1<<6)
#define KTYPE_LONG      (2<<6)
#define KTYPE_LIANFA    (3<<6)

#define KTYPE_MASK	0xC0
#define KCODE_MASK	0x3f

// ����ʹ�õ�GPIO�˿ڣ�3x4����ͬ�ӷ���Ҫ�޸�KEY_Read����
#define KEY_ROW_PORT	(GPIOD)
#define KEY_ROW1_PIN  (GPIO_PIN_5)
#define	KEY_ROW2_PIN	(GPIO_PIN_6)
#define KEY_ROW3_PIN	(GPIO_PIN_7)
#define KEY_ROW_PINALL	(KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN)

#define KEY_COLUMN_PORT	(GPIOG)
#define KEY_COLUMN1_PIN	(GPIO_PIN_5)
#define KEY_COLUMN2_PIN	(GPIO_PIN_4)
#define KEY_COLUMN3_PIN	(GPIO_PIN_3)
#define KEY_COLUMN4_PIN	(GPIO_PIN_2)
#define KEY_COLUMN_PINALL	(KEY_COLUMN1_PIN|KEY_COLUMN2_PIN|KEY_COLUMN3_PIN|KEY_COLUMN4_PIN)
#define KEY_COLUMN_NUM	4

extern void KEY_Init(void);			// ������ʼ��
extern u8 KEY_Scan(void);			// 10ms����1�Σ����ذ���ֵ
#define GET_KTYPE(k)	((k)&KTYPE_MASK)	// ��ȡ��������
#define GET_KCODE(k)	((k)&KCODE_MASK)	// ��ȡ����ֵ

#endif	/*_KEY_H_*/

