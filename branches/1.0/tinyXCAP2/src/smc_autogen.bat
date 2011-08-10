set ROOT_DIR=..\..
set SMC_HOME=%ROOT_DIR%\SMC_6_0_0
set SRC_DIR=%ROOT_DIR%\tinyXCAP\src
set GRAPH_DIR=%ROOT_DIR%\tinyXCAP\graph
set DOT_DIR="C:\Program Files\Graphviz2.24\bin"

java -jar %SMC_HOME%\Smc.jar -c -verbose -d %SRC_DIR% %SRC_DIR%\txc_client.sm
java -jar %SMC_HOME%\Smc.jar -graph -glevel 2 -verbose -d %GRAPH_DIR% %SRC_DIR%\txc_client.sm

%DOT_DIR%\dot -Tpng -o %GRAPH_DIR%\txc_client_sm.png %GRAPH_DIR%\txc_client_sm.dot