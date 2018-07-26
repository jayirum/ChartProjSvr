kill /f KRChart.exe
ping   127.0.0.1   -n 3 >   nul
kill /f KRFBOrd.exe
ping   127.0.0.1   -n 3 >   nul
kill /f ChartShmViewer.exe

NET STOP IRUM_ChartSHM