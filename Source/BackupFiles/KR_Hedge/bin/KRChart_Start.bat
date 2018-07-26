kill /f KRChart.exe

rem // 아래는 sleep 을 위한 작업
ping   127.0.0.1   -n 2 >   nul

NET START IRUM_ChartSHM
ping   127.0.0.1   -n 3 >   nul


D:
cd D:\IRUM_VS\KR_Hedge\bin


start KRChart.exe
