kill /f ChartMaker.exe
ping   127.0.0.1   -n 1 >   nul

kill /f ChartViewer.exe
ping   127.0.0.1   -n 1 >   nul

NET STOP IRUM_ChartShmDB


rem // 아래는 sleep 을 위한 작업
ping   127.0.0.1   -n 3 >   nul

NET START IRUM_ChartShmDB

ping   127.0.0.1   -n 3 >   nul

D:
cd D:\IRUM_VS\ChartProj\bin
start ChartMaker.exe