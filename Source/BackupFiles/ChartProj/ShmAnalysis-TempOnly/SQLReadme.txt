D:\IRUM_SVR\SOURCE\ChartProj\ShmAnalysis-TempOnly


[DBINFO]
DB_IP=211.247.235.45,9510
DB_ID=chartsa
DB_PWD=chartsairum!@#
DB_NAME=TRADE2010
DB_POOL_CNT=5


select artc_code from trade_security_artc where use_yn='Y'

EXEC CHART_GET_RECENTCHART_KEY 'CL', 12

select * from chart_data_min_01 where ARTC_CODE like 'CL'

EXEC CHART_GET_RECENTCHART_DATA 12, 'CLX7   12'

EXEC CHART_GET_RECENTCHART_DATA 0, 'GCZ7 00'