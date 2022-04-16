:: COMPILERS COURSE - SCRIPT ---------------------------------------------
:: SCRIPT A32 - CST8152 - Winter 2022

CLS
SET COMPILER=Compiler.exe

SET FILE1=OSLO_Empty
SET FILE2=OSLO_Hello
SET FILE3=OSLO_Volume


SET EXTENSION=osf
SET OUTPUT=out
SET ERROR=err

SET PARAM=3

if exist Compiler.exe (
    del Compiler.exe
)
:: ---------------------------------------------------------------------
:: Begin of Tests (A32 - W22) ------------------------------------------
:: ---------------------------------------------------------------------

ren *.exe %COMPILER%

::
:: BASIC TESTS  ----------------------------------------------------------
::
:: Basic Tests (A32 - W22) - - - - - - - - - - - - - - - - - - - - - -

%COMPILER% %PARAM% %FILE1%.%EXTENSION%	> %FILE1%.%OUTPUT%	2> %FILE1%.%ERROR%
%COMPILER% %PARAM% %FILE2%.%EXTENSION%	> %FILE2%.%OUTPUT%	2> %FILE2%.%ERROR%
%COMPILER% %PARAM% %FILE3%.%EXTENSION%	> %FILE3%.%OUTPUT%	2> %FILE3%.%ERROR%

:: SHOW OUTPUTS - - - - - - - - - - - - - - - - - - - - - - - - - - -
DIR *.OUT
DIR *.ERR

:: ---------------------------------------------------------------------
:: End of Tests (A32 - W22) --------------------------------------------
:: ---------------------------------------------------------------------
