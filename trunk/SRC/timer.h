#ifndef _TIMER_LXF_H
#define _TIMER_LXF_H

#define	FOSC    (11059200UL)
#define N       256
/* 
 * ��ģ��ʹ��˵����
 * ����timer_init()��ʼ����ʱ����Դ�󣬲�ѯʱ���־
*/
extern volatile unsigned char ftimer2ms_ok;
extern volatile unsigned char ftimer10ms_ok;
extern volatile unsigned char ftimer1s_ok;

extern void timer_init(void);

#endif
