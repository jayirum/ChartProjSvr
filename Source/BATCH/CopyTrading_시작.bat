kill /f HDCopyTarget.exe
ping   127.0.0.1   -n 1 >   nul

kill /f KRCopyOrder.exe
ping   127.0.0.1   -n 1 >   nul


rem // �Ʒ��� sleep �� ���� �۾�
ping   127.0.0.1   -n 3 >   nul

D:
cd D:\IRUM_VS\HDAPI\bin
start HDCopyTarget.exe


ping   127.0.0.1   -n 3 >   nul
cd D:\IRUM_VS\KR_Hedge\bin
start KRCopyOrder.exe
