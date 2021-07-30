/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * ×÷Õß    £ºÄäÃû¿Æ´´
 * ¹ÙÍø    £ºwww.anotc.com
 * ÌÔ±¦    £ºanotc.taobao.com
 * ¼¼ÊõQÈº £º190169595
 * ÃèÊö    £º·É¿Ø³õÊ¼»¯
**********************************************************************************/
#include "Drv_BSP.h"
#include "Drv_PwmOut.h"
#include "Drv_led.h"
#include "Drv_adc.h"
#include "Drv_RcIn.h"
#include "Drv_Timer.h"
#include "ANO_DT_LX.h"
#include "Drv_UbloxGPS.h"
#include "Drv_Uart.h"
#include "Drv_Timer.h"

u8 All_Init()
{
	DrvSysInit();
	//ÑÓÊ±
	MyDelayMs(100);
	
	//USART 2 Init
	DrvUart2Init(115200);
	//USART 3 Init¯
	DrvUart3Init(115200);
	//USART 4 Init
	DrvUart4Init(115200);
	//USART 5 Init
	DrvUart5Init(115200);
	//USART 6 Init
	DrvUart6Init(115200);
	//USART 7 Init
	DrvUart7Init(115200);
	MyDelayMs(100);
	
	//³õÊ¼»¯¶¨Ê±ÖÐ¶Ï
	DrvTimerFcInit();
	//³õÊ¼»¯Íê³É£¬·µ»Ø1
	return (1);
}

_rc_input_st rc_in;
void DrvRcInputInit(void)
{
	//ÈÎÒâ³õÊ¼»¯Ò»¸öÄ£Ê½
	DrvRcPpmInit();
	//DrvRcSbusInit();

	//ÏÈ±ê¼ÇÎ»¶ªÊ§
	rc_in.no_signal = 1;
}
void DrvPpmGetOneCh(u16 data)
{
	static u8 ch_sta = 0;
	if ((data > 2500 && ch_sta > 3) || ch_sta == 10)
	{
		ch_sta = 0;
		rc_in.signal_cnt_tmp++;
		rc_in.rc_in_mode_tmp = 1; //ÇÐ»»Ä£Ê½±ê¼ÇÎªppm
	}
	else if (data > 300 && data < 3000) //Òì³£µÄÂö³å¹ýÂËµô
	{
		//
		rc_in.ppm_ch[ch_sta] = data;
		ch_sta++;
	}
}
void DrvSbusGetOneByte(u8 data)
{
/*
sbus flagsµÄ½á¹¹ÈçÏÂËùÊ¾£º
flags£º
bit7 = ch17 = digital channel (0x80)
bit6 = ch18 = digital channel (0x40)
bit5 = Frame lost, equivalent red LED on receiver (0x20)
bit4 = failsafe activated (0x10) b: 0001 0000
bit3 = n/a
bit2 = n/a
bit1 = n/a
bit0 = n/a
*/
	const u8 frame_end[4] = {0x04, 0x14, 0x24, 0x34};
	static u32 sbus_time[2];
	static u8 datatmp[25];
	static u8 cnt = 0;
	static u8 frame_cnt;
	//
	sbus_time[0] = sbus_time[1];
	sbus_time[1] = GetSysRunTimeUs();
	if ((u32)(sbus_time[1] - sbus_time[0]) > 2500)
	{
		cnt = 0;
	}
	//
	datatmp[cnt++] = data;
	//
	if (cnt == 25)
	{
		cnt = 24;
		//
		//if(datatmp[0] == 0x0F && (datatmp[24] == 0x00))
		//if(datatmp[0] == 0x0F && ((datatmp[24] == 0x00)||(datatmp[24] == 0x04)||(datatmp[24] == 0x14)||(datatmp[24] == 0x24)||(datatmp[24] == 0x34)))
		if ((datatmp[0] == 0x0F && (datatmp[24] == 0x00 || datatmp[24] == frame_end[frame_cnt])))
		{
			cnt = 0;
			rc_in.sbus_ch[0] = (s16)(datatmp[2] & 0x07) << 8 | datatmp[1];
			rc_in.sbus_ch[1] = (s16)(datatmp[3] & 0x3f) << 5 | (datatmp[2] >> 3);
			rc_in.sbus_ch[2] = (s16)(datatmp[5] & 0x01) << 10 | ((s16)datatmp[4] << 2) | (datatmp[3] >> 6);
			rc_in.sbus_ch[3] = (s16)(datatmp[6] & 0x0F) << 7 | (datatmp[5] >> 1);
			rc_in.sbus_ch[4] = (s16)(datatmp[7] & 0x7F) << 4 | (datatmp[6] >> 4);
			rc_in.sbus_ch[5] = (s16)(datatmp[9] & 0x03) << 9 | ((s16)datatmp[8] << 1) | (datatmp[7] >> 7);
			rc_in.sbus_ch[6] = (s16)(datatmp[10] & 0x1F) << 6 | (datatmp[9] >> 2);
			rc_in.sbus_ch[7] = (s16)datatmp[11] << 3 | (datatmp[10] >> 5);

			rc_in.sbus_ch[8] = (s16)(datatmp[13] & 0x07) << 8 | datatmp[12];
			rc_in.sbus_ch[9] = (s16)(datatmp[14] & 0x3f) << 5 | (datatmp[13] >> 3);
			rc_in.sbus_ch[10] = (s16)(datatmp[16] & 0x01) << 10 | ((s16)datatmp[15] << 2) | (datatmp[14] >> 6);
			rc_in.sbus_ch[11] = (s16)(datatmp[17] & 0x0F) << 7 | (datatmp[16] >> 1);
			rc_in.sbus_ch[12] = (s16)(datatmp[18] & 0x7F) << 4 | (datatmp[17] >> 4);
			rc_in.sbus_ch[13] = (s16)(datatmp[20] & 0x03) << 9 | ((s16)datatmp[19] << 1) | (datatmp[18] >> 7);
			rc_in.sbus_ch[14] = (s16)(datatmp[21] & 0x1F) << 6 | (datatmp[20] >> 2);
			rc_in.sbus_ch[15] = (s16)datatmp[22] << 3 | (datatmp[21] >> 5);
			rc_in.sbus_flag = datatmp[23];

			//user
			//
			if (rc_in.sbus_flag & 0x08)
			{
				//Èç¹ûÓÐÊý¾ÝÇÒÄÜ½ÓÊÕµ½ÓÐÊ§¿Ø±ê¼Ç£¬Ôò²»´¦Àí£¬×ª¼Þ³ÉÎÞÊý¾ÝÊ§¿Ø¡£
			}
			else
			{
				rc_in.signal_cnt_tmp++;
				rc_in.rc_in_mode_tmp = 2; //ÇÐ»»Ä£Ê½±ê¼ÇÎªsbus
			}
			//Ö¡Î²´¦Àí
			frame_cnt++;
			frame_cnt %= 4;
		}
		else
		{
			for (u8 i = 0; i < 24; i++)
			{
				datatmp[i] = datatmp[i + 1];
			}
		}
	}
}
static void rcSignalCheck(float *dT_s)
{
	//
	static u8 cnt_tmp;
	static u16 time_dly;
	time_dly += (*dT_s) * 1e3f;
	//==1000ms==
	if (time_dly > 1000)
	{
		time_dly = 0;
		//
		rc_in.signal_fre = rc_in.signal_cnt_tmp;

		//==ÅÐ¶ÏÐÅºÅÊÇ·ñ¶ªÊ§
		if (rc_in.signal_fre < 5)
		{
			rc_in.no_signal = 1;
		}
		else
		{
			rc_in.no_signal = 0;
		}
		//==ÅÐ¶ÏÊÇ·ñÇÐ»»ÊäÈë·½Ê½
		if (rc_in.no_signal)
		{
			//³õÊ¼0
			if (rc_in.sig_mode == 0)
			{
				cnt_tmp++;
				cnt_tmp %= 2;
				if (cnt_tmp == 1)
				{
					DrvRcSbusInit();
				}
				else
				{
					DrvRcPpmInit();
				}
			}
		}
		else
		{
			rc_in.sig_mode = rc_in.rc_in_mode_tmp;
		}
		//==
		rc_in.signal_cnt_tmp = 0;
	}
}

void DrvRcInputTask(float dT_s)
{
	//ÐÅºÅ¼ì²â
	rcSignalCheck(&dT_s);
	//ÓÐÐÅºÅ
	if (rc_in.no_signal == 0)
	{
		//ppm
		if (rc_in.sig_mode == 1)
		{
			for (u8 i = 0; i < 10; i++) //×¢ÒâÖ»ÓÐ10¸öÍ¨µÀ
			{
				rc_in.rc_ch.st_data.ch_[i] = rc_in.ppm_ch[i]; 
			}
		}
		//sbus
		else if (rc_in.sig_mode == 2)
		{
			for (u8 i = 0; i < 10; i++) //×¢ÒâÖ»ÓÐ10¸öÍ¨µÀ
			{
				rc_in.rc_ch.st_data.ch_[i] = 0.644f * (rc_in.sbus_ch[i] - 1024) + 1500; //248 --1024 --1800×ª»»µ½1000-2000
			}
		}
		//¼ì²éÊ§¿Ø±£»¤ÉèÖÃ
		if (
			(rc_in.rc_ch.st_data.ch_[ch_5_aux1] > 1200 && rc_in.rc_ch.st_data.ch_[ch_5_aux1] < 1400) || (rc_in.rc_ch.st_data.ch_[ch_5_aux1] > 1600 && rc_in.rc_ch.st_data.ch_[ch_5_aux1] < 1800))
		{
			//Âú×ãÉèÖÃ£¬±ê¼ÇÎªÊ§¿Ø
			rc_in.fail_safe = 1;
		}
		else
		{
			rc_in.fail_safe = 0;
		}
	}
	//ÎÞÐÅºÅ
	else
	{
		//Ê§¿Ø±ê¼ÇÖÃÎ»
		rc_in.fail_safe = 1;
		//
		for (u8 i = 0; i < 10; i++) //×¢ÒâÖ»ÓÐ10¸öÍ¨µÀ
		{
			rc_in.rc_ch.st_data.ch_[i] = 0; //
		}
	}
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
