#ifndef _UART_H
#define _UART_H

pdata u8 Rx_buf[40],Tx_buf[40];

void UartInit(void)		//1200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0x00;		//�趨��ʱ��ֵ
	TH1 = 0xF7;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

	ES=1;
	EA=1;
}



#endif