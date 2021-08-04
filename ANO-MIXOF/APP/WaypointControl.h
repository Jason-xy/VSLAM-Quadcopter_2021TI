#ifndef __WAYPOINTCONTROL_H__
#define __WAYPOINTCONTROL_H__
#include "WaypointControl.h"
#include "MoveControl.h"
#include "Ano_Math.h"
extern int16_t t265_x_position, t265_y_position, t265_z_position;
extern int16_t dif_x_position, dif_y_position, dif_z_position;
extern int16_t anof_pitch, anof_roll, anof_yaw;
extern int16_t t265_yaw_base, dif_yaw;
extern uint16_t dir_angle_0_360;
extern int16_t set_x_position, set_y_position, set_z_position;

void Position_calibration(void);

#endif // __WAYPOINTCONTROL_H__