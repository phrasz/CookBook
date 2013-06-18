@echo off

%rem Busted Ass DOS (ALL VERSIONS OF WINDOWS) Version
%rem Add the Echo off in order to supress commands sent to the system 
%rem The Following Shows how to get input from the user...
%rem ECHO Enter some input, then press F6 and press Enter when ready...
%rem COPY CON USRINPUT.TMP
%rem echo.
%rem ECHO You typed:
%rem TYPE USRINPUT.TMP
%rem echo.  

%rem Windows 2000/XP/Vista/7
%rem on 7, with @echo off you'll see: "ECHO is off" when the varaible is empty
echo This is tmpvar:
echo %tmpvar%
echo.
ECHO Enter some input, then press Enter when ready...
SET /P tmpvar=
echo This is tmpvar:
echo %tmpvar%
echo.
echo Clearing tmpvar...
SET tmpvar= 
echo This is tmpvar:
echo %tmpvar%
echo.
pause