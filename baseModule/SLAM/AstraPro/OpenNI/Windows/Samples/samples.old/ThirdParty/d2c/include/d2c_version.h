/*****************************************************************************
 *  Orbbec Calibration
 *  Copyright (C) 2017 by ORBBEC Technology., Inc.
 *
 *  This file is part of Orbbec Calibration
 *
 *  This file belongs to ORBBEC Technology., Inc.
 *  It is considered a trade secret, and is not to be divulged or used by
 * parties who have NOT received written authorization from the owner.
 *
 *  Description
 ****************************************************************************/
#ifndef __DEPTH_TO_COLOR_D2C_VERSION_H__
#define __DEPTH_TO_COLOR_D2C_VERSION_H__

#define D2C_VERSION_EPOCH 0
#define D2C_VERSION_MAJOR 3
#define D2C_VERSION_MINOR 1

#define D2C_STR_EXP(__A)    #__A
#define D2C_STR(__A)        D2C_STR_EXP(__A)

#define D2C_STRW_EXP(__A)   L#__A
#define D2C_STRW(__A)       D2C_STRW_EXP(__A)

#define D2C_VERSION     D2C_STR(D2C_VERSION_EPOCH) "." D2C_STR(D2C_VERSION_MAJOR) "." D2C_STR(D2C_VERSION_MINOR)

#endif //__DEPTH_TO_COLOR_D2C_VERSION_H__

/*
V0.3.1: 1:fix Memory Overflow & access to cross-border

*/
