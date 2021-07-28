@echo off
title WeAct Studio UART Download Tool
cls
echo.
echo                            WeAct Studio UART Download Tool
echo.
echo --------------------------------------------------------------------------------------
echo -----                              ��ν���ISPģʽ                               -----
echo -----                                                                            -----
echo ----- ����1���ϵ�״̬�£���סBOOT0���͸�λ����Ȼ���ɿ���λ����0.5����ɿ�BOOT0�� -----
echo ----- ����2������״̬�£���סBOOT0�����ϵ��0.5S�ɿ�BOOT0                        -----
echo --------------------------------------------------------------------------------------
echo.
echo -------------------------------------------------------------
echo -----           Input Firmware Name                     -----
echo -----      ����������Ҫ�Ͽ����İ�USB������              -----
echo ----- ������Ҫ��¼�̼����ļ������߽��̼����뵽������    -----
echo ----- ���س���ʼ���أ�֧��hex��bin,axf���ļ���ʽ        -----
echo ----- Example: firmware.bin                             -----
echo ----- Default Download Addr: 0x08000000                 -----
echo -------------------------------------------------------------
echo.

set /p Name=Firmware:
set /p UART=UART Port Num ���ں�(����COM22,����22����):

:start

.\STM32_Programmer_CLI\STM32_Programmer_CLI.exe --connect port=COM%UART% br=115200 --download %Name% 0x08000000 --start

echo.
echo If the download fails, press any key to continue burning
echo If you do not need to continue burning, you can exit directly, press any key to continue burning
echo.
echo ������ʧ�ܣ��������������¼
echo �粻��Ҫ������¼��ֱ���˳����ɣ��������������¼
echo.

pause
goto start