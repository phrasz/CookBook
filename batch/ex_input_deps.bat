echo Enter filename...
set return=here
goto input
:here
echo You entered %str%
set return=
goto done
 
:: above input routine
:input
:: ...
goto %return%
 
:done

