@echo off
title WeAct Studio UART Download Tool
cls
echo.
echo                            WeAct Studio UART Download Tool
echo.
echo --------------------------------------------------------------------------------------
echo -----                              如何进入ISP模式                               -----
echo -----                                                                            -----
echo ----- 方法1：上电状态下，按住BOOT0键和复位键，然后松开复位键，0.5秒后松开BOOT0键 -----
echo ----- 方法2：掉电状态下，按住BOOT0键，上电后0.5S松开BOOT0                        -----
echo --------------------------------------------------------------------------------------
echo.
echo -------------------------------------------------------------
echo -----             Flash Erase All                       -----
echo -----      串口擦除需要断开核心板USB的连接              -----
echo -----    输入串口号，按回车就可以开始全盘擦除           -----
echo -------------------------------------------------------------
echo.

set /p UART=UART Port Num 串口号(例如COM22,输入22即可):

:start

.\STM32_Programmer_CLI\STM32_Programmer_CLI.exe --connect port=COM%UART% br=115200 -rdu --erase all

echo.
echo If erase fails, press any key to erase again
echo If you do not need to continue erasing, simply exit. Press any key to continue erasing
echo.
echo 如擦除失败，按任意键重新擦除
echo 如不需要继续擦除，直接退出即可，按任意键继续擦除
echo.

pause
goto start