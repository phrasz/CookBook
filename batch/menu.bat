@echo off
:menu
c:
cls
echo MY FIRST MENU
echo =============
echo.
echo 1-ping
echo 2-ipconfig
echo 3-netstat
echo 4-tree
echo.
echo Q-quit

choice /c:1234Q>nul
if errorlevel 5 goto done
if errorlevel 4 goto tree
if errorlevel 3 goto netstat
if errorlevel 2 goto ipconfig
if errorlevel 1 goto ping

echo CHOICE missing
goto done

:ping
cd ping
ping
pause
goto menu


:ipconfig
cd ipconfig
ipconfig
pause
goto menu


:netstat
cd netstat
netstat
pause
goto menu


:tree
cd tree
tree
pause
goto menu

:done
