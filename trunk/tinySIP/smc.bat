@echo off

REM SMC (The State Machine Compiler)
REM For more information about SMC: http://smc.sourceforge.net/

set TSIP_SMC_SRC=%DOUBANGO_HOME%\tinySIP\source\smc
set TSIP_SMC_SRC_TMP=%TSIP_SMC_SRC%\tmp
set TSIP_SMC_INC=%DOUBANGO_HOME%\tinySIP\include\tinysip\smc
set TSIP_SMC_SM=%DOUBANGO_HOME%\tinySIP\smc
set TSIP_SMC_GRAPH=%DOUBANGO_HOME%\tinySIP\graph

set DOT_DIR="C:\Program Files\Graphviz2.24\bin"

set TOOLS=%DOUBANGO_HOME%\thirdparties\win32\tools

MKDIR %TSIP_SMC_SRC_TMP%

FOR /f %%i IN ('dir /b %TSIP_SMC_SM%\*.sm') DO (
java -jar %DOUBANGO_HOME%\SMC_6_0_0\Smc.jar -c -d %TSIP_SMC_SRC_TMP% -headerd %TSIP_SMC_INC% %TSIP_SMC_SM%\%%i
%TOOLS%\BatchSubstitute.bat "C:\Projects\Doubango\tinySIP\include\tinysip\smc\" "tinysip/smc/" %TSIP_SMC_SRC_TMP%\%%~ni_sm.c > %TSIP_SMC_SRC%\%%~ni_sm.c

REM java -jar %DOUBANGO_HOME%\SMC_6_0_0\Smc.jar -graph -glevel 2 -d %TSIP_SMC_GRAPH% %TSIP_SMC_SM%\%%i
REM %DOT_DIR%\dot -Tpng -o %TSIP_SMC_GRAPH%\%%~ni_sm.png %TSIP_SMC_GRAPH%\%%~ni_sm.dot
)
RMDIR /S /Q %TSIP_SMC_SRC_TMP%