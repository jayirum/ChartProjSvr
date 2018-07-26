SETLOCAL
set targetdir=.
forfiles /P .  /m *.log /d -5 /c "cmd /c del @FILE"