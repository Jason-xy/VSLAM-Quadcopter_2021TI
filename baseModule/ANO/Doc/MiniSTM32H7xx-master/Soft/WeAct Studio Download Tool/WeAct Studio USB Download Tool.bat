@echo off
title WeAct Studio USB Download Tool
cls
echo.
echo                            WeAct Studio USB Download Tool
echo.
echo --------------------------------------------------------------------------------------
echo -----                              如何进入ISP模式                               -----
echo -----                                                                            -----
echo ----- 方法1：上电状态下，按住BOOT0键和复位键，然后松开复位键，0.5秒后松开BOOT0键 -----
echo ----- 方法2：掉电状态下，按住BOOT0键，上电后0.5S松开BOOT0                        -----
echo --------------------------------------------------------------------------------------
echo.
echo ----- Hold down the BOOT0 key and Connect to the computer -----
echo ----- 按住BOOT0键，然后连接电脑 (方法2)                   -----
echo ----- 设备管理器会出现DFU设备                             -----
echo.
echo -------------------------------------------------------------
echo -----           Input Firmware Name                     -----
echo ----- 输入需要烧录固件的文件名或者将固件拖入到本窗口    -----
echo ----- 按回车开始下载，支持hex，bin,axf等文件格式        -----
echo ----- Example: firmware.bin                             -----
echo ----- Default Download Addr: 0x08000000                 -----
echo -------------------------------------------------------------
echo.

set /p Name=Firmware:

:start

.\STM32_Programmer_CLI\STM32_Programmer_CLI.exe --connect port=USB1 --download %Name% 0x08000000 --start

echo.
echo If the download fails, press any key to continue burning
echo If you do not need to continue burning, you can exit directly, press any key to continue burning
echo.
echo 如下载失败，按任意键继续烧录
echo 如不需要继续烧录，直接退出即可，按任意键继续烧录
echo.

pause
goto start
