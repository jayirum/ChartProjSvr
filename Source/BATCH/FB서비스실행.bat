kill /f KRFBOrd.exe
ping   127.0.0.1   -n 1 >   nul

kill /f KRChart.exe
ping   127.0.0.1   -n 1 >   nul

kill /f FBSocketSever.exe
ping   127.0.0.1   -n 3 >   nul

NET STOP IRUM_ChartSHM


rem // 아래는 sleep 을 위한 작업
ping   127.0.0.1   -n 3 >   nul

NET START IRUM_ChartSHM
NET START Apache2.4

ping   127.0.0.1   -n 10 >   nul

D:
cd D:\IRUM_VS\KR_Hedge\bin
start KRChart.exe

ping   127.0.0.1   -n 3 >   nul
start KRFBOrd.exe

ping   127.0.0.1   -n 3 >   nul
cd D:\FBSocketSever
start FBSocketSever.exe
