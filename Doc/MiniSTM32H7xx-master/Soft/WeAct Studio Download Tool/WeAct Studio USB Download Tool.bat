@echo off
title WeAct Studio USB Download Tool
cls
echo.
echo                            WeAct Studio USB Download Tool
echo.
echo --------------------------------------------------------------------------------------
echo -----                              ��ν���ISPģʽ                               -----
echo -----                                                                            -----
echo ----- ����1���ϵ�״̬�£���סBOOT0���͸�λ����Ȼ���ɿ���λ����0.5����ɿ�BOOT0�� -----
echo ----- ����2������״̬�£���סBOOT0�����ϵ��0.5S�ɿ�BOOT0                        -----
echo --------------------------------------------------------------------------------------
echo.
echo ----- Hold down the BOOT0 key and Connect to the computer -----
echo ----- ��סBOOT0����Ȼ�����ӵ��� (����2)                   -----
echo ----- �豸�����������DFU�豸                             -----
echo.
echo -------------------------------------------------------------
echo -----           Input Firmware Name                     -----
echo ----- ������Ҫ��¼�̼����ļ������߽��̼����뵽������    -----
echo ----- ���س���ʼ���أ�֧��hex��bin,axf���ļ���ʽ        -----
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
echo ������ʧ�ܣ��������������¼
echo �粻��Ҫ������¼��ֱ���˳����ɣ��������������¼
echo.

pause
goto start
