kill /f KRChart.exe

rem // �Ʒ��� sleep �� ���� �۾�
ping   127.0.0.1   -n 2 >   nul

NET START IRUM_ChartSHM
ping   127.0.0.1   -n 3 >   nul


D:
cd D:\IRUM_VS\KR_Hedge\bin


start KRChart.exe
