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
echo -----             Flash Erase All                       -----
echo -----      ���ڲ�����Ҫ�Ͽ����İ�USB������              -----
echo -----    ���봮�ںţ����س��Ϳ��Կ�ʼȫ�̲���           -----
echo -------------------------------------------------------------
echo.

set /p UART=UART Port Num ���ں�(����COM22,����22����):

:start

.\STM32_Programmer_CLI\STM32_Programmer_CLI.exe --connect port=COM%UART% br=115200 -rdu --erase all

echo.
echo If erase fails, press any key to erase again
echo If you do not need to continue erasing, simply exit. Press any key to continue erasing
echo.
echo �����ʧ�ܣ�����������²���
echo �粻��Ҫ����������ֱ���˳����ɣ����������������
echo.

pause
goto start