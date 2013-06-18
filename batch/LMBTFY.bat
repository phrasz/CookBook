
%rem This guy: http://www.infionline.net/~wtnewton/batch/batguide.html
@echo off
:menu
c:
cd\games
cls
echo  _     __  __ ____ _____ _______   __
echo ^| ^|   ^|  \/  ^| __ )_   _^|  ___\ \ / /
echo ^| ^|   ^| ^|\/^| ^|  _ \ ^| ^| ^| ^|_   \ ` / 
echo ^| ^|___^| ^|  ^| ^| ^|_) ^|^| ^| ^|  _^|   ^| ^|  
echo ^|_____^|_^|  ^|_^|____/ ^|_^| ^|_^|     ^|_^|  
echo ======================================
echo.
echo Let Me Batch That For You ...
echo.
echo  Networking:
echo  -----------
echo  1 - Routes (Print the current routes)
echo  2 - Netstat (Look at current Net Connections)
echo  3 - NSLookUp (Look up the name server)
echo  4 - Trace Route (Find the route to an IP)
echo  5 - Ping
echo  6 - Find Files with Tree
echo.
echo  Q - Quit
choice /c:123456Q>nul
if errorlevel 7 goto done
if errorlevel 6 goto tree
if errorlevel 5 goto ping
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
route PRINT
echo.
pause
goto menu
 
:raptor
echo.
echo You Pressed:
echo raptor
netstat -an
echo.
pause
goto menu
 
:flightsim
echo.
echo TREES!!!! What would you like to find?
ECHO Enter some input, then press F6 and press Enter when ready...
COPY CON USRINPUT.TMP
echo.
ECHO You typed:
TYPE USRINPUT.TMP
echo. 
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
 
:ping
ECHO Enter the IP you would like to ping:
SET /P tmpvar=
ping %tmpvar%
SET tmpvar= 
pause
goto menu

:tree
ECHO Please enter your Items with location you'd like to find:
SET /P tmpvar=
ECHO Treeing like a lumberjack...
tree c:\ /A /F | findstr /c:\ /c:+ /c:%tmpvar% >>%CD%\tree_.txt
pause
goto menu 
 
:done