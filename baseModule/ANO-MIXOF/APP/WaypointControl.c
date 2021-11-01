#include "WaypointControl.h"
int16_t t265_yaw_base = 0, dif_yaw = 0;
int16_t anof_pitch = 0, anof_roll = 0, anof_yaw = 0;
int16_t set_x_position = 0, set_y_position = 0, set_z_position = 0;
static int16_t dif_x_position = 0, dif_y_position = 0, dif_z_position = 0;

void Position_calibration(void)
{
	uint16_t dir_angle_0_360 = 0;
	int16_t ano_dir = 0, base_dir = 0;
	int16_t distance = 0;
  dif_x_position = - set_x_position + t265_x_position;
  dif_y_position = - set_y_position + t265_y_position;
  dif_z_position = - set_z_position + t265_z_position;
	dif_yaw = - anof_yaw + t265_yaw_base;
	
	//distance
	distance = my_sqrt(dif_x_position * dif_x_position + dif_y_position * dif_y_position);
	
	//base dir
	if(dif_y_position > 0 && dif_x_position > 0)
	{
		if(dif_x_position > dif_y_position)
		{
			if(dif_x_position == 0)
				base_dir = 180;
			else 
				base_dir = 180 - fast_atan_table[(int)(256 * (float)dif_y_position / (float)dif_x_position)] * DEG_PER_RAD;
		}
		else
		{
			if(dif_y_position == 0)
				base_dir = 90;
			else 
				base_dir = 90 + fast_atan_table[(int)(256 * (float)dif_x_position / (float)dif_y_position)] * DEG_PER_RAD;
		}
		
	}
	else if(dif_y_position < 0 && dif_x_position > 0)
	{
		if(dif_x_position > -dif_y_position)
		{
			if(dif_x_position == 0)
				base_dir = 180;
			else 
				base_dir = 180 + fast_atan_table[(int)(256 * (float)-dif_y_position / (float)dif_x_position)] * DEG_PER_RAD;
		}
		else
		{
			if(dif_y_position == 0)
				base_dir = 270;
			else 
				base_dir = 270 - fast_atan_table[(int)(256 * (float)dif_x_position / (float)dif_y_position)] * DEG_PER_RAD;
		}
	}
	else if(dif_y_position > 0 && dif_x_position < 0)
	{
		if(-dif_x_position > dif_y_position)
		{
			if(dif_x_position == 0)
				base_dir = 0;
			else 
				base_dir = 0 + fast_atan_table[(int)(256 * (float)dif_y_position / (float)-dif_x_position)] * DEG_PER_RAD;
		}
		else
		{
			if(dif_y_position == 0)
				base_dir = 90;
			else 
				base_dir = 90 - fast_atan_table[(int)(256 * (float)-dif_x_position / (float)dif_y_position)] * DEG_PER_RAD;
		}			
	}
	else if(dif_y_position <= 0 && dif_x_position <= 0)
	{
		if(-dif_x_position > -dif_y_position)
		{
			if(dif_x_position == 0)
				base_dir = 360;
			else
				base_dir = 360 - fast_atan_table[(int)(256 * (float)-dif_y_position / (float)-dif_x_position)] * DEG_PER_RAD;
		}
		else
		{
			if(dif_y_position == 0)
				base_dir = 270;
			else 
				base_dir = 270 + fast_atan_table[(int)(256 * (float)-dif_x_position / (float)-dif_y_position)] * DEG_PER_RAD;
		}
	}
	//ano dir
	if(dif_yaw < 0)
		ano_dir = 360 + dif_yaw;
	else
		ano_dir = dif_yaw;
	//dir_angle_0_360
	if(base_dir - ano_dir >= 0)
		dir_angle_0_360 = base_dir - ano_dir;
	else
		dir_angle_0_360 = - base_dir + ano_dir;
	
	Horizontal_Move(distance, distance * 0.2, base_dir);
	
}
