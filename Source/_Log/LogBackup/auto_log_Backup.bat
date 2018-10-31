@echo off

SET FileDate=%date:-=%
SET H=%time:~0,2%
SET M=%time:~3,2%
SET S=%time:~6,2%
SET MS=%time:~9,2%

if "%H%"==" 0" set H=00
if "%H%"==" 1" set H=01
if "%H%"==" 2" set H=02
if "%H%"==" 3" set H=03
if "%H%"==" 4" set H=04
if "%H%"==" 5" set H=05
if "%H%"==" 6" set H=06
if "%H%"==" 7" set H=07
if "%H%"==" 8" set H=08
if "%H%"==" 9" set H=09

rem "로그압축"
CD /D "C:\Program Files\7-Zip"
7z.exe a -tzip "D:\ChartProjSvr\Source\_Log\LogBackup\LogBack_%FileDate%_%H%%M%%S%%MS%.zip" "D:\ChartProjSvr\Source\_Log\*.log"

rem "기존 로그 삭제"
del /q "D:\ChartProjSvr\Source\_Log\*.log"

call "D:\ChartProjSvr\Source\_Log\LogBackup\1month_log_del.bat"
exit