kill /f ChartMaker.exe
ping   127.0.0.1   -n 1 >   nul

kill /f ChartViewer.exe
ping   127.0.0.1   -n 1 >   nul

NET STOP IRUM_ChartShmDB


rem // �Ʒ��� sleep �� ���� �۾�
ping   127.0.0.1   -n 3 >   nul

NET START IRUM_ChartShmDB

ping   127.0.0.1   -n 3 >   nul

D:
cd D:\IRUM_VS\ChartProj\bin
start ChartMaker.exe