@echo off
ECHO Enter the IP you would like to ping:
SET /P tmpvar=
ping %tmpvar%
SET tmpvar= 
pause