set ROOT_DIR=..\..
set SMC_HOME=%ROOT_DIR%\SMC_6_0_0
set SM_DIR=%ROOT_DIR%\doubango\sm
set SRC_DIR=%ROOT_DIR%\doubango\src

java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_info.sm
java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_invite.sm
java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_message.sm
java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_options.sm
java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_publish.sm
java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_register.sm
java -jar %SMC_HOME%\Smc.jar -c++ -suffix cxx -verbose -noex -d %SRC_DIR% %SM_DIR%\sm_dialog_subscribe.sm

pause