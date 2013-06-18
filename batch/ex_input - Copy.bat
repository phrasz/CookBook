@echo off
%rem Add the Echo off in order to supress commands sent to the system 
%rem The Following Shows how to get input from the user...
ECHO Enter some input, then press F6 and press Enter when ready...
COPY CON USRINPUT.TMP
echo.
ECHO You typed:
TYPE USRINPUT.TMP
echo.  
echo.  
pause