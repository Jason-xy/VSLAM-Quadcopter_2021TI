/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * ����    �������ƴ�
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ����    ����ѭ��
**********************************************************************************/
#include "SysConfig.h"
#include "Ano_Scheduler.h"
#include "WaypointControl.h"

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	while (1)
	{
		//��ϵͳ����󣬻���������ѭ��
	}
}
#endif
//=======================================================================================
//=======================================================================================
int main(void)
{
	//���������豸�ĳ�ʼ����������ʼ���������
	All_Init();
	//��������ʼ����ϵͳΪ�㱼�������˹�����һ��ʱ�ֵ�����
	Scheduler_Setup();
	//calibrate base yaw
	MyDelayMs(5000);
	t265_yaw_base = anof_yaw;
	set_x_position = t265_x_position;
	set_y_position = t265_y_position;
	set_z_position = t265_z_position;
	while (1)
	{
		//�������������������ϵͳ���ܣ������жϷ�������������������������
		Scheduler_Run();
	}
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
