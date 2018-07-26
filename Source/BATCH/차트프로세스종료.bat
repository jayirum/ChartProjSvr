kill /f ChartMaker.exe
ping   127.0.0.1   -n 1 >   nul

kill /f ChartViewer.exe
ping   127.0.0.1   -n 1 >   nul

NET STOP IRUM_ChartShmDB
