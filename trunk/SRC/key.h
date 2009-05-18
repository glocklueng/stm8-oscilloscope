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

// ����ʹ�õ�GPIO�˿ڣ�8��������������ͬ�ӷ���Ҫ�޸�KEY_Read����
#define KEY_PORT	(GPIOD)
#define KEY_PIN     (GPIO_PIN_7)
                    
extern void KEY_Init(void);			// ������ʼ��
extern u8 KEY_Scan(void);			// 10ms����1�Σ����ذ���ֵ
#define GET_KTYPE(k)	((k)&KTYPE_MASK)	// ��ȡ��������
#define GET_KCODE(k)	((k)&KCODE_MASK)	// ��ȡ����ֵ

#endif	/*_KEY_H_*/

