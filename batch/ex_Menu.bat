
%rem This guy: http://www.infionline.net/~wtnewton/batch/batguide.html
@echo off
:menu
c:
cd\games
cls
echo    GAMES MENU
echo    ==========
echo.
echo    1 - Doom
echo    2 - Raptor
echo    3 - FlightSim
echo    4 - Lotus
echo.
echo    Q - Quit
choice /c:1234Q>nul
if errorlevel 5 goto done
if errorlevel 4 goto lotus
if errorlevel 3 goto flightsim
if errorlevel 2 goto raptor
if errorlevel 1 goto doom

rem http://www.robvanderwoude.com/errorlevel.php
rem In Windows NT 4/2000/XP this may sometimes fail, since some executables return negative numbers for errorlevels!

echo CHOICE missing
goto done
 
:doom
echo.
echo You Pressed:
echo doom
echo.
pause
goto menu
 
:raptor
echo.
echo You Pressed:
echo raptor
echo.
pause
goto menu
 
:flightsim
echo.
echo You Pressed:
echo flightsime
echo.
pause 
goto menu
 
:lotus
echo.
echo You Pressed:
echo lotus
echo.
pause
goto menu
 
:done