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
echo ----- Hold down the BOOT0 key and Connect to the computer -----
echo ----- ��סBOOT0����Ȼ�����ӵ��� (����2)                   -----
echo ----- �豸�����������DFU�豸                             -----
echo.
echo -------------------------------------------------------------
echo -----             Flash Erase All                       -----
echo -----         ���س��Ϳ��Կ�ʼȫ�̲���                  -----
echo -------------------------------------------------------------
echo.

echo ���������ʼ����
pause

:start

.\STM32_Programmer_CLI\STM32_Programmer_CLI.exe --connect port=USB1 -rdu --erase all

echo.
echo If erase fails, press any key to erase again
echo If you do not need to continue erasing, simply exit. Press any key to continue erasing
echo.
echo �����ʧ�ܣ�����������²���
echo �粻��Ҫ����������ֱ���˳����ɣ����������������
echo.

pause
goto start