<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>tinySMS API Overview</title>
    <filename>index</filename>
  </compound>
  <compound kind="file">
    <name>tinysms.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/</path>
    <filename>tinysms_8h</filename>
    <includes id="tsms__packing_8h" name="tsms_packing.h" local="yes" imported="no">tinysms/tsms_packing.h</includes>
    <includes id="tsms__rpdu_8h" name="tsms_rpdu.h" local="yes" imported="no">tinysms/rpdu/tsms_rpdu.h</includes>
    <includes id="tsms__tpdu__submit_8h" name="tsms_tpdu_submit.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_submit.h</includes>
    <includes id="tsms__tpdu__deliver_8h" name="tsms_tpdu_deliver.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_deliver.h</includes>
    <includes id="tsms__tpdu__report_8h" name="tsms_tpdu_report.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_report.h</includes>
    <includes id="tsms__tpdu__command_8h" name="tsms_tpdu_command.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_command.h</includes>
    <includes id="tsms__tpdu__status__report_8h" name="tsms_tpdu_status_report.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_status_report.h</includes>
  </compound>
  <compound kind="file">
    <name>tsms_rpdu.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/rpdu/</path>
    <filename>tsms__rpdu_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <class kind="struct">tsms_rpdu_data_s</class>
    <class kind="struct">tsms_rpdu_smma_s</class>
    <class kind="struct">tsms_rpdu_ack_s</class>
    <class kind="struct">tsms_rpdu_error_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_RPDU_DATA</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>ad910932f78e7d1caf782e785a0fdfa19</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_data_create_mo</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga4cb5e1528206b1166b0bc6f888d58390</anchor>
      <arglist>(mr, smsc, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_data_create_mt</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gabe4218acfa2ef004520c11f430867268</anchor>
      <arglist>(mr, smsc, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_data_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gafa8f26571bb60a110a2b7480b85ede5f</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_RPDU_SMMA</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>ac6b65c3afdc159a4846348a8d9d88185</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_smma_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gabc0ee8d0cb353fb39d5cab752142bcbb</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_RPDU_ACK</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>a01b3dcaae2740383a10852db7b5b99dc</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_ack_create_mo</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga69f8c3b283c13bfa92be006b152cc99e</anchor>
      <arglist>(mr, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_ack_create_mt</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gabc26f8ee68c9f42555be07cb81ca32dd</anchor>
      <arglist>(mr, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_ack_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga2aeb9084a881e35c3b1eadd082e764d3</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_RPDU_ERROR</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>a284e1c256fbf810184985ec568e9df91</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_error_create_mo</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga2cba98d965737f6ada9d012d21e82b59</anchor>
      <arglist>(mr, tpdu, cause)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_error_create_mt</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga069e8b8cdda29fa50adaae3b7365a791</anchor>
      <arglist>(mr, tpdu, cause)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_error_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gac4dac7b6e1839f32cee2e39d85ac5730</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS struct tsms_rpdu_data_s</type>
      <name>tsms_rpdu_data_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>a63ee2637de1a5e868f1635aceecc340e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_rpdu_smma_s</type>
      <name>tsms_rpdu_smma_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>aacacdf9ae6b7dc39e1cd3f8b8d12fe22</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_rpdu_ack_s</type>
      <name>tsms_rpdu_ack_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>aaf5b14bfcdeee6e3ae8dc73e7f7d2b0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_rpdu_error_s</type>
      <name>tsms_rpdu_error_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>acaa94a80785d1db708ae6f568da71f8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_rpdu_data_t *</type>
      <name>tsms_rpdu_data_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga5589e131deeab1e25eb08494f7554d55</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_tpdu_message_t *tpdu, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_rpdu_smma_t *</type>
      <name>tsms_rpdu_smma_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gac588512623e47fdfdbac5cd0a387572a</anchor>
      <arglist>(uint8_t mr)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_rpdu_ack_t *</type>
      <name>tsms_rpdu_ack_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga1464b066b6b93ed848049fb80ba4d4a5</anchor>
      <arglist>(uint8_t mr, const tsms_tpdu_message_t *tpdu, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_rpdu_error_t *</type>
      <name>tsms_rpdu_error_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga445ece4fe91ccafe88343cf6e1f1bf30</anchor>
      <arglist>(uint8_t mr, const tsms_tpdu_message_t *tpdu, uint8_t cause, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_rpdu_data_def_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>ab4eff04efc276dc095c78e9d0a3a268b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_rpdu_smma_def_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>a18ba242809e6ce1a059813d1bd18a159</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_rpdu_ack_def_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>aa716e7bcbbc819549a05738b7de9159b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_rpdu_error_def_t</name>
      <anchorfile>tsms__rpdu_8h.html</anchorfile>
      <anchor>a72c7f03f6a75ad4a8373dc72e68600c1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_command.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/tpdu/</path>
    <filename>tsms__tpdu__command_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <class kind="struct">tsms_tpdu_command_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_COMMAND</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>a87efc321007f3d5aad96c0c0f894381c</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga90864d6bee8ca4e7a84af621671a1ffa</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_tostring</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>a8746521f56ac699a5d80bc9f233d1a2f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gab7d48f6cda450b0700884ea23b11d709</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_set_cmddata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac2f78b6f771728dcf16c7897db3c4a40</anchor>
      <arglist>(self, cdata)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS enum tsms_tpdu_cmd_e</type>
      <name>tsms_tpdu_cmd_t</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>a1f02be47f10a94267b2bfd65053bef77</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_tpdu_command_s</type>
      <name>tsms_tpdu_command_t</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>ad324eb121c74f4d972cbdc96891ee1cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_tpdu_cmd_e</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>ae1426b2fc639daabd57c2c6cf0a7be3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_cmd_rel</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>ae1426b2fc639daabd57c2c6cf0a7be3fa9424f84dce19197c85329b675b516dc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_cmd_cancel</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>ae1426b2fc639daabd57c2c6cf0a7be3fa976a1405b0ff59c7222c4f98922bf157</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_cmd_delete</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>ae1426b2fc639daabd57c2c6cf0a7be3fa65bb9c06ed38cf56be40f4fdf26ba2b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_cmd_enable_status</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>ae1426b2fc639daabd57c2c6cf0a7be3fa225eefad84c3f9e2594d16f20366d44c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_tpdu_command_t *</type>
      <name>tsms_tpdu_command_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga239732e7be3081cedc403c97cc9c7497</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest, uint8_t msg_num, tsms_tpdu_cmd_t cmd)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_tpdu_command_def_t</name>
      <anchorfile>tsms__tpdu__command_8h.html</anchorfile>
      <anchor>aa5f92536bf46a9c9916f1fbf6500ee36</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_deliver.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/tpdu/</path>
    <filename>tsms__tpdu__deliver_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <class kind="struct">tsms_tpdu_deliver_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DELIVER</name>
      <anchorfile>tsms__tpdu__deliver_8h.html</anchorfile>
      <anchor>a4ae8d26eb33e6a8f6b303b5bcbbd90e3</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga4768097f9e42feff077e821c209be4e9</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_tostring</name>
      <anchorfile>tsms__tpdu__deliver_8h.html</anchorfile>
      <anchor>a1553af338facafa905e08df61f32fde0</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga78ff03cc6dceeeea4f3d0cd57e1b272b</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gaf6f60b098a075894156a378ed78e844d</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS struct tsms_tpdu_deliver_s</type>
      <name>tsms_tpdu_deliver_t</name>
      <anchorfile>tsms__tpdu__deliver_8h.html</anchorfile>
      <anchor>a83773d5438d32ad5687317079774aa04</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_tpdu_deliver_t *</type>
      <name>tsms_tpdu_deliver_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gaec75a2915fc0e29f24075b80a2ac40ff</anchor>
      <arglist>(const tsms_address_string_t smsc, const tsms_address_string_t orig)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_tpdu_deliver_def_t</name>
      <anchorfile>tsms__tpdu__deliver_8h.html</anchorfile>
      <anchor>ac49bcf070b342868e9d3537c785ab4ae</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_report.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/tpdu/</path>
    <filename>tsms__tpdu__report_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <class kind="struct">tsms_tpdu_report_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_REPORT</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>a01ab8732480ae988c81efaf67ca242c1</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_REPORT_IS_MO</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>af56ca1bcbaf4fc5f752ec6ec5b542041</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga542962bb9f989075a86b3000283d50e9</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_tostring</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>ad2af47ee7b7dd012b9990bdbb4e4d678</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga7eedf91cbe12f7560f9e67b2272245c4</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga46c37f7dc628af1fab3867a346b1223f</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS struct tsms_tpdu_report_s</type>
      <name>tsms_tpdu_report_t</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>a71f1bf474cec1217548e1307780004b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_tpdu_report_t *</type>
      <name>tsms_tpdu_report_create</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>a33672efeb750b2907604a41261262e88</anchor>
      <arglist>(const tsms_address_string_t smsc, tsk_bool_t submit, tsk_bool_t error)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API int</type>
      <name>tsms_tpdu_report_set_fcs</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>ac0ea27857d4fd018f41f4d6bfc0cfb73</anchor>
      <arglist>(tsms_tpdu_report_t *self, uint8_t code)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_tpdu_report_def_t</name>
      <anchorfile>tsms__tpdu__report_8h.html</anchorfile>
      <anchor>a755291aa428a4073a411276b948ff1b4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_status_report.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/tpdu/</path>
    <filename>tsms__tpdu__status__report_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <class kind="struct">tsms_tpdu_status_report_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_STATUS_REPORT</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>a32890b60ad34aea550e838a9dad4581e</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac0f04a5b0d9a7fd4c48b0e29a32fd453</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_tostring</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>ad2bb9dcd74eac269f81fbb3285b1598f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga82cb05b61ab2098932856ff3f94caf7f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga3d5aa918ccde3cf03373de29346db3d3</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS enum tsms_tpdu_status_type_e</type>
      <name>tsms_tpdu_status_type_t</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aef005f6482380a54c654982158fa8e77</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_tpdu_status_report_s</type>
      <name>tsms_tpdu_status_report_t</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>abe5f541f6b3fc5a8fd6b222db9cf7c87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_tpdu_status_type_e</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_received</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a2be45dfd48b3b65f25b8940a1b5ea5b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_forwarded</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a9c6459bbd5961a985a91dc0d2accd50f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_replaced</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a154ed26bae7cc6ea904c1f159d2c20a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_congestion</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786adf7fd3ac88e41056ab15a03dce87cc55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_busy</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a02323b1d1003e9dafe884fb5b238ae70</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_no_response</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786ae39332ba00813faf2bf3f90f18c7b996</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_serv_rejected</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a51be867166da427c4cb11cb9d058ce78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_no_qos</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a1139d5a85cf633faec944322c744aa5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_error_in_sme</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786adab418e468fc05164130faa36b7f856f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_remote_error</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a05d06af405ef4abd6540b4d3047ae3b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_incompatible_dest</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786ae6c15616816a6329b94144cabdf7008f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_conn_rejected</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a242994765f74f53bb045bd0dae76379f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_not_obtainable</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786adcfea3bd7b312194813fff9257341d75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_no_qos_2</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a23e6a86e27c336e89fe96cb7ae0f4cba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_no_inter_avail</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786acd49fddb7216e7f06aeb673af4dbe27c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_vp_expired</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a442f298bc94df41cedc10d2d0c472478</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_deleted_by_orig</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a8dd88157270624363cb5b1cf808210e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_deleted_by_admin</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786ac24899a558d39dce1fc1fbc8a353981c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_sm_not_exist</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a2d7392c011d7d0b9589701903d7ce08d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_congestion_3</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a05f36c81c437e4e655de353b72868a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_busy_3</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a77f402d2ca08208fb5fee36ae13b121f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_no_response_3</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a171e4714c58371d533e5954ddf4acc91</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_serv_rejected_3</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a657d38c4f23f03a1e417d8d76c679135</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_temp_no_qos_3</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786a15d6113ab01435d84a6a88aa7e76eeb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_status_error_in_sme_3</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>aab9cc77afda74d40bc6eeaf5751de786ab41300b6c110507753ca3f9d715bc652</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_tpdu_status_report_t *</type>
      <name>tsms_tpdu_status_report_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gae3bed7be4b11d147a472ba46616003b3</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t recipient, tsms_tpdu_status_type_t status, tsk_bool_t submit)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_tpdu_status_report_def_t</name>
      <anchorfile>tsms__tpdu__status__report_8h.html</anchorfile>
      <anchor>a034aee161d500066a305d4251fc7016c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_submit.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/tpdu/</path>
    <filename>tsms__tpdu__submit_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <class kind="struct">tsms_tpdu_submit_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_SUBMIT</name>
      <anchorfile>tsms__tpdu__submit_8h.html</anchorfile>
      <anchor>a2f3ad26b37878339084fbff7440530ef</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gafe89fa4b138f9f356ee4622210e36358</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_tostring</name>
      <anchorfile>tsms__tpdu__submit_8h.html</anchorfile>
      <anchor>a851a10d99d90435c7b48dd0d66ee7e5a</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga2798d593f1ceb66c05345b36c4e4d3e1</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga2fcce553f869100adb016d10f499083c</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS struct tsms_tpdu_submit_s</type>
      <name>tsms_tpdu_submit_t</name>
      <anchorfile>tsms__tpdu__submit_8h.html</anchorfile>
      <anchor>aca617c51526dd733bbb9a4de94e8e804</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_tpdu_submit_t *</type>
      <name>tsms_tpdu_submit_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gacff03a913d2a97fb9a8201c8f8e7e989</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_tpdu_submit_def_t</name>
      <anchorfile>tsms__tpdu__submit_8h.html</anchorfile>
      <anchor>a714d473125a3062e070a24a53e6c271f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/</path>
    <filename>tsms_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
  </compound>
  <compound kind="file">
    <name>tsms_address.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/</path>
    <filename>tsms__address_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <class kind="struct">tsms_address_s</class>
    <member kind="typedef">
      <type>enum tsms_address_type_e</type>
      <name>tsms_address_type_t</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a0828489bbcf7da6c524adf295662bc7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tsms_address_ton_e</type>
      <name>tsms_address_ton_t</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>afaa8be016a634d33a0d35ff2c33e2d6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tsms_address_npi_e</type>
      <name>tsms_address_npi_t</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a6682d48894029029483b9f94c9a9bcad</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_address_s</type>
      <name>tsms_address_t</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>ac58eec864cdc56e3ec25c1418ed9975b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_address_type_e</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a2b53b5276f28408b9404f08954724982</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_oa</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a2b53b5276f28408b9404f08954724982a8474a8fd3ea689112119692caece8d92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_da</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a2b53b5276f28408b9404f08954724982aa30eabc38abf3a7d3c886540ed7133b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_smsc</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a2b53b5276f28408b9404f08954724982a1f4b63abacdde3cbcaef8b8cecb93b48</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_address_ton_e</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_unknown</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a4ce89cea8e9c121dc4e9b819c5ab4adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_international</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a0bb254db5dc9152205b24cb4a4d38f96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_national</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a5e55efb47ecafa9f9da1ae020f77a77c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_network_specific</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a0e350a6aa686d9330ab672c9c7eabbee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_subscriber</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a792b672b14c7f51e2247d4b789349cd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_alphanumeric</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a29204d9d5707b1aa8b5dcbada78fda4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_abbreviated</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26a8bd02c2a2a379f7b140b652a88eef742</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_ton_reserved</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a1cc90165eb908a8689b661ef9a568f26aa93b45525b6a9d5ff11b76c49d1b0be0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_address_npi_e</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_unknown</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469ea6a003127af47339610d4282dd5f6065b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_isdn</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469eae0839c136bfd8143a8f190498a21b44e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_x121</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469ea1986d203d6af80e7d32e85a168a36111</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_telex</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469eaeaf1a8a4b9cbf4adbb272eabe9e59431</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_national</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469ea720713f66c7e7d0735ee00b8c2fe9646</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_private</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469eac3f19518b752af78a3e8d75a8be3c75b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_ermes</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469ea12dab2ccdc76a4a825a6224851920d61</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_addr_npi_reserved</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a9cc1b45edc645eb842bc64afe635469eaaec95395dc3ac31b901527c81aee9e03</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_address_serialize</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a507b9730755fcc9d3ad1027be5ace0d7</anchor>
      <arglist>(const tsms_address_t *address, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_deserialize</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>abc662334a34e4064a7a6bad52a364905</anchor>
      <arglist>(const void *data, tsk_size_t size, tsms_address_type_t xtype, tsk_size_t *length)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_create</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>ac56f24c7eca88ff0c3e14ea4ecccab10</anchor>
      <arglist>(const tsms_address_string_t digits, tsms_address_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_oa_create</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>ac76c474e2fba47adb26890326e4737ee</anchor>
      <arglist>(const tsms_address_string_t digits)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_da_create</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a43e6439ca43a91ad895dc06604d50013</anchor>
      <arglist>(const tsms_address_string_t digits)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_smsc_create</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a6537fd07058f579223b585ec3b40bf85</anchor>
      <arglist>(const tsms_address_string_t digits)</arglist>
    </member>
    <member kind="variable">
      <type>TSMS_BEGIN_DECLS typedef uint8_t</type>
      <name>tsms_address_string_t</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>aa639d2f581a1b27328d3366a24f3fc36</anchor>
      <arglist>[13]</arglist>
    </member>
    <member kind="variable">
      <type>TINYSMS_GEXTERN const tsk_object_def_t *</type>
      <name>tsms_address_def_t</name>
      <anchorfile>tsms__address_8h.html</anchorfile>
      <anchor>a0ddbef1df3e316ae05dfed05005c494c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_common.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/</path>
    <filename>tsms__common_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <includes id="tsms__address_8h" name="tsms_address.h" local="yes" imported="no">tinysms/tsms_address.h</includes>
    <class kind="struct">tsms_tpdu_message_s</class>
    <class kind="struct">tsms_rpdu_message_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ALPHA_FROM_DCS</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a29855df7a97f4604172a86b03e9a4a35</anchor>
      <arglist>(dcs)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_MTI_IS_RESERVED</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ae3f34fb52e703b4bde104533375751a6</anchor>
      <arglist>(mti)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_MTI_IS_MO</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a599f33f1f71df271a6f1bf9bb5446458</anchor>
      <arglist>(mti)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_DECLARE_TPDU_MESSAGE</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>adfae7ed3c2a0e938c6617cb4247a8e10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_MESSAGE</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a30662e72d0b18b6beaf0a4ac4793bba2</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_serialize_mo</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga87b948f6fddfc874927ec3327694aff8</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_serialize_mt</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga89ecc7dd4f55702eb6aa4491179ebf2f</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_deserialize_mo</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga1d9b57bb99775b017ad8ef672ab23690</anchor>
      <arglist>(data, size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_deserialize_mt</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga4621ebda575020c673045e72fe3aa651</anchor>
      <arglist>(data, size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_PID</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a1c578c6cfca27b93a7cae4ef60194600</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_DCS</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8bc7230f96d0577effafd7cbb2a45afe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_VP</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ac75d19e74f74caa40d4fa7aca5b9eb76</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_VPF</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ade2d12fbddb972ff91c0bcb98d781b38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_FCS</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>af5fd5126b5ec1cd7328fbf0895a109b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_PI</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a82720cf54931b8a8ffb59bf83e5f40a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_SCTS</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>aa87e60e7c29ad71d779231aeffb99669</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_DEFAULT_MMS</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>aa100abd40cae615c1a9a195c07183650</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_TPDU_APPEND_SMSC</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a36b4975358fce9494a1258de0713e093</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_RPDU_TYPE_IS_MO</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a81705e5a0ee5207e7a7a865bff9ecf0c</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_DECLARE_RPDU_MESSAGE</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>aded604170cc2bf281e1dd65202fd43e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_RPDU_MESSAGE</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a021282323a27cc99ef15cceaccd286e8</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="typedef">
      <type>TSMS_BEGIN_DECLS enum tsms_alphabet_e</type>
      <name>tsms_alphabet_t</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a13a9cc533b988588cadae86ebdf937a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tsms_tpdu_mti_e</type>
      <name>tsms_tpdu_mti_t</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8918709f71f141409094082bbbcd300a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tsms_tpdu_vpf_e</type>
      <name>tsms_tpdu_vpf_t</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a241e5a68e06bee56f2828bafcd7adaf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_tpdu_message_s</type>
      <name>tsms_tpdu_message_t</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a1e806a13943e7963d4d91e9fd67f80ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tsms_rpdu_type_e</type>
      <name>tsms_rpdu_type_t</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a6d9e66a8fe61c68830abe3cea6d7e4c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tsms_rpdu_message_s</type>
      <name>tsms_rpdu_message_t</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>aa61bf674fb0c22284461f8d5703fdad2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_alphabet_e</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ad0b5824a16041ea60903bf40505fa707</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_alpha_reserved</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ad0b5824a16041ea60903bf40505fa707a894b8f649597c4f51b2c945104aa4445</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_alpha_7bit</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ad0b5824a16041ea60903bf40505fa707a92043505be20e64f83a9cd16207ec380</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_alpha_8bit</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ad0b5824a16041ea60903bf40505fa707a02e4ee43c20d8697c60ae06be8e54689</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_alpha_ucs2</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>ad0b5824a16041ea60903bf40505fa707aac13f6a189dbf03b8507a738b34063fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_tpdu_mti_e</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_deliver_mt</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68ca55e0aff8ca043b7f6e2fbe5d304ecc00</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_deliver_report_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68ca8a1ec94841e8c679f70d5212b0fa6949</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_status_report_mt</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68ca900ce889f7066b31f9f48bd384408172</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_command_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68ca8a6a855ae9a1ce773fb86d8e99f8f55c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_submit_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68cae30123b35d44fcfbb40a38b37391d5d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_submit_report_mt</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68caefabfee689a959d7fd5cdbfcf86b8d43</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_mti_reserved</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a8e71a089f7c40066c43f9486914ff68ca17e3003fa583a37739400bd33004cb29</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_tpdu_vpf_e</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a12279bf790f28b7adcef53e3a4f0d9de</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_vpf_not_present</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a12279bf790f28b7adcef53e3a4f0d9deade73839b01ef15988dc167f61932c9a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_vpf_relative</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a12279bf790f28b7adcef53e3a4f0d9dea8cea9c578cda9a4025ac2b3303a2db96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_vpf_enhanced</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a12279bf790f28b7adcef53e3a4f0d9dea55029bd842820b94722b2fb2ea88bd71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_tpdu_vpf_absolute</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a12279bf790f28b7adcef53e3a4f0d9deae8302746fbc448e32e5e3379dbecf884</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tsms_rpdu_type_e</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_data_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167aeb1b38eccd3dce70761bbdfe9a4bed3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_data_mt</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167a437a8e02252512b6a749a53f5b29044f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_ack_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167a9220550e49dcfb2e1fec98169c5c4a2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_ack_mt</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167af28a8ba87a068a2b175642a4c932d41a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_error_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167ae038a392bb29ffad4dc1ad55c9a31baf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_error_mt</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167afacd50bbada678fc1232ab0c85f7ef25</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tsms_rpdu_type_smma_mo</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a2c7fcb742c3bc964ca917e4272cb3167a13e64ff416abbfea77a24340c1275007</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_init</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>a47eebcb0667a2fbc1bd7b26618ab2648</anchor>
      <arglist>(tsms_tpdu_message_t *self, tsms_tpdu_mti_t mti)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API int</type>
      <name>tsms_tpdu_message_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gad1792455c7f970f3e27ce00f10565f42</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_tpdu_message_t *</type>
      <name>tsms_tpdu_message_deserialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gad59f30e9edb66eb3d26eafd51dbd09fc</anchor>
      <arglist>(const void *data, tsk_size_t size, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_tpdu_message_tostring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gae87ed5dcd57b10dd47b8e96785e7f545</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_tpdu_message_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga1daaf76c6f824f4ef888c7a2c0d5b0f9</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_tpdu_message_get_payload</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga733ca5ad5d513f61b68c85570497226d</anchor>
      <arglist>(const tsms_tpdu_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API int</type>
      <name>tsms_tpdu_message_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac22be8031c22c058341786c5382d726e</anchor>
      <arglist>(tsms_tpdu_message_t *self, const tsk_buffer_t *udata, tsms_alphabet_t alpha)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_deinit</name>
      <anchorfile>tsms__common_8h.html</anchorfile>
      <anchor>aaf563a05778ce503cb518932c191d00f</anchor>
      <arglist>(tsms_tpdu_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API int</type>
      <name>tsms_rpdu_message_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga3e3d2edcb276313e27838c600ff95c87</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsms_rpdu_message_t *</type>
      <name>tsms_rpdu_message_deserialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gaddfae888c86bfcbab67b60ddcc963cf0</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_rpdu_message_tohexastring</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gaf1d06a3766290da7873b4362f4efadbb</anchor>
      <arglist>(const tsms_rpdu_message_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_etsi_gsm_03_38.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/</path>
    <filename>tsms__etsi__gsm__03__38_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ETSI_GSM_03_38_COUNT</name>
      <anchorfile>tsms__etsi__gsm__03__38_8h.html</anchorfile>
      <anchor>a4dc23fb32f1c903dfdd3acadb5abafb8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_packing.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/tinysms/</path>
    <filename>tsms__packing_8h</filename>
    <includes id="tinysms__config_8h" name="tinysms_config.h" local="yes" imported="no">tinysms_config.h</includes>
    <member kind="function">
      <type>TSMS_BEGIN_DECLS TINYSMS_API tsk_buffer_t *</type>
      <name>tsms_pack_to_7bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga5ec76b83fedbd705f29a2bb8504095c9</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsk_buffer_t *</type>
      <name>tsms_pack_to_ucs2</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>gafcfc13106399cada5e658b5cdeb520e0</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API tsk_buffer_t *</type>
      <name>tsms_pack_to_8bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga2618ee85c5f41086a2763b9cfafec223</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_pack_from_7bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga05ce76a51583893258f7ccc38f54fc42</anchor>
      <arglist>(const void *gsm7bit, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_pack_from_ucs2</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga7d3f412de3f53083056fd7fab2e7aa6b</anchor>
      <arglist>(const void *ucs2, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYSMS_API char *</type>
      <name>tsms_pack_from_8bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga56acefc52098c1a66db2bc7e5250eff2</anchor>
      <arglist>(const void *gsm8bit, tsk_size_t size)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tinysms_config.h</name>
    <path>C:/Projects/Doubango/tinySMS/include/</path>
    <filename>tinysms__config_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TINYSMS_API</name>
      <anchorfile>tinysms__config_8h.html</anchorfile>
      <anchor>a7756a01514716f7ebbb2470fef2aaed0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYSMS_GEXTERN</name>
      <anchorfile>tinysms__config_8h.html</anchorfile>
      <anchor>a9c16ccde1b4e5f708bfd20548c934232</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_BEGIN_DECLS</name>
      <anchorfile>tinysms__config_8h.html</anchorfile>
      <anchor>a084feea3d083f7bc788039073aadc346</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_END_DECLS</name>
      <anchorfile>tinysms__config_8h.html</anchorfile>
      <anchor>afc2f6fd3c9090e24b5274c5a0ebaee53</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_rpdu.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/rpdu/</path>
    <filename>tsms__rpdu_8c</filename>
    <includes id="tsms__rpdu_8h" name="tsms_rpdu.h" local="yes" imported="no">tinysms/rpdu/tsms_rpdu.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ERROR_TOO_SHORT</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a5cce6a73db203a29a95b1a04e931750b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rpdata_deserialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>adb99738343fb13e155c842b5453fb1e7</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rpdata_serialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>ae9ac47639ac1bfeb8e7240ce669178f4</anchor>
      <arglist>(const tsms_rpdu_message_t *_self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_data_t *</type>
      <name>tsms_rpdu_data_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga5589e131deeab1e25eb08494f7554d55</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_tpdu_message_t *tpdu, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rpsmma_deserialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a42545971352879d43a73cbb8cceaf978</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rpsmma_serialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a5b023ad9c429f7ffc7b917a0636588a6</anchor>
      <arglist>(const tsms_rpdu_message_t *_self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_smma_t *</type>
      <name>tsms_rpdu_smma_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gac588512623e47fdfdbac5cd0a387572a</anchor>
      <arglist>(uint8_t mr)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rpack_deserialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>aa8180fc4f5ba93fb71ba377e5fe1afb3</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rpack_serialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>ad6c821ed10857f4a87e666ee330ce586</anchor>
      <arglist>(const tsms_rpdu_message_t *_self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_ack_t *</type>
      <name>tsms_rpdu_ack_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga1464b066b6b93ed848049fb80ba4d4a5</anchor>
      <arglist>(uint8_t mr, const tsms_tpdu_message_t *tpdu, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rperror_deserialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a7cd5468dc0cd7a1131b83c739936f786</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rperror_serialize</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a36d8067ced8447ec0c025c902e7ac632</anchor>
      <arglist>(const tsms_rpdu_message_t *_self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_error_t *</type>
      <name>tsms_rpdu_error_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga445ece4fe91ccafe88343cf6e1f1bf30</anchor>
      <arglist>(uint8_t mr, const tsms_tpdu_message_t *tpdu, uint8_t cause, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_rpdu_data_def_t</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a5c986cfb3af3555b43f00b2864a9d39a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_rpdu_smma_def_t</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a40a7aea052b217811def43fa33069889</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_rpdu_ack_def_t</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a730afd9d5b464baa516b7c8c6d411b29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_rpdu_error_def_t</name>
      <anchorfile>tsms__rpdu_8c.html</anchorfile>
      <anchor>a2adbd080b2ec726a8c1dc139a360b8a8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_command.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/tpdu/</path>
    <filename>tsms__tpdu__command_8c</filename>
    <includes id="tsms__tpdu__command_8h" name="tsms_tpdu_command.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_command.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ERROR_TOO_SHORT</name>
      <anchorfile>tsms__tpdu__command_8c.html</anchorfile>
      <anchor>add2e5155902e6fe23c5aca49835c9d2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_command_deserialize</name>
      <anchorfile>tsms__tpdu__command_8c.html</anchorfile>
      <anchor>a2a4ef83ce35bd81e61f3bd5db9be92fd</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_command_serialize</name>
      <anchorfile>tsms__tpdu__command_8c.html</anchorfile>
      <anchor>a7030a76cfe083ff075274aa487aff4c3</anchor>
      <arglist>(const tsms_tpdu_command_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_command_t *</type>
      <name>tsms_tpdu_command_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga239732e7be3081cedc403c97cc9c7497</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest, uint8_t msg_num, tsms_tpdu_cmd_t cmd)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_tpdu_command_def_t</name>
      <anchorfile>tsms__tpdu__command_8c.html</anchorfile>
      <anchor>a930685ea13f056c3ac305ab27cdc8897</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_deliver.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/tpdu/</path>
    <filename>tsms__tpdu__deliver_8c</filename>
    <includes id="tsms__tpdu__deliver_8h" name="tsms_tpdu_deliver.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_deliver.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ERROR_TOO_SHORT</name>
      <anchorfile>tsms__tpdu__deliver_8c.html</anchorfile>
      <anchor>add2e5155902e6fe23c5aca49835c9d2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_deliver_deserialize</name>
      <anchorfile>tsms__tpdu__deliver_8c.html</anchorfile>
      <anchor>a15b7e7b03ef295e7489577754eda678e</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_deliver_serialize</name>
      <anchorfile>tsms__tpdu__deliver_8c.html</anchorfile>
      <anchor>a44d3fc4fabf696737301a5477d870b05</anchor>
      <arglist>(const tsms_tpdu_deliver_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_deliver_t *</type>
      <name>tsms_tpdu_deliver_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gaec75a2915fc0e29f24075b80a2ac40ff</anchor>
      <arglist>(const tsms_address_string_t smsc, const tsms_address_string_t orig)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_tpdu_deliver_def_t</name>
      <anchorfile>tsms__tpdu__deliver_8c.html</anchorfile>
      <anchor>a6228aa1c67ce911325f84ba60356923c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_report.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/tpdu/</path>
    <filename>tsms__tpdu__report_8c</filename>
    <includes id="tsms__tpdu__report_8h" name="tsms_tpdu_report.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_report.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ERROR_TOO_SHORT</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>add2e5155902e6fe23c5aca49835c9d2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_report_deserialize_2</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>af5e6a8f6a19983a7cfd85be48c266780</anchor>
      <arglist>(const void *data, tsk_size_t size, tsk_bool_t error)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_report_deserialize</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>abebbb58c898c033aacc70f921da5fb15</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_report_serialize</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>a777bc436cd4de3c23e60943b4fb2e70e</anchor>
      <arglist>(const tsms_tpdu_report_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_report_t *</type>
      <name>tsms_tpdu_report_create</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>aa3aa4930a806d47ec6199984e867f0d0</anchor>
      <arglist>(const tsms_address_string_t smsc, tsk_bool_t submit, tsk_bool_t error)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_report_set_fcs</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>a82bfca59830ed60b72aa166780c20011</anchor>
      <arglist>(tsms_tpdu_report_t *self, uint8_t code)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_tpdu_report_def_t</name>
      <anchorfile>tsms__tpdu__report_8c.html</anchorfile>
      <anchor>aaa6fd791d91ce0545cdd116c4c431ce7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_status_report.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/tpdu/</path>
    <filename>tsms__tpdu__status__report_8c</filename>
    <includes id="tsms__tpdu__status__report_8h" name="tsms_tpdu_status_report.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_status_report.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ERROR_TOO_SHORT</name>
      <anchorfile>tsms__tpdu__status__report_8c.html</anchorfile>
      <anchor>add2e5155902e6fe23c5aca49835c9d2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_status_report_deserialize</name>
      <anchorfile>tsms__tpdu__status__report_8c.html</anchorfile>
      <anchor>a5449be5149020e659b9961651324044a</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_status_report_serialize</name>
      <anchorfile>tsms__tpdu__status__report_8c.html</anchorfile>
      <anchor>a3b3358ac5b86c1e99cebca53100032b3</anchor>
      <arglist>(const tsms_tpdu_status_report_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_status_report_t *</type>
      <name>tsms_tpdu_status_report_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gae3bed7be4b11d147a472ba46616003b3</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t recipient, tsms_tpdu_status_type_t status, tsk_bool_t submit)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_tpdu_status_report_def_t</name>
      <anchorfile>tsms__tpdu__status__report_8c.html</anchorfile>
      <anchor>a4b24621760af7a7f904baa2dc271e345</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_tpdu_submit.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/tpdu/</path>
    <filename>tsms__tpdu__submit_8c</filename>
    <includes id="tsms__tpdu__submit_8h" name="tsms_tpdu_submit.h" local="yes" imported="no">tinysms/tpdu/tsms_tpdu_submit.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TSMS_ERROR_TOO_SHORT</name>
      <anchorfile>tsms__tpdu__submit_8c.html</anchorfile>
      <anchor>add2e5155902e6fe23c5aca49835c9d2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_submit_deserialize</name>
      <anchorfile>tsms__tpdu__submit_8c.html</anchorfile>
      <anchor>ab2380303ce3c61ead34acbebc552f0db</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_submit_serialize</name>
      <anchorfile>tsms__tpdu__submit_8c.html</anchorfile>
      <anchor>a1500ba088ecdab7158439a7358858ec8</anchor>
      <arglist>(const tsms_tpdu_submit_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_submit_t *</type>
      <name>tsms_tpdu_submit_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gacff03a913d2a97fb9a8201c8f8e7e989</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_tpdu_submit_def_t</name>
      <anchorfile>tsms__tpdu__submit_8c.html</anchorfile>
      <anchor>a962877743e46916338aacc6d393ac0a9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/</path>
    <filename>tsms_8c</filename>
    <includes id="tsms_8h" name="tsms.h" local="yes" imported="no">tinysms/tsms.h</includes>
  </compound>
  <compound kind="file">
    <name>tsms_address.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/</path>
    <filename>tsms__address_8c</filename>
    <includes id="tsms__address_8h" name="tsms_address.h" local="yes" imported="no">tinysms/tsms_address.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>tsms_address_swap</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>adc866491ab290e7acc65bec7a8e827cd</anchor>
      <arglist>(const char *in, tsk_size_t in_len)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_create</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>ac56f24c7eca88ff0c3e14ea4ecccab10</anchor>
      <arglist>(const tsms_address_string_t digits, tsms_address_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_oa_create</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>ac76c474e2fba47adb26890326e4737ee</anchor>
      <arglist>(const tsms_address_string_t digits)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_da_create</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>a43e6439ca43a91ad895dc06604d50013</anchor>
      <arglist>(const tsms_address_string_t digits)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_smsc_create</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>a6537fd07058f579223b585ec3b40bf85</anchor>
      <arglist>(const tsms_address_string_t digits)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_address_serialize</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>a507b9730755fcc9d3ad1027be5ace0d7</anchor>
      <arglist>(const tsms_address_t *address, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_address_t *</type>
      <name>tsms_address_deserialize</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>abc662334a34e4064a7a6bad52a364905</anchor>
      <arglist>(const void *data, tsk_size_t size, tsms_address_type_t xtype, tsk_size_t *length)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tsms_address_def_t</name>
      <anchorfile>tsms__address_8c.html</anchorfile>
      <anchor>aec178867058428f667610f0ef1750e18</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_common.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/</path>
    <filename>tsms__common_8c</filename>
    <includes id="tsms__common_8h" name="tsms_common.h" local="yes" imported="no">tinysms/tsms_common.h</includes>
    <includes id="tsms__packing_8h" name="tsms_packing.h" local="yes" imported="no">tinysms/tsms_packing.h</includes>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_submit_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>ab2380303ce3c61ead34acbebc552f0db</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_deliver_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a15b7e7b03ef295e7489577754eda678e</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_command_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a2a4ef83ce35bd81e61f3bd5db9be92fd</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_report_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>abebbb58c898c033aacc70f921da5fb15</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>_tsms_tpdu_status_report_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a5449be5149020e659b9961651324044a</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_submit_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>ab87aff09c449a3baeb5f4dc95588cf49</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_deliver_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a3c7cb3012cba9bd4f61018db4f976608</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_command_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a1ea01962f453472e423bf04c45097551</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_report_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a1f1bdbac5c207ec1b9cb844446f891ff</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_tpdu_status_report_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>ad0b95d89481aba76c566e272137fde86</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_init</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a47eebcb0667a2fbc1bd7b26618ab2648</anchor>
      <arglist>(tsms_tpdu_message_t *self, tsms_tpdu_mti_t mti)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gad1792455c7f970f3e27ce00f10565f42</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>tsms_tpdu_message_deserialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gad59f30e9edb66eb3d26eafd51dbd09fc</anchor>
      <arglist>(const void *data, tsk_size_t size, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_tpdu_message_tostring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gae87ed5dcd57b10dd47b8e96785e7f545</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_tpdu_message_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga1daaf76c6f824f4ef888c7a2c0d5b0f9</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_tpdu_message_get_payload</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga733ca5ad5d513f61b68c85570497226d</anchor>
      <arglist>(const tsms_tpdu_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac22be8031c22c058341786c5382d726e</anchor>
      <arglist>(tsms_tpdu_message_t *self, const tsk_buffer_t *udata, tsms_alphabet_t alpha)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_deinit</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>aaf563a05778ce503cb518932c191d00f</anchor>
      <arglist>(tsms_tpdu_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rpdata_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a27c84245caf989f3f407183e7d657817</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rpsmma_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a539fe1c0b79b91057c510dfeaa401830</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rpack_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>ad2309bbddc1c122466646141bb52a87d</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tsms_rpdu_rperror_serialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a2034b5f160068d5ecac2dd05823c92c3</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rpdata_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>adb99738343fb13e155c842b5453fb1e7</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rpsmma_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a42545971352879d43a73cbb8cceaf978</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rpack_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>aa8180fc4f5ba93fb71ba377e5fe1afb3</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>_tsms_rpdu_rperror_deserialize</name>
      <anchorfile>tsms__common_8c.html</anchorfile>
      <anchor>a7cd5468dc0cd7a1131b83c739936f786</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_rpdu_message_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga3e3d2edcb276313e27838c600ff95c87</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>tsms_rpdu_message_deserialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gaddfae888c86bfcbab67b60ddcc963cf0</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_rpdu_message_tohexastring</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gaf1d06a3766290da7873b4362f4efadbb</anchor>
      <arglist>(const tsms_rpdu_message_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tsms_packing.c</name>
    <path>C:/Projects/Doubango/tinySMS/src/</path>
    <filename>tsms__packing_8c</filename>
    <includes id="tsms__packing_8h" name="tsms_packing.h" local="yes" imported="no">tinysms/tsms_packing.h</includes>
    <includes id="tsms__etsi__gsm__03__38_8h" name="tsms_etsi_gsm_03_38.h" local="yes" imported="no">tinysms/tsms_etsi_gsm_03_38.h</includes>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tsms_pack_to_7bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga5ec76b83fedbd705f29a2bb8504095c9</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tsms_pack_to_ucs2</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>gafcfc13106399cada5e658b5cdeb520e0</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tsms_pack_to_8bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga2618ee85c5f41086a2763b9cfafec223</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_pack_from_7bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga05ce76a51583893258f7ccc38f54fc42</anchor>
      <arglist>(const void *gsm7bit, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_pack_from_ucs2</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga7d3f412de3f53083056fd7fab2e7aa6b</anchor>
      <arglist>(const void *ucs2, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_pack_from_8bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga56acefc52098c1a66db2bc7e5250eff2</anchor>
      <arglist>(const void *gsm8bit, tsk_size_t size)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tsms_tpdu_group</name>
    <title>Service provided by the SM-TL (Transport)</title>
    <filename>group__tsms__tpdu__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga90864d6bee8ca4e7a84af621671a1ffa</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gab7d48f6cda450b0700884ea23b11d709</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_command_set_cmddata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac2f78b6f771728dcf16c7897db3c4a40</anchor>
      <arglist>(self, cdata)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga4768097f9e42feff077e821c209be4e9</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga78ff03cc6dceeeea4f3d0cd57e1b272b</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_deliver_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gaf6f60b098a075894156a378ed78e844d</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga542962bb9f989075a86b3000283d50e9</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga7eedf91cbe12f7560f9e67b2272245c4</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_report_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga46c37f7dc628af1fab3867a346b1223f</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac0f04a5b0d9a7fd4c48b0e29a32fd453</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga82cb05b61ab2098932856ff3f94caf7f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_status_report_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga3d5aa918ccde3cf03373de29346db3d3</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gafe89fa4b138f9f356ee4622210e36358</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga2798d593f1ceb66c05345b36c4e4d3e1</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_submit_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga2fcce553f869100adb016d10f499083c</anchor>
      <arglist>(self, udata, alpha)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_serialize_mo</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga87b948f6fddfc874927ec3327694aff8</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_serialize_mt</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga89ecc7dd4f55702eb6aa4491179ebf2f</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_deserialize_mo</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga1d9b57bb99775b017ad8ef672ab23690</anchor>
      <arglist>(data, size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_tpdu_message_deserialize_mt</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga4621ebda575020c673045e72fe3aa651</anchor>
      <arglist>(data, size)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_command_t *</type>
      <name>tsms_tpdu_command_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga239732e7be3081cedc403c97cc9c7497</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest, uint8_t msg_num, tsms_tpdu_cmd_t cmd)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_deliver_t *</type>
      <name>tsms_tpdu_deliver_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gaec75a2915fc0e29f24075b80a2ac40ff</anchor>
      <arglist>(const tsms_address_string_t smsc, const tsms_address_string_t orig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_status_report_t *</type>
      <name>tsms_tpdu_status_report_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gae3bed7be4b11d147a472ba46616003b3</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t recipient, tsms_tpdu_status_type_t status, tsk_bool_t submit)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_submit_t *</type>
      <name>tsms_tpdu_submit_create</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gacff03a913d2a97fb9a8201c8f8e7e989</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t dest)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_serialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gad1792455c7f970f3e27ce00f10565f42</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_buffer_t *output, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_tpdu_message_t *</type>
      <name>tsms_tpdu_message_deserialize</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gad59f30e9edb66eb3d26eafd51dbd09fc</anchor>
      <arglist>(const void *data, tsk_size_t size, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_tpdu_message_tostring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gae87ed5dcd57b10dd47b8e96785e7f545</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_tpdu_message_tohexastring</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga1daaf76c6f824f4ef888c7a2c0d5b0f9</anchor>
      <arglist>(const tsms_tpdu_message_t *self, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_tpdu_message_get_payload</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>ga733ca5ad5d513f61b68c85570497226d</anchor>
      <arglist>(const tsms_tpdu_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_tpdu_message_set_userdata</name>
      <anchorfile>group__tsms__tpdu__group.html</anchorfile>
      <anchor>gac22be8031c22c058341786c5382d726e</anchor>
      <arglist>(tsms_tpdu_message_t *self, const tsk_buffer_t *udata, tsms_alphabet_t alpha)</arglist>
    </member>
    <docanchor file="group__tsms__tpdu__group">tsms_tpdu_group_SUBMIT</docanchor>
    <docanchor file="group__tsms__tpdu__group">tsms_tpdu_group_STATUS_REPORT</docanchor>
    <docanchor file="group__tsms__tpdu__group">tsms_tpdu_group_COMMAND</docanchor>
    <docanchor file="group__tsms__tpdu__group">tsms_tpdu_group_DELIVER</docanchor>
  </compound>
  <compound kind="group">
    <name>tsms_rpdu_group</name>
    <title>Service provided by the SM-RL (Relay)</title>
    <filename>group__tsms__rpdu__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_data_create_mo</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga4cb5e1528206b1166b0bc6f888d58390</anchor>
      <arglist>(mr, smsc, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_data_create_mt</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gabe4218acfa2ef004520c11f430867268</anchor>
      <arglist>(mr, smsc, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_data_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gafa8f26571bb60a110a2b7480b85ede5f</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_smma_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gabc0ee8d0cb353fb39d5cab752142bcbb</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_ack_create_mo</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga69f8c3b283c13bfa92be006b152cc99e</anchor>
      <arglist>(mr, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_ack_create_mt</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gabc26f8ee68c9f42555be07cb81ca32dd</anchor>
      <arglist>(mr, tpdu)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_ack_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga2aeb9084a881e35c3b1eadd082e764d3</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_error_create_mo</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga2cba98d965737f6ada9d012d21e82b59</anchor>
      <arglist>(mr, tpdu, cause)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_error_create_mt</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga069e8b8cdda29fa50adaae3b7365a791</anchor>
      <arglist>(mr, tpdu, cause)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tsms_rpdu_error_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gac4dac7b6e1839f32cee2e39d85ac5730</anchor>
      <arglist>(self, output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_data_t *</type>
      <name>tsms_rpdu_data_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga5589e131deeab1e25eb08494f7554d55</anchor>
      <arglist>(uint8_t mr, const tsms_address_string_t smsc, const tsms_tpdu_message_t *tpdu, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_smma_t *</type>
      <name>tsms_rpdu_smma_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gac588512623e47fdfdbac5cd0a387572a</anchor>
      <arglist>(uint8_t mr)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_ack_t *</type>
      <name>tsms_rpdu_ack_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga1464b066b6b93ed848049fb80ba4d4a5</anchor>
      <arglist>(uint8_t mr, const tsms_tpdu_message_t *tpdu, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_error_t *</type>
      <name>tsms_rpdu_error_create</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga445ece4fe91ccafe88343cf6e1f1bf30</anchor>
      <arglist>(uint8_t mr, const tsms_tpdu_message_t *tpdu, uint8_t cause, tsk_bool_t MobOrig)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tsms_rpdu_message_serialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>ga3e3d2edcb276313e27838c600ff95c87</anchor>
      <arglist>(const tsms_rpdu_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsms_rpdu_message_t *</type>
      <name>tsms_rpdu_message_deserialize</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gaddfae888c86bfcbab67b60ddcc963cf0</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_rpdu_message_tohexastring</name>
      <anchorfile>group__tsms__rpdu__group.html</anchorfile>
      <anchor>gaf1d06a3766290da7873b4362f4efadbb</anchor>
      <arglist>(const tsms_rpdu_message_t *self)</arglist>
    </member>
    <docanchor file="group__tsms__rpdu__group">tsms_rpdu_group_ACK</docanchor>
    <docanchor file="group__tsms__rpdu__group">tsms_rpdu_group_ERROR</docanchor>
    <docanchor file="group__tsms__rpdu__group">tsms_rpdu_group_SMMA</docanchor>
    <docanchor file="group__tsms__rpdu__group">tsms_rpdu_group_DATA</docanchor>
  </compound>
  <compound kind="group">
    <name>tsms_packing_group</name>
    <title>SMS Packing</title>
    <filename>group__tsms__packing__group.html</filename>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tsms_pack_to_7bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga5ec76b83fedbd705f29a2bb8504095c9</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tsms_pack_to_ucs2</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>gafcfc13106399cada5e658b5cdeb520e0</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tsms_pack_to_8bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga2618ee85c5f41086a2763b9cfafec223</anchor>
      <arglist>(const char *ascii)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_pack_from_7bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga05ce76a51583893258f7ccc38f54fc42</anchor>
      <arglist>(const void *gsm7bit, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_pack_from_ucs2</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga7d3f412de3f53083056fd7fab2e7aa6b</anchor>
      <arglist>(const void *ucs2, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tsms_pack_from_8bit</name>
      <anchorfile>group__tsms__packing__group.html</anchorfile>
      <anchor>ga56acefc52098c1a66db2bc7e5250eff2</anchor>
      <arglist>(const void *gsm8bit, tsk_size_t size)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_address_s</name>
    <filename>structtsms__address__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtsms__address__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_ton_t</type>
      <name>ton</name>
      <anchorfile>structtsms__address__s.html</anchorfile>
      <anchor>a95079efd1c4e93614773267bb9a4dffa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_npi_t</type>
      <name>npi</name>
      <anchorfile>structtsms__address__s.html</anchorfile>
      <anchor>adefcd3395d99f67ab125abc5449ec75e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_type_t</type>
      <name>type</name>
      <anchorfile>structtsms__address__s.html</anchorfile>
      <anchor>a60327bf05c699312651b1f69e4539231</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>digits</name>
      <anchorfile>structtsms__address__s.html</anchorfile>
      <anchor>ad27b3581399ea814fa77a0e76f7dba82</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_rpdu_ack_s</name>
    <filename>structtsms__rpdu__ack__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_RPDU_MESSAGE</name>
      <anchorfile>structtsms__rpdu__ack__s.html</anchorfile>
      <anchor>ad34c3b1ed52661c3624730d0a77ac3e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>udata</name>
      <anchorfile>structtsms__rpdu__ack__s.html</anchorfile>
      <anchor>a9bcf3ed5adc35d2101b4fdb398a7924c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_rpdu_data_s</name>
    <filename>structtsms__rpdu__data__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_RPDU_MESSAGE</name>
      <anchorfile>structtsms__rpdu__data__s.html</anchorfile>
      <anchor>ad34c3b1ed52661c3624730d0a77ac3e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>oa</name>
      <anchorfile>structtsms__rpdu__data__s.html</anchorfile>
      <anchor>aace8fb066adc91b9b611b8ca1f68d615</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>da</name>
      <anchorfile>structtsms__rpdu__data__s.html</anchorfile>
      <anchor>a880175daede78c0951a4504a186d0dcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>udata</name>
      <anchorfile>structtsms__rpdu__data__s.html</anchorfile>
      <anchor>a9bcf3ed5adc35d2101b4fdb398a7924c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_rpdu_error_s</name>
    <filename>structtsms__rpdu__error__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_RPDU_MESSAGE</name>
      <anchorfile>structtsms__rpdu__error__s.html</anchorfile>
      <anchor>ad34c3b1ed52661c3624730d0a77ac3e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>cause</name>
      <anchorfile>structtsms__rpdu__error__s.html</anchorfile>
      <anchor>a4043e3447cb38a051b81832fd15d98e2</anchor>
      <arglist>[3]</arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>udata</name>
      <anchorfile>structtsms__rpdu__error__s.html</anchorfile>
      <anchor>a9bcf3ed5adc35d2101b4fdb398a7924c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_rpdu_message_s</name>
    <filename>structtsms__rpdu__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtsms__rpdu__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_rpdu_type_t</type>
      <name>mti</name>
      <anchorfile>structtsms__rpdu__message__s.html</anchorfile>
      <anchor>a621af770c025ef2c39f37c6682dd4c94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>mr</name>
      <anchorfile>structtsms__rpdu__message__s.html</anchorfile>
      <anchor>ab9c147a8a545b6ffc9640b0ad4532d1c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_rpdu_smma_s</name>
    <filename>structtsms__rpdu__smma__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_RPDU_MESSAGE</name>
      <anchorfile>structtsms__rpdu__smma__s.html</anchorfile>
      <anchor>ad34c3b1ed52661c3624730d0a77ac3e1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_tpdu_command_s</name>
    <filename>structtsms__tpdu__command__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_TPDU_MESSAGE</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>a3e10964f384560d83a82cb4eb61f2226</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>smsc</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>a38e40692d827af97d4bcedb4dbbffab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>udhi</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>a4bb414943b60bf3e2891e2e74f4ba6bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>srr</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>a03bfe36a49496db5c02e536f55bdf8ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>mr</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>ab9c147a8a545b6ffc9640b0ad4532d1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_tpdu_cmd_t</type>
      <name>ct</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>a3b099cecc32eb10c444a0d323c29d67e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>mn</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>aa56680e8399c7c13b715d67ab6d083e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>da</name>
      <anchorfile>structtsms__tpdu__command__s.html</anchorfile>
      <anchor>a880175daede78c0951a4504a186d0dcd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_tpdu_deliver_s</name>
    <filename>structtsms__tpdu__deliver__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_TPDU_MESSAGE</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>a3e10964f384560d83a82cb4eb61f2226</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>smsc</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>a38e40692d827af97d4bcedb4dbbffab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>mms</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>ad9fabb1dca4bee43c9a920a254d9f26f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>lp</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>a6444e99fe8e0d97fe5453f7592ac515c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>rp</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>a1118d743a178dc0d92dd8a096b77524f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>udhi</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>a4bb414943b60bf3e2891e2e74f4ba6bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>sri</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>a722252e68f2b52cf31f74ece5662a49f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>oa</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>aace8fb066adc91b9b611b8ca1f68d615</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>scts</name>
      <anchorfile>structtsms__tpdu__deliver__s.html</anchorfile>
      <anchor>ada219ad73e4dfbce10e3ff2f70464f59</anchor>
      <arglist>[7]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_tpdu_message_s</name>
    <filename>structtsms__tpdu__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>MobOrig</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a6e6aeef330a9a8dec2b2e35027ad65c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_tpdu_mti_t</type>
      <name>mti</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a5343c332e8f0900254d58a3ff8111163</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>pid</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a108af4844b813f4ea332bab4e84aa87e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>dcs</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a3670021244908632f923d920b860e433</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>udl</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a17382327252db61e15f14aa7447f4631</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>ud</name>
      <anchorfile>structtsms__tpdu__message__s.html</anchorfile>
      <anchor>a3db0d301634cbd0a517ab31fd2801bfd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_tpdu_report_s</name>
    <filename>structtsms__tpdu__report__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_TPDU_MESSAGE</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>a3e10964f384560d83a82cb4eb61f2226</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>smsc</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>a38e40692d827af97d4bcedb4dbbffab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>error</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>a6e79ee96ed2260a83b37ddffadd0a3bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>udhi</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>a4bb414943b60bf3e2891e2e74f4ba6bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>fcs</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>a551cefcd48f2a929589e54d67f029107</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>pi</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>a1459382aaf3e683abc00c4d7d242425d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>scts</name>
      <anchorfile>structtsms__tpdu__report__s.html</anchorfile>
      <anchor>ada219ad73e4dfbce10e3ff2f70464f59</anchor>
      <arglist>[7]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_tpdu_status_report_s</name>
    <filename>structtsms__tpdu__status__report__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_TPDU_MESSAGE</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a3e10964f384560d83a82cb4eb61f2226</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>smsc</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a38e40692d827af97d4bcedb4dbbffab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>udhi</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a4bb414943b60bf3e2891e2e74f4ba6bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>mms</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>ad9fabb1dca4bee43c9a920a254d9f26f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>lp</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a6444e99fe8e0d97fe5453f7592ac515c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>srq</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a687595b129dd6852c930d410fb987b5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>mr</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>ada5dc0a6758b35f32c0827370e8a9141</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>ra</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a71c646440dfbe9ed0d5277b1665f27a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>scts</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>ada219ad73e4dfbce10e3ff2f70464f59</anchor>
      <arglist>[7]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>dt</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a2827a99332f6e3d5eb6b00d2586f26d1</anchor>
      <arglist>[7]</arglist>
    </member>
    <member kind="variable">
      <type>tsms_tpdu_status_type_t</type>
      <name>st</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a992dede7e7c799965fbb107a0873f83f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>pi</name>
      <anchorfile>structtsms__tpdu__status__report__s.html</anchorfile>
      <anchor>a1459382aaf3e683abc00c4d7d242425d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tsms_tpdu_submit_s</name>
    <filename>structtsms__tpdu__submit__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSMS_DECLARE_TPDU_MESSAGE</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a3e10964f384560d83a82cb4eb61f2226</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>smsc</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a38e40692d827af97d4bcedb4dbbffab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>rd</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a76f4ecb9ec6bab244793e2c729ccddca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_tpdu_vpf_t</type>
      <name>vpf</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>ab5a68e196359edf0c74fbedb99727113</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>rp</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a1118d743a178dc0d92dd8a096b77524f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>udhi</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a4bb414943b60bf3e2891e2e74f4ba6bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>srr</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a03bfe36a49496db5c02e536f55bdf8ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>mr</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>ab9c147a8a545b6ffc9640b0ad4532d1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsms_address_t *</type>
      <name>da</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a880175daede78c0951a4504a186d0dcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>vp</name>
      <anchorfile>structtsms__tpdu__submit__s.html</anchorfile>
      <anchor>a1771b7b4f4d81ba158d309419427647e</anchor>
      <arglist>[7]</arglist>
    </member>
  </compound>
</tagfile>
