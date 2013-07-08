<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>tnet_dhcp.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp_8c</filename>
    <includes id="tnet__dhcp_8h" name="tnet_dhcp.h" local="yes" imported="no">tnet_dhcp.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dhcp_ctx_t *</type>
      <name>tnet_dhcp_ctx_create</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga4047baa7341d04b728d5884460214802</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_params_t *</type>
      <name>tnet_dhcp_params_create</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga7d73362e75ec846e6ce440ba9178069a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_reply_t *</type>
      <name>tnet_dhcp_send_request</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>gafd37386ea787357f72a4c71f8f66df4f</anchor>
      <arglist>(tnet_dhcp_ctx_t *ctx, tnet_dhcp_request_t *request)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_reply_t *</type>
      <name>tnet_dhcp_query</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga66e173bcf5fd24b06a204be44312c56a</anchor>
      <arglist>(tnet_dhcp_ctx_t *ctx, tnet_dhcp_message_type_t type, tnet_dhcp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_params_add_code</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga9fb8994c0bb692b29ccd0454cc592bc5</anchor>
      <arglist>(tnet_dhcp_params_t *params, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_ctx_def_t</name>
      <anchorfile>tnet__dhcp_8c.html</anchorfile>
      <anchor>aa58d75ace9326fe257d66da67933825e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_params_def_t</name>
      <anchorfile>tnet__dhcp_8c.html</anchorfile>
      <anchor>a15f3b7d6540c0da1287fe092220e7dee</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dhcp__message_8h" name="tnet_dhcp_message.h" local="yes" imported="no">tnet_dhcp_message.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <class kind="struct">tnet_dhcp_params_s</class>
    <class kind="struct">tnet_dhcp_ctx_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga4c4d364928ec9c9b7c2dbc6d59187b94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_CLIENT_PORT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga40d19eada507a56dd275125e3b1c195c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_SERVER_PORT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>gae5229a21da05427c690040fa34a6a2b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_VENDOR_ID_DEFAULT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga68fdef222cf61845b6d961766b4cf276</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MAX_CODES</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga2e7d7c17df6e0c697014a20ab344d1a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MAX_MSG_SIZE</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga9489b2e11946fde982adc9f88a148e5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_dhcp_query_discover</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>aedc60f87e2d0b05b0986eafe0d720322</anchor>
      <arglist>(ctx, params)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_dhcp_query_request</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>a0df10b4454c97b7612cfbcb581ef684b</anchor>
      <arglist>(ctx, params)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_dhcp_query_decline</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>a782749ba0da4d5e477f1e69f8483073d</anchor>
      <arglist>(ctx, params)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_dhcp_query_release</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>abf827e1e98d86cf6618c5dac9c10d39d</anchor>
      <arglist>(ctx, params)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_dhcp_query_inform</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>a85d77babd4233b7bc6009a312e10c326</anchor>
      <arglist>(ctx, params)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_params_s</type>
      <name>tnet_dhcp_params_t</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga6fc1dfe6651b6c630b32fb6744a269e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_ctx_s</type>
      <name>tnet_dhcp_ctx_t</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>gab40673583cd866001d0afc7db3e01e59</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_reply_t *</type>
      <name>tnet_dhcp_query</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga66e173bcf5fd24b06a204be44312c56a</anchor>
      <arglist>(tnet_dhcp_ctx_t *ctx, tnet_dhcp_message_type_t type, tnet_dhcp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dhcp_params_add_code</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga9fb8994c0bb692b29ccd0454cc592bc5</anchor>
      <arglist>(tnet_dhcp_params_t *params, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_ctx_t *</type>
      <name>tnet_dhcp_ctx_create</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga4047baa7341d04b728d5884460214802</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_params_t *</type>
      <name>tnet_dhcp_params_create</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga7d73362e75ec846e6ce440ba9178069a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_ctx_def_t</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>ae5301bee980eef0385c17c41b962aa4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_params_def_t</name>
      <anchorfile>tnet__dhcp_8h.html</anchorfile>
      <anchor>a8d6a9c960bfc72dc4226975e2ac545a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp_message.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp__message_8c</filename>
    <includes id="tnet__dhcp__message_8h" name="tnet_dhcp_message.h" local="yes" imported="no">tnet_dhcp_message.h</includes>
    <includes id="tnet__dhcp_8h" name="tnet_dhcp.h" local="yes" imported="no">tnet_dhcp.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">../tnet_utils.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dhcp_message_t *</type>
      <name>tnet_dhcp_message_create</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>adaf70c6f4ce8d349f9384427ef62769f</anchor>
      <arglist>(tnet_dhcp_message_op_t opcode)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_request_t *</type>
      <name>tnet_dhcp_request_create</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>aff1cfddd83d0cab8d5ae460ef6b64e72</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_message_t *</type>
      <name>tnet_dhcp_reply_create</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>a9ecc81df46370cec3691acc48339a273</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dhcp_message_serialize</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>adeb1e06721751ce944cb3b5b7b4fba72</anchor>
      <arglist>(const tnet_dhcp_ctx_t *ctx, const tnet_dhcp_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_message_t *</type>
      <name>tnet_dhcp_message_deserialize</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>aacd32aa714f4873545d046d96da49a2e</anchor>
      <arglist>(const struct tnet_dhcp_ctx_s *ctx, const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_dhcp_option_t *</type>
      <name>tnet_dhcp_message_find_option</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>a2b229a376755970e8ac2d2eea2c7ff24</anchor>
      <arglist>(const tnet_dhcp_message_t *message, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_message_add_codes</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>aed293c9712d53931456c003b566abbd4</anchor>
      <arglist>(tnet_dhcp_message_t *self, tnet_dhcp_option_code_t codes[], unsigned codes_count)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_message_def_t</name>
      <anchorfile>tnet__dhcp__message_8c.html</anchorfile>
      <anchor>a001d2d362157b75638399472c448348e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp_message.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp__message_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dhcp__option_8h" name="tnet_dhcp_option.h" local="yes" imported="no">tnet_dhcp_option.h</includes>
    <includes id="tnet__hardwares_8h" name="tnet_hardwares.h" local="yes" imported="no">tnet_hardwares.h</includes>
    <class kind="struct">tnet_dhcp_message_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MESSAGE_IS_REQUEST</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>ae2e302cbcea8869129bcc91161428d81</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MESSAGE_IS_REPLY</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>ab571915b071fb6ed6e577f8d68e0c3d6</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MAGIC_COOKIE</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a33f88fe4e708a2afc5a2b229bf22f821</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MESSAGE_MIN_SIZE</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>ad9883a8e753ae872d4883f561283add6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dhcp_message_type_e</type>
      <name>tnet_dhcp_message_type_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a0ca808a6b2e6ae7bf7ce313f258cf346</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dhcp_message_op_e</type>
      <name>tnet_dhcp_message_op_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a89d25a994f6584c738384c18de0f8090</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_message_s</type>
      <name>tnet_dhcp_message_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a659f843c5d28cdf6294fbde36667bb3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dhcp_messages_L_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>ae1f846a04b26fccc138b06cf931006ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dhcp_message_t</type>
      <name>tnet_dhcp_request_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a25e00e6e9184caed3b65675dfe7604f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dhcp_message_t</type>
      <name>tnet_dhcp_reply_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a6486614e440217fbb7d794f0c160a89f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp_message_type_e</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_discover</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a23945bd14286dcfe8684e72907eb8cef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_offer</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a99a05a69b4ebabad25fa9ebffbd6f93f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_request</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a1f97becc1fd671298a8ffc840f280509</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_decline</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a6d1e6eb6c6f8003d9d44df59b8005472</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_ack</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905abaf0807c55fca70088fe1c9c40616023</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_nack</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a1bc59be9cf0b3a33f81185bded360513</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_release</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a149634ef5af759448d1c351d14372c2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_type_inform</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a9ad24c42adb00d70b28a7ed0b62fe905a508ab5b809a40af17dd020c3e18cbd78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp_message_op_e</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a1dc99290aacbbc3f78905c89a35a5e92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_op_bootrequest</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a1dc99290aacbbc3f78905c89a35a5e92aa2edacd8fccb677b3700a851bc8b7d4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_op_bootreply</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a1dc99290aacbbc3f78905c89a35a5e92a98bb49a25fe584d571047853182966d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dhcp_message_serialize</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>aad66010e56c32d932bda60f073f53fd5</anchor>
      <arglist>(const struct tnet_dhcp_ctx_s *ctx, const tnet_dhcp_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_message_t *</type>
      <name>tnet_dhcp_message_deserialize</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>aacd32aa714f4873545d046d96da49a2e</anchor>
      <arglist>(const struct tnet_dhcp_ctx_s *ctx, const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_dhcp_option_t *</type>
      <name>tnet_dhcp_message_find_option</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>ada261fbb37de7bc6e63a0596355a9d2c</anchor>
      <arglist>(const tnet_dhcp_message_t *self, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_message_add_codes</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>aed293c9712d53931456c003b566abbd4</anchor>
      <arglist>(tnet_dhcp_message_t *self, tnet_dhcp_option_code_t codes[], unsigned codes_count)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_message_t *</type>
      <name>tnet_dhcp_message_create</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a58ad2c16d636e848c247ab618addabb3</anchor>
      <arglist>(tnet_dhcp_message_op_t opcode)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_request_t *</type>
      <name>tnet_dhcp_request_create</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a58d28c4dfab786f9b53d28e15c86feeb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_message_t *</type>
      <name>tnet_dhcp_reply_create</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a46fe33dfa57b8c43858b4445333851bf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_message_def_t</name>
      <anchorfile>tnet__dhcp__message_8h.html</anchorfile>
      <anchor>a90d96e17271118fc819dd15d4c6545f7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp_option.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp__option_8c</filename>
    <includes id="tnet__dhcp__option_8h" name="tnet_dhcp_option.h" local="yes" imported="no">tnet_dhcp_option.h</includes>
    <includes id="tnet__dhcp__option__sip_8h" name="tnet_dhcp_option_sip.h" local="yes" imported="no">tnet_dhcp_option_sip.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dhcp_option_t *</type>
      <name>tnet_dhcp_option_create</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>a8c191e2708eb2daf4e8386fa2f638830</anchor>
      <arglist>(tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_option_paramslist_t *</type>
      <name>tnet_dhcp_option_paramslist_create</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>acfb7f4858abc9bc7597f43c84209ca6b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_option_dns_t *</type>
      <name>tnet_dhcp_option_dns_create</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>adda6aa01b12a617cee461a1406e4675d</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_init</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>aa86c774dfbc192fcf37731deac3009af</anchor>
      <arglist>(tnet_dhcp_option_t *self, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_deinit</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>aede9304d533f9fec7529149b345da1ae</anchor>
      <arglist>(tnet_dhcp_option_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_option_t *</type>
      <name>tnet_dhcp_option_deserialize</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>a4f9f1042f27b33be6150ce5a7eea09f8</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_serialize</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>a155a3fdeef17dc15f70dfe8ff41dddf7</anchor>
      <arglist>(const tnet_dhcp_option_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_serializeex</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>a01c369b06ee3e99ed20ec3bcb7a1ce16</anchor>
      <arglist>(tnet_dhcp_option_code_t code, uint8_t length, const void *value, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_paramslist_add_code</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>a08821769abef66cf1e459fb89ba442a2</anchor>
      <arglist>(tnet_dhcp_option_paramslist_t *self, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_def_t</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>a3ed2f79cb57a68233ab3d5c7cbb8f6b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_paramslist_def_t</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>ace02e02ac52286b3f0e8894e54d51f25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_dns_def_t</name>
      <anchorfile>tnet__dhcp__option_8c.html</anchorfile>
      <anchor>abaf0ca973a723dea4becaaa62b28340f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp_option.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp__option_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <class kind="struct">tnet_dhcp_option_s</class>
    <class kind="struct">tnet_dhcp_option_paramslist_s</class>
    <class kind="struct">tnet_dhcp_option_dns_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_OPTION</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>aaaecef93a24661e93b845cff3e739374</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DECLARE_DHCP_OPTION</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a1215ccb86d8d3db9fb79e490e07a32e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dhcp_option_code_e</type>
      <name>tnet_dhcp_option_code_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a0d884b63b7233a9e16b0403d57601832</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_option_s</type>
      <name>tnet_dhcp_option_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a8309fe5e24a8dd2452e8f94e6ddf9903</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dhcp_options_L_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a38aa38901b7e776af2652fc15fa55356</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_option_paramslist_s</type>
      <name>tnet_dhcp_option_paramslist_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>af36405ef3b15a68849ef0660905e0973</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_option_dns_s</type>
      <name>tnet_dhcp_option_dns_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a142e55f732f165a5c5148d62cf920647</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp_option_code_e</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Pad</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3db16c179b4e803c4727c6e06da3176a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Subnet_Mask</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a22afaeba4902bd5b93252d8f7565d91e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Time_Offset</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267acb1125be7938317663790fd5abdad638</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Router</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a53d4594993d9fa1333144da093207ca9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Time_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aa9014537e2e713c7445c469d8742e00e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Name_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a9e227834898d7fd25765c893ad57311d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Domain_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a455ac19797e8925d54828634afc57e53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Log_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ae134f18d7811269d06871af23e124690</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Quotes_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ad2d3ef93abecf2ec87b6bc732fa9ff7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_LPR_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267afa6659c124ebe18b68bebf56d1a99606</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Impress_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267adc43d7c842dca6214876b2335f7ebf7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_RLP_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ab47d13b0b5f234c20b1fd8594e903054</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Hostname</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267acddef5f414494943a3dccf5f1c60b622</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Boot_File_Size</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1df26a2b2ba9a101b4722c07a95ad8b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Merit_Dump_File</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a9485cc76a7f5a955f6c557e39949e936</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Domain_Name</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a5e722990c22229b122cc82ef9a6860f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Swap_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aae77ca18e62d574a8ce3cb8b0c41306d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Root_Path</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a17c525c0f329ea844e2949047c9cf3c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Extension_File</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ae53b005959bb221f8a987a12b229607e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Forward_On_Off</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a01a678dee4780da54ba84a927a226c2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_SrcRte_On_Off</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a11c4aa754009c765dca9af0d38354d83</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Policy_Filter</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aa639ce27b22dd94f73230254fa5e614b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Max_DG_Assembly</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a6c8e408e641f168d0e16d6bf9f73a86b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Default_IP_TTL</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a172d60b4d8b665c640cc0e6d8d1ee4f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_MTU_Timeout</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267afd57e8f5523a60577f9f28929a96b5af</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_MTU_Plateau</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a6da75f842ab197b0dce33099a6c8b9a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_MTU_Interface</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267acc58dcea43623cb816a8d3aeb76b5ef1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_MTU_Subnet</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a7f0c5784c9c6c09beb001420c9dcd27b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Broadcast_Address</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1549b63a5d3d29b1bb1cc2edb244727b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Mask_Discovery</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac83c18e46a5eda9312734480a87052e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Mask_Supplier</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ae664fa2f96f74afa2d4b06b630df3918</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Router_Discovery</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3c7e0a7996a56850372085eeb17cb05b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Router_Request</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267adfa5bf99208f509c29482af852e947e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Static_Route</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a69ae5b7e45a10e01ec6ed2bfc3c5d432</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Trailers</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267af9afedc6aae1478dddf221cb50ff5e90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_ARP_Timeout</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267af695b120918d3b6184496e4f8dfbd08d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Ethernet</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a6c4d8483ef2919f61d483ca148373fe2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Default_TCP_TTL</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a2e040e5f455dbf8f0e58ce211d008b2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Keepalive_Time</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ab7c01bb939de46cebcf2fd59f4681c9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Keepalive_Data</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a4848cd5b247e19a53e25b8b19f0cda79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NIS_Domain</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a61b2484e865d87160c2fdc57f4d9cf26</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NIS_Servers</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3d38266d6255cc2758c7835c14dc4e64</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NTP_Servers</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a9f6393d845be5470b70541a8b0d61f6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Vendor_Specific</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1a117c6bf7e8529e47f15b7bb98134c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NETBIOS_Name_Srv</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a0a6dab62f4b3dbef55cd463baddcfbc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NETBIOS_Dist_Srv</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aa32205b2ce09e43636610da33af3f67a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NETBIOS_Node_Type</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a7f147e4d047d2520d72d112a982051a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NETBIOS_Scope</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac81ec528c4853567162a84d927201989</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_X_Window_Font</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1029f12f49c85b6632334bfa50c41470</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_X_Window_Manager</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac9f843f778dec2ea8c9a3448878de16a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Address_Request</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ad907c2d58a9105c09cadff20dd637cdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Address_Time</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ab6c1afaf4d45ffd3a7cba7a89a2b3842</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Overload</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1ae58715af5dc64d31636b52e8e1c9b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_DHCP_Msg_Type</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aec311da2af406a328f277b0f2fb3a82f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_DHCP_Server_Id</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a55b2feac2916ca0359a166cd181f90b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Parameter_List</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac8f3b4449ccfb57512bcf8512d8e6bc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_DHCP_Error_Message</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a6087dafa9de89a09e86673cf911401ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_DHCP_Max_Msg_Size</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a8eec205b20430ae6ceb30e6c0f6f7821</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Renewal_Time</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a22bb26a2d3bb13080d4e60d36629de84</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Rebinding_Time</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac93fb941a9a459ac948a04bc7d7046ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Class_Id</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ab718f2a66a86656d0b7a0a0054debef4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Client_Id</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3a053d95af35d07fd6b89fa221ecb25f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NetWare_IP_Domain</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a9debdf18426621bd3fa7766a1232f92b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NetWare_IP_Option</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3869b8054427578ddeb921307152709b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NIS_Domain_Name</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac5052d4f68689bfb4ed19aa48a199031</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NIS_Server_Addr</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1bbc999cc35cdaa528f6917e232ced1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Server_Name</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a12b8f8d1b8ef85c2b7ae1ceeccaaedf9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Bootfile_Name</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267abfdc7bfff9ec4ce7f31c70ebe998b954</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Home_Agent_Addrs</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a25976b54480f46182a27685193166544</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_SMTP_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a18acd1ebc079947540d8510028ade9c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_POP3_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aea60603b068a1e9c3092935a46a35620</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NNTP_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aa39ab80643b0d53856642e7bcb08e851</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_WWW_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ad6e4eaae6c03142e9cf34c5dcce1c9b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Finger_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a26997d21c22819aa026f17931526c3fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_IRC_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ab4f06c56ebc8542ff036874462fa06cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_StreetTalk_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a2c2a9461ead25b638470d117f62c693c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_STDA_Server</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a90bb3aa3d636f453b814e62f6b01afa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_User_Class</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac2d94d8867208f0b7dd7db0c8f4f0410</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Directory_Agent</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267addf51622995424216a5c9b093a09690d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Service_Scope</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a490bfcb5b1edc4d38e889445b2bae438</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Rapid_Commit</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a57eef60d953036aa71ca0f86f503277c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Client_FQDN</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ab71fc6aa6acd194b6168fd90d9416b79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Relay_Agent_Information</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac9e7b477c65828c675aa983550f20708</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_iSNS</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267af3056722f57c9e2deb11b7ac89bc6aad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NDS_Servers</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267af38d03ef84426035e4e98b0b150f67a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NDS_Tree_Name</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a6d5e0fd3c4e10ee5ca759f580df85379</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_NDS_Context</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267adda2c9999a502bcbea1a2d939c83722a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_BCMCS_Controller_Domain_Name_list</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a309d59bc829f89e327d564ca83f9ea35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_BCMCS_Controller_IPv4_address_option</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a26fb514ac58a40a4e929125ddf8444a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Authentication</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aebc20bbbd856ecf20e83bcc686ce9d47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_client_last_transaction_time</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a816b14a051325d9f932ff4716756f800</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_associated_ip</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a43d2e59cb8d933a8572342c8b6d866ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Client_System</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a37af94707d0f562976bfc6317c43bde2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Client_NDI</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267af147b15ea7029f0bbb947826e8b814a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_LDAP</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267adaaf1df2b4fbef075fb785a8711dea01</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_REMOVED_Unassigned</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1fa630140e4d92299c63be6d10781105</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_UUID_GUID</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ad7588ad44a5224ae76b821b7c62bb07b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_User_Auth</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267acd1fe71d641ec7dfcdb33ed9f43e66da</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_GEOCONF_CIVIC</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a52bdfbace1a1b2934fc0811dcfa06f68</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_PCode</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a21d49fa0ae4548d7af0f95baa07ac017</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_TCode</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a10bc22328c68c3000cf8d7b1628818b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Netinfo_Address</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a78bc5859b268bcc2d3545c8f5dfbae96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Netinfo_Tag</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ae887e6c0a82fb5ec307065228c51c790</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a52bc7b00fd1d68b5dc04151c326e0818</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Auto_Config</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267adbacec91caecd4a7fbe7e101b9ab6d2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Name_Service_Search</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a523555ccbf327ecff12b085ae5d7db3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Subnet_Selection_Option</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a29bd9fe0badcd3468055ce6cdae92a24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Domain_Search</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a80cba2bedb41e20701ae27ace44ad477</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_SIP_Servers_DHCP_Option</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a22345c46a4d62c70d3b1339e581a3731</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Classless_Static_Route_Option</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aa388f714c3767f8c9a86e48d7d525b4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_CCC</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1ad3a007b2b9cbad13ba5a008b3ec0df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_GeoConf_Option</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a906df978c0c2f24cb8f5336e1b914d23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_V_I_Vendor_Class</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a8ad8e03ea332ead5aa48ce48bf2be268</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_V_I_Vendor_Specific_Information</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1a93a89cce7f5de694b59d18e970318e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Etherboot_signature</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a76811d968f0658217813a3f03f301821</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_DOCSIS</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a4ba4c7001da4ee3c95ce6c881fca4d91</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_TFTP_Server_IP</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a13bd9cc98ebc3ecc966ee435816795c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Kernel_options</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a0cd77d0466916c4dc19d382d3b31640b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Call_Server_IP</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ae7888f655111c7372d59fb3b678173ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Ethernet_interface</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267abc32b875f56efbd40f3012886e274144</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Discrimination</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a38daa9f7725d36d4a2815f2f00653942</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Remote_statistics_server_IP</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a301449c7264ff3ebaf8ecd8ac0319acb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_IEEE_802_1Q_VLAN_ID</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a570b3c33b9673eae85a2c6606f554d4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_IEEE_802_1D_p</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a69a488af2129f44b5bf4a857b2121c53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_DSCP</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a6eba8281d22872a7399ef0a251dc45c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_HTTP_Proxy</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a8a3dbcbf519c72394a53ae0f8a2550c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_OPTION_PANA_AGENT</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a7eda4742b715b5ea197c7b34437e90a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_OPTION_V4_LOST</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a5619ebc89deebdb25f555edb9f9a9ad7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_OPTION_CAPWAP_AC_V4</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3eb6db2d74930e5a1491566009ceb455</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_OPTION_IPv4_Address_MoS</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a86da375324b341191ef9d26b17c9b3f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_OPTION_IPv4_FQDN_MoS</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a12103feb04edb440553a019d355acdd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_TFTP_server_address</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267afa0c82213301d607a4d9a2c96f455637</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Etherboot</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac5a27f085ec2c7f16344780104ea594a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_GRUB_configuration_path_name</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac85874f026b1900e5b4d49c7b81b5324</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_IP_Telephone</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a3cc7dd46e6f5c867f73ffdd8b63075a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_PacketCable_and_CableHome</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a7a523e971005cd539de416bf08f808e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_PXELINUX_Magic</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a1be705d680c1dfb50a4028aa97b1a57d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Configuration_File</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267aab40cb17d9a5b3a08b606e2689b4c3c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Path_Prefix</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267ac454853cf81c135e7ba5120dab807b8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Reboot_Time</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a987d24bb1c47b0a2e1104e4a1eae40d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Subnet_Allocation</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267af600259f2b6bf657860363d3ff54a36e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_Virtual_Subnet</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a9a5537489715198359224a72a8333f5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_null</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a5248003e467b342ad8d367d77c03a102</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp_code_End</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ae8a9751325d4773a0a83f3b9acd1c267a57f24581049c7b6b50678bedf5d1e18b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_init</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>aa86c774dfbc192fcf37731deac3009af</anchor>
      <arglist>(tnet_dhcp_option_t *self, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_deinit</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>aede9304d533f9fec7529149b345da1ae</anchor>
      <arglist>(tnet_dhcp_option_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_option_t *</type>
      <name>tnet_dhcp_option_deserialize</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a4f9f1042f27b33be6150ce5a7eea09f8</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_serialize</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a155a3fdeef17dc15f70dfe8ff41dddf7</anchor>
      <arglist>(const tnet_dhcp_option_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_serializeex</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a01c369b06ee3e99ed20ec3bcb7a1ce16</anchor>
      <arglist>(tnet_dhcp_option_code_t code, uint8_t length, const void *value, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_option_paramslist_add_code</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a08821769abef66cf1e459fb89ba442a2</anchor>
      <arglist>(tnet_dhcp_option_paramslist_t *self, tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_option_t *</type>
      <name>tnet_dhcp_option_create</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>ab9c6dfc8352393b8052b651930c1a417</anchor>
      <arglist>(tnet_dhcp_option_code_t code)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_option_paramslist_t *</type>
      <name>tnet_dhcp_option_paramslist_create</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a2b702a769d6ec8e978fbf96883f16702</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_option_dns_t *</type>
      <name>tnet_dhcp_option_dns_create</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a1b1f05e525002a7604ae09b3a93c4392</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_def_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a4375d338dbe4882ff1e4b6f50a5930c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_ns_def_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a848fe1ef765215af38d449d4b0fd5943</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_paramslist_def_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a4188002caa3f5d304c2fc8d05f12ff95</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_dns_def_t</name>
      <anchorfile>tnet__dhcp__option_8h.html</anchorfile>
      <anchor>a5fb79a339ebd4e5f8d3f4a660a1e1b5d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp_option_sip.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp__option__sip_8c</filename>
    <includes id="tnet__dhcp__option__sip_8h" name="tnet_dhcp_option_sip.h" local="yes" imported="no">tnet_dhcp_option_sip.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">dns/tnet_dns_rr.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dhcp_option_sip_t *</type>
      <name>tnet_dhcp_option_sip_create</name>
      <anchorfile>tnet__dhcp__option__sip_8c.html</anchorfile>
      <anchor>ae86d8d7fc2165ad48e3b0045d9a46741</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_sip_def_t</name>
      <anchorfile>tnet__dhcp__option__sip_8c.html</anchorfile>
      <anchor>a52f05e713d97bbab043093691ffb7b5c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp_option_sip.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>tnet__dhcp__option__sip_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dhcp__option_8h" name="tnet_dhcp_option.h" local="yes" imported="no">tnet_dhcp_option.h</includes>
    <class kind="struct">tnet_dhcp_option_sip_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dhcp_option_sip_s</type>
      <name>tnet_dhcp_option_sip_t</name>
      <anchorfile>tnet__dhcp__option__sip_8h.html</anchorfile>
      <anchor>a39b62ac1f962ef42faaa1243c19d0700</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp_option_sip_t *</type>
      <name>tnet_dhcp_option_sip_create</name>
      <anchorfile>tnet__dhcp__option__sip_8h.html</anchorfile>
      <anchor>a987bc86af29c0bf1f00f6af77c6a9794</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp_option_sip_def_t</name>
      <anchorfile>tnet__dhcp__option__sip_8h.html</anchorfile>
      <anchor>a722bd3581d65a608a7eec06a40920992</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6_8c</filename>
    <includes id="tnet__dhcp6_8h" name="tnet_dhcp6.h" local="yes" imported="no">tnet_dhcp6.h</includes>
    <member kind="function">
      <type>tnet_dhcp6_ctx_t *</type>
      <name>tnet_dhcp6_ctx_create</name>
      <anchorfile>tnet__dhcp6_8c.html</anchorfile>
      <anchor>a65328f23502499947219f80b285349b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_reply_t *</type>
      <name>tnet_dhcp6_send_request</name>
      <anchorfile>group__tnet__dhcp6__group.html</anchorfile>
      <anchor>gadac4a203c542c67bcd608974944ade8d</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, tnet_dhcp6_request_t *request)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_reply_t *</type>
      <name>tnet_dhcp6_requestinfo</name>
      <anchorfile>group__tnet__dhcp6__group.html</anchorfile>
      <anchor>gac8dd99f4e54866bbdf190ce5061b5b3a</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, const tnet_dhcp6_option_orequest_t *orequest)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_ctx_def_t</name>
      <anchorfile>tnet__dhcp6_8c.html</anchorfile>
      <anchor>aedef4f922e3c639b6bcbf4625f347050</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dhcp6__message_8h" name="tnet_dhcp6_message.h" local="yes" imported="no">tnet_dhcp6_message.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <class kind="struct">tnet_dhcp6_ctx_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_VENDOR_CLASS_DATA_DEFAULT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a77a47e057fcd3feee6a6ea6ca67be5a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_All_DHCP_Relay_Agents_and_Servers</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>aba226ab3184b0eeb37fd2cd175993af0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_All_DHCP_Servers</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a3daf3c978170cc4d1c3efa71704bdffb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_SOL_MAX_DELAY</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>ac7651259caa112f43790703a8d982957</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_SOL_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>acd358db10e5574bacf05dd459a80669a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_SOL_MAX_RT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a8d86fe584a433bf33683590b92b472f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REQ_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>ac73b545aaf7be2d2508ef5c4a4dbbcba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REQ_MAX_RT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a4e30fbce41200c6c8948f11108d945d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REQ_MAX_RC</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>ac63784bde12e796094e5c7c574cad858</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_CNF_MAX_DELAY</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a705366e8798017d5a91cf672ecd427cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_CNF_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a88918fd42ad5bdad2ee38c647eb9ab55</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_CNF_MAX_RT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a05b88aad73b2fdd90877a7d8b12c6e17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_CNF_MAX_RD</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a0beba10b142e2504854d5e6b745d637c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REN_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a9b642dc3551499424e4ccb5fd7de1a87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REN_MAX_RT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>aad8e25c55996b4b539415572040509bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REB_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a62c4bbc046ae2586dc53c87dbad79354</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REB_MAX_RT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>ab5db7aad6ff064b544ce449f2041f2d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_INF_MAX_DELAY</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>ab0c98c0588182dcc5659b6d2b6d60742</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_INF_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a2a5bc3707cd31848a9e8d17786b3b9b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_INF_MAX_RT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>afce91c3c151c45351e44fa6a84620321</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REL_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a51722f784ecd9892c789c5a68d230db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REL_MAX_RC</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a0269ce1104f8f96dbf7d28b70135c4fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_DEC_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a2bd1d5297f92edbdd1537df9fadd2d8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_DEC_MAX_RC</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>aa378cfd76fb91722de7c82dc149b9351</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REC_TIMEOUT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a2e5f4d83b55504ac3176a030530920bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_REC_MAX_RC</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>aef253c7ac2234bf2fd261a3713575d9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_RT_HOP_COUNT_LIMIT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>aa42aa205bd76ebe8eb41499de2fddc6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_TIMEVAL_INFINITY</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>abde3fc1a4d5a887fb742215832866018</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_CLIENT_PORT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a102edccd001f74ce44438d0b7836d4eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_SERVER_PORT</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a8088025204cff9dc319aa02f0e448a94</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_ctx_s</type>
      <name>tnet_dhcp6_ctx_t</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>aa5ef731f53320126a141fcf6e6ff244e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_reply_t *</type>
      <name>tnet_dhcp6_requestinfo</name>
      <anchorfile>group__tnet__dhcp6__group.html</anchorfile>
      <anchor>gac8dd99f4e54866bbdf190ce5061b5b3a</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, const tnet_dhcp6_option_orequest_t *orequest)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_ctx_t *</type>
      <name>tnet_dhcp6_ctx_create</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a356d67baebf7f38649454a27dac20983</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_ctx_def_t</name>
      <anchorfile>tnet__dhcp6_8h.html</anchorfile>
      <anchor>a4b2db90770aebf579b3c4413ec5e0afd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6_duid.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6__duid_8c</filename>
    <includes id="tnet__dhcp6__duid_8h" name="tnet_dhcp6_duid.h" local="yes" imported="no">tnet_dhcp6_duid.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_llt_serialize</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a6158e8d5f12ebb59d063aaa2188b8c88</anchor>
      <arglist>(const tnet_dhcp6_duid_llt_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_en_serialize</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a49faf9f7df8f9bec56a7513d80760bbf</anchor>
      <arglist>(const tnet_dhcp6_duid_en_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_ll_serialize</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>ad3b89b93ceb86c6eefb69edb4c4e0468</anchor>
      <arglist>(const tnet_dhcp6_duid_ll_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_duid_llt_t *</type>
      <name>tnet_dhcp6_duid_llt_create</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>ad669e57db28c397394675192c5491045</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_duid_en_t *</type>
      <name>tnet_dhcp6_duid_en_create</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a9676e3d562b936c4527404cc2f7efff9</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_duid_ll_t *</type>
      <name>tnet_dhcp6_duid_ll_create</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>ab0782d8fce2411236be32df83653c626</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_init</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a77fedc7d6fdba85f14b495e79cbc49a2</anchor>
      <arglist>(tnet_dhcp6_duid_t *self, tnet_dhcp6_duid_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_deinit</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a300a2a4fd5176167fdb36690e56614a8</anchor>
      <arglist>(tnet_dhcp6_duid_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_duid_t *</type>
      <name>tnet_dhcp6_duid_deserialize</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a3199da6b9c0b69f3624797ca453b34a8</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_serialize</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a9e5ba71e2a4e5d7e97fe1a5638ff49d7</anchor>
      <arglist>(const tnet_dhcp6_duid_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_duid_llt_def_t</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a6272c96c0db70f87dfd83fa2830b1293</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_duid_en_def_t</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>a00e01ddd6f88a34736506ecf3e61cefe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_duid_ll_def_t</name>
      <anchorfile>tnet__dhcp6__duid_8c.html</anchorfile>
      <anchor>adace29407e9d71220f2c226413e06f00</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6_duid.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6__duid_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__hardwares_8h" name="tnet_hardwares.h" local="yes" imported="no">tnet_hardwares.h</includes>
    <class kind="struct">tnet_dhcp6_duid_s</class>
    <class kind="struct">tnet_dhcp6_duid_llt_s</class>
    <class kind="struct">tnet_dhcp6_duid_en_s</class>
    <class kind="struct">tnet_dhcp6_duid_ll_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_DUID</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ab4c6f2cd628bd38f47a2b1ee40cbd0ea</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_DUID_LLT</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ae98050166aa26df3653f93a294a9f050</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_DUID_EN</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a414aa40eb67f6d0fd897d35558068d4c</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_DUID_LL</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a6dcc472aadbac1899c34516d268e2e53</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DECLARE_DHCP6_DUID</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>aecb22b4bb4e7cfa73066e7e24fa273bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dhcp6_duid_type_e</type>
      <name>tnet_dhcp6_duid_type_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a6ace50507f0d1ff6a3d11ad7a3abcc45</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_duid_s</type>
      <name>tnet_dhcp6_duid_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ac98eadf10c17c6e565659d1ca092ce2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dhcp6_duids_L_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a2ad6d664409b477704d224683c27c637</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_duid_llt_s</type>
      <name>tnet_dhcp6_duid_llt_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a4588ecc95aef0ad9aef18c98f434e30a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_duid_en_s</type>
      <name>tnet_dhcp6_duid_en_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>aa2bd03ce3ce8d569d40613c1acfe2eb9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_duid_ll_s</type>
      <name>tnet_dhcp6_duid_ll_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a1ea496b636bc7aad4c75421a88c07304</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp6_duid_type_e</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ae4487e952ac42c84e59514abd6b42fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_duid_linklayer_plus_time</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ae4487e952ac42c84e59514abd6b42fcaaf8d0386e94677b1e9ebddb38af60c0e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_duid_Vendor_assigned_id</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ae4487e952ac42c84e59514abd6b42fcaab9178b2917c5cea836d2139dc30074c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_duid_linklayer</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ae4487e952ac42c84e59514abd6b42fcaaad211ab6478327692901e93bad0ae975</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_init</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a77fedc7d6fdba85f14b495e79cbc49a2</anchor>
      <arglist>(tnet_dhcp6_duid_t *self, tnet_dhcp6_duid_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_deinit</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a300a2a4fd5176167fdb36690e56614a8</anchor>
      <arglist>(tnet_dhcp6_duid_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_duid_t *</type>
      <name>tnet_dhcp6_duid_deserialize</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a3199da6b9c0b69f3624797ca453b34a8</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_duid_serialize</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a9e5ba71e2a4e5d7e97fe1a5638ff49d7</anchor>
      <arglist>(const tnet_dhcp6_duid_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_duid_llt_t *</type>
      <name>tnet_dhcp6_duid_llt_create</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>af6415090eae08032278ca9bbc9cb05a6</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_duid_en_t *</type>
      <name>tnet_dhcp6_duid_en_create</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a6ffd8bf8ee46ce8ffc113d0ace87aaf1</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_duid_ll_t *</type>
      <name>tnet_dhcp6_duid_ll_create</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ae87a2c797c94c599aa338d7e9968a218</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_duid_llt_def_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ad547787afd9e2da4814a0d1521d4f0ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_duid_en_def_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>ab238a01b1746af439c0bb24454461929</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_duid_ll_def_t</name>
      <anchorfile>tnet__dhcp6__duid_8h.html</anchorfile>
      <anchor>a023c6eeebfc906ffc76746420d96dcf7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6_message.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6__message_8c</filename>
    <includes id="tnet__dhcp6__message_8h" name="tnet_dhcp6_message.h" local="yes" imported="no">tnet_dhcp6_message.h</includes>
    <includes id="tnet__dhcp6_8h" name="tnet_dhcp6.h" local="yes" imported="no">tnet_dhcp6.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dhcp6_message_t *</type>
      <name>tnet_dhcp6_message_create</name>
      <anchorfile>tnet__dhcp6__message_8c.html</anchorfile>
      <anchor>a2fc793a413bfb883283381f34af6fbc6</anchor>
      <arglist>(tnet_dhcp6_message_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_request_t *</type>
      <name>tnet_dhcp6_request_create</name>
      <anchorfile>tnet__dhcp6__message_8c.html</anchorfile>
      <anchor>ae36dd840f14b603a475783ef25c76810</anchor>
      <arglist>(tnet_dhcp6_message_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dhcp6_message_serialize</name>
      <anchorfile>tnet__dhcp6__message_8c.html</anchorfile>
      <anchor>a9e395202fd1e6d0c6ba0c55682540c3d</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, const tnet_dhcp6_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_message_t *</type>
      <name>tnet_dhcp6_message_deserialize</name>
      <anchorfile>tnet__dhcp6__message_8c.html</anchorfile>
      <anchor>a49db33d334a348c07b63a482690ac5ec</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_message_def_t</name>
      <anchorfile>tnet__dhcp6__message_8c.html</anchorfile>
      <anchor>a11a2de3d84357d63c0f4908c458f975a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6_message.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6__message_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dhcp6__option_8h" name="tnet_dhcp6_option.h" local="yes" imported="no">tnet_dhcp6_option.h</includes>
    <class kind="struct">tnet_dhcp6_message_s</class>
    <member kind="typedef">
      <type>enum tnet_dhcp6_message_type_e</type>
      <name>tnet_dhcp6_message_type_t</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a0bcb6998750e054507a274a9e472d9a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_message_s</type>
      <name>tnet_dhcp6_message_t</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a7760565583413b629d7a07a9e29b4127</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dhcp6_messages_L_t</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a0d743e984418aa468d03b72d71ad8feb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dhcp6_message_t</type>
      <name>tnet_dhcp6_request_t</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>aa865622fe65b53de943c7a707bcae9bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dhcp6_message_t</type>
      <name>tnet_dhcp6_reply_t</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a45b3436dedd8df1cc9dbcc5926250860</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp6_message_type_e</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_solicit</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa752d897b810360fba91c3c8d8ec3312c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_advertise</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fafd50f635f56e1c6430c7a7d3a4b07747</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_request</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa8c4f308655ddd23c677ed36652c76cce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_confirm</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa269988c1d51a8ba5aaec3a166baa1eba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_renew</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa951d6681cdfed91bb96b57b17e9aeff7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_rebind</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa969ca16f1829dd91323dc2fa5394736f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_reply</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051faeb4607fab695e013e71f0620586e3210</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_release</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa2d441ba472324c0bc30c2242491eac6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_decline</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051faa761896a9e21e6973d0525e014198edf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_reconfigure</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa935a038fee6b2fee577300c88540bdbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_information_request</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fa6ea39e333ea3009f149c5c5a5d0c9fc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_relay_forw</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051fac719efc885894060c1c2981d88bcb43c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_type_relay_repl</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a08323bace5a61ab47ec9035230fd051facd0f656ce45fa652dbfe61b9e174eb68</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dhcp6_message_serialize</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a765adabb5c24a3202851887aede7b2a6</anchor>
      <arglist>(const struct tnet_dhcp6_ctx_s *ctx, const tnet_dhcp6_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_message_t *</type>
      <name>tnet_dhcp6_message_deserialize</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a4ce79b2c9ad5e947922a0dc8bf1a4b86</anchor>
      <arglist>(const struct tnet_dhcp6_ctx_s *ctx, const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_message_t *</type>
      <name>tnet_dhcp6_message_create</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>ad4f84b23d7525a65b49b4cf0959cb2e7</anchor>
      <arglist>(tnet_dhcp6_message_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_request_t *</type>
      <name>tnet_dhcp6_request_create</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a86e344131239a1abbc0a5b2020ba570c</anchor>
      <arglist>(tnet_dhcp6_message_type_t type)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_message_def_t</name>
      <anchorfile>tnet__dhcp6__message_8h.html</anchorfile>
      <anchor>a3a81bc718d8e4a5337b9323898674b65</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6_option.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6__option_8c</filename>
    <includes id="tnet__dhcp6__option_8h" name="tnet_dhcp6_option.h" local="yes" imported="no">tnet_dhcp6_option.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dhcp6_option_t *</type>
      <name>tnet_dhcp6_option_create</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a85748a874ec0bffb44e93bc8736b5d3e</anchor>
      <arglist>(tnet_dhcp6_option_code_t code, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_identifier_t *</type>
      <name>tnet_dhcp6_option_indentifer_create</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a40b898765178c4cd270f8e434a60e99c</anchor>
      <arglist>(tnet_dhcp6_option_code_t code, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_identifier_t *</type>
      <name>tnet_dhcp6_option_clientid_create</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a9580a04b7683486bed424ddabde6363a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_identifier_t *</type>
      <name>tnet_dhcp6_option_serverid_create</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>ac09403b11d38fa71d5e43e8764b8b3d0</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_orequest_t *</type>
      <name>tnet_dhcp6_option_orequest_create</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>aee59d1f340f3dcb41b5f2d26d3d4205f</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_orequest_t *</type>
      <name>tnet_dhcp6_option_orequest_create_null</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>ab0c5c5a2ebd3c1c52d37baf37b325125</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_vendorclass_t *</type>
      <name>tnet_dhcp6_option_vendorclass_create</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a5531acf377107cd2aa88ffd3870ba380</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_vendorclass_t *</type>
      <name>tnet_dhcp6_option_vendorclass_create_null</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>ad966bc23c12478675fc8f67345bc2398</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_t *</type>
      <name>tnet_dhcp6_option_deserialize</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a6deadd8ce3e250a9c2d0d5f9d2aa1417</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_option_serialize</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a616180ce101ac37af7279cd50f754176</anchor>
      <arglist>(const tnet_dhcp6_option_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_option_serializeex</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a1365ca6c4618af8148b33da50e5014ce</anchor>
      <arglist>(tnet_dhcp6_option_code_t code, uint8_t length, const void *value, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_option_orequest_add_code</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a78b0a34a3f71f481b62bef2e62a62ae2</anchor>
      <arglist>(tnet_dhcp6_option_orequest_t *self, uint16_t code)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_def_t</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a4d539d4293a5ffe07892de7e47446c0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_identifier_def_t</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>ac27e5694e1acf4290f84bb4a3d6f9922</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_orequest_def_t</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a1d68df186f478624ee10190391fec0ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_vendorclass_def_t</name>
      <anchorfile>tnet__dhcp6__option_8c.html</anchorfile>
      <anchor>a58062e3e28be055815c209a17e73d277</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dhcp6_option.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>tnet__dhcp6__option_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dhcp6__duid_8h" name="tnet_dhcp6_duid.h" local="yes" imported="no">tnet_dhcp6_duid.h</includes>
    <class kind="struct">tnet_dhcp6_option_data_s</class>
    <class kind="struct">tnet_dhcp6_option_s</class>
    <class kind="struct">tnet_dhcp6_option_identifier_s</class>
    <class kind="struct">tnet_dhcp6_option_orequest_s</class>
    <class kind="struct">tnet_dhcp6_option_vendorclass_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP6_OPTION</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>aee20957dcd988b9eeb518c7dd5b29b0c</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DECLARE_DHCP6_OPTION_DATA</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>af4e4efa6aafda6f9655e1f107ebcff64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DECLARE_DHCP6_OPTION</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a1ed5fab473cbbdae0872f7b01f7b1901</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dhcp6_option_code_e</type>
      <name>tnet_dhcp6_option_code_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a831211e647d27d29c4137a406e0aa754</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dhcp6_statuscode_e</type>
      <name>tnet_dhcp6_statuscode_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a8a4ebada9e439ad886c5749e74d059dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_option_data_s</type>
      <name>tnet_dhcp6_option_data_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a80e20b0d831003308feb3f8b0521639a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_option_s</type>
      <name>tnet_dhcp6_option_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>ae1d01310fe3e2833f086e881ce3f9def</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dhcp6_options_L_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>adda8965a7c025d246c08c179c4025166</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_option_identifier_s</type>
      <name>tnet_dhcp6_option_identifier_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>ae7b3e87849bdddd627541ac8b5cfa1d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dhcp6_option_identifier_t</type>
      <name>tnet_dhcp6_option_clientid_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>aee3178ccfb9a0b673049f4e810ff8533</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dhcp6_option_identifier_t</type>
      <name>tnet_dhcp6_option_serverid_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>ad07c128ed8dfd9a9f478cada29af691a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_option_orequest_s</type>
      <name>tnet_dhcp6_option_orequest_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a17908a94b442306f9ff55fbef7fc2007</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp6_option_vendorclass_s</type>
      <name>tnet_dhcp6_option_vendorclass_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>acbc7975ffa9445566f76460082c17c63</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp6_option_code_e</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_clientid</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a3327501be0a9487e75ac42ce86a36e67</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_serverid</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755adca89895d7bbd1fe8e20c28398075ab3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_ia_na</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755ad64dd81543ed891886c102099798c36a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_ia_ta</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755adf0877126eb37e961e0fc9d6e404c224</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_iaaddr</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755add94edfa6121b8778986194871b47ab0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_oro</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a45f7a09638657fb5acd4f3985d446551</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_preference</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a61ffa98b9e00f00c48788b1501ff3ea2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_elapsed_time</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755ab4c8310c86603be2e86b3aedf5b5101a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_relay_msg</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a5fd852298a96a6fe86e96dab116182c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_auth</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755aca52feda034b487bfcc7b0d916ec52a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_unicast</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a27798813cf5efbd66b3ae6930798ebcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_status_code</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755af21f3fecb13fccab021ddf3b6935b27f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_rapid_commit</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755ad1d6fa5166fa0c40fdcc30a8cb702340</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_user_class</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a5d8ae4e5e4f262b7d11a913eaecd3c0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_vendor_class</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755ac165ac987ca81ee5d869584fbcdadcf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_vendor_opts</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a9e056b5604dfe319deb2e8cd58861afa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_interface_id</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755ad0304c4f94eb24f4c16c84253c003ec6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_reconf_msg</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a7ed5183ce5d4957dcad4bba1e0fb0553</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_code_reconf_accept</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a18e26c5402e469dd0b563e74157755a7dd4ea51d8c318bce7fc7176e7372c2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dhcp6_statuscode_e</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_statuscode_Success</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3aa5fe1bbfa18c93352d763406a6abee895</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_statuscode_UnspecFail</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3aa5211f6b0b6572ff6639fe19868d2a1a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_statuscode_NoAddrsAvail</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3aab7aaf46c2c61305b063f2d95c0cdced0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_statuscode_NoBinding</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3aa82d74c3472d527e8fb7ee35d683c561c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_statuscode_NotOnLink</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3aaf150cd0a65b9dcf8b149df553c9829a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dhcp6_statuscode_UseMulticast</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6a1e604c89f4b34f7c0aa133c0d9ec3aa5f0a83e8f0700574bd962c5e67ed0ba0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_option_t *</type>
      <name>tnet_dhcp6_option_deserialize</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a6deadd8ce3e250a9c2d0d5f9d2aa1417</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_option_serialize</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a616180ce101ac37af7279cd50f754176</anchor>
      <arglist>(const tnet_dhcp6_option_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp6_option_serializeex</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a1365ca6c4618af8148b33da50e5014ce</anchor>
      <arglist>(tnet_dhcp6_option_code_t code, uint8_t length, const void *value, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dhcp6_option_orequest_add_code</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a254494dfc26351e8535871b753f8ef92</anchor>
      <arglist>(tnet_dhcp6_option_orequest_t *self, uint16_t code)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_t *</type>
      <name>tnet_dhcp6_option_create</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a9123e1057d08e16a258f31a7b946141d</anchor>
      <arglist>(tnet_dhcp6_option_code_t code, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_identifier_t *</type>
      <name>tnet_dhcp6_option_indentifer_create</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>adfef6b59485ca292c871e7099990a983</anchor>
      <arglist>(tnet_dhcp6_option_code_t code, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_identifier_t *</type>
      <name>tnet_dhcp6_option_clientid_create</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a41729803caa5cfed4cc32b628d12cea3</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_identifier_t *</type>
      <name>tnet_dhcp6_option_serverid_create</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a32baebeda950dddbcb7b097fa802610b</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_orequest_t *</type>
      <name>tnet_dhcp6_option_orequest_create</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>aa01c6f0e1dab917ad548e6b9ca24c5a3</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_orequest_t *</type>
      <name>tnet_dhcp6_option_orequest_create_null</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a226b391dadb710d3ad724d2dd9e01ff4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_vendorclass_t *</type>
      <name>tnet_dhcp6_option_vendorclass_create</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>abc11e9ad6fde65adb466965e97519a49</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dhcp6_option_vendorclass_t *</type>
      <name>tnet_dhcp6_option_vendorclass_create_null</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a194c3360bfcc440c40876408aa66f827</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_def_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>aca4bbdf87981922b053808636c8c7dfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_identifier_def_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a01cef52de83efd6444c6b70f2ecb2e91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_orequest_def_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a7b8899d2a304e8dd5295cb126123ebee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dhcp6_option_vendorclass_def_t</name>
      <anchorfile>tnet__dhcp6__option_8h.html</anchorfile>
      <anchor>a8820aedacaef53f0e229fdc8fa0f37fb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns_8c</filename>
    <includes id="tnet__dns_8h" name="tnet_dns.h" local="yes" imported="no">tnet_dns.h</includes>
    <includes id="tnet__dns__regexp_8h" name="tnet_dns_regexp.h" local="yes" imported="no">tnet_dns_regexp.h</includes>
    <includes id="tnet__dns__message_8h" name="tnet_dns_message.h" local="yes" imported="no">tnet_dns_message.h</includes>
    <includes id="tnet__dns__opt_8h" name="tnet_dns_opt.h" local="yes" imported="no">tnet_dns_opt.h</includes>
    <includes id="tnet__dns__srv_8h" name="tnet_dns_srv.h" local="yes" imported="no">tnet_dns_srv.h</includes>
    <includes id="tnet__dns__naptr_8h" name="tnet_dns_naptr.h" local="yes" imported="no">tnet_dns_naptr.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_cache_maintenance</name>
      <anchorfile>tnet__dns_8c.html</anchorfile>
      <anchor>a16c6661ccdbde58b205fa5c9366f0581</anchor>
      <arglist>(tnet_dns_ctx_t *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_cache_entry_add</name>
      <anchorfile>tnet__dns_8c.html</anchorfile>
      <anchor>a9ec0ba4b1b141c296cba22236a4fa179</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t *response)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_dns_cache_entry_t *</type>
      <name>tnet_dns_cache_entry_get</name>
      <anchorfile>tnet__dns_8c.html</anchorfile>
      <anchor>a510ba4e4ee2029a62b548239bed72e5e</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_ctx_t *</type>
      <name>tnet_dns_ctx_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga080c974f8f6bdb3779bfae98956e9821</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_cache_entry_t *</type>
      <name>tnet_dns_cache_entry_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga130888ddea8e08e520a2ffd02278d90d</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t *answer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_cache_clear</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga4908f53f564e46f39472e1dc825aa3ce</anchor>
      <arglist>(tnet_dns_ctx_t *ctx)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_resolve</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaaaee535d51058d67d9f170239de354b3</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_enum</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaf08d4dbbaf1025a17613bd553bf4db78</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *e164num, const char *domain)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tnet_dns_enum_2</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga1aa07c721d9e2e7bac6eea9f44a515c8</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *service, const char *e164num, const char *domain)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_query_srv</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gab4b9b4f145decf0d2886c8a4ea25a313</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *service, char **hostname, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_query_naptr_srv</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga0c2c6cdb03f205b6ce16af9398d91730</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *domain, const char *service, char **hostname, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_add_server</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga9d617f0cf0f0dc70e4a3bca53ab23824</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *host)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_cache_entry_def_t</name>
      <anchorfile>tnet__dns_8c.html</anchorfile>
      <anchor>a3558b767074cab929ebb5ff83ea07e87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_ctx_def_t</name>
      <anchorfile>tnet__dns_8c.html</anchorfile>
      <anchor>a9794de95cdab15b74749bba2f785f9d8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__message_8h" name="tnet_dns_message.h" local="yes" imported="no">tnet_dns_message.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <class kind="struct">tnet_dns_cache_entry_s</class>
    <class kind="struct">tnet_dns_ctx_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_CACHE_TTL</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gad0036c81833a0d67783075346183f741</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga27019bac31c0cd87fcf5f1d440d44db2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_DGRAM_SIZE_DEFAULT</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga3f0a0842a1099131e50c4ce1358a8d66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_SERVER_PORT_DEFAULT</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga3687b2e64d73b85594bd93a79df252af</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dns_cache_entry_s</type>
      <name>tnet_dns_cache_entry_t</name>
      <anchorfile>tnet__dns_8h.html</anchorfile>
      <anchor>a0f0ec5cf80cda2b90ebbfb2ea91b8546</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dns_cache_entries_L_t</name>
      <anchorfile>tnet__dns_8h.html</anchorfile>
      <anchor>ac5807116c43b05393fa66ca5137ea76e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dns_cache_entries_L_t</type>
      <name>tnet_dns_cache_t</name>
      <anchorfile>tnet__dns_8h.html</anchorfile>
      <anchor>a5a93db2cfefb5ba985653fa2b22d330c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dns_ctx_s</type>
      <name>tnet_dns_ctx_t</name>
      <anchorfile>tnet__dns_8h.html</anchorfile>
      <anchor>a4eda9ed5b03bf8b4886156c1f103a4ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dns_cache_clear</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga4908f53f564e46f39472e1dc825aa3ce</anchor>
      <arglist>(tnet_dns_ctx_t *ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dns_response_t *</type>
      <name>tnet_dns_resolve</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaaaee535d51058d67d9f170239de354b3</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dns_response_t *</type>
      <name>tnet_dns_enum</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaf08d4dbbaf1025a17613bd553bf4db78</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *e164num, const char *domain)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API char *</type>
      <name>tnet_dns_enum_2</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga1aa07c721d9e2e7bac6eea9f44a515c8</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *service, const char *e164num, const char *domain)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dns_query_srv</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gab4b9b4f145decf0d2886c8a4ea25a313</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *service, char **hostname, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dns_query_naptr_srv</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga0c2c6cdb03f205b6ce16af9398d91730</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *domain, const char *service, char **hostname, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dns_add_server</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga9d617f0cf0f0dc70e4a3bca53ab23824</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *host)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dns_ctx_t *</type>
      <name>tnet_dns_ctx_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga080c974f8f6bdb3779bfae98956e9821</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_ctx_def_t</name>
      <anchorfile>tnet__dns_8h.html</anchorfile>
      <anchor>abbdfc081a1491c06e89261c44cd60d5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_cache_entry_def_t</name>
      <anchorfile>tnet__dns_8h.html</anchorfile>
      <anchor>a4efb02fd4cb984ebac4138e560ee9905</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_a.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__a_8c</filename>
    <includes id="tnet__dns__a_8h" name="tnet_dns_a.h" local="yes" imported="no">tnet_dns_a.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_a_t *</type>
      <name>tnet_dns_a_create</name>
      <anchorfile>tnet__dns__a_8c.html</anchorfile>
      <anchor>a9defb7c8612dda2afe5c22a9d8af2cc2</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_a_def_t</name>
      <anchorfile>tnet__dns__a_8c.html</anchorfile>
      <anchor>ac8c60b42680e0c32a283b160f4a879ab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_a.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__a_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_a_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_a_s</type>
      <name>tnet_dns_a_t</name>
      <anchorfile>tnet__dns__a_8h.html</anchorfile>
      <anchor>ab141c5a0e9b9d0cf3409eb8360ec73f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dns_a_t *</type>
      <name>tnet_dns_a_create</name>
      <anchorfile>tnet__dns__a_8h.html</anchorfile>
      <anchor>a127c4a7530752dd17159d6cce2ec933d</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_a_def_t</name>
      <anchorfile>tnet__dns__a_8h.html</anchorfile>
      <anchor>aba9a5fca5fea194ada56d10229863ccf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_aaaa.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__aaaa_8c</filename>
    <includes id="tnet__dns__aaaa_8h" name="tnet_dns_aaaa.h" local="yes" imported="no">tnet_dns_aaaa.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_aaaa_t *</type>
      <name>tnet_dns_aaaa_create</name>
      <anchorfile>tnet__dns__aaaa_8c.html</anchorfile>
      <anchor>a9af77e5771ba53c408d586b064292f57</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_aaaa_def_t</name>
      <anchorfile>tnet__dns__aaaa_8c.html</anchorfile>
      <anchor>acb37a50064cef53a2492b240ffeb0451</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_aaaa.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__aaaa_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_aaaa_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_aaaa_s</type>
      <name>tnet_dns_aaaa_t</name>
      <anchorfile>tnet__dns__aaaa_8h.html</anchorfile>
      <anchor>af029948ea55fd7149a3690373f3fc8b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_aaaa_t *</type>
      <name>tnet_dns_aaaa_create</name>
      <anchorfile>tnet__dns__aaaa_8h.html</anchorfile>
      <anchor>a9af77e5771ba53c408d586b064292f57</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_aaaa_def_t</name>
      <anchorfile>tnet__dns__aaaa_8h.html</anchorfile>
      <anchor>a96c0dd56a4c355044d58f0d53ff4aee8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_cname.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__cname_8c</filename>
    <includes id="tnet__dns__cname_8h" name="tnet_dns_cname.h" local="yes" imported="no">tnet_dns_cname.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <member kind="function">
      <type>tnet_dns_cname_t *</type>
      <name>tnet_dns_cname_create</name>
      <anchorfile>tnet__dns__cname_8c.html</anchorfile>
      <anchor>a5dcf5f68f15132d89bafa3a061a62626</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_cname_def_t</name>
      <anchorfile>tnet__dns__cname_8c.html</anchorfile>
      <anchor>a8c03019ed6c906c4ec3bea95751654be</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_cname.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__cname_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_cname_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_cname_s</type>
      <name>tnet_dns_cname_t</name>
      <anchorfile>tnet__dns__cname_8h.html</anchorfile>
      <anchor>a03c83c67bce0fe05e2417f0d4d94425c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_cname_t *</type>
      <name>tnet_dns_cname_create</name>
      <anchorfile>tnet__dns__cname_8h.html</anchorfile>
      <anchor>a5dcf5f68f15132d89bafa3a061a62626</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_cname_def_t</name>
      <anchorfile>tnet__dns__cname_8h.html</anchorfile>
      <anchor>a7d6aea0757005c803e88b2fbb58b7ec6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_message.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__message_8c</filename>
    <includes id="tnet__dns__message_8h" name="tnet_dns_message.h" local="yes" imported="no">tnet_dns_message.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">../tnet_utils.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga98138daa781d9c12a4ba43f3371d0895</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tsk_bool_t isquery)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_create_null</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga353b8a830ee67c2a2d2a6e810f081ea9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_response_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaca11f63942b2ed04516a0d2323c75e7f</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_query_t *</type>
      <name>tnet_dns_query_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gac97c055c0a0a84882eb586feb22ef097</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dns_message_serialize</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga98f52e21a3d361e2a837993c31756871</anchor>
      <arglist>(const tnet_dns_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_deserialize</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga72d0a558aa990326e3724bd06260ac65</anchor>
      <arglist>(const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_message_def_t</name>
      <anchorfile>tnet__dns__message_8c.html</anchorfile>
      <anchor>a7b52fae969a8d3fdbcaf82892ad14803</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_message.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__message_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_message_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_MESSAGE_IS_RESPONSE</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga2a5cf37ea80a5c81714042074eac92bf</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_MESSAGE_IS_QUERY</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaf24bf109d5da866b600989d53c24f2aa</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_RESPONSE_IS_SUCCESS</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaefa196185f91151ec1b70542d73f230b</anchor>
      <arglist>(response)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_RESPONSE_IS_ERROR</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gab7b1d2b4373729da855fad1816ef9e81</anchor>
      <arglist>(response)</arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dns_rcode_e</type>
      <name>tnet_dns_rcode_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a10a2d61b100eaa8fa40121a04227746e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dns_opcode_e</type>
      <name>tnet_dns_opcode_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>ac70fc820dc3345398e3d71dec208a0ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dns_message_s</type>
      <name>tnet_dns_message_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>adfe20e6afb683c719dfa690aa0ae82ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dns_messages_L_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a70167cb0e8bc6f7df28b176d58616b65</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dns_message_t</type>
      <name>tnet_dns_query_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>aa5e144b50fc52b3cd87fd9cfe230cb79</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_dns_message_t</type>
      <name>tnet_dns_response_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a5c96624b434eaa634b5dde4dee92ff61</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dns_rcode_e</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>rcode_noerror</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fdafb49d1855d22e2a14b9164e83f21c4d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>rcode_error_format</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fda9eed8ffe73f0b6ac420350008aee371d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>rcode_server_failure</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fda2a9b39e282a286f2200701c6a6f5e2e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>rcode_error_name</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fda0a69068bb631f39be6036eff8d973530</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>rcode_notimplemented</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fda51b705ad2e1bc9a41fd7289e69fc0217</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>rcode_refused</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>a2d9be471d959638233c90adb121505fdaa19a2787f2f80124a9d8a26bdcec5f0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dns_opcode_e</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>ae948f57e10a3e473e1f37d13d7e571da</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>opcode_query</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>ae948f57e10a3e473e1f37d13d7e571daaf16852803537cc6a78d86024593654ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>opcode_iquery</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>ae948f57e10a3e473e1f37d13d7e571daa1d4afedc5e0d6721a623d5420110d53a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>opcode_status</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>ae948f57e10a3e473e1f37d13d7e571daab0ef3fad224dc5971f97e02b2fb8b714</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dns_message_serialize</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga98f52e21a3d361e2a837993c31756871</anchor>
      <arglist>(const tnet_dns_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_deserialize</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga72d0a558aa990326e3724bd06260ac65</anchor>
      <arglist>(const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga98138daa781d9c12a4ba43f3371d0895</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tsk_bool_t isquery)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_create_null</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga353b8a830ee67c2a2d2a6e810f081ea9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_response_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaca11f63942b2ed04516a0d2323c75e7f</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_query_t *</type>
      <name>tnet_dns_query_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gac97c055c0a0a84882eb586feb22ef097</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_message_def_t</name>
      <anchorfile>tnet__dns__message_8h.html</anchorfile>
      <anchor>afa9682338ba81c46be181f6e7a63d53d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_mx.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__mx_8c</filename>
    <includes id="tnet__dns__mx_8h" name="tnet_dns_mx.h" local="yes" imported="no">tnet_dns_mx.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_mx_t *</type>
      <name>tnet_dns_mx_create</name>
      <anchorfile>tnet__dns__mx_8c.html</anchorfile>
      <anchor>a016b5719ecb6b707228b4350281ecee2</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_mx_def_t</name>
      <anchorfile>tnet__dns__mx_8c.html</anchorfile>
      <anchor>a005d5f47204c5bb134f6b0fa5b1a1ced</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_mx.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__mx_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_mx_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_mx_s</type>
      <name>tnet_dns_mx_t</name>
      <anchorfile>tnet__dns__mx_8h.html</anchorfile>
      <anchor>a612a857249dd90d7bb72d186b534fd7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_mx_t *</type>
      <name>tnet_dns_mx_create</name>
      <anchorfile>tnet__dns__mx_8h.html</anchorfile>
      <anchor>a016b5719ecb6b707228b4350281ecee2</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_mx_def_t</name>
      <anchorfile>tnet__dns__mx_8h.html</anchorfile>
      <anchor>aa421813d4ec3a8eed3f247fef75ddfe9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_naptr.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__naptr_8c</filename>
    <includes id="tnet__dns__naptr_8h" name="tnet_dns_naptr.h" local="yes" imported="no">tnet_dns_naptr.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">./tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_naptr_t *</type>
      <name>tnet_dns_naptr_create</name>
      <anchorfile>tnet__dns__naptr_8c.html</anchorfile>
      <anchor>a8903290f814d88d1b685e3401591e8c3</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_naptr_def_t</name>
      <anchorfile>tnet__dns__naptr_8c.html</anchorfile>
      <anchor>aad6c6ba633c90e84931baa0af523a42d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_naptr.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__naptr_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_naptr_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_naptr_s</type>
      <name>tnet_dns_naptr_t</name>
      <anchorfile>tnet__dns__naptr_8h.html</anchorfile>
      <anchor>a40135eba84ad5108ead55d1198e9df63</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dns_naptr_t *</type>
      <name>tnet_dns_naptr_create</name>
      <anchorfile>tnet__dns__naptr_8h.html</anchorfile>
      <anchor>aa59f7ef27b64bf1340e75bfa3b8ded0d</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_naptr_def_t</name>
      <anchorfile>tnet__dns__naptr_8h.html</anchorfile>
      <anchor>a0f60311c1ef28f1999ecba475352922f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_ns.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__ns_8c</filename>
    <includes id="tnet__dns__ns_8h" name="tnet_dns_ns.h" local="yes" imported="no">tnet_dns_ns.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <member kind="function">
      <type>tnet_dns_ns_t *</type>
      <name>tnet_dns_ns_create</name>
      <anchorfile>tnet__dns__ns_8c.html</anchorfile>
      <anchor>a1f6a6ec6af069ac55d709f6313ea0f89</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_ns_def_t</name>
      <anchorfile>tnet__dns__ns_8c.html</anchorfile>
      <anchor>a30c52ab96205e922203a9cb983b1814c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_ns.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__ns_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_ns_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_ns_s</type>
      <name>tnet_dns_ns_t</name>
      <anchorfile>tnet__dns__ns_8h.html</anchorfile>
      <anchor>a20afc97fb196ce2a3b1a4eb94603891f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dns_ns_t *</type>
      <name>tnet_dns_ns_create</name>
      <anchorfile>tnet__dns__ns_8h.html</anchorfile>
      <anchor>a32c62e90da38fed47351cd1ecdfe7323</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_ns_def_t</name>
      <anchorfile>tnet__dns__ns_8h.html</anchorfile>
      <anchor>a848fe1ef765215af38d449d4b0fd5943</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_opt.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__opt_8c</filename>
    <includes id="tnet__dns__opt_8h" name="tnet_dns_opt.h" local="yes" imported="no">tnet_dns_opt.h</includes>
    <member kind="function">
      <type>tnet_dns_opt_t *</type>
      <name>tnet_dns_opt_create</name>
      <anchorfile>tnet__dns__opt_8c.html</anchorfile>
      <anchor>a42d82b4cfecece6f35d08124861cd624</anchor>
      <arglist>(tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_opt_def_t</name>
      <anchorfile>tnet__dns__opt_8c.html</anchorfile>
      <anchor>ac54263af0e117f7ddd77eeb5bb77add3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_opt.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__opt_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_opt_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_opt_s</type>
      <name>tnet_dns_opt_t</name>
      <anchorfile>tnet__dns__opt_8h.html</anchorfile>
      <anchor>aad1e4934eb5b8f462bf2fd70d3557d0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_opt_t *</type>
      <name>tnet_dns_opt_create</name>
      <anchorfile>tnet__dns__opt_8h.html</anchorfile>
      <anchor>a42d82b4cfecece6f35d08124861cd624</anchor>
      <arglist>(tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_opt_def_t</name>
      <anchorfile>tnet__dns__opt_8h.html</anchorfile>
      <anchor>a87bc155048ab772c6e275779c7a903fe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_ptr.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__ptr_8c</filename>
    <includes id="tnet__dns__ptr_8h" name="tnet_dns_ptr.h" local="yes" imported="no">tnet_dns_ptr.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <member kind="function">
      <type>tnet_dns_ptr_t *</type>
      <name>tnet_dns_ptr_create</name>
      <anchorfile>tnet__dns__ptr_8c.html</anchorfile>
      <anchor>a0781972d80ae72b2836b931106dc922b</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_ptr_def_t</name>
      <anchorfile>tnet__dns__ptr_8c.html</anchorfile>
      <anchor>a322649cfc864a32b9a169f89a4d88ece</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_ptr.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__ptr_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_ptr_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_ptr_s</type>
      <name>tnet_dns_ptr_t</name>
      <anchorfile>tnet__dns__ptr_8h.html</anchorfile>
      <anchor>a36877f2a0e53fa1227a87f776d5ed6c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_ptr_t *</type>
      <name>tnet_dns_ptr_create</name>
      <anchorfile>tnet__dns__ptr_8h.html</anchorfile>
      <anchor>a0781972d80ae72b2836b931106dc922b</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_ptr_def_t</name>
      <anchorfile>tnet__dns__ptr_8h.html</anchorfile>
      <anchor>a7645989139aeb9e6c24c0c8995536ed5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_regexp.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__regexp_8c</filename>
    <includes id="tnet__dns__regexp_8h" name="tnet_dns_regexp.h" local="yes" imported="no">tnet_dns_regexp.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>tnet_dns_regex_parse</name>
      <anchorfile>tnet__dns__regexp_8c.html</anchorfile>
      <anchor>a9ed1816bfa7833863cf0743db5f26830</anchor>
      <arglist>(const char *e164num, const char *regexp)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_regexp.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__regexp_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="function">
      <type>TNET_BEGIN_DECLS TINYNET_API char *</type>
      <name>tnet_dns_regex_parse</name>
      <anchorfile>tnet__dns__regexp_8h.html</anchorfile>
      <anchor>ad9b4290eea6c5833a15079a9ba6315b5</anchor>
      <arglist>(const char *e164num, const char *regexp)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_resolvconf.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__resolvconf_8c</filename>
    <includes id="tnet__dns__resolvconf_8h" name="tnet_dns_resolvconf.h" local="yes" imported="no">tnet_dns_resolvconf.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <includes id="tnet__dns_8h" name="tnet_dns.h" local="yes" imported="no">dns/tnet_dns.h</includes>
    <member kind="function">
      <type>tnet_addresses_L_t *</type>
      <name>tnet_dns_resolvconf_parse</name>
      <anchorfile>tnet__dns__resolvconf_8c.html</anchorfile>
      <anchor>a450c767da8f966731be469a146759494</anchor>
      <arglist>(const char *path)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_resolvconf.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__resolvconf_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <member kind="function">
      <type>TNET_BEGIN_DECLS TINYNET_API tnet_addresses_L_t *</type>
      <name>tnet_dns_resolvconf_parse</name>
      <anchorfile>tnet__dns__resolvconf_8h.html</anchorfile>
      <anchor>a7c7f9edfb32f7b57b9332973d1b7b217</anchor>
      <arglist>(const char *path)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_rr.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__rr_8c</filename>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <includes id="tnet__dns__a_8h" name="tnet_dns_a.h" local="yes" imported="no">tnet_dns_a.h</includes>
    <includes id="tnet__dns__aaaa_8h" name="tnet_dns_aaaa.h" local="yes" imported="no">tnet_dns_aaaa.h</includes>
    <includes id="tnet__dns__cname_8h" name="tnet_dns_cname.h" local="yes" imported="no">tnet_dns_cname.h</includes>
    <includes id="tnet__dns__mx_8h" name="tnet_dns_mx.h" local="yes" imported="no">tnet_dns_mx.h</includes>
    <includes id="tnet__dns__naptr_8h" name="tnet_dns_naptr.h" local="yes" imported="no">tnet_dns_naptr.h</includes>
    <includes id="tnet__dns__ns_8h" name="tnet_dns_ns.h" local="yes" imported="no">tnet_dns_ns.h</includes>
    <includes id="tnet__dns__opt_8h" name="tnet_dns_opt.h" local="yes" imported="no">tnet_dns_opt.h</includes>
    <includes id="tnet__dns__ptr_8h" name="tnet_dns_ptr.h" local="yes" imported="no">tnet_dns_ptr.h</includes>
    <includes id="tnet__dns__soa_8h" name="tnet_dns_soa.h" local="yes" imported="no">tnet_dns_soa.h</includes>
    <includes id="tnet__dns__srv_8h" name="tnet_dns_srv.h" local="yes" imported="no">tnet_dns_srv.h</includes>
    <includes id="tnet__dns__txt_8h" name="tnet_dns_txt.h" local="yes" imported="no">tnet_dns_txt.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_rr_t *</type>
      <name>tnet_dns_rr_create</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>ac2a4807ec467a0c0d68a72cdcf2809c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_init</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>abbf82aaca6c13787652a732688b951f0</anchor>
      <arglist>(tnet_dns_rr_t *rr, tnet_dns_qtype_t qtype, tnet_dns_qclass_t qclass)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_deinit</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>a96fbca3132c00d7f47d17a20eaf3384b</anchor>
      <arglist>(tnet_dns_rr_t *rr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_charstring_deserialize</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>a120e1c355fecace1874329b58d700d4d</anchor>
      <arglist>(const void *data, char **charstring, tsk_size_t *offset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_qname_deserialize</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>a1ade7e249886b86b6fb68dd6ee807d60</anchor>
      <arglist>(const void *data, char **name, tsk_size_t *offset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_qname_serialize</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>aacf6eb1f4fc48c1b662c704ea0e6bb5a</anchor>
      <arglist>(const char *qname, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_rr_t *</type>
      <name>tnet_dns_rr_deserialize</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>a6132fbb1895bc96764e773a0cbe8137f</anchor>
      <arglist>(const void *data, tsk_size_t size, tsk_size_t *offset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_serialize</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>ad79d614d7a6e9cf9d9f69b381874cca1</anchor>
      <arglist>(const tnet_dns_rr_t *rr, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_rr_def_t</name>
      <anchorfile>tnet__dns__rr_8c.html</anchorfile>
      <anchor>a1c8c9b8399afa3a95932cea38da3b855</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_rr.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__rr_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <class kind="struct">tnet_dns_rr_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_RR</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>aa1292467e3d3978fd7ac6c7b10f87cc2</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a5cccd210b99ecceb030738248f17b76a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dns_qtype_e</type>
      <name>tnet_dns_qtype_t</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a62961afef3b1d8a3ce06b52a8a73db1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dns_qclass_e</type>
      <name>tnet_dns_qclass_t</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a69f1e6e11e7ef63d49fd0911e5ac3357</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dns_rr_s</type>
      <name>tnet_dns_rr_t</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>ace26a1f5215afe27b8d0a16460ef1beb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_dns_rrs_L_t</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a92294f9508f61a03b7b5191acfa35f20</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dns_qtype_e</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_a</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea6b82b9692226f36f5553fd6d9780f9d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_ns</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea2c626ab43259347ad612a7230b478e9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_md</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea99c0993087c8dbbc55e4b6afdc4e4c8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_mf</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea36ce8cf8155642e552675d2fc25eae44</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_cname</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea45425b4d0e6bd387ecb36c90ae99d370</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_soa</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeab40d6242e1e9586362fbd35086b64006</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_mb</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeaa11cd279e87a961352188184c2f2d8d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_mg</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeaae9f1f64f391b7b455272f6cbde3ef8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_mr</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeab75b84867779598974a29609294ca0bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_null</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea86f36a2d858188aed1c34b67fe62ffd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_wks</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea4059a57c201d0101a37c4d3f9eabf31a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_ptr</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeaed5edacf992f416dadc864f78c133459</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_hinfo</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea57b532f99f7b8333d46d1852336850d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_minfo</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeabea007dc92b66ce9be0d1c1d269e78f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_mx</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea54ce802cf2003da33264c8e03812dfc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_txt</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeabc2ed8cc5ac7785b1494fc4d2cea915b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_aaaa</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea9352d81362b3407179adef085bdbce97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_srv</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea00a47d3105646a15138910bbb4a2ea94</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_naptr</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea05b83408990091d7a963bfa4ed6651f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_opt</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea8d2334865c3df5ee1ba23b9ca3a112d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_ipseckey</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea252b893a0977eb9e3bd102654a71d825</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_spf</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aeac027ceb8ae33fe01d79b9b9e09f869cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qtype_any</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6d7667bb60a37e3cac21f13a0d2af6aea1a5b9dd4a833ad763b9bafc88a5cea5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dns_qclass_e</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a39c6ee8ca1d8f814024a17cfc3a88b5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qclass_in</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a39c6ee8ca1d8f814024a17cfc3a88b5caa8fb176df6c5bdca6b7f088a84fd0384</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qclass_ics</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a39c6ee8ca1d8f814024a17cfc3a88b5ca3fa1f9f5dd15959e92668ddcec709850</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qclass_ch</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a39c6ee8ca1d8f814024a17cfc3a88b5ca8503dc4ab6ee9b2293740abb26631d4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qclass_hs</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a39c6ee8ca1d8f814024a17cfc3a88b5cab00871fa3fb8dccd9c3b5e4eed53bbe2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>qclass_any</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a39c6ee8ca1d8f814024a17cfc3a88b5cac9a4560ad311dfe6ee260f6578f35e73</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_init</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>abbf82aaca6c13787652a732688b951f0</anchor>
      <arglist>(tnet_dns_rr_t *rr, tnet_dns_qtype_t qtype, tnet_dns_qclass_t qclass)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_deinit</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a96fbca3132c00d7f47d17a20eaf3384b</anchor>
      <arglist>(tnet_dns_rr_t *rr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_charstring_deserialize</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6ec4f0742d53673299905dbcf2285af0</anchor>
      <arglist>(const void *data, char **name, tsk_size_t *offset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_qname_deserialize</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a1ade7e249886b86b6fb68dd6ee807d60</anchor>
      <arglist>(const void *data, char **name, tsk_size_t *offset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_qname_serialize</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>aacf6eb1f4fc48c1b662c704ea0e6bb5a</anchor>
      <arglist>(const char *qname, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_rr_t *</type>
      <name>tnet_dns_rr_deserialize</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a6132fbb1895bc96764e773a0cbe8137f</anchor>
      <arglist>(const void *data, tsk_size_t size, tsk_size_t *offset)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_rr_serialize</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>ad79d614d7a6e9cf9d9f69b381874cca1</anchor>
      <arglist>(const tnet_dns_rr_t *rr, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_rr_def_t</name>
      <anchorfile>tnet__dns__rr_8h.html</anchorfile>
      <anchor>a15a335d6e37b391bfdfe754266ff7ee2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_soa.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__soa_8c</filename>
    <includes id="tnet__dns__soa_8h" name="tnet_dns_soa.h" local="yes" imported="no">tnet_dns_soa.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_soa_t *</type>
      <name>tnet_dns_soa_create</name>
      <anchorfile>tnet__dns__soa_8c.html</anchorfile>
      <anchor>a554c9fb10b89674333c4358d9b22971f</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_soa_def_t</name>
      <anchorfile>tnet__dns__soa_8c.html</anchorfile>
      <anchor>a26a935ec9f0f1ee97cebff4c2bcd1d30</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_soa.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__soa_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_soa_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_soa_s</type>
      <name>tnet_dns_soa_t</name>
      <anchorfile>tnet__dns__soa_8h.html</anchorfile>
      <anchor>a47c38512228462f2a7a2af3fdbbe5ee0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_soa_t *</type>
      <name>tnet_dns_soa_create</name>
      <anchorfile>tnet__dns__soa_8h.html</anchorfile>
      <anchor>a554c9fb10b89674333c4358d9b22971f</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_soa_def_t</name>
      <anchorfile>tnet__dns__soa_8h.html</anchorfile>
      <anchor>a3ee61ed56ac01b434cb2ee7cbe7a3e47</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_srv.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__srv_8c</filename>
    <includes id="tnet__dns__srv_8h" name="tnet_dns_srv.h" local="yes" imported="no">tnet_dns_srv.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_dns_srv_t *</type>
      <name>tnet_dns_srv_create</name>
      <anchorfile>tnet__dns__srv_8c.html</anchorfile>
      <anchor>a0989e7656444afa8eccb494162c1a840</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_srv_def_t</name>
      <anchorfile>tnet__dns__srv_8c.html</anchorfile>
      <anchor>a446d9cf2ccb2fb1d5f3534c9bdabf047</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_srv.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__srv_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_srv_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_srv_s</type>
      <name>tnet_dns_srv_t</name>
      <anchorfile>tnet__dns__srv_8h.html</anchorfile>
      <anchor>a4ebe2b222b25e6f185628a01bd80088f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_srv_t *</type>
      <name>tnet_dns_srv_create</name>
      <anchorfile>tnet__dns__srv_8h.html</anchorfile>
      <anchor>a0989e7656444afa8eccb494162c1a840</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_srv_def_t</name>
      <anchorfile>tnet__dns__srv_8h.html</anchorfile>
      <anchor>a9ba8975bee55d2728c8902f752f5a064</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_txt.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__txt_8c</filename>
    <includes id="tnet__dns__txt_8h" name="tnet_dns_txt.h" local="yes" imported="no">tnet_dns_txt.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <member kind="function">
      <type>tnet_dns_txt_t *</type>
      <name>tnet_dns_txt_create</name>
      <anchorfile>tnet__dns__txt_8c.html</anchorfile>
      <anchor>aae4a21dd9f1ea22c7f2f171dc63d501b</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dns_txt_def_t</name>
      <anchorfile>tnet__dns__txt_8c.html</anchorfile>
      <anchor>ae11da0ea12bd1bd4de45bace4264cf81</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dns_txt.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>tnet__dns__txt_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__dns__rr_8h" name="tnet_dns_rr.h" local="yes" imported="no">tnet_dns_rr.h</includes>
    <class kind="struct">tnet_dns_txt_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_dns_txt_s</type>
      <name>tnet_dns_txt_t</name>
      <anchorfile>tnet__dns__txt_8h.html</anchorfile>
      <anchor>ab03bd98bda63e6584c344f0fb68dd50a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_txt_t *</type>
      <name>tnet_dns_txt_create</name>
      <anchorfile>tnet__dns__txt_8h.html</anchorfile>
      <anchor>aae4a21dd9f1ea22c7f2f171dc63d501b</anchor>
      <arglist>(const char *name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void *data, tsk_size_t offset)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dns_txt_def_t</name>
      <anchorfile>tnet__dns__txt_8h.html</anchorfile>
      <anchor>a7e04769891a173ae81dded18394e3992</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice_8c</filename>
    <includes id="tnet__ice_8h" name="tnet_ice.h" local="yes" imported="no">tnet_ice.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_ice.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">../tinynet_config.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_ice_candidate.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__candidate_8c</filename>
    <includes id="tnet__ice__candidate_8h" name="tnet_ice_candidate.h" local="yes" imported="no">tnet_ice_candidate.h</includes>
    <includes id="tnet__ice__utils_8h" name="tnet_ice_utils.h" local="yes" imported="no">tnet_ice_utils.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TRANSPORT_GET</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>ac0c81284ba3210ccad9e97f32a6d361a</anchor>
      <arglist>(STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRANSPORT_GET</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>a2a343a55abad1a887708009d2249c052</anchor>
      <arglist>(STR, str)</arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_candidate_t *</type>
      <name>tnet_ice_candidate_create</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>a87afdc02b700ac02deef8f9db2e3b7d0</anchor>
      <arglist>(tnet_ice_cand_type_t type_e, tnet_socket_t *socket, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtp, tsk_bool_t is_video, const char *ufrag, const char *pwd, const char *foundation)</arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_candidate_t *</type>
      <name>tnet_ice_candidate_parse</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>adcafd3c75b3a0567af1c2923bbd479e9</anchor>
      <arglist>(const char *str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_set_credential</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>a88081be58407f8a55fed19635683d17c</anchor>
      <arglist>(tnet_ice_candidate_t *self, const char *ufrag, const char *pwd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_set_rflx_addr</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>abfc9b2e65927ce5f84dd58f168dc93ba</anchor>
      <arglist>(tnet_ice_candidate_t *self, const char *addr, tnet_port_t port)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_candidate_get_att_value</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>aec0c1278bea426308f7820a1a982bbb4</anchor>
      <arglist>(const tnet_ice_candidate_t *self, const char *att_name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_set_local_pref</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>ab2dbca1a8eb4356de3b660c9beb72540</anchor>
      <arglist>(tnet_ice_candidate_t *self, uint16_t local_pref)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_candidate_tostring</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>ab93d6f47bf868773438e47f47e077d13</anchor>
      <arglist>(tnet_ice_candidate_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_send_stun_bind_request</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>a5b8c31b3a03f88f22e610c77d1482da9</anchor>
      <arglist>(tnet_ice_candidate_t *self, struct sockaddr_storage *server_addr, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_process_stun_response</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>a5b4466b24e0e536e9ac6413c23c0c6b2</anchor>
      <arglist>(tnet_ice_candidate_t *self, const tnet_stun_response_t *response, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_candidate_t *</type>
      <name>tnet_ice_candidate_find_by_fd</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>ad8cc71f1264c8dcc74cc690c468c1bbe</anchor>
      <arglist>(tnet_ice_candidates_L_t *candidates, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_candidate_get_ufrag</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>aa9bafda787d4b69a36fe1aa20c8af4d4</anchor>
      <arglist>(const tnet_ice_candidate_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_candidate_get_pwd</name>
      <anchorfile>tnet__ice__candidate_8c.html</anchorfile>
      <anchor>a0edc7f4dee8171abbb1e6fc678bc82f8</anchor>
      <arglist>(const tnet_ice_candidate_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_candidate.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__candidate_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__stun__message_8h" name="tnet_stun_message.h" local="yes" imported="no">stun/tnet_stun_message.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <class kind="struct">tnet_ice_candidate_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TRANSPORT_UDP</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ae1b662d5e6b0d21c09fd3f838b786f9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TRANSPORT_TCP</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a41130d885b4b5aa83a5f38c67eb9c9fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TRANSPORT_TLS</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>abc5d9b200c226b93a2daae472636fa8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TRANSPORT_SCTP</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a377a94ab79d2f01490b544755f0ed41b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TRANSPORT_WS</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>af5d81604b62ec33d25e36a67c1251b4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TRANSPORT_WSS</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>aefca7739c4338f6fd3862d7dbf3bd637</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TYPE_HOST</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a0006a60bf22085ddc6c3d8678a1fff7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TYPE_SRFLX</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>afd3cbe41244f7e2269eebdf3eb634b95</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TYPE_PRFLX</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>aacd9ce8fb4b3bc07a4f03dbe6a36a945</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_TYPE_RELAY</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a52affc7b42846807f024604827377f41</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_PREF_HOST</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>af2364751ec9aa5d325db99afd8c10818</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_PREF_SRFLX</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a222b836f5b110bfe3f37ceab60d618db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_PREF_PRFLX</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>afb9ae2f89f8226dde22048fb5fb8354a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_PREF_RELAY</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a7ac1c6c32135a99d1ca552c3bd58c1c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_FOUNDATION_HOST</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a4145345d2588a18b53cae5c2f2cfa27f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_FOUNDATION_SRFLX</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ab506ffe464a7b1a294850cd06a3a00df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_FOUNDATION_PRFLX</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a430eedeb2ce8ffa4d9c4c30d01dd85b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_FOUNDATION_RELAY</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a585f8eab3dc73fd575467af9d930fce0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_COMPID_RTP</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a7d6a3ad5bb6bb0cfaa4e46a4c7882f6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_COMPID_RTCP</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a13ac3d2da0fd98a4409861a222fceb40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CANDIDATE_FOUND_SIZE_PREF</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a49640bfaea947b298170919d5e49cbba</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_ice_cand_type_e</type>
      <name>tnet_ice_cand_type_t</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a6241699c76dc566092b4d7d5c29acc4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_ice_candidate_s</type>
      <name>tnet_ice_candidate_t</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a3d658380df87bf28a24603119b2870cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_ice_candidates_L_t</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad107252b8f7eb93e7561e7b664dffbaa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_ice_cand_type_e</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad04e165e02bc0661c9c16230e92f0cb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_cand_type_unknown</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad04e165e02bc0661c9c16230e92f0cb8a76e2e17a49bd0bf350e301e80e126422</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_cand_type_host</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad04e165e02bc0661c9c16230e92f0cb8af95692833ce82ab03827bbb5e252807b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_cand_type_srflx</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad04e165e02bc0661c9c16230e92f0cb8a49074a8972e6db8227397ec318cb8b7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_cand_type_prflx</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad04e165e02bc0661c9c16230e92f0cb8a05b142a102f38682b7edc9a2d1e6170d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_cand_type_relay</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad04e165e02bc0661c9c16230e92f0cb8a3d21db646212621771c99c69eccd9458</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_candidate_t *</type>
      <name>tnet_ice_candidate_create</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a50b8cad64e556dd901cee0b443192502</anchor>
      <arglist>(tnet_ice_cand_type_t type_e, struct tnet_socket_s *socket, tsk_bool_t is_ice_jingle, tsk_bool_t is_rtp, tsk_bool_t is_video, const char *ufrag, const char *pwd, const char *foundation)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_ice_candidate_t *</type>
      <name>tnet_ice_candidate_parse</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a3c7cfe0e32560ddb9c95fff93102218f</anchor>
      <arglist>(const char *str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_set_credential</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a88081be58407f8a55fed19635683d17c</anchor>
      <arglist>(tnet_ice_candidate_t *self, const char *ufrag, const char *pwd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_set_rflx_addr</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>abfc9b2e65927ce5f84dd58f168dc93ba</anchor>
      <arglist>(tnet_ice_candidate_t *self, const char *addr, tnet_port_t port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const char *</type>
      <name>tnet_ice_candidate_get_att_value</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a521930480b15845f6c971415c077f41d</anchor>
      <arglist>(const tnet_ice_candidate_t *self, const char *att_name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_set_local_pref</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ab2dbca1a8eb4356de3b660c9beb72540</anchor>
      <arglist>(tnet_ice_candidate_t *self, uint16_t local_pref)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const char *</type>
      <name>tnet_ice_candidate_tostring</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a74f2f386a6a36578192cc54788bc3027</anchor>
      <arglist>(tnet_ice_candidate_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_send_stun_bind_request</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a5b8c31b3a03f88f22e610c77d1482da9</anchor>
      <arglist>(tnet_ice_candidate_t *self, struct sockaddr_storage *server_addr, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_candidate_process_stun_response</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a5b4466b24e0e536e9ac6413c23c0c6b2</anchor>
      <arglist>(tnet_ice_candidate_t *self, const tnet_stun_response_t *response, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_candidate_t *</type>
      <name>tnet_ice_candidate_find_by_fd</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>ad8cc71f1264c8dcc74cc690c468c1bbe</anchor>
      <arglist>(tnet_ice_candidates_L_t *candidates, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_candidate_get_ufrag</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>aa9bafda787d4b69a36fe1aa20c8af4d4</anchor>
      <arglist>(const tnet_ice_candidate_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_candidate_get_pwd</name>
      <anchorfile>tnet__ice__candidate_8h.html</anchorfile>
      <anchor>a0edc7f4dee8171abbb1e6fc678bc82f8</anchor>
      <arglist>(const tnet_ice_candidate_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_ctx.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__ctx_8c</filename>
    <includes id="tnet__ice__ctx_8h" name="tnet_ice_ctx.h" local="yes" imported="no">tnet_ice_ctx.h</includes>
    <includes id="tnet__ice__event_8h" name="tnet_ice_event.h" local="yes" imported="no">tnet_ice_event.h</includes>
    <includes id="tnet__ice__candidate_8h" name="tnet_ice_candidate.h" local="yes" imported="no">tnet_ice_candidate.h</includes>
    <includes id="tnet__ice__pair_8h" name="tnet_ice_pair.h" local="yes" imported="no">tnet_ice_pair.h</includes>
    <includes id="tnet__ice__utils_8h" name="tnet_ice_utils.h" local="yes" imported="no">tnet_ice_utils.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">tnet_endianness.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">stun/tnet_stun.h</includes>
    <includes id="tnet__stun__message_8h" name="tnet_stun_message.h" local="yes" imported="no">stun/tnet_stun_message.h</includes>
    <class kind="struct">tnet_ice_ctx_s</class>
    <class kind="struct">tnet_ice_action_s</class>
    <member kind="define">
      <type>#define</type>
      <name>LONG_MAX</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a50fece4db74f09568b2938db583c5655</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_DEBUG_STATE_MACHINE</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ab2ab9ef5e0fd2ad07b778d4453c6a180</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_DEFAULT_RTO</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga25beb3cb889e80636f91ed1e316ef083</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_DEFAULT_RC</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gaf2d937114195afa44a46b88b898a781b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_CONFLICT_ERROR_CODE</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a6fe14da13dbefeff267554a95245b0c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_ice_ctx_s</type>
      <name>tnet_ice_ctx_t</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a5337006422b6a6d5410461f964d6e3a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_ice_action_s</type>
      <name>tnet_ice_action_t</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a179dbc88e6e0fabf4f458f14aed80ea2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _fsm_state_e</type>
      <name>_fsm_state_t</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ac06f5d03925a1287a4f89e0d57f347fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _fsm_action_e</type>
      <name>_fsm_action_t</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>afe20aca1bd90c447e89dfd4b30661acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>_fsm_state_e</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_Started</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba76034bb262163ff2e14ebbdbe3b3b2ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_GatheringHostCandidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcbac7bf056c75335dcdf2b754e0093a6186</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_GatheringHostCandidatesDone</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba1b0f1991b1b431c4648a29a649752404</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_GatheringReflexiveCandidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba7e0c8e38780560c73698c8cdefb6e280</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_GatheringReflexiveCandidatesDone</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba9eb9e11bb1eb0cae53360c13058e440e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_GatheringCompleted</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba65194ba2f9bb66c268ad079b11414e73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_ConnChecking</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcbaad6e69dd6419d0e8da9eb42f03aa5240</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_ConnCheckingCompleted</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcbabcb7f6f071eb6a6ef27ab368a153cdba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_Terminated</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcbaeab8f5e66ccc9c9df9e4edd20400e037</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>_fsm_action_e</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_Success</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da5635821eb87b10ae2cc08e32aac16cb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_Failure</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da8a615bd3454c07df86bd9d900cc8ad76</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_GatherHostCandidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da1959b1a775dfc61f4a91016e3e69ca42</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_GatherReflexiveCandidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da161e59357719a7f611c796abc5b660b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_GatheringComplet</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da523af28f3ff1ed15f5b8ce50f62eb4ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_ConnCheck</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da8e7bfe7f383fad3fc058d54eb52700de</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_Cancel</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da82d57ea2cca20e4afe702353cca10b27</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_Error</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61dac394d62ff536294f276acf0f0ba36dfe</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_ctx_t *</type>
      <name>tnet_ice_ctx_create</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a5ca9c2ad39aafb1c9134b3e70269399f</anchor>
      <arglist>(tsk_bool_t is_ice_jingle, tsk_bool_t use_ipv6, tsk_bool_t use_rtcp, tsk_bool_t is_video, tnet_ice_callback_f callback, const void *userdata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_set_userdata</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a5423751e16041a4ae2980a2e61fad527</anchor>
      <arglist>(tnet_ice_ctx_t *self, const void *userdata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_set_stun</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a56e2e0474a35f73d9ddf7d749bbed280</anchor>
      <arglist>(tnet_ice_ctx_t *self, const char *server_addr, uint16_t server_port, const char *software, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_start</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>aed85886ca3d3f140810988d7ee43dac3</anchor>
      <arglist>(tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_rtp_callback</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a5b542845608c288a53c97dd31ebc14db</anchor>
      <arglist>(tnet_ice_ctx_t *self, tnet_ice_rtp_callback_f rtp_callback, const void *rtp_callback_data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_set_concheck_timeout</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a1e715ce9adbcadcfe3695cc7441cf7f3</anchor>
      <arglist>(tnet_ice_ctx_t *self, int64_t timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_set_remote_candidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a63e833e5a438da0a2a50bcdbfa8f2240</anchor>
      <arglist>(tnet_ice_ctx_t *self, const char *candidates, const char *ufrag, const char *pwd, tsk_bool_t is_controlling, tsk_bool_t is_ice_jingle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_set_rtcpmux</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>aa9980c495bca6e6413fe8185742602e6</anchor>
      <arglist>(tnet_ice_ctx_t *self, tsk_bool_t use_rtcpmux)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tnet_ice_ctx_count_local_candidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a203a51c4d8c0f278dd17361df0ef2e53</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_got_local_candidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a55ef5867dee0b10bba1a0d98f89d37e4</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_candidate_t *</type>
      <name>tnet_ice_ctx_get_local_candidate_at</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a52868e342a03d64d13f7e2d98d1b6b6a</anchor>
      <arglist>(const tnet_ice_ctx_t *self, tsk_size_t index)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_is_started</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a7839820b10a59e1bd58a3fccb1e4b4a3</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_is_active</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>aa6432bc0167c9a5409e775e1bd119ccb</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_is_connected</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a47ba1e31a451454ad9039db323af98aa</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_is_can_send</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>ace07f467c6f2365594be25662d977de4</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_is_can_recv</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>aa54f36ec73b95f5df1de849d5bcdb322</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_use_ipv6</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>aa94d177fced068791fb4a8366985d7b4</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_ctx_use_rtcp</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a362a7747e13d2a43f453c4a8574f2714</anchor>
      <arglist>(const tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_get_nominated_symetric_candidates</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>af8debc0978b4c0647fe939050e28e533</anchor>
      <arglist>(const tnet_ice_ctx_t *self, uint32_t comp_id, const tnet_ice_candidate_t **candidate_offer, const tnet_ice_candidate_t **candidate_answer_src, const tnet_ice_candidate_t **candidate_answer_dest)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_recv_stun_message</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a8576bba1c6e679901ba50dd4196f68a2</anchor>
      <arglist>(tnet_ice_ctx_t *self, const void *data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr, tsk_bool_t *role_conflict)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_ctx_get_ufrag</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>af4bb2eb46e5f830afa57bc3f205fda39</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_ice_ctx_get_pwd</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a3839f3ad301a60ed037d165ea96d0b8a</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_cancel</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a3f4609063f5e164bf179e9426147b268</anchor>
      <arglist>(tnet_ice_ctx_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_ctx_stop</name>
      <anchorfile>tnet__ice__ctx_8c.html</anchorfile>
      <anchor>a408b03fe70b52c1956d5958ecd17a630</anchor>
      <arglist>(tnet_ice_ctx_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_ctx.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__ctx_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <member kind="typedef">
      <type>int(*</type>
      <name>tnet_ice_callback_f</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a27ec12a843557e55a95a0fb1ecfa773b</anchor>
      <arglist>)(const struct tnet_ice_event_s *e)</arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>tnet_ice_rtp_callback_f</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a5663852680900fb56fac193335da7e1d</anchor>
      <arglist>)(const void *callback_data, const uint8_t *data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API struct tnet_ice_ctx_s *</type>
      <name>tnet_ice_ctx_create</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a12ee5c476bd9dbde49a91859c009acdc</anchor>
      <arglist>(tsk_bool_t is_ice_jingle, tsk_bool_t use_ipv6, tsk_bool_t use_rtcp, tsk_bool_t is_video, tnet_ice_callback_f callback, const void *userdata)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_set_userdata</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a755c575a5ed684bf9a8f725e723d7e9b</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, const void *userdata)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_set_stun</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a9810feb9fc3f4b65cddcc0ae4f9cdf91</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, const char *server_addr, uint16_t server_port, const char *software, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_start</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>ad22f9137c77b1ec27d3a2e113ebc24ab</anchor>
      <arglist>(struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_rtp_callback</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a483946fd722c8e7abb679842a9a9680f</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, tnet_ice_rtp_callback_f rtp_callback, const void *rtp_callback_data)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_set_concheck_timeout</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a7c9ee53291152f8305b5286192a2e065</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, int64_t timeout)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_set_remote_candidates</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a3898ece620d9e4b7aac8dd782c32192c</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, const char *candidates, const char *ufrag, const char *pwd, tsk_bool_t is_controlling, tsk_bool_t is_ice_jingle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_set_rtcpmux</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a40521555048afd300d4b25daa623f1cd</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, tsk_bool_t use_rtcpmux)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_size_t</type>
      <name>tnet_ice_ctx_count_local_candidates</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a1df3093c5795ce6d3d53a8eb2080be5e</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_got_local_candidates</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a847a4b0980b025c73dacb6348896ae20</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API struct tnet_ice_candidate_s *</type>
      <name>tnet_ice_ctx_get_local_candidate_at</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a911f1de5737af15363c06a1b461496a9</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self, tsk_size_t index)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_is_started</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a282bdc4713a8bc105ee5ab2674f62bc6</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_is_active</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>adcaaff8a00a20dfd5d189f2f7db558ef</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_is_connected</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>aea90d2adab4f24e325bca5d5d89cdfb6</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_is_can_send</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>aa648ed1192ae6efe86fc5f6cf3d5b612</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_is_can_recv</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>aa05115be04d05a7b9e691afa566d3fc3</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_use_ipv6</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>aa8e3918e531c841cd836d8adafea12c4</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_ice_ctx_use_rtcp</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a07737c773d683cb497790a988cb1ebf9</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_get_nominated_symetric_candidates</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>ace906c0ae2a2a54a1690ec7d3918d304</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self, uint32_t comp_id, const struct tnet_ice_candidate_s **candidate_offer, const struct tnet_ice_candidate_s **candidate_answer_src, const struct tnet_ice_candidate_s **candidate_answer_dest)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_recv_stun_message</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>adb56901e00a28eed02082636b67799b8</anchor>
      <arglist>(struct tnet_ice_ctx_s *self, const void *data, tsk_size_t size, tnet_fd_t local_fd, const struct sockaddr_storage *remote_addr, tsk_bool_t *role_conflict)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const char *</type>
      <name>tnet_ice_ctx_get_ufrag</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a6110b9ac7acef0309f2001017bc429eb</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const char *</type>
      <name>tnet_ice_ctx_get_pwd</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>a57e4ecac36e4347afdd7e8ec7ea293fb</anchor>
      <arglist>(const struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_cancel</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>acfb50b643529ac2786ea877b8deb7bea</anchor>
      <arglist>(struct tnet_ice_ctx_s *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_ice_ctx_stop</name>
      <anchorfile>tnet__ice__ctx_8h.html</anchorfile>
      <anchor>afa968e2e280b850087de96990ea6d2fd</anchor>
      <arglist>(struct tnet_ice_ctx_s *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_event.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__event_8c</filename>
    <includes id="tnet__ice__event_8h" name="tnet_ice_event.h" local="yes" imported="no">tnet_ice_event.h</includes>
    <member kind="function">
      <type>tnet_ice_event_t *</type>
      <name>tnet_ice_event_create</name>
      <anchorfile>tnet__ice__event_8c.html</anchorfile>
      <anchor>a77cbab4b4330794f85d9eb5273c058ec</anchor>
      <arglist>(const struct tnet_ice_ctx_s *ctx, tnet_ice_event_type_t type, const char *phrase, const void *userdata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_event_set_action</name>
      <anchorfile>tnet__ice__event_8c.html</anchorfile>
      <anchor>aaa5b3edfeec0d647b25dc6acfd06ff6e</anchor>
      <arglist>(tnet_ice_event_t *self, struct tnet_ice_action_s *action)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_ice_event_def_t</name>
      <anchorfile>tnet__ice__event_8c.html</anchorfile>
      <anchor>a25e7d74caa3ad2b9dcd4ae867e5df04d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_event.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__event_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <class kind="struct">tnet_ice_event_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS enum tnet_ice_event_type_e</type>
      <name>tnet_ice_event_type_t</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a975b8ce3f09b4e1472b0b14500f20f7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_ice_event_s</type>
      <name>tnet_ice_event_t</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a0390cfd3c36181301f676cf6a78c5d62</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_ice_event_type_e</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_started</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618caf2dea4d71ac8de9c60542876a4e36582</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_start_failed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618cacb800acde7ad4618439f914565fef7be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_stopped</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca51db6ab0e95e5de86c99df1411cfacaa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_stop_failed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca4575cdfcaa7d3d2361cc9f739ebe8afd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_gathering_host_candidates_failed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca0fb6cfc0f458b79541b161acda4c0821</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_gathering_host_candidates_succeed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca475effc574a2ffae5ab99ebcbb3feb09</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_gathering_reflexive_candidates_failed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca71124ba92e03873ec44f1536c89ae9a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_gathering_reflexive_candidates_succeed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca5a2e9fc2e3f239d84e6182c15a4ad158</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_gathering_completed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca279c70a496fad0887bb6210b26b72ad3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_conncheck_succeed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca4e0d385966d85596916ae77d7c05d68e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_conncheck_failed</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca0769a7f9355a5fdf3adb31227f86992a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_cancelled</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618ca38cc53ca1eb9e56d1c1f50707648c7fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_event_type_action</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a994d6e614c765273d8a18c4f2f25618cab79c4a6d366909e30fd37f07705d3f7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_event_t *</type>
      <name>tnet_ice_event_create</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>a77cbab4b4330794f85d9eb5273c058ec</anchor>
      <arglist>(const struct tnet_ice_ctx_s *ctx, tnet_ice_event_type_t type, const char *phrase, const void *userdata)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_event_set_action</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>aaa5b3edfeec0d647b25dc6acfd06ff6e</anchor>
      <arglist>(tnet_ice_event_t *self, struct tnet_ice_action_s *action)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_ice_event_def_t</name>
      <anchorfile>tnet__ice__event_8h.html</anchorfile>
      <anchor>aa28619d31a504bbda6e2216cc753edd7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_pair.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__pair_8c</filename>
    <includes id="tnet__ice__pair_8h" name="tnet_ice_pair.h" local="yes" imported="no">tnet_ice_pair.h</includes>
    <includes id="tnet__ice__utils_8h" name="tnet_ice_utils.h" local="yes" imported="no">tnet_ice_utils.h</includes>
    <includes id="tnet__ice__candidate_8h" name="tnet_ice_candidate.h" local="yes" imported="no">tnet_ice_candidate.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">stun/tnet_stun.h</includes>
    <includes id="tnet__stun__message_8h" name="tnet_stun_message.h" local="yes" imported="no">stun/tnet_stun_message.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">tnet_endianness.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_PAIR_FULL_DEBUG</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>af56c18fe90446697a5fc4d3dc0141a31</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_PAIR_DEBUG_INFO</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a4a20a7b0344e3395289e4d2a91df2871</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_ice_pairs_none_succeed_answer</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a91556bbcaf447b1a1def8f41ba42268b</anchor>
      <arglist>(pairs, comp_id, foundation)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_ice_pairs_none_succeed_offer</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>ad60742a24a92d792c2819155da19b8d9</anchor>
      <arglist>(pairs, comp_id, foundation)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_ice_pairs_get_nominated_offer_at</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a94bbca3121738958a49e6548361185cb</anchor>
      <arglist>(pairs, index, comp_id, check_fullness, ret)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_ice_pairs_get_nominated_answer_at</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>af8bd4c48cf08abaf5c1411e548eb2327</anchor>
      <arglist>(pairs, index, comp_id, check_fullness, ret)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_ice_pairs_get_nominated_at</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a7a7acb9867360b14dd28fb7778a367cd</anchor>
      <arglist>(pairs, dir_1, dir_2, index, _comp_id, check_fullness, ret)</arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_pair_t *</type>
      <name>tnet_ice_pair_create</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a9954ff5858e695dc00daa4fedceef7bc</anchor>
      <arglist>(const tnet_ice_candidate_t *candidate_offer, const tnet_ice_candidate_t *candidate_answer, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle)</arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_pair_t *</type>
      <name>tnet_ice_pair_prflx_create</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>ab216201ed60c63e86ccdbac02bc25841</anchor>
      <arglist>(tnet_ice_pairs_L_t *pairs, uint16_t local_fd, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_send_conncheck</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a85c6042e32b7b9cf53b7fa7e0bcc426f</anchor>
      <arglist>(tnet_ice_pair_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_send_response</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a5df18532be4825241faefee37ee03225</anchor>
      <arglist>(tnet_ice_pair_t *self, const tnet_stun_request_t *request, const short code, const char *phrase, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_auth_conncheck</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a66ac3130aa0e23a7afd19626cb672ae5</anchor>
      <arglist>(const tnet_ice_pair_t *self, const tnet_stun_request_t *request, const void *request_buff, tsk_size_t request_buff_size, short *resp_code, char **resp_phrase)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_recv_response</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a677afd408bd9b486277f3b7565143b30</anchor>
      <arglist>(tnet_ice_pair_t *self, const tnet_stun_response_t *response)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_pair_t *</type>
      <name>tnet_ice_pairs_find_by_response</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a01153e3a78f12fcb8624b7bd220be109</anchor>
      <arglist>(tnet_ice_pairs_L_t *pairs, const tnet_stun_message_t *response)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_pair_t *</type>
      <name>tnet_ice_pairs_find_by_fd_and_addr</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>ab4af64f71b6d5cb647bd1dfa06cd1f55</anchor>
      <arglist>(tnet_ice_pairs_L_t *pairs, uint16_t local_fd, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_pairs_have_nominated_offer</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a7b1fe8483a52a82f8991df84072cb2d6</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, tsk_bool_t check_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_pairs_have_nominated_answer</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>acb27f1ec09cbca2e5741bcf725dc0554</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, tsk_bool_t check_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_pairs_have_nominated_symetric</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a2a13d75cb4dbe073c33f39866522bc7b</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, tsk_bool_t check_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pairs_get_nominated_symetric</name>
      <anchorfile>tnet__ice__pair_8c.html</anchorfile>
      <anchor>a66edabbbb2d10827fa8e5b1a016a757d</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, uint32_t comp_id, const tnet_ice_candidate_t **candidate_offer, const tnet_ice_candidate_t **candidate_answer_src, const tnet_ice_candidate_t **candidate_answer_dest)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_pair.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__pair_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <class kind="struct">tnet_ice_pair_s</class>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_ice_pairs_L_t</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>ac830cf0356d199d5a0c4ef65b6f82271</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_ice_pair_state_e</type>
      <name>tnet_ice_pair_state_t</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a83c4d1c62e53329dacef0b1ad5248800</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_ice_pair_s</type>
      <name>tnet_ice_pair_t</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>af480f16d58e42c4530c35be2369071fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_ice_pair_state_e</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a14944762848a13004196843f3247d4fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_pair_state_frozen</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a14944762848a13004196843f3247d4faa96505cd6b130519ff496ddead305f015</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_pair_state_waiting</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a14944762848a13004196843f3247d4faaf0de95577da0434c3c14a34ae8784626</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_pair_state_in_progress</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a14944762848a13004196843f3247d4faaf7210c9f40a6004b8ac784539b3cc33f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_pair_state_succeed</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a14944762848a13004196843f3247d4faaa2fd1062153893d0cfe21af4c4ed7858</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_ice_pair_state_failed</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a14944762848a13004196843f3247d4faa50dcec8ceca592e1b285319d481e5433</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_pair_t *</type>
      <name>tnet_ice_pair_create</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>ab5af472e9d0b64eccbd2c7ff6ae00569</anchor>
      <arglist>(const struct tnet_ice_candidate_s *candidate_offer, const struct tnet_ice_candidate_s *candidate_answer, tsk_bool_t is_controlling, uint64_t tie_breaker, tsk_bool_t is_ice_jingle)</arglist>
    </member>
    <member kind="function">
      <type>tnet_ice_pair_t *</type>
      <name>tnet_ice_pair_prflx_create</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>ab216201ed60c63e86ccdbac02bc25841</anchor>
      <arglist>(tnet_ice_pairs_L_t *pairs, uint16_t local_fd, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_send_conncheck</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a85c6042e32b7b9cf53b7fa7e0bcc426f</anchor>
      <arglist>(tnet_ice_pair_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_send_response</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>aab77228c7d44cc1d6fce074d87ec1d14</anchor>
      <arglist>(tnet_ice_pair_t *self, const struct tnet_stun_message_s *request, const short code, const char *phrase, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_auth_conncheck</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>adb20520b5d6265211ef5f50e68b5db67</anchor>
      <arglist>(const tnet_ice_pair_t *self, const struct tnet_stun_message_s *request, const void *request_buff, tsk_size_t request_buff_size, short *resp_code, char **resp_phrase)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pair_recv_response</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a45bac2585f4e128e950b650e4178cce6</anchor>
      <arglist>(tnet_ice_pair_t *self, const struct tnet_stun_message_s *response)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_pair_t *</type>
      <name>tnet_ice_pairs_find_by_response</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a252eab33e96ac1a00b324c04f09f6302</anchor>
      <arglist>(tnet_ice_pairs_L_t *pairs, const struct tnet_stun_message_s *response)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_ice_pair_t *</type>
      <name>tnet_ice_pairs_find_by_fd_and_addr</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>ab4af64f71b6d5cb647bd1dfa06cd1f55</anchor>
      <arglist>(tnet_ice_pairs_L_t *pairs, uint16_t local_fd, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_pairs_have_nominated_offer</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a7b1fe8483a52a82f8991df84072cb2d6</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, tsk_bool_t check_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_pairs_have_nominated_answer</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>acb27f1ec09cbca2e5741bcf725dc0554</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, tsk_bool_t check_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_ice_pairs_have_nominated_symetric</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a2a13d75cb4dbe073c33f39866522bc7b</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, tsk_bool_t check_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_pairs_get_nominated_symetric</name>
      <anchorfile>tnet__ice__pair_8h.html</anchorfile>
      <anchor>a511052e10a99af4091c493efafe89834</anchor>
      <arglist>(const tnet_ice_pairs_L_t *pairs, uint32_t comp_id, const struct tnet_ice_candidate_s **candidate_offer, const struct tnet_ice_candidate_s **candidate_answer_src, const struct tnet_ice_candidate_s **candidate_answer_dest)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_utils.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__utils_8c</filename>
    <includes id="tnet__ice__utils_8h" name="tnet_ice_utils.h" local="yes" imported="no">tnet_ice_utils.h</includes>
    <includes id="tnet__ice__candidate_8h" name="tnet_ice_candidate.h" local="yes" imported="no">tnet_ice_candidate.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <member kind="function">
      <type>uint32_t</type>
      <name>tnet_ice_utils_get_priority</name>
      <anchorfile>tnet__ice__utils_8c.html</anchorfile>
      <anchor>aa9f6decbe3be7f3e6e053ac5eccbc006</anchor>
      <arglist>(tnet_ice_cand_type_t type, uint16_t local_pref, tsk_bool_t is_rtp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_compute_foundation</name>
      <anchorfile>tnet__ice__utils_8c.html</anchorfile>
      <anchor>a20aa1508b722706b3f79a66fd6cf1f53</anchor>
      <arglist>(char *foundation, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_create_sockets</name>
      <anchorfile>tnet__ice__utils_8c.html</anchorfile>
      <anchor>a036f56c1171682aeaa8afe0b38c3e213</anchor>
      <arglist>(tnet_socket_type_t socket_type, const char *local_ip, tnet_socket_t **socket_rtp, tnet_socket_t **socket_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_stun_address_tostring</name>
      <anchorfile>tnet__ice__utils_8c.html</anchorfile>
      <anchor>aa4b1d320ede078f895b468b654bb1547</anchor>
      <arglist>(const uint8_t in_ip[16], enum tnet_stun_addr_family_e family, char **out_ip)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_set_ufrag</name>
      <anchorfile>tnet__ice__utils_8c.html</anchorfile>
      <anchor>a3a67080a7126486eb3851b1b1ccdb13c</anchor>
      <arglist>(char **ufrag)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_set_pwd</name>
      <anchorfile>tnet__ice__utils_8c.html</anchorfile>
      <anchor>ae3494b188d4676184783f3431eefc91f</anchor>
      <arglist>(char **pwd)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_ice_utils.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>tnet__ice__utils_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="function">
      <type>uint32_t</type>
      <name>tnet_ice_utils_get_priority</name>
      <anchorfile>tnet__ice__utils_8h.html</anchorfile>
      <anchor>a0a76443414a9f5fb9ca22e5b1efd0527</anchor>
      <arglist>(enum tnet_ice_cand_type_e type, uint16_t local_pref, tsk_bool_t is_rtp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_compute_foundation</name>
      <anchorfile>tnet__ice__utils_8h.html</anchorfile>
      <anchor>a20aa1508b722706b3f79a66fd6cf1f53</anchor>
      <arglist>(char *foundation, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_create_sockets</name>
      <anchorfile>tnet__ice__utils_8h.html</anchorfile>
      <anchor>aaea4db85c414e483888ad84b462fe128</anchor>
      <arglist>(enum tnet_socket_type_e socket_type, const char *local_ip, struct tnet_socket_s **socket_rtp, struct tnet_socket_s **socket_rtcp)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_stun_address_tostring</name>
      <anchorfile>tnet__ice__utils_8h.html</anchorfile>
      <anchor>aa4b1d320ede078f895b468b654bb1547</anchor>
      <arglist>(const uint8_t in_ip[16], enum tnet_stun_addr_family_e family, char **out_ip)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_set_ufrag</name>
      <anchorfile>tnet__ice__utils_8h.html</anchorfile>
      <anchor>a3a67080a7126486eb3851b1b1ccdb13c</anchor>
      <arglist>(char **ufrag)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_ice_utils_set_pwd</name>
      <anchorfile>tnet__ice__utils_8h.html</anchorfile>
      <anchor>ae3494b188d4676184783f3431eefc91f</anchor>
      <arglist>(char **pwd)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_stun.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>tnet__stun_8c</filename>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">tnet_stun.h</includes>
    <includes id="tnet__nat_8h" name="tnet_nat.h" local="yes" imported="no">../tnet_nat.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">../tnet_utils.h</includes>
    <member kind="function">
      <type>tnet_stun_binding_t *</type>
      <name>tnet_stun_binding_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga60801a4caec4d4335f0b4fea0e63025e</anchor>
      <arglist>(tnet_fd_t fd, tnet_socket_type_t socket_type, const char *server_address, tnet_port_t server_port, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_create_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga481bcd3d5bfdfda6561227bfcf1fc359</anchor>
      <arglist>(const tnet_stun_binding_t *binding)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_send_reliably</name>
      <anchorfile>tnet__stun_8c.html</anchorfile>
      <anchor>a237dcc868a064df02b762deab4137f3f</anchor>
      <arglist>(const tnet_stun_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_response_t *</type>
      <name>tnet_stun_send_unreliably</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga36147911b3c99cbf897930c971608a5f</anchor>
      <arglist>(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_message_t *message, struct sockaddr *server)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_send_bind</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8151700d692ca34abf3cd42982723aa7</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_stun_binding_t *binding)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_binding_id_t</type>
      <name>tnet_stun_bind</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gafad0d44c67c44e0cf91de149a32e8079</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_transacid_cmp</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gad2cd556a7d81575118646c3e3002af66</anchor>
      <arglist>(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_binding_def_t</name>
      <anchorfile>tnet__stun_8c.html</anchorfile>
      <anchor>a0dd86286d48f57fdd7107fd84788f79a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_stun.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>tnet__stun_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__stun__message_8h" name="tnet_stun_message.h" local="yes" imported="no">stun/tnet_stun_message.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <class kind="struct">tnet_stun_binding_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_INVALID_BINDING_ID</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeb338f5c8a9b3ac08bba78293fdc22d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_IS_VALID_BINDING_ID</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga809cd1386eaa84094e78ef17a414ad7d</anchor>
      <arglist>(id)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_TCP_UDP_DEFAULT_PORT</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga840d6af2bf3bd3a96d120c942abefd7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_TLS_DEFAULT_PORT</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab0ae6bc64ad691f9bc5bd2b4c88c4569</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MAGIC_COOKIE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gac5e54e4b4576b54b53d2f4242f3e30de</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_HEADER_SIZE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga1df2b74747811b1bbdad28b18e0d80c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_binding_s</type>
      <name>tnet_stun_binding_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabb378355a46b1aef733acdeef3a38368</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_stun_bindings_L_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga994b6db4d6ac6e78c4ece450f1d5edff</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_send_reliably</name>
      <anchorfile>tnet__stun_8h.html</anchorfile>
      <anchor>a237dcc868a064df02b762deab4137f3f</anchor>
      <arglist>(const tnet_stun_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_response_t *</type>
      <name>tnet_stun_send_unreliably</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga36147911b3c99cbf897930c971608a5f</anchor>
      <arglist>(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_message_t *message, struct sockaddr *server)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_binding_id_t</type>
      <name>tnet_stun_bind</name>
      <anchorfile>tnet__stun_8h.html</anchorfile>
      <anchor>af34e4d8f55deb66b7ff5414552a7e9e6</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_transacid_cmp</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gad2cd556a7d81575118646c3e3002af66</anchor>
      <arglist>(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)</arglist>
    </member>
    <member kind="variable">
      <type>TNET_BEGIN_DECLS typedef uint64_t</type>
      <name>tnet_stun_binding_id_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab95871f186b8bf46b4630ab2ebb146aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_binding_def_t</name>
      <anchorfile>tnet__stun_8h.html</anchorfile>
      <anchor>acaa4cac3d3d802dd9090d36d6a7dd465</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_stun_attribute.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>tnet__stun__attribute_8c</filename>
    <includes id="tnet__stun__attribute_8h" name="tnet_stun_attribute.h" local="yes" imported="no">tnet_stun_attribute.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">tnet_stun.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <includes id="tnet__turn__attribute_8h" name="tnet_turn_attribute.h" local="yes" imported="no">../turn/tnet_turn_attribute.h</includes>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_stun_attribute_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga9a8fea6f7fb4c6236bc4df85153d6af4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_mapped_addr_t *</type>
      <name>tnet_stun_attribute_mapped_address_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga28ffe32f830bbe0f5b32a207538eb8b6</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_xmapped_addr_t *</type>
      <name>tnet_stun_attribute_xmapped_address_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gacd4af1df8c9e298a23d34df27e5dcc45</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_username_t *</type>
      <name>tnet_stun_attribute_username_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab92c8f62b2d188c9ac1528aa03da4b7e</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_integrity_t *</type>
      <name>tnet_stun_attribute_integrity_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3c5f79313850ea8643132b093e11ef6a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_fingerprint_t *</type>
      <name>tnet_stun_attribute_fingerprint_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2432db4c4bb63da2327bb357361eef21</anchor>
      <arglist>(uint32_t fingerprint)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_errorcode_t *</type>
      <name>tnet_stun_attribute_errorcode_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaa51e8bc565195298fb3de24070f5614a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_realm_t *</type>
      <name>tnet_stun_attribute_realm_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2d7ab50986b3af03951358ec4c2cfa8b</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_nonce_t *</type>
      <name>tnet_stun_attribute_nonce_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0cf404a4977b95a47c252f8101b099ce</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_unknowns_t *</type>
      <name>tnet_stun_attribute_unknowns_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8a497b2b11b1a776ec37170ee14b6ac2</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_software_t *</type>
      <name>tnet_stun_attribute_software_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga719d8c3ac6588124533f03f70877e437</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_altserver_t *</type>
      <name>tnet_stun_attribute_altserver_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga586636541b8ebb9ea46d4df39a448121</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_priority_t *</type>
      <name>tnet_stun_attribute_ice_priority_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7e2992cbcf5e4d93fce285c8fdad6018</anchor>
      <arglist>(uint32_t value)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_use_candidate_t *</type>
      <name>tnet_stun_attribute_ice_use_candidate_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gafc6efba58197cb3d697502bbad7378cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_controlled_t *</type>
      <name>tnet_stun_attribute_ice_controlled_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaf2e4854c90f7c8c3abae9915e59ad12f</anchor>
      <arglist>(uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_controlling_t *</type>
      <name>tnet_stun_attribute_ice_controlling_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga94da0e7a21d255c8647c190387229ba3</anchor>
      <arglist>(uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_stun_attribute_deserialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga642d9b382cea0cbbf0590b8b60be54c6</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_attribute_serialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabb81a19c0eeb9f47f0b9599d65d61d6e</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_stun_attribute_pad</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga9364909d8a9c38fa1be0dd48efa3fac3</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a39de14fd624ed0daca8ee80b3b788167</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_mapped_addr_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a2069575f6768e7dbf84b722ab49e2092</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_xmapped_addr_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>aafd262dbecb13f75062287d1a53f79cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_username_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a061d0206949d7884f94db65fd4d13ff8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_integrity_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>adb93c2f122fac924c896e1d6e41dee12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_fingerprint_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a1a8d87cb810361528f08db9ac6ad08d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_errorcode_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>ae68e0c928ed436d9f61f3924b5bc937f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_realm_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a7e250a9748696036a59bd90577a803eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_nonce_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>aa0af7d26bae0b56988d370be6640c1c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_unknowns_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a78dfa6acb03b4b2097cb5cdf1cfc7559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_software_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a2b6d86a4a4f4ce12b7053a8804b6b773</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_altserver_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>ad40ed47dfee2cb3133324064dce05d37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_priority_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a1c25a2b5a16a48e1c229f81e0f0cf1ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_use_candidate_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>aef82b09b0928979e9559be947d4a2f4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_controlled_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a975b2da5a481b51af116f5a3775f2889</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_controlling_def_t</name>
      <anchorfile>tnet__stun__attribute_8c.html</anchorfile>
      <anchor>a684b7fd78ca1f9002093ac01b5792600</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_stun_attribute.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>tnet__stun__attribute_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <class kind="struct">tnet_stun_attribute_s</class>
    <class kind="struct">tnet_stun_attribute_mapped_addr_s</class>
    <class kind="struct">tnet_stun_attribute_xmapped_addr_s</class>
    <class kind="struct">tnet_stun_attribute_username_s</class>
    <class kind="struct">tnet_stun_attribute_integrity_s</class>
    <class kind="struct">tnet_stun_attribute_fingerprint_s</class>
    <class kind="struct">tnet_stun_attribute_errorcode_s</class>
    <class kind="struct">tnet_stun_attribute_realm_s</class>
    <class kind="struct">tnet_stun_attribute_nonce_s</class>
    <class kind="struct">tnet_stun_attribute_unknowns_s</class>
    <class kind="struct">tnet_stun_attribute_software_s</class>
    <class kind="struct">tnet_stun_attribute_altserver_s</class>
    <class kind="struct">tnet_stun_attribute_ice_priority_s</class>
    <class kind="struct">tnet_stun_attribute_ice_use_candidate_s</class>
    <class kind="struct">tnet_stun_attribute_ice_controlled_s</class>
    <class kind="struct">tnet_stun_attribute_ice_controlling_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_ATTRIBUTE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeec0275bf11e81b2b89c149df5194c51</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a62f9fefaa02d86218a754f2f8f9d9b68</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_addr_family_e</type>
      <name>tnet_stun_addr_family_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7080ca2a8d2b45a22d21c522a2a5a28f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_attribute_type_e</type>
      <name>tnet_stun_attribute_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2e77e50a4af676b754191403ae102933</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_s</type>
      <name>tnet_stun_attribute_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3ce2a25dae1324a8dd545a68a2d8ccf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_stun_attributes_L_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a423021672c1cb1cfc6e87418925fee9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_mapped_addr_s</type>
      <name>tnet_stun_attribute_mapped_addr_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga6ef5c5c6d47376dc7cf7ac14b923ad26</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_xmapped_addr_s</type>
      <name>tnet_stun_attribute_xmapped_addr_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8885759ce02dd0bebc0bee97ec48171c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_username_s</type>
      <name>tnet_stun_attribute_username_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga33d875930c4e6213e7516e580a88eebb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_integrity_s</type>
      <name>tnet_stun_attribute_integrity_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8b2b266a46e44c471708eca71ac8670a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_fingerprint_s</type>
      <name>tnet_stun_attribute_fingerprint_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7ead14f441ddf014894c17fcef2feeaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_errorcode_s</type>
      <name>tnet_stun_attribute_errorcode_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0e804aae488d73e576dd9c4453a93a04</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_realm_s</type>
      <name>tnet_stun_attribute_realm_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaf4ef4fa43bec46b6313c0c400d6c6ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_nonce_s</type>
      <name>tnet_stun_attribute_nonce_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7e4d37384aa42524d68d8e9e0ddc9ea2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_unknowns_s</type>
      <name>tnet_stun_attribute_unknowns_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab27dd830e06e223350a584c41383015d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_software_s</type>
      <name>tnet_stun_attribute_software_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga5744df35d8bf1f1f366097f6a63f1177</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_altserver_s</type>
      <name>tnet_stun_attribute_altserver_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga1912ae8bc9ea407a04f1d16aa570dbc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_priority_s</type>
      <name>tnet_stun_attribute_ice_priority_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga782dcf37c6503a9287b479cef6fe0132</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_use_candidate_s</type>
      <name>tnet_stun_attribute_ice_use_candidate_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga47de3ced777b9c46f863e513b934ab23</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_controlled_s</type>
      <name>tnet_stun_attribute_ice_controlled_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab46c08e93d0c514a72fc8b3f73bc7a77</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_controlling_s</type>
      <name>tnet_stun_attribute_ice_controlling_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gac4832ed589696b4a9ef6274b96333adf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_addr_family_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3326fbcc063f264d3d359e922637e2e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ipv4</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3326fbcc063f264d3d359e922637e2e4a5663f4b60301ed2f9e7823613407dbed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ipv6</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3326fbcc063f264d3d359e922637e2e4a97e1e424d39a8af2791c1187090f111e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_attribute_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab696586b61219965987af1e017e6ffa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reserved</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a5bf34ce06a004ad9f8db1dc687786273</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_mapped_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a9aadc5f8467c936cc7359889ba120e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_response_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a203b0a7edf4f8fb3381ac769595a2098</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_change_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6aeb35b7b83152b7a99bf46b4014928adc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_source_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a69f3448100d4f1a61536eb2c7315cd90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_changed_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a0bd0d2c8f1d6ae85ffb58aeef4c01134</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_username</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a3247970572dad0c63e537b647dbd0861</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_password</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a432698b7b67cf13adf504f969abb60fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_message_integrity</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ac20851e6e8fd2c479c0438b376987a85</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_error_code</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ab8571c9a5c334530ef9aa45adc7fd71e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_unknown_attributes</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a455d442e5c380953ff11c7e0d055f3ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reflected_from</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ae129dfcf945fe995400dba3e8b9895c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_realm</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a0d8e28c40313d6d3b397ac957714cfee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_nonce</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ae5dda080590c37e95953334f715ed3ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_xor_mapped_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6af70dcad086bff5d4ee77f093941642d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_software</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ad5a2f9a89fbc825142e4d754b51bc125</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_alternate_server</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a4f263d754aaccf30fd861a934dfc12ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_fingerprint</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6aac669613b66507fba6c08d23a4283bf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channel_number</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a7559421f1d46a400cdda94c32c3aff74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_lifetime</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6aff4ed6f5e451f77b39d5113b9085493c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reserved2</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a626c6684c81f69c1649e178cc9847069</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_xor_peer_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ae14d3b3903934eab51356ad28584fc66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_data</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ab85dfed04ff37affea00d10a517c72dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_xor_relayed_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ada0a3d93b81c32a3d0af2ed20efffe71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_even_port</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ac99da9ae2b9a6a885631e5e476293df2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_requested_transport</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a033a9a9cdad515c45291007be25dd028</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_dont_fragment</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a3f854135f19b767abc7e8031af6b5f88</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reserved3</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a57c9e8d7229d0989d73a5efc2fef36e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reservation_token</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a88373f0f1635103b2091910a9584c58d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_priority</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a99410d8e75f7ab0d4421fe71634604bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_use_candidate</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a275c187aabe3a9d9bdc32a180c39b1d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_controlled</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a4cfd4eab6287ed2b1126e6ba40ac5d4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_controlling</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a21100a7a716c88ed5b5544cdb28549aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_stun_attribute_deserialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga642d9b382cea0cbbf0590b8b60be54c6</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_attribute_serialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabb81a19c0eeb9f47f0b9599d65d61d6e</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_stun_attribute_pad</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga9364909d8a9c38fa1be0dd48efa3fac3</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_stun_attribute_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga9a8fea6f7fb4c6236bc4df85153d6af4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_mapped_addr_t *</type>
      <name>tnet_stun_attribute_mapped_address_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga28ffe32f830bbe0f5b32a207538eb8b6</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_xmapped_addr_t *</type>
      <name>tnet_stun_attribute_xmapped_address_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gacd4af1df8c9e298a23d34df27e5dcc45</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_username_t *</type>
      <name>tnet_stun_attribute_username_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab92c8f62b2d188c9ac1528aa03da4b7e</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_integrity_t *</type>
      <name>tnet_stun_attribute_integrity_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3c5f79313850ea8643132b093e11ef6a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_fingerprint_t *</type>
      <name>tnet_stun_attribute_fingerprint_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2432db4c4bb63da2327bb357361eef21</anchor>
      <arglist>(uint32_t fingerprint)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_errorcode_t *</type>
      <name>tnet_stun_attribute_errorcode_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaa51e8bc565195298fb3de24070f5614a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_realm_t *</type>
      <name>tnet_stun_attribute_realm_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2d7ab50986b3af03951358ec4c2cfa8b</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_nonce_t *</type>
      <name>tnet_stun_attribute_nonce_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0cf404a4977b95a47c252f8101b099ce</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_unknowns_t *</type>
      <name>tnet_stun_attribute_unknowns_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8a497b2b11b1a776ec37170ee14b6ac2</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_software_t *</type>
      <name>tnet_stun_attribute_software_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga719d8c3ac6588124533f03f70877e437</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_altserver_t *</type>
      <name>tnet_stun_attribute_altserver_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga586636541b8ebb9ea46d4df39a448121</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_ice_priority_t *</type>
      <name>tnet_stun_attribute_ice_priority_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7e2992cbcf5e4d93fce285c8fdad6018</anchor>
      <arglist>(uint32_t value)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_ice_use_candidate_t *</type>
      <name>tnet_stun_attribute_ice_use_candidate_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gafc6efba58197cb3d697502bbad7378cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_ice_controlled_t *</type>
      <name>tnet_stun_attribute_ice_controlled_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaf2e4854c90f7c8c3abae9915e59ad12f</anchor>
      <arglist>(uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_attribute_ice_controlling_t *</type>
      <name>tnet_stun_attribute_ice_controlling_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga94da0e7a21d255c8647c190387229ba3</anchor>
      <arglist>(uint64_t value)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a1ed3e8d18968c427abbca5046e9a2aae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_mapped_addr_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>ae158b2a2e20ebe910d5b937b6c31e8f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_xmapped_addr_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>ade3c87fd23c4418da071a962f4fa3a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_username_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a3618ce3b069dc37797b7a39e60342a2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_integrity_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a76de57dbb7322cf91c856dd8d4bba3e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_fingerprint_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a8d8cbe79ce3fa75cad33acb8edab5b98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_errorcode_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a81aa99bbc4ef57f453b41485a6f2aa90</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_realm_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a0327c124b1eff63f330af0b55f02c1fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_nonce_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a6d2b7027e8dccf917a94b53703137b8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_unknowns_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a0db4a8abf86d2e730755456eb372f7dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_software_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a2fcc436a3a9b8217cb57779f66ab6844</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_altserver_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a8d426ae80202c572513916de48cab014</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_priority_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a2aa16d9877e44a873017f40ea3712fc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_use_candidate_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>aadbce41b24c9f38a157ca28baaf3a4c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_controlled_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a2f8cbba757f3065b192603ea5648db71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_attribute_ice_controlling_def_t</name>
      <anchorfile>tnet__stun__attribute_8h.html</anchorfile>
      <anchor>a0339fb43d3cc554b59bfec0fd41d89d5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_stun_message.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>tnet__stun__message_8c</filename>
    <includes id="tnet__stun__message_8h" name="tnet_stun_message.h" local="yes" imported="no">tnet_stun_message.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">tnet_stun.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <includes id="tnet__turn__attribute_8h" name="tnet_turn_attribute.h" local="yes" imported="no">../turn/tnet_turn_attribute.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SERIALIZE_N_ADD_ATTRIBUTE</name>
      <anchorfile>tnet__stun__message_8c.html</anchorfile>
      <anchor>a22b95434b0e041992dd2058e5688e696</anchor>
      <arglist>(att_name, payload, payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaf1892e2a30f8b85e4e5b47622a6b9f3</anchor>
      <arglist>(const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_create_null</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga938bc40f8bb818f87c94eb42352cd0f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_stun_message_serialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaefd2306ef33e3611a98394e5b274a069</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_deserialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0f5f3b814bb84f3cb62802b1d9af275e</anchor>
      <arglist>(const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_stun_message_has_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga250b97b6a26f99f9d545533e20d29acb</anchor>
      <arglist>(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_message_add_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gacb982af5ead9faf9e85b4e2f69e05d47</anchor>
      <arglist>(tnet_stun_message_t *self, tnet_stun_attribute_t **attribute)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_message_remove_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8c3cf4006d4c030e0114b8c00f2dc5ec</anchor>
      <arglist>(tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_stun_attribute_t *</type>
      <name>tnet_stun_message_get_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeac2966bdc80f798ec61f6f7ebfc6a95</anchor>
      <arglist>(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>short</type>
      <name>tnet_stun_message_get_errorcode</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7638d5816c344718874a113abd9739d5</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_stun_message_get_realm</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga6115407c7af73b3041c215ccd34e35b8</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_stun_message_get_nonce</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga5e068d5474bfbd73e8cece833deac6b2</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>tnet_stun_message_get_lifetime</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga12380617e5a883d3c175216c48245839</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_stun_message_transac_id_equals</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga401b2a061923a11340da833dc248fa9d</anchor>
      <arglist>(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_stun_message_def_t</name>
      <anchorfile>tnet__stun__message_8c.html</anchorfile>
      <anchor>a19626e26e58ad7a9c09264174314053d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_stun_message.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>tnet__stun__message_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__stun__attribute_8h" name="tnet_stun_attribute.h" local="yes" imported="no">stun/tnet_stun_attribute.h</includes>
    <class kind="struct">tnet_stun_message_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_CLASS_REQUEST_MASK</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>aa3d2e023082dd0468d788a204e4f5588</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_CLASS_INDICATION_MASK</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>a6cb86b711e3aa24320405b60f2da1beb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_CLASS_SUCCESS_MASK</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>a0134dd005c09929e5e557fe0d4b00ff4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_CLASS_ERROR_MASK</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>aa1bad5b0786da02f7562d45b83e538fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MESSAGE_IS_REQUEST</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga50742ed21d77367f7d6d0b1ece487d94</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MESSAGE_IS_RESPONSE</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>a67a78f2b21fdd0243800c240d2453fa3</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MESSAGE_IS_INDICATION</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga05a95d304163ba125defae8444a28f73</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_RESPONSE_IS_SUCCESS</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga98049d686a5db4369a7554a77bf26838</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_RESPONSE_IS_ERROR</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga10bdb77a5f51b4670be89302eb445974</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_IS_STUN2_MSG</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3e9c76b717211f3d8c27d624a07e2353</anchor>
      <arglist>(PU8, SIZE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_IS_STUN2</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>a308ab872ed8673719c1d0cb120b2ba03</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_TRANSACID_SIZE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaa796e7b0e870b335336c50024ef5823</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>tnet_stun_transacid_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaab3bfd494f75601a0f26177c6d851810</anchor>
      <arglist>[TNET_STUN_TRANSACID_SIZE]</arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_class_type_e</type>
      <name>tnet_stun_class_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8db95f74a4953795bf182744e63eff78</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_method_type_e</type>
      <name>tnet_stun_method_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga145552721a7b4f524ceb40ac2da1038d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_message_type_e</type>
      <name>tnet_stun_message_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab3ed2f3313ef6914347c2e604d7cf104</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_message_s</type>
      <name>tnet_stun_message_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga4e9470802bdd2d8b0a1b90631369721e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_stun_message_t</type>
      <name>tnet_stun_response_t</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>ae36d2cd92b9d0f1ae545b4ba4241f35b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_stun_message_t</type>
      <name>tnet_stun_request_t</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>a5f7a4c74272f8549b89d8bb2ad6206b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_class_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3faef946079e315842f5ad4d41aa0929</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a4617c080243a43b3d8f93bdfcc3f0f4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a204a6136670e4ad622276ef22a2ae65e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a0f27f07bfc9be6aa217799fa088cf059</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a271a1bc249f8bc1af5b9e6308b166232</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_method_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga4701a0e3395592a6d6a742901212801c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_binding</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801cafd301f45e6297ffca65062c30b89ef53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_allocate</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca166402bfafe37fa33b31f1466265798b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_refresh</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca6081fbd713a271d81264c919a589ce97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_send</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca3c8e39a747df8f52dcc70d5d7db3e423</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_data</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca72ac38a767b77800980a5577dd13059c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_createpermission</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca45791d8de7420d9d3afccfc78ac8ba8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_channelbind</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca9a883e8885d8eb35a93beccda692233c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_message_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabf2f8d8b846fe491470941a7e237559c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caef49ee403fb2e849831649e4c09729d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca40f5bbd220cf9c1ec50654becc93c75f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca45e30ced8713c5c159bb45641ce04e79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca9526f90725e2c9be01cd4ea2aa818e50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cab3d8c425c241c43ab3bdbec7fdddc2dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cadda6f40f23dc4bd53d1d6c068484ada0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caf20455522d8e72c9e055b8ea5ef2dabb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cae4d21e386431edf815a9a652f3d745a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca3f15b1671e604a2fb7a65401756dccc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca5130e27fd810862da942fcde58eb0c42</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caead4a26ba4d373a96e4dac68ff9fdb45</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca17067ce9dae33780b432c50b2a53130b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_send_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cac7424fdd7360f72326f8f9c74d36d15b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_data_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca66d958d23caaac30ab000f37c36a3406</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca55ffb1e77c92edb303a51f7221049c66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cad1099efddc3ea3f8e91bd7953fef5c50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caa804d83ad6928163af8e5b8c29c003dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cad3da1d3b613236f00e9747a3874eb3c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caf9886f762e46af82fc8122ba35a95a8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cae56d07904cc9d8091f32a6973258146f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cafcb4db651d71fd5c21f7bbb2def064a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cab65200966087d7dd85dd17de30cde9ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_buffer_t *</type>
      <name>tnet_stun_message_serialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaefd2306ef33e3611a98394e5b274a069</anchor>
      <arglist>(const tnet_stun_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_deserialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0f5f3b814bb84f3cb62802b1d9af275e</anchor>
      <arglist>(const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_stun_message_has_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga250b97b6a26f99f9d545533e20d29acb</anchor>
      <arglist>(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_stun_message_add_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gacb982af5ead9faf9e85b4e2f69e05d47</anchor>
      <arglist>(tnet_stun_message_t *self, tnet_stun_attribute_t **attribute)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_message_remove_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8c3cf4006d4c030e0114b8c00f2dc5ec</anchor>
      <arglist>(tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_stun_attribute_t *</type>
      <name>tnet_stun_message_get_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeac2966bdc80f798ec61f6f7ebfc6a95</anchor>
      <arglist>(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>short</type>
      <name>tnet_stun_message_get_errorcode</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7638d5816c344718874a113abd9739d5</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_stun_message_get_realm</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga6115407c7af73b3041c215ccd34e35b8</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_stun_message_get_nonce</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga5e068d5474bfbd73e8cece833deac6b2</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>tnet_stun_message_get_lifetime</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga12380617e5a883d3c175216c48245839</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_stun_message_transac_id_equals</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga401b2a061923a11340da833dc248fa9d</anchor>
      <arglist>(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_message_t *</type>
      <name>tnet_stun_message_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaf1892e2a30f8b85e4e5b47622a6b9f3</anchor>
      <arglist>(const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_message_t *</type>
      <name>tnet_stun_message_create_null</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga938bc40f8bb818f87c94eb42352cd0f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_stun_message_def_t</name>
      <anchorfile>tnet__stun__message_8h.html</anchorfile>
      <anchor>a447a48c88e1eb9124f6ac0669a1f4000</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tinynet.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tinynet_8h</filename>
    <includes id="tnet_8h" name="tnet.h" local="yes" imported="no">tnet.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">tnet_endianness.h</includes>
    <includes id="tnet__nat_8h" name="tnet_nat.h" local="yes" imported="no">tnet_nat.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <includes id="tnet__transport_8h" name="tnet_transport.h" local="yes" imported="no">tnet_transport.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">stun/tnet_stun.h</includes>
    <includes id="tnet__ice__event_8h" name="tnet_ice_event.h" local="yes" imported="no">ice/tnet_ice_event.h</includes>
    <includes id="tnet__ice__candidate_8h" name="tnet_ice_candidate.h" local="yes" imported="no">ice/tnet_ice_candidate.h</includes>
    <includes id="tnet__ice__ctx_8h" name="tnet_ice_ctx.h" local="yes" imported="no">ice/tnet_ice_ctx.h</includes>
    <includes id="tnet__dns_8h" name="tnet_dns.h" local="yes" imported="no">dns/tnet_dns.h</includes>
    <includes id="tnet__dns__naptr_8h" name="tnet_dns_naptr.h" local="yes" imported="no">dns/tnet_dns_naptr.h</includes>
    <includes id="tnet__dns__regexp_8h" name="tnet_dns_regexp.h" local="yes" imported="no">dns/tnet_dns_regexp.h</includes>
    <includes id="tnet__dns__resolvconf_8h" name="tnet_dns_resolvconf.h" local="yes" imported="no">dns/tnet_dns_resolvconf.h</includes>
    <includes id="tnet__dhcp_8h" name="tnet_dhcp.h" local="yes" imported="no">dhcp/tnet_dhcp.h</includes>
    <includes id="tnet__dhcp__option__sip_8h" name="tnet_dhcp_option_sip.h" local="yes" imported="no">dhcp/tnet_dhcp_option_sip.h</includes>
    <includes id="tnet__dhcp6_8h" name="tnet_dhcp6.h" local="yes" imported="no">dhcp6/tnet_dhcp6.h</includes>
    <includes id="tnet__dhcp6__option_8h" name="tnet_dhcp6_option.h" local="yes" imported="no">dhcp6/tnet_dhcp6_option.h</includes>
  </compound>
  <compound kind="file">
    <name>tinynet_config.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tinynet__config_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TINYNET_API</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>aae93e76d17d1ca6ff95aad28b3abf658</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYNET_GEXTERN</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>a81d479293190a58ef229b695fb4a8ab0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_BEGIN_DECLS</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>a75bf1f5d1f1371c87bf2f5d9d61f458b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_END_DECLS</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>a48c3cca1fa120fe82738d64e85c34351</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_INLINE</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>a6e01306b61ca63a7ff08571c01bcd5e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOFTWARE</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>ae2d04c0df9b8a96c09fde862ec6e4304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_IANA_PEN</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>a612c5d1310d848ba03078217088c3d26</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_RESOLV_CONF_PATH</name>
      <anchorfile>tinynet__config_8h.html</anchorfile>
      <anchor>ab55f5a2cc9ce6e6b281afa7f869956f5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dtls.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls/</path>
    <filename>tnet__dtls_8c</filename>
    <includes id="tnet__dtls_8h" name="tnet_dtls.h" local="yes" imported="no">tnet_dtls.h</includes>
    <includes id="tnet__tls_8h" name="tnet_tls.h" local="yes" imported="no">tnet_tls.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <class kind="struct">tnet_dtls_socket_s</class>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_dtls_socket_do_handshake</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>ace7d8b69e95ad5631aa022097c832d8a</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_dtls_socket_raise_event</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>afd9fe6841791319d91514235cb7a3395</anchor>
      <arglist>(self, type, data, size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tnet_dtls_socket_raise_event_dataless</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a63e098461c2ff5c68fe8dea627634ce1</anchor>
      <arglist>(self, type)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dtls_socket_s</type>
      <name>tnet_dtls_socket_t</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a24f671f502be206e1120cdff7a6a50a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_dtls_is_srtp_supported</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>ad09a2debbb13fe5c0cc698a61df65dc7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_dtls_is_supported</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a25238311d78e2aca33c0d6c215896bae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dtls_hash_type_t</type>
      <name>tnet_dtls_get_hash_from_string</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>aadd696d262808b42f37a1a2030d815c3</anchor>
      <arglist>(const char *hash)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dtls_setup_t</type>
      <name>tnet_dtls_get_setup_from_string</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a40fb953e7deaaf95bd89ca1c1099a928</anchor>
      <arglist>(const char *setup)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_get_fingerprint</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>afb717cd09e0f914acefc0a62f6ee68df</anchor>
      <arglist>(const char *certfile, tnet_fingerprint_t *fingerprint, tnet_dtls_hash_type_t hash)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dtls_socket_handle_t *</type>
      <name>tnet_dtls_socket_create</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a44afb468495ae1cec8aea9457dc392d1</anchor>
      <arglist>(tnet_fd_t fd, struct ssl_ctx_st *ssl_ctx)</arglist>
    </member>
    <member kind="function">
      <type>tnet_fd_t</type>
      <name>tnet_dtls_socket_get_fd</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a16d3cdc446403a2aef45e3fb2d1cde40</anchor>
      <arglist>(const tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>struct sockaddr_storage *</type>
      <name>tnet_dtls_socket_get_remote_addr</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a8f9c9ffba8efe2f3acf1fbcb9f5eca87</anchor>
      <arglist>(const tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_socket_set_callback</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a99f946ceca66bc754ef1e0b0d5e4dc39</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const void *usrdata, tnet_dtls_socket_cb_f func)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_socket_set_remote_fingerprint</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a39f580983e0ad861df949c142ca785a2</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const tnet_fingerprint_t *fingerprint, tnet_dtls_hash_type_t hash)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_socket_use_srtp</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a50ad734fe046869215f1651013054d37</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_socket_set_setup</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>acaee108ca648f7c328c355081bfb70dc</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, tnet_dtls_setup_t setup)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_dtls_socket_is_remote_cert_fp_match</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>aa244e20933fc3301323cc3c28918964e</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_socket_do_handshake</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a73d03cb325831285b68fd7c291fe2efc</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_dtls_socket_is_handshake_completed</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a208edc774b7e0000f82a51144222a041</anchor>
      <arglist>(const tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dtls_socket_handle_incoming_data</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>af48bb9403a9bac2421f5914a20c36cff</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_dtls_socket_def_t</name>
      <anchorfile>tnet__dtls_8c.html</anchorfile>
      <anchor>a91eab3d9db44ae1e119a3d7c174e505c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_dtls.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls/</path>
    <filename>tnet__dtls_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <member kind="typedef">
      <type>void</type>
      <name>tnet_dtls_socket_handle_t</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a31d0de4ae3dd651cffe744aa94d36f27</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dtls_socket_event_type_e</type>
      <name>tnet_dtls_socket_event_type_t</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>aa4e6c5e99dc54e204378391e54ad14d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>tnet_dtls_socket_cb_f</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a3529a64ddc3fa5f07ed3c692bebc560a</anchor>
      <arglist>)(const void *usrdata, tnet_dtls_socket_event_type_t e, const tnet_dtls_socket_handle_t *handle, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dtls_socket_event_type_e</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0de</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_handshake_started</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0dea43948921572953c4e898af4cf1c22702</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_handshake_succeed</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0deafb14934d988f7708669f370b537a7def</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_handshake_failed</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0dea114444f752950d9f75fcfcf74d1c51bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_fingerprint_mismatch</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0deadfc3a56addce201f9e50bdd655081828</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_error</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0deaed3d12a2bc8ab5c18c15e3a92ec749c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_dtls_srtp_profile_selected</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0dea7dd0a2996205b1a37530f288f48a1089</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_socket_event_type_dtls_srtp_data</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a5161f0204780718c0b573c7970d4a0dea975c7bcd42f5cfd82262b79b0864ace2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_dtls_is_srtp_supported</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a43e4fc390be71148720b2a191702ef8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_dtls_is_supported</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>ac6f9f9b863ff1105fbb27cfb37f9b53a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dtls_hash_type_t</type>
      <name>tnet_dtls_get_hash_from_string</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>ac8028156e7807585a4c621821ca93564</anchor>
      <arglist>(const char *hash)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dtls_setup_t</type>
      <name>tnet_dtls_get_setup_from_string</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a1e8ae82dce1152a51863aa93ccf5a1c1</anchor>
      <arglist>(const char *setup)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_get_fingerprint</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a624c230b834e6b4f6cc82bd4affd4b13</anchor>
      <arglist>(const char *certfile, tnet_fingerprint_t *fingerprint, tnet_dtls_hash_type_t hash)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_dtls_socket_handle_t *</type>
      <name>tnet_dtls_socket_create</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>ad22dbfe69ef4b9aebe1a58ba6ac28df9</anchor>
      <arglist>(tnet_fd_t fd, struct ssl_ctx_st *ssl_ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_fd_t</type>
      <name>tnet_dtls_socket_get_fd</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>ad86275a4b6a016bb8abdff853d468aa5</anchor>
      <arglist>(const tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API struct sockaddr_storage *</type>
      <name>tnet_dtls_socket_get_remote_addr</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a4904903b026ee11936300d9ab68ff6a5</anchor>
      <arglist>(const tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_socket_set_callback</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a20024a2fa68ba9bd25c931dfa7a57dab</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const void *usrdata, tnet_dtls_socket_cb_f func)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_socket_set_remote_fingerprint</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a3cc5666b7c805601e87b39efc6e54c47</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const tnet_fingerprint_t *fingerprint, tnet_dtls_hash_type_t hash)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_socket_use_srtp</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a3345ecf76fd86aeee8849291f8acde0f</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_socket_set_setup</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a4392b4e116b50e93986b71412f5745aa</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, tnet_dtls_setup_t setup)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_dtls_socket_is_remote_cert_fp_match</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a1d12a561c9bfadae2047d57f1f7069c0</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_socket_do_handshake</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a396d2828d6a41bf9aa8b015234378913</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const struct sockaddr_storage *remote_addr)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_dtls_socket_is_handshake_completed</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a2c482e33bedc851800e9c52b257c4b73</anchor>
      <arglist>(const tnet_dtls_socket_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_dtls_socket_handle_incoming_data</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>afb18054bdb7bff7c36c8ed83bd705e51</anchor>
      <arglist>(tnet_dtls_socket_handle_t *handle, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_dtls_socket_def_t</name>
      <anchorfile>tnet__dtls_8h.html</anchorfile>
      <anchor>a935d64508e3f73649a7ccdbec9dcbf91</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_tls.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls/</path>
    <filename>tnet__tls_8c</filename>
    <includes id="tnet__tls_8h" name="tnet_tls.h" local="yes" imported="no">tnet_tls.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <class kind="struct">tnet_tls_socket_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TLS_TIMEOUT</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>a8a5893e48b2a03cdf2ad2fdf4dab27fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TLS_RETRY_COUNT</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>acf3b352408903e8954c01cdc9999d0ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_tls_socket_s</type>
      <name>tnet_tls_socket_t</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>ab3bf41a1258ce44672ff13f23a2ecf17</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_tls_is_supported</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>ad04214197a9e36d79b813a2c13de54e3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_tls_socket_handle_t *</type>
      <name>tnet_tls_socket_create</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>ac6938704c21f0790191b153e3e9ac593</anchor>
      <arglist>(tnet_fd_t fd, struct ssl_ctx_st *ssl_ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_connect</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>a188bd068a96c3acc2f031cb6d04d4458</anchor>
      <arglist>(tnet_tls_socket_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_accept</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>a2bf506e70d5eb1b45ee6a9f8b5a6d690</anchor>
      <arglist>(tnet_tls_socket_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_write</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>a43612982a13f6f14301034c5ce1b7cb7</anchor>
      <arglist>(tnet_tls_socket_handle_t *self, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_recv</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>a989798393b8e30bc55f3904c0324da6f</anchor>
      <arglist>(tnet_tls_socket_handle_t *self, void **data, tsk_size_t *size, tsk_bool_t *isEncrypted)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_tls_socket_def_t</name>
      <anchorfile>tnet__tls_8c.html</anchorfile>
      <anchor>a4e4baa33e452176cd4a4a52ca5148bcf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_tls.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls/</path>
    <filename>tnet__tls_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>tnet_tls_socket_send</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>ae8efefb3d96440fe53fc8a4572613c5e</anchor>
      <arglist>(self, data, size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_connect</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a188bd068a96c3acc2f031cb6d04d4458</anchor>
      <arglist>(tnet_tls_socket_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_accept</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a2bf506e70d5eb1b45ee6a9f8b5a6d690</anchor>
      <arglist>(tnet_tls_socket_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_write</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a43612982a13f6f14301034c5ce1b7cb7</anchor>
      <arglist>(tnet_tls_socket_handle_t *self, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_tls_socket_recv</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a989798393b8e30bc55f3904c0324da6f</anchor>
      <arglist>(tnet_tls_socket_handle_t *self, void **data, tsk_size_t *size, tsk_bool_t *isEncrypted)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_tls_is_supported</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a8adca95fbe88dc5bd5ab67632055bf6e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_tls_socket_handle_t *</type>
      <name>tnet_tls_socket_create</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>ae406f12f84152b4c6f735e6ae8b4df7d</anchor>
      <arglist>(tnet_fd_t fd, struct ssl_ctx_st *ssl_ctx)</arglist>
    </member>
    <member kind="variable">
      <type>TNET_BEGIN_DECLS typedef void</type>
      <name>tnet_tls_socket_handle_t</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a6f1311b1d1c8744b36092ebcbf1a3b87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_tls_socket_def_t</name>
      <anchorfile>tnet__tls_8h.html</anchorfile>
      <anchor>a8ed545d5b0d5aad6da18b942e2b53e1a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet_8c</filename>
    <includes id="tnet_8h" name="tnet.h" local="yes" imported="no">tnet.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <member kind="function">
      <type>int</type>
      <name>tnet_startup</name>
      <anchorfile>tnet_8c.html</anchorfile>
      <anchor>ae58106beb601241b29d944fe40c8f300</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_cleanup</name>
      <anchorfile>tnet_8c.html</anchorfile>
      <anchor>ac42b22a7ac5831f04326aee9de033c84</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>tnet_isBigEndian</name>
      <anchorfile>tnet_8c.html</anchorfile>
      <anchor>acd8f51d06c3a3d408e2b52428034cc24</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="function">
      <type>TNET_BEGIN_DECLS TINYNET_API int</type>
      <name>tnet_startup</name>
      <anchorfile>tnet_8h.html</anchorfile>
      <anchor>a4ed9dafe102a2a8e0789f0c901206963</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_cleanup</name>
      <anchorfile>tnet_8h.html</anchorfile>
      <anchor>a3347306376677277d94b22ea3770cac6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_auth.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__auth_8c</filename>
    <includes id="tnet__auth_8h" name="tnet_auth.h" local="yes" imported="no">tnet_auth.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_auth.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__auth_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_endianness.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__endianness_8c</filename>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">tnet_endianness.h</includes>
    <includes id="tnet_8h" name="tnet.h" local="yes" imported="no">tnet.h</includes>
    <member kind="function">
      <type>unsigned short</type>
      <name>tnet_htons</name>
      <anchorfile>tnet__endianness_8c.html</anchorfile>
      <anchor>a81f024d1f14e062410246f86802d12b3</anchor>
      <arglist>(unsigned short x)</arglist>
    </member>
    <member kind="function">
      <type>unsigned short</type>
      <name>tnet_htons_2</name>
      <anchorfile>tnet__endianness_8c.html</anchorfile>
      <anchor>a275f5dd051fd16045c4e893c32bac06c</anchor>
      <arglist>(const void *px)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>tnet_htonl</name>
      <anchorfile>tnet__endianness_8c.html</anchorfile>
      <anchor>a9c62f72b9d9f37546da167712dd3d706</anchor>
      <arglist>(unsigned long x)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>tnet_htonl_2</name>
      <anchorfile>tnet__endianness_8c.html</anchorfile>
      <anchor>a8f8cd820e624e51cce6a5c4f801a6f46</anchor>
      <arglist>(const void *px)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_is_BE</name>
      <anchorfile>tnet__endianness_8c.html</anchorfile>
      <anchor>a8e88a1ef1bc84108223787779f504a41</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>tnet_isBigEndian</name>
      <anchorfile>tnet__endianness_8c.html</anchorfile>
      <anchor>acd8f51d06c3a3d408e2b52428034cc24</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_endianness.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__endianness_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>tnet_ntohs</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>a173b0ead154f408f9431cb89f167db7d</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_ntohs_2</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>a75320a16b5817e68c17bb33f4e10c811</anchor>
      <arglist>(px)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_ntohl</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>a3421e42a8d227dfd846bccb37f0dc490</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_ntohl_2</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>ae1122c736d013aa5cd562e6b9f2a2096</anchor>
      <arglist>(px)</arglist>
    </member>
    <member kind="function">
      <type>TNET_BEGIN_DECLS TINYNET_API TNET_INLINE unsigned short</type>
      <name>tnet_htons</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>a9128b3b5b1eb078b0ae063c279cf6614</anchor>
      <arglist>(unsigned short x)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API TNET_INLINE unsigned short</type>
      <name>tnet_htons_2</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>a76d99c559f54d176c76e1d0b7cec11d1</anchor>
      <arglist>(const void *px)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API TNET_INLINE unsigned long</type>
      <name>tnet_htonl</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>ae3b9343ade3115c02db59eaacf9c07a3</anchor>
      <arglist>(unsigned long x)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API TNET_INLINE unsigned long</type>
      <name>tnet_htonl_2</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>aaa4d43dde222ab2d0b7191b071073f2c</anchor>
      <arglist>(const void *px)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API TNET_INLINE tsk_bool_t</type>
      <name>tnet_is_BE</name>
      <anchorfile>tnet__endianness_8h.html</anchorfile>
      <anchor>a1c7a87c0f35e81028d0f636b398e43d9</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_hardwares.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__hardwares_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS enum tnet_hardware_type_e</type>
      <name>tnet_hardware_type_t</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>ab3965c39fd276c764c0e59d2a1bb010a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_hardware_type_e</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Ethernet_10Mb</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cbab973f191210bbc4e5c4c2fe741d81b78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Ethernet_3Mb</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cbac897b3c01ee006b71c41fba02c145056</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_AX_25</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba6886c8486caa3f75207f3a1c17b6fa24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Token_Ring</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba2f09deb1fad80172d241c4abbddc81cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Chaos</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba6bada0b3a49c4d4eb9407ade58dc340e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_IEEE_802_Networks</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba30df3e42f825f3ea51d8f6dbdf015649</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_ARCNET</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba6fe010a6818866fa788b3a77d559c61a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Hyperchannel</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba1e8e9de9afe46777392cd2df3770bdc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Lanstar</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cbae739a32782957a1d35cac1fe0baae7ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Autonet_Short_Address</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba9112021ec25e617eadbb93a764765563</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_ALocalTalk</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba4cd5f244ef725671f9dff6bedf451765</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_LocalNet</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba37870cb45352e7de380499f0b4b42c22</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Ultra_link</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba0940480918527435321bdd93a46557a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_SMDS</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba73dc9b1bdd675ecf6f3933383fa8d551</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_Frame_Relay</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cba4acacd1d8fadbdb927193083052d5e0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_htype_ATM</name>
      <anchorfile>tnet__hardwares_8h.html</anchorfile>
      <anchor>a9707b4f020ac44c0ccd61ec6636a91cbaaca0d9ed7748a27d0f4dd7c05b9b28e8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_nat.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__nat_8c</filename>
    <includes id="tnet__nat_8h" name="tnet_nat.h" local="yes" imported="no">tnet_nat.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_nat_context_handle_t *</type>
      <name>tnet_nat_context_create</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga49c2521bd48d6df2ce3a46e066ebe874</anchor>
      <arglist>(tnet_socket_type_t socket_type, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>__pred_find_turn_allocation</name>
      <anchorfile>tnet__nat_8c.html</anchorfile>
      <anchor>ace6d31bc9dea175a5262eac6fcff6780</anchor>
      <arglist>(const tsk_list_item_t *item, const void *id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>__pred_find_stun_binding</name>
      <anchorfile>tnet__nat_8c.html</anchorfile>
      <anchor>a8c276667804d2cf335a29d74008a46e2</anchor>
      <arglist>(const tsk_list_item_t *item, const void *id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>__pred_find_turn_channel_binding</name>
      <anchorfile>tnet__nat_8c.html</anchorfile>
      <anchor>ae3753168b39e7bc9f746ba229c7d277e</anchor>
      <arglist>(const tsk_list_item_t *item, const void *id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_address_tostring</name>
      <anchorfile>tnet__nat_8c.html</anchorfile>
      <anchor>aae3c07155263489e90e7575060571a28</anchor>
      <arglist>(const uint8_t in_ip[16], tnet_stun_addr_family_t family, char **out_ip)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_set_server_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga60f64045a1a894a41afdb73a332f8732</anchor>
      <arglist>(tnet_nat_context_handle_t *self, const char *server_address)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_set_server</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gadb5209238b5596dac88606e5130dc93f</anchor>
      <arglist>(tnet_nat_context_handle_t *self, const char *server_address, tnet_port_t server_port)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_binding_id_t</type>
      <name>tnet_nat_stun_bind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gaedaee292e59564e952b1ddda3b82f0a6</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, const tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_stun_get_reflexive_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga806a269daee5625262abe17c730d6bfc</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_stun_binding_id_t id, char **ipaddress, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_stun_unbind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gac7501503711b402a1863967000970e16</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_stun_binding_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_id_t</type>
      <name>tnet_nat_turn_allocate</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga0388eae3c1cb2390070e49845a9e7a33</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, const tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_get_reflexive_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4417d9c60fcfae071ffee563b1e5f4dd</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, char **ipaddress, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_allocation_refresh</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga6c0eb272fdaf34b696f40aa7f5279c24</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_unallocate</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga5ffc9393817f43bdd1331454a6f8af6e</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_id_t</type>
      <name>tnet_nat_turn_channel_bind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga2c1bed31feafaa25d30cf8cc92b0b9e5</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, struct sockaddr_storage *peer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_channel_refresh</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4995c26dc8e520f2820924082b1c5193</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_channel_binding_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_channel_send</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga69719fb972fa3ccf667dd3999c221e8c</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_channel_binding_id_t id, const void *data, tsk_size_t size, int indication)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_add_permission</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga67181e6f65d829d44089841fd72429f0</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, const char *ipaddress, uint32_t timeout)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_nat_context_def_t</name>
      <anchorfile>tnet__nat_8c.html</anchorfile>
      <anchor>a9129d9b49b76e3c71058b17443d1fd21</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_nat.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__nat_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">stun/tnet_stun.h</includes>
    <includes id="tnet__turn_8h" name="tnet_turn.h" local="yes" imported="no">turn/tnet_turn.h</includes>
    <includes id="tnet__proto_8h" name="tnet_proto.h" local="yes" imported="no">tnet_proto.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <class kind="struct">tnet_nat_context_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_NAT_DEFAULT_RTO</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga18cf72a341f3c785ab00a746286db1e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_NAT_DEFAULT_RC</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga5e809d1461c049e1830e5d376a07bd11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_NAT_TCP_UDP_DEFAULT_PORT</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga0a871f1c40c7acb827f0513958ec8b21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_nat_turn_channel_sendindication</name>
      <anchorfile>tnet__nat_8h.html</anchorfile>
      <anchor>a90eee7a015088fda4281cb913d998f6f</anchor>
      <arglist>(context, channel_id, data, size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_nat_turn_channel_senddata</name>
      <anchorfile>tnet__nat_8h.html</anchorfile>
      <anchor>a66bbb47e80ab5c9ba3252d2dae1dbc06</anchor>
      <arglist>(context, channel_id, data, size)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_nat_context_s</type>
      <name>tnet_nat_context_t</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4d037b7a4416b7e876163d3c05256a1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>tnet_nat_context_handle_t</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga82ef754b59414ada0129fa558e9f3ec2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_set_server_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga60f64045a1a894a41afdb73a332f8732</anchor>
      <arglist>(tnet_nat_context_handle_t *self, const char *server_address)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_set_server</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gadb5209238b5596dac88606e5130dc93f</anchor>
      <arglist>(tnet_nat_context_handle_t *self, const char *server_address, tnet_port_t server_port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_stun_binding_id_t</type>
      <name>tnet_nat_stun_bind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gaedaee292e59564e952b1ddda3b82f0a6</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, const tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_stun_get_reflexive_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga806a269daee5625262abe17c730d6bfc</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_stun_binding_id_t id, char **ipaddress, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_stun_unbind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gac7501503711b402a1863967000970e16</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_stun_binding_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_turn_allocation_id_t</type>
      <name>tnet_nat_turn_allocate</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga0388eae3c1cb2390070e49845a9e7a33</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, const tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_turn_get_reflexive_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4417d9c60fcfae071ffee563b1e5f4dd</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, char **ipaddress, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_turn_allocation_refresh</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga6c0eb272fdaf34b696f40aa7f5279c24</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_turn_unallocate</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga5ffc9393817f43bdd1331454a6f8af6e</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_turn_channel_binding_id_t</type>
      <name>tnet_nat_turn_channel_bind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga2c1bed31feafaa25d30cf8cc92b0b9e5</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, struct sockaddr_storage *peer)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_turn_channel_refresh</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4995c26dc8e520f2820924082b1c5193</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_channel_binding_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_turn_channel_send</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga69719fb972fa3ccf667dd3999c221e8c</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_channel_binding_id_t id, const void *data, tsk_size_t size, int indication)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_nat_turn_add_permission</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga67181e6f65d829d44089841fd72429f0</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, const char *ipaddress, uint32_t timeout)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_nat_context_handle_t *</type>
      <name>tnet_nat_context_create</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga49c2521bd48d6df2ce3a46e066ebe874</anchor>
      <arglist>(tnet_socket_type_t socket_type, const char *username, const char *password)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_nat_context_def_t</name>
      <anchorfile>tnet__nat_8h.html</anchorfile>
      <anchor>a6efcbbf185da7cd50b10cc78f223e46a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_poll.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__poll_8c</filename>
    <includes id="tnet__poll_8h" name="tnet_poll.h" local="yes" imported="no">tnet_poll.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_poll.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__poll_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_proto.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__proto_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="typedef">
      <type>enum tnet_proto_e</type>
      <name>tnet_proto_t</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a27bb5837a79968f327ad1a267a8c889c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_proto_e</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_HOPOPT</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf3e47a1629582de15246a7167a0bdd16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ICMP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf865505c8426585ea585708127320a83</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IGMP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea26433dfcf3cded8b37cf14e85cc3f7ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_GGP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaace31a772721b89e781980decbd241df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae52d5e72b84fb84fa5eaa10540d793d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ST</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeabb3aad067064efaca8edfafca3dc5313</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TCP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea9435391a4b92958ff1b572fda6e5de06</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CBT</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea50c34cc36ebac6575280c91a53f55e56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_EGP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeacaba03f5ed57c57010aa4f7ce160e028</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IGP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaefec4433c2ebe190a48a58eb86370522</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_BBN_RCC_MON</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea4e9f8d8e68b675dccc06b04d6d5dd6f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_NVP_II</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaa3c0b656773e9e04e8bc86dcdd1420a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PUP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea4fb43aa371a2864f156e4cce1b6e388f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ARGUS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea4f8301edc15722f5262a086bf98184ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_EMCON</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea39ab7c2ef09951ca1c1f0c3c67524f3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_XNET</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea054eba5ab5a811bbd20049468a61ec9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CHAOS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeabda0e6c71bb52cb79a58b9d158ed4f5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_UDP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab30520cf1f872f6295ad002ebbdeada2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MUX</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaaa50ec2e5cb8efcae5d4280e852f440e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_DCN_MEAS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeabd9ac558d333c6f5e100980dd61a4f1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_HMP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea04aa90429252022197d71301737d9e91</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PRM</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aead370323784a60ce792f6302ec283b3e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_XNS_IDP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea6395e15ee84f3267e20862601af11c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TRUNK_1</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea20a62b2324ba93e34219549ccf6616cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TRUNK_2</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaee772df2109724660e22843029a78001</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_LEAF_1</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaba4591261fca995da12939a87dca2f8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_LEAF_2</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab46c29f0e2e062d3fe07daf39273bd34</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_RDP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea517b71e70817cc73e07779730b2f7165</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IRTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae13acc04aa32d48d6d5e223e8850b2ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ISO_TP4</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea93464153b54e36073d5800322ec2f0a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_NETBLT</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea17bc22f36029bd02a60768c4d617f017</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MFE_NSP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea012f95544ab4b0e9abb0aab5727ec85e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MERIT_INP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea70b4c6a63c4f2e5165df28537a87e35a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_DCCP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaa833da1668e07e6cd23c58e5a4453cb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_3PC</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea64cfc68e827206b5aa16a2044a635ef6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IDPR</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea0f404ea94e7869b1c291cf691a5142ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_XTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea77985b0994cdd86feb7f237229ec2510</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_DDP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea51f5684c0ecd0df92cea76008334042b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IDPR_CMTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea8b5687e2751a055d7bb1211c35a41f16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TP_PP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea47843d626abcb8aa8d8ae64b2a85eb2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IL</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea9693931b5c7c19c1a4a9536b6cd0c927</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPv6</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea1e3279cb942513c78e96d07611e1c807</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SDRP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea15392913ac5f2dd4449991c9979d7dfd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPv6_Route</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf61f8f71a9115ac879452ba0882d1233</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPv6_Frag</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaa1d78d263cd7197a91160b15bb6a26b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IDRP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea1400b8a16f2ac5e35aa201a1deeaf35f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_RSVP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea93fc82a4b0f05472fdf72bcbb3a78039</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_GRE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab02932b56775158f9defaebbb6d67cdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_DSR</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea81a55e871f9b9eae24cbc113b2f97494</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_BNA</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab394f5c29aa46f830e2183589ffe5099</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ESP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea29a02ced688139bff0bef253bb461ee1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_AH</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea9059e24e96769fcf1a382d8abe3d256e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_I_NLSP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea0c94154394b65fd6d5d3038e5936700e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SWIPE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea570fafde2ba504c828e5e1ea29fc0e29</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_NARP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea8bf22c4615143141a45f59c76b1f47ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MOBILE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf0ccd8ed530ff2ff6d4d71cf5d317565</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TLSP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea8fb5426051f59e24b779b3fc7698ddf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SKIP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea225f1051b106ed58e22addff8d499376</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPv6_ICMP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea62eff837f8816a052c697ab4e41ecc97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPv6_NoNxt</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea6085956fd26c4a20ce79363979848d8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPv6_Opts</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea95b1f2a54b04b3a179772ba3130db77e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ANY_INTERNAL</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea2859d72882404810011b78d7012e061b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CFTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea00950076d09f14d08d0f0858c26361e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ANY_LOCAL</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaaed167548d4677aff1c8597b2fe38355</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SAT_EXPAK</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea29981fe207506ea4d111ccc2cd1b706b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_KRYPTOLAN</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea55b992f416f6568a7491c2dc92003a02</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_RVD</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf7b535cc4f183a300ef3585fd0c7709a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPPC</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea7f5a3ca6ee5590c96204b3c760e0aab7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ANY_DISTRIBUTED</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeac335cdea345a239d64afc7ea15df455b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SAT_MON</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeadfa9bdbf063ec12290969b909bbbae7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_VISA</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeadc121b25976e2b89f687eeaaa7198926</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPCV</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea7ca1714ed0a1befdea1322004750834a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CPNX</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea21702fc04028001091718e7ec3f68049</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CPHB</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aead30a6979e388107238e864df64d5fa52</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_WSN</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeacfa6a2dcb25628b1eb837db3f198384c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PVP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae3a08bedc4bfc8504f9b6c69c16640de</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_BR_SAT_MON</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aead4217c62a62f2db7ba92b397c090083e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SUN_ND</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea0906af73c851e2328283a7bb7cf2db51</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_WB_MON</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea2f96458c8c0785a44c3755a2eb38a24a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_WB_EXPAK</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea18d3794110ef7a0d3a2872c32170bac3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ISO_IP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea896824b5ad131da8acb7524b4d3f1e00</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_VMTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea89f9c06f2e5b1a8e27e2df1f3c678e9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SECURE_VMTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea047328a5839ab985585203106f27a555</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_VINES</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea020bc53b165ce3d2fbf6ed68d5f7de16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea545a0d51c04bde9d068d6bc847af2446</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_NSFNET_IGP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea3159d9719df7c3c1957cd41318ad3364</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_DGP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaac8dd71e6b5d9ecaaf6e3d4cfcd2bded</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_TCF</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaad09c0dc637474467e948e826b3491c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_EIGRP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea5c35e9e935b96097e9509b307b7c90c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_OSPFIGP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea01586d5552d8819526e76c9e8432e7bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_Sprite_RPC</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae3294f17f28d481cb31855f51e1ec5ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_LARP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea258f4746d9685bc2fb3b9f0220f3f598</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae94ca21d099378d112215a30bac5ea31</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_AX25</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae94173efea22c7d9f7cf5794bda60aa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPIP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaa16dd3a301dcbc54559e455faa214812</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MICP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaa0922213e13e21bdc6fc47e3bb8cb4b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SCC_SP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaddac121f42d652639b9f1a4ab6334063</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ETHERIP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea4a7ee55e10ffa2889ec864061dfd9b9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ENCAP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab986fd0f9979b7ac8e455cbdc9a04f1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ANY_PRIV_ENC</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae6f6341420590c4c0040bbf83431ea17</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_GMTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea75d6d6bc3194fc7c7f599bec6c8bed2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IFMP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeae8deddd55ef97c55b1707eb27120eb6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PNNI</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea4e650d53603aa759da5fb46f782c7e56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PIM</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaec3d01e4d23894337b910be097850f23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ARIS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeac43b97e29a7379567ee6251c769bd8e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SCPS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeafb9e0fa6695a4f06fd7301d447626e3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_QNX</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaa2e009b3e9fdcd0df179541b4759dc78</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_AN</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaaee72de21586a71ec9c1de6d26103d1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPComp</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea005d08c204eb3c9ce4fda7c5fc4878e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SNP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea8a22527178dadb5364f7db341ffeabdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_Compaq_Peer</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab461495ddf6658ccc480ebeb206164b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPX_in_IP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea97059663628127b1738ab943f2a9a328</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_VRRP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea438c7667da58dc528ebca5585434395f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PGM</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea9b0f254b4d98d7400ccca0e118e5fc37</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ANY_ZERO_HOP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea7deb4eda317e225c0e27d77a3567f74e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_L2TP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea58bd0e0146b11ee58c6d32174222cf71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_DDX</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeabe680f2fc13b17024c29652b1518ebb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IATP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf87e18441522469fb469e52e1d61906e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_STP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea2ab5fe4b262d01b655f3c925836e15a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SRP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea06e1214849c2b9dc1cfff07e56e925e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_UTI</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea6877ea94d0755a7204ff222e19c79e95</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SMP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaecc99400bff657c2154f4594f37facb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SM</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea2487fe9fb7cd4bdaa17b2c657ba15018</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab4f0e82e3b661d2de0c4d78eb03c5bc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_ISIS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea1a55c6e1112e448829b94b4091c71e7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_FIRE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea38e6407861f800399ec1a34ec0fde2a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CRTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea838510d7c0e658ee495c6f0dfbafbb91</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_CRUDP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab56bfcfdcf725c415669866200591241</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SSCOPMCE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea8fe3318bbe97b7715d8ff65d83a5749e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_IPLT</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeafc93dfcadfa47d380c5d9a392dca117f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SPS</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeac588ec81d589776abbcf05424414f920</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_PIPE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeab3c05bc815d37d488645a9a605e883b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_SCTP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea02944136413e06785c6cbd44f52359e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_FC</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaaafd42469743174f7fb479a473a3a035</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_RSVP_E2E_IGNORE</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaf96abbab80993204b88377ca806d4235</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_Mobility_Header</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeabc2a94748b995a2c3068d534f65d604a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_UDPLite</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea0f2f4b0c97c3919d1c1db0742ae80af4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_MPLS_in_IP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea42147d339151a3053ddcb842e92fa187</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_manet</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea7d7940d2179419a2dfefe6c2242032a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_HIP</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea37643c9ab3ed8de3edd86dc9e05e562a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_Shim6</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea5d9aea6e62dbac43c596bc996dfc025a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_EXP1</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea9c841d3e3f7f40a12b5b6851e87dd61b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_EXP2</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aea0f8c7369c55e0c77d35a9de71a4a21a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_PROTO_Reserved</name>
      <anchorfile>tnet__proto_8h.html</anchorfile>
      <anchor>a0a5a9fc22fd159c4a6c3ba14951133aeaed0bace331ff7d5ce6a0fcffa58cfd2d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_socket.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__socket_8c</filename>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <member kind="function">
      <type>tnet_socket_t *</type>
      <name>tnet_socket_create_2</name>
      <anchorfile>group__tnet__socket__group.html</anchorfile>
      <anchor>gaa4d96c3d0bac908bd2d3413fbd19d30d</anchor>
      <arglist>(const char *host, tnet_port_t port_, tnet_socket_type_t type, tsk_bool_t nonblocking, tsk_bool_t bindsocket)</arglist>
    </member>
    <member kind="function">
      <type>tnet_socket_t *</type>
      <name>tnet_socket_create</name>
      <anchorfile>group__tnet__socket__group.html</anchorfile>
      <anchor>ga44cfc3407bc375c5b932bfe45a9de849</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_socket_def_t</name>
      <anchorfile>tnet__socket_8c.html</anchorfile>
      <anchor>a0b84352684cbba68fd098ba5801da787</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_socket.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__socket_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <includes id="tnet__tls_8h" name="tnet_tls.h" local="yes" imported="no">tls/tnet_tls.h</includes>
    <includes id="tnet__dtls_8h" name="tnet_dtls.h" local="yes" imported="no">tls/tnet_dtls.h</includes>
    <class kind="struct">tnet_socket_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IPV4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a74e1b90dfbc2a5eb72f86e20c3391b59</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_UDP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a4a39d9f77a2c0f974f3a5ebb8a80992d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_DTLS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>abce7f796c5fca03a606ce218950650d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_TCP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aa95c678d99efa12a7a5400f8b82ddad5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_TLS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aca18b3796e1a0244078292db73a5c20e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SCTP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a48fcfca9ecec785918601fd863ba10cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_WS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a542569e83b7300b1f231843e1c796d05</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_WSS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a2ec694b5b9296070f6cfac1ab5344f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IPSEC</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a63282a2cae8654c1d2882662580d05ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IPV6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aa41f8ec0c07e2ee94feb2b6be80fca2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IPV46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aed444b967bd2b8f0bd89b4b3c80d8762</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_IS_VALID</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aaea09b2a77f6f12d48823836a8f44e3c</anchor>
      <arglist>(socket)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_VALID</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>abb72c89f5e4637106faf36e5dfa4c004</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_STREAM</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a524680417e129af2019545bef449225f</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_DGRAM</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a33975122769a5a67cbeccaee9eb9a97b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_IPV4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a8eeb8682e7f29f4eda0b53e4a2d23b73</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_IPV6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a0b6df4149ae8f6b96974152b910cf518</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_IPV46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a1c7630358e600dfea260ef4de88a04cd</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_IPSEC</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a691d1054b9ba60c8acfd939ee1ac30e0</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_UDP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>afed2197fb95bdd5b24a2241717b9785a</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_DTLS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a469601122019599b23d3abeba5b7aa94</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_TCP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aeb0cd6fa3f1666aa122242e90d2a8983</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_TLS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ae1ae1b3c50657b6202cfc962085db773</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_SCTP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a502f2a14d70745355c989c7d171ccc6d</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_WS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ac377ded5b93456ca31d670c9c6dbf5b9</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_WSS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>af4e55c1c5ec20c90ee19c4ac14cbd30d</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_IS_SECURE</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ae71d33e0a1a343f2cf4d2e54f3370730</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_UNSET</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab4ef32a80f5d86de4033f2b0467cf848</anchor>
      <arglist>(type, OP)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_IPV4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a97be443ebd7a06b835cd545eed66d3dc</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_IPV4Only</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aba4aad26261c9807d70f6fb2020d684a</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_IPV6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a53363b809815b0fdbcc8b2b3e52d08b0</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_IPV6Only</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a1b5a72e73a32f688d1c22a3b6ce20804</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_IPSEC</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aadb0b3ed0d2be77ea464ddcffba4b36d</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_UDP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a2db025967772ce44fb5844b9e70b02ea</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_DTLS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a78be84fa580e6b474cf3d060b050e91f</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_TCP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a901c93fdc876a6787bf71867318e3f80</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_TLS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a69a1c4e47a7412f8739d922dd294c1d6</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_SCTP</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a2520e87cbac118e7de1dc8062f534271</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_WS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a8c4a5116f4ac9a8c3165233573aeb67b</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_TYPE_SET_WSS</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a21f939504f60fab74e3059a324c72fa0</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_HOST_ANY</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>aa6e6dcfb2a211eed85fe668cfc0c3632</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_SOCKET_PORT_ANY</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ad247279879162ea5d5484ce8e9b244fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS enum tnet_socket_type_e</type>
      <name>tnet_socket_type_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a576cd9bf9d800834987f993c8f6c17e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_socket_s</type>
      <name>tnet_socket_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a25420c64ae82e009d5e593315c02a8a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_socket_t</type>
      <name>tnet_socket_udp_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a4b3efb6a5a35e78092888202401e1c5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_socket_t</type>
      <name>tnet_socket_tcp_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a23360f55c1880d6abea27966a299ee96</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_socket_t</type>
      <name>tnet_socket_tls_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a84d882faf5cb0e466532c54b05c36036</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_socket_t</type>
      <name>tnet_socket_ws_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a9a1ca8a86fbfc3bf4dceb177506358c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_socket_t</type>
      <name>tnet_socket_wss_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a3d437e9d3ee3d89d3587575816786f60</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tnet_socket_t</type>
      <name>tnet_socket_ipsec_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a02562d642430b5eee0000d90e96af005</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_sockets_L_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ac361e0e716e059ae9ef1f51d01e817a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_socket_type_e</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_invalid</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba4867e88cadabdb296e11da6cd8374e81</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_udp_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba9878c017dac96d17dfb8ff6f90f7f119</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_dtls_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbad478122de9e6c0d8b5218c5c21cda411</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tcp_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba040c10bb67bd4ac2eaf26fb3548484bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tls_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba06c82b0a6be847347a4f7bb7a4627df6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_sctp_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba27609c62adde9edffb8a8c46d2df5f87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_ws_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba8c0daa338550cf5701428c1516cf5439</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_wss_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbac8516012b70312ab8d7c341e7e4deb64</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_udp_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbacf582a552aa9901a7863f6d2e1e156fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_dtls_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbaf0f8dd6e0fab9c10f61021035f056f0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tcp_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbad65dcbd7cf453cfde4d4e855c912733e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tls_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba578b670c64d83c907a4fca82b0bb308c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_sctp_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba7cc7206c5c80e58a1d29b8fac81d589c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_ws_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba751d905a4a5a4631b09f506859ccf918</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_wss_ipsec_ipv4</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba935a0985c07a4c49317ef88d80e78a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_udp_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba12b789591e7ded6685e3809a2d63133b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_dtls_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba90a963ee591fb48b0f520bab62a85c23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tcp_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba6101c967b4d3ce42af97d9fc25334131</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tls_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba1d6b4fdddc80aa1b12e735e3235c939f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_sctp_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbaf8ce34f17339807de6fba014675c2cea</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_ws_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba0121d29620c9bea409c293d1f44a53f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_wss_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba6b4f8a51b95f79db7b998038d0b17a47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_udp_ipsec_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba2af7612a9b8b9de23693a56f63fc3294</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tcp_ipsec_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba3fba0645a3ff38ace6a902c634c0735e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tls_ipsec_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbadd2b59ff43c54857d0879ba551dc4d72</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_sctp_ipsec_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbaee18824f664be3a782685568110ebd7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_ws_ipsec_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba0528c055b394dfb54e98fb89b92bfeca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_wss_ipsec_ipv6</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba75db9cfd6290a9b3825e4869a10e4207</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_udp_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba63e7d80e87188729eab2445346144e73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_dtls_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba050969bd5ae7693d610006dfe3cdc8f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tcp_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba229d0cef3abe327bbef97e8ba800bc6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tls_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbaef5605551def96072ef6a422f93f4d66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_sctp_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba21d71335634ff4fe2ec328560ee6f608</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_ws_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba738eb76803f30691a46e91c62713302e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_wss_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba9d0241d8fc72284aaf570fcc9828ffeb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_udp_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbabca42b8ab7ee8f53bd5e7142eb683d38</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_dtls_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbaf5d5d8170cd35b0b32c74d818e85233d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tcp_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba9b1ab58092d574e6754634b1e673c62e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_tls_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbabeef19aa4eb525b42ac2ef840239205e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_sctp_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba4d0c11246cdd62f0271b15e9ecf85e3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_ws_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bba798031e7b87799cb4502d865950bc1c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_socket_type_wss_ipsec_ipv46</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>ab37bfdb5ac374384872091d48e3895bbad3d316ccc36b63c9cd55da308846c9b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_socket_t *</type>
      <name>tnet_socket_create_2</name>
      <anchorfile>group__tnet__socket__group.html</anchorfile>
      <anchor>gaa4d96c3d0bac908bd2d3413fbd19d30d</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, tsk_bool_t nonblocking, tsk_bool_t bindsocket)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_socket_t *</type>
      <name>tnet_socket_create</name>
      <anchorfile>group__tnet__socket__group.html</anchorfile>
      <anchor>ga44cfc3407bc375c5b932bfe45a9de849</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_socket_def_t</name>
      <anchorfile>tnet__socket_8h.html</anchorfile>
      <anchor>a1c58ffbcdb15d377d4075335ea8001a3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_transport.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__transport_8c</filename>
    <includes id="tnet__transport_8h" name="tnet_transport.h" local="yes" imported="no">tnet_transport.h</includes>
    <includes id="tnet__tls_8h" name="tnet_tls.h" local="yes" imported="no">tls/tnet_tls.h</includes>
    <includes id="tnet__dtls_8h" name="tnet_dtls.h" local="yes" imported="no">tls/tnet_dtls.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TNET_CIPHER_LIST</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a0272165dcd11efdaf43bac2d288a9a0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_prepare</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>aafdc2431c02385e300c520ca985ed55b</anchor>
      <arglist>(tnet_transport_t *transport)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_unprepare</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a9763a05c45da16d31e3c99a79fdf1761</anchor>
      <arglist>(tnet_transport_t *transport)</arglist>
    </member>
    <member kind="function">
      <type>void *TSK_STDCALL</type>
      <name>tnet_transport_mainthread</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a7fb95b1359ad08465766812b225cb579</anchor>
      <arglist>(void *param)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_stop</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a4dc920abb12866b5154692fa31885d99</anchor>
      <arglist>(tnet_transport_t *transport)</arglist>
    </member>
    <member kind="function">
      <type>tnet_transport_t *</type>
      <name>tnet_transport_create</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a395c18a684a068680a8a1d33fe2e05b7</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, const char *description)</arglist>
    </member>
    <member kind="function">
      <type>tnet_transport_t *</type>
      <name>tnet_transport_create_2</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>aab38b03dc47731a7b0ccdae1937f3f46</anchor>
      <arglist>(tnet_socket_t *master, const char *description)</arglist>
    </member>
    <member kind="function">
      <type>tnet_transport_event_t *</type>
      <name>tnet_transport_event_create</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a14597eb136e537f1d4d4e5ce3b884b63</anchor>
      <arglist>(tnet_transport_event_type_t type, const void *callback_data, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_tls_set_certs</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a894866de936b19458529ba4ca96bfaad</anchor>
      <arglist>(tnet_transport_handle_t *handle, const char *ca, const char *pbk, const char *pvk, tsk_bool_t verify)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_start</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a505614c61dd978a668d82b827727e086</anchor>
      <arglist>(tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_issecure</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a9b7747658fa19bf00991b51e300f38aa</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_transport_get_description</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a47a6418d80182973b2a1d590a3121bca</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_get_ip_n_port</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a47145a97b9d61dd54920479755addda6</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_get_ip_n_port_2</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>aaaabe13c957df654256b2e9ea31e9b10</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_set_natt_ctx</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a5e2f170d3cdb1d161a7be4225b9d2c2e</anchor>
      <arglist>(tnet_transport_handle_t *handle, tnet_nat_context_handle_t *natt_ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_get_public_ip_n_port</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>adaa653753ccbf3a5c9bfb0ea26cfb985</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_transport_dtls_get_local_fingerprint</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>ade804a4fc6cb8143eeaad0fd1f62c91f</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_dtls_hash_type_t hash)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_dtls_use_srtp</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>aa0647587ca32a91c1238351b0a423f36</anchor>
      <arglist>(tnet_transport_handle_t *handle, const char *srtp_profiles, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_dtls_set_remote_fingerprint</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a4cc2b127f08b567b3f1b47b33d27d8b9</anchor>
      <arglist>(tnet_transport_handle_t *handle, const tnet_fingerprint_t *fingerprint, tnet_dtls_hash_type_t hash, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_transport_dtls_is_enabled</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>aebb3e8611b3dec54cd65c2b41f9ea92d</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_dtls_set_enabled</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>acebacaf2c4236d879be5d3d94ef4a67b</anchor>
      <arglist>(tnet_transport_handle_t *handle, tsk_bool_t enabled, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_dtls_set_setup</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>acf4762d81cfd8832a80672f1d216e2ed</anchor>
      <arglist>(tnet_transport_handle_t *handle, tnet_dtls_setup_t setup, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_dtls_do_handshake</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a7e7cbd1d1d1b4c8cdaa7d081141259a6</anchor>
      <arglist>(tnet_transport_handle_t *handle, struct tnet_socket_s **sockets, tsk_size_t sockets_count, const struct sockaddr_storage **remote_addrs, tsk_size_t remote_addrs_count)</arglist>
    </member>
    <member kind="function">
      <type>tnet_socket_type_t</type>
      <name>tnet_transport_get_type</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>ab5732e5c7845d6cb39cdda33bc347dd3</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tnet_fd_t</type>
      <name>tnet_transport_get_master_fd</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>aa745da0b969e4f7fe05884929b331fb8</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tnet_fd_t</type>
      <name>tnet_transport_connectto</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a4199eaa8e910079521330730e8faefb4</anchor>
      <arglist>(const tnet_transport_handle_t *handle, const char *host, tnet_port_t port, tnet_socket_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_set_callback</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>adebb516df3a574bf4192ceea1060febe</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_transport_cb_f callback, const void *callback_data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_transport_shutdown</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a89e04cc29fbf6f856055e092c54cd9ea</anchor>
      <arglist>(tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_transport_def_t</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>add6f857e2ce9fa399da32f65d9c6ed42</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_transport_event_def_t</name>
      <anchorfile>tnet__transport_8c.html</anchorfile>
      <anchor>a537e63cab92a28745e37c389813a5812</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_transport.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__transport_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <includes id="tnet__nat_8h" name="tnet_nat.h" local="yes" imported="no">tnet_nat.h</includes>
    <class kind="struct">tnet_transport_event_s</class>
    <class kind="struct">tnet_transport_s</class>
    <member kind="define">
      <type>#define</type>
      <name>DGRAM_MAX_SIZE</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a303845dce0d691a1fd531da3a49b815d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STREAM_MAX_SIZE</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a0022d825df5d3de6c3a1bd74f76030ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TRANSPORT_CB_F</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>ad21c17552c1931c2326fa1febacdd2af</anchor>
      <arglist>(callback)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_transport_connectto_2</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aed1a2d620b05906167e9f59f20d249fd</anchor>
      <arglist>(handle, host, port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_transport_dtls_set_certs</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>ad735ab3e4ee5f9760226bd5e024fd2d6</anchor>
      <arglist>(self, ca, pbk, pvk, verify)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_transport_dtls_srtp_set_certs</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a8bd4050682a0194cf1f9cd58cf4e63d0</anchor>
      <arglist>(self, ca, pbk, pvk, verify)</arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>tnet_transport_handle_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a5c5148c35787a35dabdc83e71b6337c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_transport_event_type_e</type>
      <name>tnet_transport_event_type_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>acdbc1d61b4bdd46f3d5cb8db8e077397</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_transport_event_s</type>
      <name>tnet_transport_event_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a093af931f4723f331238a2be0a8f5d71</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>tnet_transport_cb_f</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>af1e810a07af093bdd8b84528a906cef2</anchor>
      <arglist>)(const tnet_transport_event_t *e)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_transport_s</type>
      <name>tnet_transport_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>ab8f9d63467d6c16f5d9e8400705f46f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_transport_event_type_e</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_data</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba8f94c6e260626ca15997e0384a6a34f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_closed</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5baabebea7f92e558c8e09fe1f79b434c1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_error</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5babf46e68136f9cb4d34130283a775a202</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_removed</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba0cd9c3fadb25a5aa52a867c05798982b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_connected</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba56ba0a0c0f79ca66025f8a32c28a9b44</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_accepted</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5bad931ba02a9181775756c4e40d9e540bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_handshake_started</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba6bd61302adcb28659548f1855d6a7266</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_handshake_succeed</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba2a0f8e9915fa839e00dfcd35400ba7eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_handshake_failed</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba299a1d91f87662ebfc331568cc68208f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_fingerprint_mismatch</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba9a5daf572cd52c02dc603296b914d3d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_srtp_data</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba6ce922aa5ef586d6af2f6375bb0d7eb7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_srtp_profile_selected</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5baaa14922cdd57d1d3381630c443a39c2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>event_dtls_error</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abbf94f6cbb0048f5f737fb0fb850ab5ba7a5615d24a55510ce08dc51f25ee09df</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_tls_set_certs</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a13c127e8c4117a4b700ac5a491ea1a90</anchor>
      <arglist>(tnet_transport_handle_t *self, const char *ca, const char *pbk, const char *pvk, tsk_bool_t verify)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_start</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a2a387a33860dee5a55a31ee1ba3997cf</anchor>
      <arglist>(tnet_transport_handle_t *transport)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_issecure</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>ae297b8dcbd3f82f3e51335ef1c542659</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const char *</type>
      <name>tnet_transport_get_description</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aa1964324e317dcc38893c6ad482b4e6e</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_get_ip_n_port</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aa5a9608d24db9099a6511224fd3471c4</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_get_ip_n_port_2</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>ae6d44bddc3ef0e6d04f64eaaf1e222bd</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_set_natt_ctx</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a220be41b652653fbe3a0724f0091ea29</anchor>
      <arglist>(tnet_transport_handle_t *handle, tnet_nat_context_handle_t *natt_ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_get_public_ip_n_port</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a1c466c2c9934b0a1291c11a9e0dd423a</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_isconnected</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a38e55eab24973fbb9a44062e51d1df75</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_have_socket</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a2fe2a1b3010b9863f70be84209d85e8b</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const tnet_tls_socket_handle_t *</type>
      <name>tnet_transport_get_tlshandle</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a086def0ecfd39c41becf8ce271602477</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_add_socket</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a4ddc0942b34eb9127fea1c811627f78a</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd, tnet_socket_type_t type, tsk_bool_t take_ownership, tsk_bool_t isClient, tnet_tls_socket_handle_t *tlsHandle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_pause_socket</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a5d8f7a60ac7d836e8167195a62fa1151</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t fd, tsk_bool_t pause)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_remove_socket</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a0f4b9a66fce9e219c2d61cffeda27ab4</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_fd_t</type>
      <name>tnet_transport_connectto</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aa262527a36bd4651a46edfdbe837487b</anchor>
      <arglist>(const tnet_transport_handle_t *handle, const char *host, tnet_port_t port, tnet_socket_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_size_t</type>
      <name>tnet_transport_send</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aa50503da55477a13caffc15d75e0dbf3</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t from, const void *buf, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_size_t</type>
      <name>tnet_transport_sendto</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aa4a559c780cb647d80d3345e0598930a</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_fd_t from, const struct sockaddr *to, const void *buf, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_set_callback</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a30977582f872b66ea7a3f3175853b1cc</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_transport_cb_f callback, const void *callback_data)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API const char *</type>
      <name>tnet_transport_dtls_get_local_fingerprint</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a54db6ae9e13d34cbd08b5508a1a8d8aa</anchor>
      <arglist>(const tnet_transport_handle_t *handle, tnet_dtls_hash_type_t hash)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_dtls_use_srtp</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a7716418ff59ea68f572db3a059993db7</anchor>
      <arglist>(tnet_transport_handle_t *handle, const char *srtp_profiles, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_dtls_set_remote_fingerprint</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a6bea98bd13e69f696b5268e7f032504d</anchor>
      <arglist>(tnet_transport_handle_t *handle, const tnet_fingerprint_t *fingerprint, tnet_dtls_hash_type_t hash, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_bool_t</type>
      <name>tnet_transport_dtls_is_enabled</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>aaa6fca161f564e8586ba38b487cece20</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_dtls_set_enabled</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>ab4e5fe447eca34dbb555d96b4bfb1405</anchor>
      <arglist>(tnet_transport_handle_t *handle, tsk_bool_t enabled, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_dtls_set_setup</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>abb8f01a636827641262a8fcd9cb3d0bb</anchor>
      <arglist>(tnet_transport_handle_t *handle, tnet_dtls_setup_t setup, struct tnet_socket_s **sockets, tsk_size_t sockets_count)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_dtls_do_handshake</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a6478aee395b8a66c7ac15a43840c3682</anchor>
      <arglist>(tnet_transport_handle_t *handle, struct tnet_socket_s **sockets, tsk_size_t sockets_count, const struct sockaddr_storage **remote_addrs, tsk_size_t remote_addrs_count)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_socket_type_t</type>
      <name>tnet_transport_get_type</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a33ffcdc497c1550202731abaaf05b8ab</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_fd_t</type>
      <name>tnet_transport_get_master_fd</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a93691e2da74102b4814817aa997b3b67</anchor>
      <arglist>(const tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_transport_shutdown</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a79a3f3604ec1cb059c8b2d319d163e87</anchor>
      <arglist>(tnet_transport_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_object_t *</type>
      <name>tnet_transport_context_create</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a158a59ec9e717cbbab586e299e4cd866</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_transport_t *</type>
      <name>tnet_transport_create</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a7b5874cdbc14ee5107151244857bfbd1</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, const char *description)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_transport_t *</type>
      <name>tnet_transport_create_2</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a55fde6bcc30dcd6ab80e9449c2ec6d88</anchor>
      <arglist>(tnet_socket_t *master, const char *description)</arglist>
    </member>
    <member kind="function">
      <type>tnet_transport_event_t *</type>
      <name>tnet_transport_event_create</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a14597eb136e537f1d4d4e5ce3b884b63</anchor>
      <arglist>(tnet_transport_event_type_t type, const void *callback_data, tnet_fd_t fd)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_transport_def_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>a9828b22dcbec0f1c54ededd31fd39490</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_transport_event_def_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>afb173e3ef8b21f78740052af526e69f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_transport_context_def_t</name>
      <anchorfile>tnet__transport_8h.html</anchorfile>
      <anchor>acfd90b5ed268b13b24f377992954d065</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_transport_cfsocket.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__transport__cfsocket_8c</filename>
    <includes id="tnet__transport_8h" name="tnet_transport.h" local="yes" imported="no">tnet_transport.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_transport_poll.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__transport__poll_8c</filename>
    <includes id="tnet__transport_8h" name="tnet_transport.h" local="yes" imported="no">tnet_transport.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_transport_win32.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__transport__win32_8c</filename>
    <includes id="tnet__transport_8h" name="tnet_transport.h" local="yes" imported="no">tnet_transport.h</includes>
  </compound>
  <compound kind="file">
    <name>tnet_types.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__types_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TNET_FINGERPRINT_MAX</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>afe5781b6502203e6d121aa86a998c72f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_INVALID_SOCKET</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a6f5bbd2094794d14c754526899ed3555</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ERROR_WOULDBLOCK</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a6b07aaef86d309e376f2b84c8e47371d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ERROR_INPROGRESS</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a6ea526bb93648fb77f7c4c64663c9768</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ERROR_CONNRESET</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7c721d5339257e316df23f5d72da8061</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ERROR_INTR</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>ab678436a332c2e2da1c3036bd0fda68c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ERROR_ISCONN</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a2e226a2261679abd9369fe7df869953e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ERROR_EAGAIN</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>afd4439f649d80deea1da5ba4b9f407ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_gai_strerror</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>aabe506ad80551db8d5687e48afaa3d3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_INVALID_FD</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a78ffc6813a665347c56c3dbc2b59e084</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>tnet_fd_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>ad310050a573244ca7810e0b11c2d2363</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>tnet_port_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a3a4edd243b59f5b9e3b0a2bfaa74fcd0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>tnet_family_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7e6c5c9f3462512e8093438daf3f4d3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>char</type>
      <name>tnet_host_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a0e365398826bc67ee809d5d8619cff6f</anchor>
      <arglist>[NI_MAXHOST]</arglist>
    </member>
    <member kind="typedef">
      <type>char</type>
      <name>tnet_ip_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a317920febe452aa28432d0df4d448694</anchor>
      <arglist>[INET6_ADDRSTRLEN]</arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>tnet_fingerprint_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>ae170648503e4db4d27f097f59dc0bcd5</anchor>
      <arglist>[TNET_FINGERPRINT_MAX+1]</arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_interfaces_L_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a08fde7fadc3d2f2f5a654ebe5d51c5d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_addresses_L_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>af5b5e78f9d75c00cbff4aabee7d4e534</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dtls_setup_e</type>
      <name>tnet_dtls_setup_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a5ada4bc5b60674bf52c2fcd4cf354c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_dtls_hash_type_e</type>
      <name>tnet_dtls_hash_type_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a90e3b533ed21bcbf0c31113405c0bf14</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>char</type>
      <name>tnet_error_t</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a79e339c2bb6c0e841bce06c380c6d376</anchor>
      <arglist>[512]</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dtls_setup_e</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a2f346b4438e78d7096d08bcfd8ce51</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_setup_none</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a2f346b4438e78d7096d08bcfd8ce51ada8cd208467a4f2e36638b997bd52ec5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_setup_actpass</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a2f346b4438e78d7096d08bcfd8ce51a843e99bce81a983533499a2813684995</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_setup_active</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a2f346b4438e78d7096d08bcfd8ce51ae85331edf6e6d15edde3a8336eb52509</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_setup_passive</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a2f346b4438e78d7096d08bcfd8ce51ad7fdba2fdb1911557ad82dc4be40e57e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_DTLS_SETUP_MAX</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a7a2f346b4438e78d7096d08bcfd8ce51ae8e3e4805b16d3212c4c53daa8336cb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_dtls_hash_type_e</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_hash_type_none</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79a3b5f26eb71f64f135c4588d82d3ff798</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_hash_type_md5</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79abdd2c218483e6a22a00003f808691a63</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_hash_type_sha1</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79a70d72aada5320cab9d31e007039bd5e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_hash_type_sha256</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79a2408f43f4ebc54c26808dad5166bd4db</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tnet_dtls_hash_type_sha512</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79a00de2ff7b734266d98a2cd5501620619</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TNET_DTLS_HASH_TYPE_MAX</name>
      <anchorfile>tnet__types_8h.html</anchorfile>
      <anchor>a9a098624f10ecf3c9bdd16867d22ef79ae4fa7baa3b729d8600fb6d62868a9887</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_utils.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__utils_8c</filename>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">tnet_utils.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">tnet_endianness.h</includes>
    <includes id="tnet__dns__resolvconf_8h" name="tnet_dns_resolvconf.h" local="yes" imported="no">dns/tnet_dns_resolvconf.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>AF_LINK</name>
      <anchorfile>tnet__utils_8c.html</anchorfile>
      <anchor>a7a40a39aa9dfaf771780ebe061ffa940</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_interface_t *</type>
      <name>tnet_interface_create</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga519d8f6f2415bdf328e364f61bb47a39</anchor>
      <arglist>(const char *description, const void *mac_address, tsk_size_t mac_address_length)</arglist>
    </member>
    <member kind="function">
      <type>tnet_address_t *</type>
      <name>tnet_address_create</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga3e826cd2939bd297a73802b860262003</anchor>
      <arglist>(const char *ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_getlasterror</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac2b13fc342bfffa18868d8d91596961f</anchor>
      <arglist>(tnet_error_t *error)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_geterrno</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaf5def7b9d46329d20e8939a98482603f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_interfaces_L_t *</type>
      <name>tnet_get_interfaces</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gad982a81d6170a3e4826180ff72efe5fe</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_addresses_L_t *</type>
      <name>tnet_get_addresses</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga6c6add3dc5bdf1f3b9cddd4430254fc4</anchor>
      <arglist>(tnet_family_t family, tsk_bool_t unicast, tsk_bool_t anycast, tsk_bool_t multicast, tsk_bool_t dnsserver, long if_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getbestsource</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac336e82f244c99e56b440b3b55c60c6a</anchor>
      <arglist>(const char *destination, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *source)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getaddrinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga36ace730e3924849994ea87548d495be</anchor>
      <arglist>(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_freeaddrinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga18b2d2888f220d3117ee3b681c464922</anchor>
      <arglist>(struct addrinfo *ai)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getsockname</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaa1646c470ce392009805d7b46e5fe085</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr_storage *result)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getpeername</name>
      <anchorfile>tnet__utils_8c.html</anchorfile>
      <anchor>a90fda61ebce05fdb19b09bcdfa9fd3f6</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr_storage *result)</arglist>
    </member>
    <member kind="function">
      <type>tnet_socket_type_t</type>
      <name>tnet_get_socket_type</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac13e28b8f1c767306245bd3bf13f5e3c</anchor>
      <arglist>(tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>tnet_family_t</type>
      <name>tnet_get_family</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga00003bf6a8749b31d4b60b09cf5337a3</anchor>
      <arglist>(const char *host, tnet_port_t port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_get_sockip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga66eefd3c3bb0cc2c7a287374449b700e</anchor>
      <arglist>(const struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_get_peerip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga9a18f5ae763f323f38200e11b9ba369d</anchor>
      <arglist>(tnet_fd_t localFD, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_get_ip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga7feb0af8302b35bf835036ab7a48c4e9</anchor>
      <arglist>(tnet_fd_t fd, tsk_bool_t getlocal, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getnameinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga043fd7c4105a0d2014f3e0670b888d94</anchor>
      <arglist>(const struct sockaddr *sa, socklen_t salen, char *node, socklen_t nodelen, char *service, socklen_t servicelen, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_gethostname</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga906ade6f28556babf47cb08dc947abd1</anchor>
      <arglist>(tnet_host_t *result)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_waitUntil</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga3365d143e90fc4a1f83d330f489feb08</anchor>
      <arglist>(tnet_fd_t fd, long timeout, tsk_bool_t writable)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_joingroup6</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga39276ddf27fe08518cf2431ae46a2045</anchor>
      <arglist>(tnet_fd_t fd, const char *multiaddr, unsigned iface_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_leavegroup6</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaf54873a5fed045c9eff1200ebb368266</anchor>
      <arglist>(tnet_fd_t fd, const char *multiaddr, unsigned iface_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_resolve</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga903b843f79e03448a90d70bdd92a4779</anchor>
      <arglist>(const char *fqdn, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *out_ip, tnet_port_t *out_port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockaddrinfo_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaeb05ffebc3d308fd2ebb39de546c70a5</anchor>
      <arglist>(const char *host, tnet_port_t port, enum tnet_socket_type_e type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockaddr_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga0449294a58b8db4f81d835af2a535b90</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga02e3ba851395a9688b964cc228b732a8</anchor>
      <arglist>(const char *host, tnet_port_t port, enum tnet_socket_type_e type, tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_set_mode</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gab9616a14cc2c468ad99b510e276abac0</anchor>
      <arglist>(tnet_fd_t fd, int nonBlocking)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_sendto</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga92fcf0392c9a93e2e578578479d4f35e</anchor>
      <arglist>(tnet_fd_t fd, const struct sockaddr *to, const void *buf, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_recvfrom</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gabbf0cddd16af300a66d0859ed79b3cda</anchor>
      <arglist>(tnet_fd_t fd, void *buf, tsk_size_t size, int flags, struct sockaddr *from)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tnet_sockfd_send</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga93356b04edf02a163bfaa421676f1efc</anchor>
      <arglist>(tnet_fd_t fd, const void *buf, tsk_size_t size, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_recv</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaba9d8246797b995ecfb81f42f1bd1b96</anchor>
      <arglist>(tnet_fd_t fd, void *buf, tsk_size_t size, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_connectto</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gafe03cf813e644a3d2cd2de8666d7f9c8</anchor>
      <arglist>(tnet_fd_t fd, const struct sockaddr_storage *to)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_listen</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga92a295abbf85a361ee5c609994e21f05</anchor>
      <arglist>(tnet_fd_t fd, int backlog)</arglist>
    </member>
    <member kind="function">
      <type>tnet_fd_t</type>
      <name>tnet_sockfd_accept</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gae4350aa61a296325ce5dfc34fae616f8</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr *addr, socklen_t *addrlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_close</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gad13808723477879ca310a70c527d850a</anchor>
      <arglist>(tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_shutdown</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaba68ca319965780023e3209dae7167fa</anchor>
      <arglist>(tnet_fd_t fd)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_interface_def_t</name>
      <anchorfile>tnet__utils_8c.html</anchorfile>
      <anchor>a86ced52797e3eb79032b310357338788</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_address_def_t</name>
      <anchorfile>tnet__utils_8c.html</anchorfile>
      <anchor>a4b41ff43f60511629f506e066fd57565</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_utils.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>tnet__utils_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <class kind="struct">tnet_interface_s</class>
    <class kind="struct">tnet_address_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_CONNECT_TIMEOUT</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga2d7e6e8cff043b7f3c0bda66a7a728bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_all</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>ab526869bb2226efadbe9b24fbeaa5bd9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_all_unicast</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>add37eeae5ab502c5a41b713b52c6fbf4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_unicast4</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a80c23788d4ee1faa50dd09dabf6b5888</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_unicast6</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a8308f69587e66f13587c758781b1ce06</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_all_anycast</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a320dbd3806f63877741e60fb562a8fdc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_anycast4</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a38799bb6289ad0204fe2d321c0d88ffc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_anycast6</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aaa5c0dcb6c4d170162de234d4b39187b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_all_multicast</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>ac8abdec533160f662824533eec17ae0b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_multicast4</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a871424099f761764f01125ab50e27cd3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_multicast6</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a19c1df691b3df50b7dc478cc4d516a6d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_all_dnsservers</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a81dd24adc37519a2de448759ee44ccaf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_dnsservers4</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>ab397bc8cc3d0a295c0260703b808fdfa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_addresses_dnsservers6</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aea0bfe2cc680f63140ac4854136846b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_ip</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a2017b5ef78a3f74af946a5681cf0e017</anchor>
      <arglist>(fd, getlocal, ip)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_port</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a4c211f3c9cf30bab873cdfb673b90c7f</anchor>
      <arglist>(fd, getlocal, port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_sockip</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a96a295f733a973e755687eb18b76cafc</anchor>
      <arglist>(addr, ip)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_sockport</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aad605b48c4c0b260b8b2eda1ab3b3db4</anchor>
      <arglist>(addr, port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_peerip</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a079ef23d171be2c33c86d88276749b1d</anchor>
      <arglist>(localFD, ip)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_peerport</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aa7f35176e1af1f381db152a4e393c1bc</anchor>
      <arglist>(localFD, port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_get_sockaddr_size</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aefb30f9ee6e1e8f5dd4e34727cd76bab</anchor>
      <arglist>(psockaddr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_ioctlt</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>ac2410f7b3e6a9d089408da51ef40bcf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_soccket</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>af594457cda94f455aea25b5942d7cb79</anchor>
      <arglist>(family, type, protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_sockfd_waitUntilWritable</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aeed8af0119b20431ad1a7789e61f2256</anchor>
      <arglist>(fd, timeout)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_sockfd_waitUntilReadable</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>abe8fbe85cf4fb5526d8addee586f1714</anchor>
      <arglist>(fd, timeout)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_sockfd_set_nonblocking</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a72e2456a98a5317aad673cdf5b14d229</anchor>
      <arglist>(fd)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tnet_sockfd_set_blocking</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a2840609d8aadb9b0f62be4fbe1b84591</anchor>
      <arglist>(fd)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_PRINT_LAST_ERROR</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>ad84b1312befef0c09d4fa9a37e5bbe60</anchor>
      <arglist>(FMT,...)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_interface_s</type>
      <name>tnet_interface_t</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>a7c49ac3e5325961dd8c430edec84d106</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_address_s</type>
      <name>tnet_address_t</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>abec2d2434c892e0b38503fba6e04d981</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API void</type>
      <name>tnet_getlasterror</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac2b13fc342bfffa18868d8d91596961f</anchor>
      <arglist>(tnet_error_t *error)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_geterrno</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaf5def7b9d46329d20e8939a98482603f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_interfaces_L_t *</type>
      <name>tnet_get_interfaces</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gad982a81d6170a3e4826180ff72efe5fe</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_addresses_L_t *</type>
      <name>tnet_get_addresses</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga6c6add3dc5bdf1f3b9cddd4430254fc4</anchor>
      <arglist>(tnet_family_t family, tsk_bool_t unicast, tsk_bool_t anycast, tsk_bool_t multicast, tsk_bool_t dnsserver, long if_index)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_getbestsource</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac336e82f244c99e56b440b3b55c60c6a</anchor>
      <arglist>(const char *destination, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *source)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_getaddrinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga36ace730e3924849994ea87548d495be</anchor>
      <arglist>(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API void</type>
      <name>tnet_freeaddrinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga18b2d2888f220d3117ee3b681c464922</anchor>
      <arglist>(struct addrinfo *ai)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_getsockname</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaa1646c470ce392009805d7b46e5fe085</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr_storage *result)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_getpeername</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>af7a97c97fbe63065150329bc02dd5cbb</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr_storage *result)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_socket_type_t</type>
      <name>tnet_get_socket_type</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac13e28b8f1c767306245bd3bf13f5e3c</anchor>
      <arglist>(tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_family_t</type>
      <name>tnet_get_family</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga00003bf6a8749b31d4b60b09cf5337a3</anchor>
      <arglist>(const char *host, tnet_port_t port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_get_ip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga7feb0af8302b35bf835036ab7a48c4e9</anchor>
      <arglist>(tnet_fd_t fd, tsk_bool_t getlocal, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_get_sockip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga66eefd3c3bb0cc2c7a287374449b700e</anchor>
      <arglist>(const struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_get_peerip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga9a18f5ae763f323f38200e11b9ba369d</anchor>
      <arglist>(tnet_fd_t localFD, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_getnameinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga043fd7c4105a0d2014f3e0670b888d94</anchor>
      <arglist>(const struct sockaddr *sa, socklen_t salen, char *node, socklen_t nodelen, char *service, socklen_t servicelen, int flags)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_gethostname</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga906ade6f28556babf47cb08dc947abd1</anchor>
      <arglist>(tnet_host_t *result)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_waitUntil</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga3365d143e90fc4a1f83d330f489feb08</anchor>
      <arglist>(tnet_fd_t fd, long timeout, tsk_bool_t writable)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_joingroup6</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga39276ddf27fe08518cf2431ae46a2045</anchor>
      <arglist>(tnet_fd_t fd, const char *multiaddr, unsigned iface_index)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_leavegroup6</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaf54873a5fed045c9eff1200ebb368266</anchor>
      <arglist>(tnet_fd_t fd, const char *multiaddr, unsigned iface_index)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_resolve</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga903b843f79e03448a90d70bdd92a4779</anchor>
      <arglist>(const char *fqdn, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *out_ip, tnet_port_t *out_port)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockaddrinfo_init</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>ad61bbea344fdf1113ca96aa8b4d03ecb</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockaddr_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga0449294a58b8db4f81d835af2a535b90</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_init</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>adeb40eb2cb1616168082403075e13d51</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_set_mode</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gab9616a14cc2c468ad99b510e276abac0</anchor>
      <arglist>(tnet_fd_t fd, int nonBlocking)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_sendto</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga92fcf0392c9a93e2e578578479d4f35e</anchor>
      <arglist>(tnet_fd_t fd, const struct sockaddr *to, const void *buf, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_recvfrom</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gabbf0cddd16af300a66d0859ed79b3cda</anchor>
      <arglist>(tnet_fd_t fd, void *buf, tsk_size_t size, int flags, struct sockaddr *from)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tsk_size_t</type>
      <name>tnet_sockfd_send</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga93356b04edf02a163bfaa421676f1efc</anchor>
      <arglist>(tnet_fd_t fd, const void *buf, tsk_size_t size, int flags)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_recv</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaba9d8246797b995ecfb81f42f1bd1b96</anchor>
      <arglist>(tnet_fd_t fd, void *buf, tsk_size_t size, int flags)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_connectto</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gafe03cf813e644a3d2cd2de8666d7f9c8</anchor>
      <arglist>(tnet_fd_t fd, const struct sockaddr_storage *to)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_listen</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga92a295abbf85a361ee5c609994e21f05</anchor>
      <arglist>(tnet_fd_t fd, int backlog)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API tnet_fd_t</type>
      <name>tnet_sockfd_accept</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gae4350aa61a296325ce5dfc34fae616f8</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr *addr, socklen_t *addrlen)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_close</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gad13808723477879ca310a70c527d850a</anchor>
      <arglist>(tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>TINYNET_API int</type>
      <name>tnet_sockfd_shutdown</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaba68ca319965780023e3209dae7167fa</anchor>
      <arglist>(tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>tnet_interface_t *</type>
      <name>tnet_interface_create</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga519d8f6f2415bdf328e364f61bb47a39</anchor>
      <arglist>(const char *description, const void *mac_address, tsk_size_t mac_address_length)</arglist>
    </member>
    <member kind="function">
      <type>tnet_address_t *</type>
      <name>tnet_address_create</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga3e826cd2939bd297a73802b860262003</anchor>
      <arglist>(const char *ip)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_interface_def_t</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>aa481fcf4bb5fab0c6027398b3910e409</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_address_def_t</name>
      <anchorfile>tnet__utils_8h.html</anchorfile>
      <anchor>acc7cf74fd6b2d76973dd6dad9cfabe33</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_turn.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>tnet__turn_8c</filename>
    <includes id="tnet__turn_8h" name="tnet_turn.h" local="yes" imported="no">tnet_turn.h</includes>
    <includes id="tnet__turn__message_8h" name="tnet_turn_message.h" local="yes" imported="no">tnet_turn_message.h</includes>
    <includes id="tnet__nat_8h" name="tnet_nat.h" local="yes" imported="no">../tnet_nat.h</includes>
    <includes id="tnet__utils_8h" name="tnet_utils.h" local="yes" imported="no">../tnet_utils.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="typedef">
      <type>tnet_stun_request_t *(*</type>
      <name>tnet_turn_create_request_func</name>
      <anchorfile>tnet__turn_8c.html</anchorfile>
      <anchor>a381f4d2195b9e8be75743e8e5561cc92</anchor>
      <arglist>)(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_t *</type>
      <name>tnet_turn_channel_binding_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga103e23a60bb64dd069e6ab091784a6d4</anchor>
      <arglist>(const tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_permission_t *</type>
      <name>tnet_turn_permission_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8b42fc6a1e3f09cef7c7bf7c681a8218</anchor>
      <arglist>(uint32_t timeout)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_t *</type>
      <name>tnet_turn_allocation_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8ab0618c59d331267df779a881eb5bab</anchor>
      <arglist>(tnet_fd_t fd, tnet_socket_type_t socket_type, const char *server_address, tnet_port_t server_port, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaddc8b26510c425a60140d25f131dcb85</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, tnet_stun_message_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_allocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae1e001971df0be2311a06976e2f33201</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga2fde61ef85bcab916a3c9ce62789bd7e</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_unallocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga54c0fc2d0586f3160ab4f804c8a3950e</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_channel_bind</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gacca17b6face01e3f046e75f0624545eb</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_channel_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga86f4cd5cd926f84b75a3f8c86bcc348f</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_sendindication</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga346d00b42c27cb7f9a367d790137a351</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_permission</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gad8ed3de2531edf3185d41109d9411f2b</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_send_request</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaae576cf98cdc0ef40b0b3361327ad9cf</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, tnet_turn_create_request_func funcptr,...)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_id_t</type>
      <name>tnet_turn_allocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gad66c1ecb8963678e0d8451fef43eafd0</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, const tnet_fd_t localFD, tnet_socket_type_t socket_type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_allocation_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8f8e9c71f4fcbddc9a95e7cf0ce333a6</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_unallocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gadfec48a9b5519234d73373a46b24606e</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_id_t</type>
      <name>tnet_turn_channel_bind</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga3e6e2ae486927450c8a32d3000dd0e85</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_turn_allocation_t *allocation, struct sockaddr_storage *peer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_channel_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga39d90117f000959af24720570c240b75</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_turn_channel_binding_t *channel_bind)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_channel_senddata</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8623d3c7fec8d357ad29f76633643109</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_turn_channel_binding_t *channel_bind, const void *data, tsk_size_t size, int indication)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_add_permission</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gad49bc0f0bdd9832cf210825cc0b2156d</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_turn_allocation_t *allocation, const char *ipaddress, uint32_t timeout)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_channel_binding_def_t</name>
      <anchorfile>tnet__turn_8c.html</anchorfile>
      <anchor>a8c96cc1e81b39329d57e913cf3a22ab7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_permission_def_t</name>
      <anchorfile>tnet__turn_8c.html</anchorfile>
      <anchor>ac2c37805caacd556fb6b9e32b817e635</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_allocation_def_t</name>
      <anchorfile>tnet__turn_8c.html</anchorfile>
      <anchor>a9edbc36c8c4f6a56c71a5a00e98f9719</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_turn.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>tnet__turn_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__turn__attribute_8h" name="tnet_turn_attribute.h" local="yes" imported="no">turn/tnet_turn_attribute.h</includes>
    <includes id="tnet__proto_8h" name="tnet_proto.h" local="yes" imported="no">tnet_proto.h</includes>
    <includes id="tnet__socket_8h" name="tnet_socket.h" local="yes" imported="no">tnet_socket.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">tnet_types.h</includes>
    <class kind="struct">tnet_turn_permission_s</class>
    <class kind="struct">tnet_turn_channel_binding_s</class>
    <class kind="struct">tnet_turn_allocation_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_PERMISSION_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae355f01c55abfcddc5e24b59acc170f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_CHANBIND_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8aa082bcb28e67d8ec00e95b2fe5406f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_INVALID_ALLOCATION_ID</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a52d97a7fd715a0dc770ad8afe885e50f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_IS_VALID_ALLOCATION_ID</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a84362f7b61f67dc1f3a2543078e74bdd</anchor>
      <arglist>(id)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_INVALID_CHANNEL_BINDING_ID</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a23d5f1efb08ac449cc9e3d1ef859711e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_IS_VALID_CHANNEL_BINDING_ID</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a491748f8fbd7476bbd33b4ba678dc0de</anchor>
      <arglist>(id)</arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>tnet_turn_allocation_id_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a78fc33b60d08b8c6c4480d855f98051b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>tnet_turn_channel_binding_id_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a613a5cdd5a850369c7b547934ae6c544</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_permission_s</type>
      <name>tnet_turn_permission_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga022e47cd11dd0cd6b5985948e4f47421</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_turn_permissions_L_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>ae6574d0d25b43a7f1655d073c086d9d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_channel_binding_s</type>
      <name>tnet_turn_channel_binding_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga869e95eb9bf494895d66ad7a68f39cd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_turn_channel_bindings_L_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a08a04f2fc0fd1f5db142d37ee61ffff8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_allocation_s</type>
      <name>tnet_turn_allocation_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga4962ff19fd67702f01a1b1c107b19a56</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_turn_allocations_L_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>af966c78dcc79a47d86636c5710180663</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_id_t</type>
      <name>tnet_turn_allocate</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a06d72656df436b315bbab5299e7947f1</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_fd_t localFD, tnet_socket_type_t socket_type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_allocation_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8f8e9c71f4fcbddc9a95e7cf0ce333a6</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_unallocate</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>ab7b1b333af35a2367821c41793d745eb</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_id_t</type>
      <name>tnet_turn_channel_bind</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>aca13f95e34db4a637194a013b7ef594c</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_turn_allocation_t *allocation, struct sockaddr_storage *peer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_channel_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga39d90117f000959af24720570c240b75</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_turn_channel_binding_t *channel_bind)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_channel_senddata</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8623d3c7fec8d357ad29f76633643109</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_turn_channel_binding_t *channel_bind, const void *data, tsk_size_t size, int indication)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_add_permission</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a41d5b5a2d4e282954b8503a1dcf14ba8</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_turn_allocation_t *allocation, const char *ipaddress, uint32_t timeout)</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_permission_def_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a82139781df85506bc0fafe0f03f3e215</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_channel_binding_def_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>ae45bdb387bc5636788b75b2f8e7670af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_allocation_def_t</name>
      <anchorfile>tnet__turn_8h.html</anchorfile>
      <anchor>a9f3b2fc224b7da612fef43c5df6ecee4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_turn_attribute.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>tnet__turn__attribute_8c</filename>
    <includes id="tnet__turn__attribute_8h" name="tnet_turn_attribute.h" local="yes" imported="no">tnet_turn_attribute.h</includes>
    <includes id="tnet__stun_8h" name="tnet_stun.h" local="yes" imported="no">../stun/tnet_stun.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_turn_attribute_channelnum_t *</type>
      <name>tnet_turn_attribute_channelnum_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga407a64bdadf74e0cc58808f2f16de2fd</anchor>
      <arglist>(uint16_t number)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_lifetime_t *</type>
      <name>tnet_turn_attribute_lifetime_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga97892194ee263338e36c11d02d3c5ea2</anchor>
      <arglist>(uint32_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaf66bebd5bd8319c454a450dc092973c5</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_create_null</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>afebd53010c1edb07cbe663cee6aaaf50</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_data_t *</type>
      <name>tnet_turn_attribute_data_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga6e749efbc0e6c2d181b7b5572dd4a84a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xrelayed_addr_t *</type>
      <name>tnet_turn_attribute_xrelayed_addr_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga27dd53bb4f5afd8fbe1b3c6450764d68</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_even_port_t *</type>
      <name>tnet_turn_attribute_even_port_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga9201f3b08080e3d9d73d91cce22fdb6b</anchor>
      <arglist>(unsigned R)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_reqtrans_t *</type>
      <name>tnet_turn_attribute_reqtrans_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga0f8e1e1c00b3c62c9c451ad2dbf8419e</anchor>
      <arglist>(tnet_proto_t protocol)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_dontfrag_t *</type>
      <name>tnet_turn_attribute_dontfrag_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gab1baec0bd9b214f816737ccaf36846a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_restoken_t *</type>
      <name>tnet_turn_attribute_restoken_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga498409f84c06d567eeeeb913b01117c8</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_turn_attribute_deserialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae0fac81dd69579c064501d497cd03ea8</anchor>
      <arglist>(tnet_stun_attribute_type_t type, uint16_t length, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_attribute_serialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga5c64288790f379c04d8aae14117e305c</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_channelnum_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a72f2fd30ca18baf219d74649601c1aad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_lifetime_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a60a2ffe2a549923f8a5ea6c778005fa8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a99670a5af5f3a8ed7fd4efad17408487</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_data_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a4aabd9e2dbe97cb3be22393bfd7a5c22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_xrelayed_addr_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a79af40ee574c272240e82525ffe3ad27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_even_port_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a048a8a7f4c325d410b81ff93868f91a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_reqtrans_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>ad5b4ff0b5c890a8d80ebd7488980f6a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_dontfrag_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a59bb091518a3d8acb50372f498441daf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_restoken_def_t</name>
      <anchorfile>tnet__turn__attribute_8c.html</anchorfile>
      <anchor>a28eb707dbdbb91a8a63f112d6046d27c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_turn_attribute.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>tnet__turn__attribute_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">tinynet_config.h</includes>
    <includes id="tnet__proto_8h" name="tnet_proto.h" local="yes" imported="no">tnet_proto.h</includes>
    <includes id="tnet__stun__attribute_8h" name="tnet_stun_attribute.h" local="yes" imported="no">stun/tnet_stun_attribute.h</includes>
    <class kind="struct">tnet_turn_attribute_channelnum_s</class>
    <class kind="struct">tnet_turn_attribute_lifetime_s</class>
    <class kind="struct">tnet_turn_attribute_xpeer_addr_s</class>
    <class kind="struct">tnet_turn_attribute_data_s</class>
    <class kind="struct">tnet_turn_attribute_xrelayed_addr_s</class>
    <class kind="struct">tnet_turn_attribute_even_port_s</class>
    <class kind="struct">tnet_turn_attribute_reqtrans_s</class>
    <class kind="struct">tnet_turn_attribute_dontfrag_s</class>
    <class kind="struct">tnet_turn_attribute_restoken_s</class>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_channelnum_s</type>
      <name>tnet_turn_attribute_channelnum_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaa77bcb197c9e99794d67fd934d8c50c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_lifetime_s</type>
      <name>tnet_turn_attribute_lifetime_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga26a916f263289acaad0a56b7e374d028</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_xpeer_addr_s</type>
      <name>tnet_turn_attribute_xpeer_addr_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaa7fa88cb2c5f7269b94a916b66083235</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_data_s</type>
      <name>tnet_turn_attribute_data_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaaeab89d23a21fcfdaec9920fea7af597</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_xrelayed_addr_s</type>
      <name>tnet_turn_attribute_xrelayed_addr_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga07d7abdfc37b58ca81964c22a5012e41</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_even_port_s</type>
      <name>tnet_turn_attribute_even_port_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaa895b29c22d94e8cfcad5530a7d3264b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_reqtrans_s</type>
      <name>tnet_turn_attribute_reqtrans_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga2ca2caa07d491a0d562e5b2e456d8f59</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_dontfrag_s</type>
      <name>tnet_turn_attribute_dontfrag_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gab703b2d48941e0a376c5c5d82d0c2b83</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_restoken_s</type>
      <name>tnet_turn_attribute_restoken_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga981d08f23d6a6c2f660e0b8631703093</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_turn_attribute_deserialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae0fac81dd69579c064501d497cd03ea8</anchor>
      <arglist>(tnet_stun_attribute_type_t type, uint16_t length, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_attribute_serialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga5c64288790f379c04d8aae14117e305c</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_channelnum_t *</type>
      <name>tnet_turn_attribute_channelnum_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga407a64bdadf74e0cc58808f2f16de2fd</anchor>
      <arglist>(uint16_t number)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_lifetime_t *</type>
      <name>tnet_turn_attribute_lifetime_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga97892194ee263338e36c11d02d3c5ea2</anchor>
      <arglist>(uint32_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaf66bebd5bd8319c454a450dc092973c5</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_create_null</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>afebd53010c1edb07cbe663cee6aaaf50</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_data_t *</type>
      <name>tnet_turn_attribute_data_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga6e749efbc0e6c2d181b7b5572dd4a84a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xrelayed_addr_t *</type>
      <name>tnet_turn_attribute_xrelayed_addr_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga27dd53bb4f5afd8fbe1b3c6450764d68</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_even_port_t *</type>
      <name>tnet_turn_attribute_even_port_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga9201f3b08080e3d9d73d91cce22fdb6b</anchor>
      <arglist>(unsigned R)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_reqtrans_t *</type>
      <name>tnet_turn_attribute_reqtrans_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga0f8e1e1c00b3c62c9c451ad2dbf8419e</anchor>
      <arglist>(tnet_proto_t protocol)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_dontfrag_t *</type>
      <name>tnet_turn_attribute_dontfrag_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gab1baec0bd9b214f816737ccaf36846a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_restoken_t *</type>
      <name>tnet_turn_attribute_restoken_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga498409f84c06d567eeeeb913b01117c8</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="variable">
      <type>TNET_BEGIN_DECLS typedef tnet_stun_attribute_t</type>
      <name>tnet_turn_attribute_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>a00de15de99bdb6cce62f1ae7e326ff33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_channelnum_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>aa9ccbc071dd9dc40b459e98e329a7b7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_lifetime_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>ab6683e297789efe2ecb14d7fa617ced3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>a18c5987c8947a764223d1c6479aed130</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_data_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>aa8268a52ec6ea4183cb6cbcc4092a2d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_xrelayed_addr_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>a695a13f8c65383eb1fda5d1e0e086cc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_even_port_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>af8e79d87fc8a281fe70a27b8e4cf6b21</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_reqtrans_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>ace9cf4c1412342af44a6c01127c3c22f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_dontfrag_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>aa25f381fb1d4df9a64231c74163e9ec9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_attribute_restoken_def_t</name>
      <anchorfile>tnet__turn__attribute_8h.html</anchorfile>
      <anchor>a1d2d08c197e85e06c7f3f1abd75b0140</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_turn_message.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>tnet__turn__message_8c</filename>
    <includes id="tnet__turn__message_8h" name="tnet_turn_message.h" local="yes" imported="no">tnet_turn_message.h</includes>
    <includes id="tnet__types_8h" name="tnet_types.h" local="yes" imported="no">../tnet_types.h</includes>
    <includes id="tnet__endianness_8h" name="tnet_endianness.h" local="yes" imported="no">../tnet_endianness.h</includes>
    <member kind="function">
      <type>tnet_turn_channel_data_t *</type>
      <name>tnet_turn_channel_data_create</name>
      <anchorfile>tnet__turn__message_8c.html</anchorfile>
      <anchor>aea95662fe6dbd4eca6e02d1a05ef42b9</anchor>
      <arglist>(uint16_t number, uint16_t length, const void *data)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_data_t *</type>
      <name>tnet_turn_channel_data_create_null</name>
      <anchorfile>tnet__turn__message_8c.html</anchorfile>
      <anchor>aed557f9b1a06efc770b67fe6633b4373</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_turn_channel_data_serialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga00439f15cae25f78c4fd515c2ad689fc</anchor>
      <arglist>(const tnet_turn_channel_data_t *message)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tnet_turn_channel_data_def_t</name>
      <anchorfile>tnet__turn__message_8c.html</anchorfile>
      <anchor>ac5ac4021323667d38b4d942a391da23c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tnet_turn_message.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>tnet__turn__message_8h</filename>
    <includes id="tinynet__config_8h" name="tinynet_config.h" local="yes" imported="no">../tinynet_config.h</includes>
    <class kind="struct">tnet_turn_channel_data_s</class>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_turn_channel_data_s</type>
      <name>tnet_turn_channel_data_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga4878f9790955d14d532e4de51ac0fa5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_turn_channel_data_serialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga00439f15cae25f78c4fd515c2ad689fc</anchor>
      <arglist>(const tnet_turn_channel_data_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_data_t *</type>
      <name>tnet_turn_channel_data_create</name>
      <anchorfile>tnet__turn__message_8h.html</anchorfile>
      <anchor>aea95662fe6dbd4eca6e02d1a05ef42b9</anchor>
      <arglist>(uint16_t number, uint16_t length, const void *data)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_data_t *</type>
      <name>tnet_turn_channel_data_create_null</name>
      <anchorfile>tnet__turn__message_8h.html</anchorfile>
      <anchor>aed557f9b1a06efc770b67fe6633b4373</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYNET_GEXTERN const tsk_object_def_t *</type>
      <name>tnet_turn_channel_data_def_t</name>
      <anchorfile>tnet__turn__message_8h.html</anchorfile>
      <anchor>a52e135f0633a3a35dc8726b093b86ef9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_dhcp_group</name>
    <title>DHCPv4/BOOTP (RFC 2131) implementation.</title>
    <filename>group__tnet__dhcp__group.html</filename>
    <class kind="struct">tnet_dhcp_params_s</class>
    <class kind="struct">tnet_dhcp_ctx_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga4c4d364928ec9c9b7c2dbc6d59187b94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_CLIENT_PORT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga40d19eada507a56dd275125e3b1c195c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_SERVER_PORT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>gae5229a21da05427c690040fa34a6a2b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_VENDOR_ID_DEFAULT</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga68fdef222cf61845b6d961766b4cf276</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MAX_CODES</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga2e7d7c17df6e0c697014a20ab344d1a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DHCP_MAX_MSG_SIZE</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga9489b2e11946fde982adc9f88a148e5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_params_s</type>
      <name>tnet_dhcp_params_t</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga6fc1dfe6651b6c630b32fb6744a269e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_dhcp_ctx_s</type>
      <name>tnet_dhcp_ctx_t</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>gab40673583cd866001d0afc7db3e01e59</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_ctx_t *</type>
      <name>tnet_dhcp_ctx_create</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga4047baa7341d04b728d5884460214802</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_params_t *</type>
      <name>tnet_dhcp_params_create</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga7d73362e75ec846e6ce440ba9178069a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_reply_t *</type>
      <name>tnet_dhcp_send_request</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>gafd37386ea787357f72a4c71f8f66df4f</anchor>
      <arglist>(tnet_dhcp_ctx_t *ctx, tnet_dhcp_request_t *request)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp_reply_t *</type>
      <name>tnet_dhcp_query</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga66e173bcf5fd24b06a204be44312c56a</anchor>
      <arglist>(tnet_dhcp_ctx_t *ctx, tnet_dhcp_message_type_t type, tnet_dhcp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dhcp_params_add_code</name>
      <anchorfile>group__tnet__dhcp__group.html</anchorfile>
      <anchor>ga9fb8994c0bb692b29ccd0454cc592bc5</anchor>
      <arglist>(tnet_dhcp_params_t *params, tnet_dhcp_option_code_t code)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_dhcp6_group</name>
    <title>DHCPv6 (RFC 3315) implementation.</title>
    <filename>group__tnet__dhcp6__group.html</filename>
    <member kind="function">
      <type>tnet_dhcp6_reply_t *</type>
      <name>tnet_dhcp6_send_request</name>
      <anchorfile>group__tnet__dhcp6__group.html</anchorfile>
      <anchor>gadac4a203c542c67bcd608974944ade8d</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, tnet_dhcp6_request_t *request)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dhcp6_reply_t *</type>
      <name>tnet_dhcp6_requestinfo</name>
      <anchorfile>group__tnet__dhcp6__group.html</anchorfile>
      <anchor>gac8dd99f4e54866bbdf190ce5061b5b3a</anchor>
      <arglist>(const tnet_dhcp6_ctx_t *ctx, const tnet_dhcp6_option_orequest_t *orequest)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_dns_group</name>
    <title>DNS utility functions (RFCS [1034 1035] [3401 3402 3403 3404]).</title>
    <filename>group__tnet__dns__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_CACHE_TTL</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gad0036c81833a0d67783075346183f741</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga27019bac31c0cd87fcf5f1d440d44db2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_DGRAM_SIZE_DEFAULT</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga3f0a0842a1099131e50c4ce1358a8d66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_SERVER_PORT_DEFAULT</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga3687b2e64d73b85594bd93a79df252af</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_MESSAGE_IS_RESPONSE</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga2a5cf37ea80a5c81714042074eac92bf</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_MESSAGE_IS_QUERY</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaf24bf109d5da866b600989d53c24f2aa</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_RESPONSE_IS_SUCCESS</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaefa196185f91151ec1b70542d73f230b</anchor>
      <arglist>(response)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_DNS_RESPONSE_IS_ERROR</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gab7b1d2b4373729da855fad1816ef9e81</anchor>
      <arglist>(response)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_ctx_t *</type>
      <name>tnet_dns_ctx_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga080c974f8f6bdb3779bfae98956e9821</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_cache_entry_t *</type>
      <name>tnet_dns_cache_entry_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga130888ddea8e08e520a2ffd02278d90d</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tnet_dns_response_t *answer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_cache_clear</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga4908f53f564e46f39472e1dc825aa3ce</anchor>
      <arglist>(tnet_dns_ctx_t *ctx)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_resolve</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaaaee535d51058d67d9f170239de354b3</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_enum</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaf08d4dbbaf1025a17613bd553bf4db78</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *e164num, const char *domain)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>tnet_dns_enum_2</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga1aa07c721d9e2e7bac6eea9f44a515c8</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *service, const char *e164num, const char *domain)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_query_srv</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gab4b9b4f145decf0d2886c8a4ea25a313</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *service, char **hostname, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_query_naptr_srv</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga0c2c6cdb03f205b6ce16af9398d91730</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *domain, const char *service, char **hostname, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_dns_add_server</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga9d617f0cf0f0dc70e4a3bca53ab23824</anchor>
      <arglist>(tnet_dns_ctx_t *ctx, const char *host)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga98138daa781d9c12a4ba43f3371d0895</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tsk_bool_t isquery)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_create_null</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga353b8a830ee67c2a2d2a6e810f081ea9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_response_t *</type>
      <name>tnet_dns_response_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gaca11f63942b2ed04516a0d2323c75e7f</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_query_t *</type>
      <name>tnet_dns_query_create</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>gac97c055c0a0a84882eb586feb22ef097</anchor>
      <arglist>(const char *qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_dns_message_serialize</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga98f52e21a3d361e2a837993c31756871</anchor>
      <arglist>(const tnet_dns_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tnet_dns_message_t *</type>
      <name>tnet_dns_message_deserialize</name>
      <anchorfile>group__tnet__dns__group.html</anchorfile>
      <anchor>ga72d0a558aa990326e3724bd06260ac65</anchor>
      <arglist>(const uint8_t *data, tsk_size_t size)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_stun_group</name>
    <title>STUN2 (RFC 5389) implementation.</title>
    <filename>group__tnet__stun__group.html</filename>
    <class kind="struct">tnet_stun_binding_s</class>
    <class kind="struct">tnet_stun_attribute_s</class>
    <class kind="struct">tnet_stun_attribute_mapped_addr_s</class>
    <class kind="struct">tnet_stun_attribute_xmapped_addr_s</class>
    <class kind="struct">tnet_stun_attribute_username_s</class>
    <class kind="struct">tnet_stun_attribute_integrity_s</class>
    <class kind="struct">tnet_stun_attribute_fingerprint_s</class>
    <class kind="struct">tnet_stun_attribute_errorcode_s</class>
    <class kind="struct">tnet_stun_attribute_realm_s</class>
    <class kind="struct">tnet_stun_attribute_nonce_s</class>
    <class kind="struct">tnet_stun_attribute_unknowns_s</class>
    <class kind="struct">tnet_stun_attribute_software_s</class>
    <class kind="struct">tnet_stun_attribute_altserver_s</class>
    <class kind="struct">tnet_stun_attribute_ice_priority_s</class>
    <class kind="struct">tnet_stun_attribute_ice_use_candidate_s</class>
    <class kind="struct">tnet_stun_attribute_ice_controlled_s</class>
    <class kind="struct">tnet_stun_attribute_ice_controlling_s</class>
    <class kind="struct">tnet_stun_message_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_INVALID_BINDING_ID</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeb338f5c8a9b3ac08bba78293fdc22d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_IS_VALID_BINDING_ID</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga809cd1386eaa84094e78ef17a414ad7d</anchor>
      <arglist>(id)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_TCP_UDP_DEFAULT_PORT</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga840d6af2bf3bd3a96d120c942abefd7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_TLS_DEFAULT_PORT</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab0ae6bc64ad691f9bc5bd2b4c88c4569</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MAGIC_COOKIE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gac5e54e4b4576b54b53d2f4242f3e30de</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_HEADER_SIZE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga1df2b74747811b1bbdad28b18e0d80c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_ATTRIBUTE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeec0275bf11e81b2b89c149df5194c51</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MESSAGE_IS_REQUEST</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga50742ed21d77367f7d6d0b1ece487d94</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_MESSAGE_IS_INDICATION</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga05a95d304163ba125defae8444a28f73</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_RESPONSE_IS_SUCCESS</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga98049d686a5db4369a7554a77bf26838</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_RESPONSE_IS_ERROR</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga10bdb77a5f51b4670be89302eb445974</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_IS_STUN2_MSG</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3e9c76b717211f3d8c27d624a07e2353</anchor>
      <arglist>(PU8, SIZE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_STUN_TRANSACID_SIZE</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaa796e7b0e870b335336c50024ef5823</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_binding_s</type>
      <name>tnet_stun_binding_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabb378355a46b1aef733acdeef3a38368</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tnet_stun_bindings_L_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga994b6db4d6ac6e78c4ece450f1d5edff</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_addr_family_e</type>
      <name>tnet_stun_addr_family_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7080ca2a8d2b45a22d21c522a2a5a28f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_attribute_type_e</type>
      <name>tnet_stun_attribute_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2e77e50a4af676b754191403ae102933</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_s</type>
      <name>tnet_stun_attribute_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3ce2a25dae1324a8dd545a68a2d8ccf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_mapped_addr_s</type>
      <name>tnet_stun_attribute_mapped_addr_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga6ef5c5c6d47376dc7cf7ac14b923ad26</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_xmapped_addr_s</type>
      <name>tnet_stun_attribute_xmapped_addr_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8885759ce02dd0bebc0bee97ec48171c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_username_s</type>
      <name>tnet_stun_attribute_username_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga33d875930c4e6213e7516e580a88eebb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_integrity_s</type>
      <name>tnet_stun_attribute_integrity_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8b2b266a46e44c471708eca71ac8670a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_fingerprint_s</type>
      <name>tnet_stun_attribute_fingerprint_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7ead14f441ddf014894c17fcef2feeaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_errorcode_s</type>
      <name>tnet_stun_attribute_errorcode_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0e804aae488d73e576dd9c4453a93a04</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_realm_s</type>
      <name>tnet_stun_attribute_realm_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaf4ef4fa43bec46b6313c0c400d6c6ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_nonce_s</type>
      <name>tnet_stun_attribute_nonce_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7e4d37384aa42524d68d8e9e0ddc9ea2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_unknowns_s</type>
      <name>tnet_stun_attribute_unknowns_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab27dd830e06e223350a584c41383015d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_software_s</type>
      <name>tnet_stun_attribute_software_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga5744df35d8bf1f1f366097f6a63f1177</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_altserver_s</type>
      <name>tnet_stun_attribute_altserver_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga1912ae8bc9ea407a04f1d16aa570dbc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_priority_s</type>
      <name>tnet_stun_attribute_ice_priority_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga782dcf37c6503a9287b479cef6fe0132</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_use_candidate_s</type>
      <name>tnet_stun_attribute_ice_use_candidate_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga47de3ced777b9c46f863e513b934ab23</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_controlled_s</type>
      <name>tnet_stun_attribute_ice_controlled_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab46c08e93d0c514a72fc8b3f73bc7a77</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_attribute_ice_controlling_s</type>
      <name>tnet_stun_attribute_ice_controlling_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gac4832ed589696b4a9ef6274b96333adf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>tnet_stun_transacid_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaab3bfd494f75601a0f26177c6d851810</anchor>
      <arglist>[TNET_STUN_TRANSACID_SIZE]</arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_class_type_e</type>
      <name>tnet_stun_class_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8db95f74a4953795bf182744e63eff78</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_method_type_e</type>
      <name>tnet_stun_method_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga145552721a7b4f524ceb40ac2da1038d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tnet_stun_message_type_e</type>
      <name>tnet_stun_message_type_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab3ed2f3313ef6914347c2e604d7cf104</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_stun_message_s</type>
      <name>tnet_stun_message_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga4e9470802bdd2d8b0a1b90631369721e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_addr_family_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3326fbcc063f264d3d359e922637e2e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ipv4</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3326fbcc063f264d3d359e922637e2e4a5663f4b60301ed2f9e7823613407dbed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ipv6</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3326fbcc063f264d3d359e922637e2e4a97e1e424d39a8af2791c1187090f111e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_attribute_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab696586b61219965987af1e017e6ffa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reserved</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a5bf34ce06a004ad9f8db1dc687786273</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_mapped_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a9aadc5f8467c936cc7359889ba120e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_response_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a203b0a7edf4f8fb3381ac769595a2098</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_change_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6aeb35b7b83152b7a99bf46b4014928adc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_source_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a69f3448100d4f1a61536eb2c7315cd90</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_changed_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a0bd0d2c8f1d6ae85ffb58aeef4c01134</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_username</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a3247970572dad0c63e537b647dbd0861</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_password</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a432698b7b67cf13adf504f969abb60fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_message_integrity</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ac20851e6e8fd2c479c0438b376987a85</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_error_code</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ab8571c9a5c334530ef9aa45adc7fd71e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_unknown_attributes</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a455d442e5c380953ff11c7e0d055f3ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reflected_from</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ae129dfcf945fe995400dba3e8b9895c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_realm</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a0d8e28c40313d6d3b397ac957714cfee</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_nonce</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ae5dda080590c37e95953334f715ed3ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_xor_mapped_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6af70dcad086bff5d4ee77f093941642d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_software</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ad5a2f9a89fbc825142e4d754b51bc125</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_alternate_server</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a4f263d754aaccf30fd861a934dfc12ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_fingerprint</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6aac669613b66507fba6c08d23a4283bf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channel_number</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a7559421f1d46a400cdda94c32c3aff74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_lifetime</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6aff4ed6f5e451f77b39d5113b9085493c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reserved2</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a626c6684c81f69c1649e178cc9847069</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_xor_peer_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ae14d3b3903934eab51356ad28584fc66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_data</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ab85dfed04ff37affea00d10a517c72dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_xor_relayed_address</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ada0a3d93b81c32a3d0af2ed20efffe71</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_even_port</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6ac99da9ae2b9a6a885631e5e476293df2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_requested_transport</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a033a9a9cdad515c45291007be25dd028</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_dont_fragment</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a3f854135f19b767abc7e8031af6b5f88</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reserved3</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a57c9e8d7229d0989d73a5efc2fef36e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_reservation_token</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a88373f0f1635103b2091910a9584c58d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_priority</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a99410d8e75f7ab0d4421fe71634604bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_use_candidate</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a275c187aabe3a9d9bdc32a180c39b1d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_controlled</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a4cfd4eab6287ed2b1126e6ba40ac5d4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_ice_controlling</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggab696586b61219965987af1e017e6ffa6a21100a7a716c88ed5b5544cdb28549aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_class_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3faef946079e315842f5ad4d41aa0929</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a4617c080243a43b3d8f93bdfcc3f0f4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a204a6136670e4ad622276ef22a2ae65e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a0f27f07bfc9be6aa217799fa088cf059</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_class_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga3faef946079e315842f5ad4d41aa0929a271a1bc249f8bc1af5b9e6308b166232</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_method_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga4701a0e3395592a6d6a742901212801c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_binding</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801cafd301f45e6297ffca65062c30b89ef53</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_allocate</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca166402bfafe37fa33b31f1466265798b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_refresh</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca6081fbd713a271d81264c919a589ce97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_send</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca3c8e39a747df8f52dcc70d5d7db3e423</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_data</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca72ac38a767b77800980a5577dd13059c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_createpermission</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca45791d8de7420d9d3afccfc78ac8ba8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_method_channelbind</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gga4701a0e3395592a6d6a742901212801ca9a883e8885d8eb35a93beccda692233c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tnet_stun_message_type_e</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabf2f8d8b846fe491470941a7e237559c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caef49ee403fb2e849831649e4c09729d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca40f5bbd220cf9c1ec50654becc93c75f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca45e30ced8713c5c159bb45641ce04e79</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_binding_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca9526f90725e2c9be01cd4ea2aa818e50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cab3d8c425c241c43ab3bdbec7fdddc2dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cadda6f40f23dc4bd53d1d6c068484ada0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caf20455522d8e72c9e055b8ea5ef2dabb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_allocate_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cae4d21e386431edf815a9a652f3d745a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca3f15b1671e604a2fb7a65401756dccc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca5130e27fd810862da942fcde58eb0c42</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caead4a26ba4d373a96e4dac68ff9fdb45</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_refresh_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca17067ce9dae33780b432c50b2a53130b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_send_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cac7424fdd7360f72326f8f9c74d36d15b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_data_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca66d958d23caaac30ab000f37c36a3406</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559ca55ffb1e77c92edb303a51f7221049c66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cad1099efddc3ea3f8e91bd7953fef5c50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caa804d83ad6928163af8e5b8c29c003dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_createpermission_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cad3da1d3b613236f00e9747a3874eb3c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559caf9886f762e46af82fc8122ba35a95a8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_indication</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cae56d07904cc9d8091f32a6973258146f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_success_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cafcb4db651d71fd5c21f7bbb2def064a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stun_channelbind_error_response</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ggabf2f8d8b846fe491470941a7e237559cab65200966087d7dd85dd17de30cde9ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_binding_t *</type>
      <name>tnet_stun_binding_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga60801a4caec4d4335f0b4fea0e63025e</anchor>
      <arglist>(tnet_fd_t fd, tnet_socket_type_t socket_type, const char *server_address, tnet_port_t server_port, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_create_request</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga481bcd3d5bfdfda6561227bfcf1fc359</anchor>
      <arglist>(const tnet_stun_binding_t *binding)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_response_t *</type>
      <name>tnet_stun_send_unreliably</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga36147911b3c99cbf897930c971608a5f</anchor>
      <arglist>(tnet_fd_t localFD, uint16_t RTO, uint16_t Rc, const tnet_stun_message_t *message, struct sockaddr *server)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_send_bind</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8151700d692ca34abf3cd42982723aa7</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_stun_binding_t *binding)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_binding_id_t</type>
      <name>tnet_stun_bind</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gafad0d44c67c44e0cf91de149a32e8079</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_transacid_cmp</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gad2cd556a7d81575118646c3e3002af66</anchor>
      <arglist>(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_stun_attribute_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga9a8fea6f7fb4c6236bc4df85153d6af4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_mapped_addr_t *</type>
      <name>tnet_stun_attribute_mapped_address_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga28ffe32f830bbe0f5b32a207538eb8b6</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_xmapped_addr_t *</type>
      <name>tnet_stun_attribute_xmapped_address_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gacd4af1df8c9e298a23d34df27e5dcc45</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_username_t *</type>
      <name>tnet_stun_attribute_username_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab92c8f62b2d188c9ac1528aa03da4b7e</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_integrity_t *</type>
      <name>tnet_stun_attribute_integrity_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga3c5f79313850ea8643132b093e11ef6a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_fingerprint_t *</type>
      <name>tnet_stun_attribute_fingerprint_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2432db4c4bb63da2327bb357361eef21</anchor>
      <arglist>(uint32_t fingerprint)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_errorcode_t *</type>
      <name>tnet_stun_attribute_errorcode_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaa51e8bc565195298fb3de24070f5614a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_realm_t *</type>
      <name>tnet_stun_attribute_realm_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga2d7ab50986b3af03951358ec4c2cfa8b</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_nonce_t *</type>
      <name>tnet_stun_attribute_nonce_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0cf404a4977b95a47c252f8101b099ce</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_unknowns_t *</type>
      <name>tnet_stun_attribute_unknowns_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8a497b2b11b1a776ec37170ee14b6ac2</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_software_t *</type>
      <name>tnet_stun_attribute_software_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga719d8c3ac6588124533f03f70877e437</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_altserver_t *</type>
      <name>tnet_stun_attribute_altserver_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga586636541b8ebb9ea46d4df39a448121</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_priority_t *</type>
      <name>tnet_stun_attribute_ice_priority_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7e2992cbcf5e4d93fce285c8fdad6018</anchor>
      <arglist>(uint32_t value)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_use_candidate_t *</type>
      <name>tnet_stun_attribute_ice_use_candidate_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gafc6efba58197cb3d697502bbad7378cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_controlled_t *</type>
      <name>tnet_stun_attribute_ice_controlled_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaf2e4854c90f7c8c3abae9915e59ad12f</anchor>
      <arglist>(uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_ice_controlling_t *</type>
      <name>tnet_stun_attribute_ice_controlling_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga94da0e7a21d255c8647c190387229ba3</anchor>
      <arglist>(uint64_t value)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_stun_attribute_deserialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga642d9b382cea0cbbf0590b8b60be54c6</anchor>
      <arglist>(const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_attribute_serialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gabb81a19c0eeb9f47f0b9599d65d61d6e</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_stun_attribute_pad</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga9364909d8a9c38fa1be0dd48efa3fac3</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_create</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaaf1892e2a30f8b85e4e5b47622a6b9f3</anchor>
      <arglist>(const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_create_null</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga938bc40f8bb818f87c94eb42352cd0f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_stun_message_serialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaefd2306ef33e3611a98394e5b274a069</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_message_t *</type>
      <name>tnet_stun_message_deserialize</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga0f5f3b814bb84f3cb62802b1d9af275e</anchor>
      <arglist>(const uint8_t *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_stun_message_has_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga250b97b6a26f99f9d545533e20d29acb</anchor>
      <arglist>(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_message_add_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gacb982af5ead9faf9e85b4e2f69e05d47</anchor>
      <arglist>(tnet_stun_message_t *self, tnet_stun_attribute_t **attribute)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_stun_message_remove_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga8c3cf4006d4c030e0114b8c00f2dc5ec</anchor>
      <arglist>(tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>const tnet_stun_attribute_t *</type>
      <name>tnet_stun_message_get_attribute</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gaeac2966bdc80f798ec61f6f7ebfc6a95</anchor>
      <arglist>(const tnet_stun_message_t *self, tnet_stun_attribute_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>short</type>
      <name>tnet_stun_message_get_errorcode</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga7638d5816c344718874a113abd9739d5</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_stun_message_get_realm</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga6115407c7af73b3041c215ccd34e35b8</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>tnet_stun_message_get_nonce</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga5e068d5474bfbd73e8cece833deac6b2</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>tnet_stun_message_get_lifetime</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga12380617e5a883d3c175216c48245839</anchor>
      <arglist>(const tnet_stun_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tnet_stun_message_transac_id_equals</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>ga401b2a061923a11340da833dc248fa9d</anchor>
      <arglist>(const tnet_stun_transacid_t id1, const tnet_stun_transacid_t id2)</arglist>
    </member>
    <member kind="variable">
      <type>TNET_BEGIN_DECLS typedef uint64_t</type>
      <name>tnet_stun_binding_id_t</name>
      <anchorfile>group__tnet__stun__group.html</anchorfile>
      <anchor>gab95871f186b8bf46b4630ab2ebb146aa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_nat_group</name>
    <title>NAT Traversal API (STUN, TURN and ICE).</title>
    <filename>group__tnet__nat__group.html</filename>
    <class kind="struct">tnet_nat_context_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_DEFAULT_RTO</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga25beb3cb889e80636f91ed1e316ef083</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_ICE_DEFAULT_RC</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gaf2d937114195afa44a46b88b898a781b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_NAT_DEFAULT_RTO</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga18cf72a341f3c785ab00a746286db1e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_NAT_DEFAULT_RC</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga5e809d1461c049e1830e5d376a07bd11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_NAT_TCP_UDP_DEFAULT_PORT</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga0a871f1c40c7acb827f0513958ec8b21</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_nat_context_s</type>
      <name>tnet_nat_context_t</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4d037b7a4416b7e876163d3c05256a1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>tnet_nat_context_handle_t</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga82ef754b59414ada0129fa558e9f3ec2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_nat_context_handle_t *</type>
      <name>tnet_nat_context_create</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga49c2521bd48d6df2ce3a46e066ebe874</anchor>
      <arglist>(tnet_socket_type_t socket_type, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_set_server_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga60f64045a1a894a41afdb73a332f8732</anchor>
      <arglist>(tnet_nat_context_handle_t *self, const char *server_address)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_set_server</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gadb5209238b5596dac88606e5130dc93f</anchor>
      <arglist>(tnet_nat_context_handle_t *self, const char *server_address, tnet_port_t server_port)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_binding_id_t</type>
      <name>tnet_nat_stun_bind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gaedaee292e59564e952b1ddda3b82f0a6</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, const tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_stun_get_reflexive_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga806a269daee5625262abe17c730d6bfc</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_stun_binding_id_t id, char **ipaddress, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_stun_unbind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>gac7501503711b402a1863967000970e16</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_stun_binding_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_id_t</type>
      <name>tnet_nat_turn_allocate</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga0388eae3c1cb2390070e49845a9e7a33</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, const tnet_fd_t localFD)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_get_reflexive_address</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4417d9c60fcfae071ffee563b1e5f4dd</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, char **ipaddress, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_allocation_refresh</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga6c0eb272fdaf34b696f40aa7f5279c24</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_unallocate</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga5ffc9393817f43bdd1331454a6f8af6e</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_id_t</type>
      <name>tnet_nat_turn_channel_bind</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga2c1bed31feafaa25d30cf8cc92b0b9e5</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, struct sockaddr_storage *peer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_channel_refresh</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga4995c26dc8e520f2820924082b1c5193</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_channel_binding_id_t id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_channel_send</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga69719fb972fa3ccf667dd3999c221e8c</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_channel_binding_id_t id, const void *data, tsk_size_t size, int indication)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_nat_turn_add_permission</name>
      <anchorfile>group__tnet__nat__group.html</anchorfile>
      <anchor>ga67181e6f65d829d44089841fd72429f0</anchor>
      <arglist>(const tnet_nat_context_handle_t *self, tnet_turn_allocation_id_t id, const char *ipaddress, uint32_t timeout)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_socket_group</name>
    <title>Protocol agnostic socket</title>
    <filename>group__tnet__socket__group.html</filename>
    <member kind="function">
      <type>tnet_socket_t *</type>
      <name>tnet_socket_create_2</name>
      <anchorfile>group__tnet__socket__group.html</anchorfile>
      <anchor>gaa4d96c3d0bac908bd2d3413fbd19d30d</anchor>
      <arglist>(const char *host, tnet_port_t port_, tnet_socket_type_t type, tsk_bool_t nonblocking, tsk_bool_t bindsocket)</arglist>
    </member>
    <member kind="function">
      <type>tnet_socket_t *</type>
      <name>tnet_socket_create</name>
      <anchorfile>group__tnet__socket__group.html</anchorfile>
      <anchor>ga44cfc3407bc375c5b932bfe45a9de849</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_utils_group</name>
    <title>Network utility functions.</title>
    <filename>group__tnet__utils__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>TNET_CONNECT_TIMEOUT</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga2d7e6e8cff043b7f3c0bda66a7a728bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_interface_t *</type>
      <name>tnet_interface_create</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga519d8f6f2415bdf328e364f61bb47a39</anchor>
      <arglist>(const char *description, const void *mac_address, tsk_size_t mac_address_length)</arglist>
    </member>
    <member kind="function">
      <type>tnet_address_t *</type>
      <name>tnet_address_create</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga3e826cd2939bd297a73802b860262003</anchor>
      <arglist>(const char *ip)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_getlasterror</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac2b13fc342bfffa18868d8d91596961f</anchor>
      <arglist>(tnet_error_t *error)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_geterrno</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaf5def7b9d46329d20e8939a98482603f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_interfaces_L_t *</type>
      <name>tnet_get_interfaces</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gad982a81d6170a3e4826180ff72efe5fe</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_addresses_L_t *</type>
      <name>tnet_get_addresses</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga6c6add3dc5bdf1f3b9cddd4430254fc4</anchor>
      <arglist>(tnet_family_t family, tsk_bool_t unicast, tsk_bool_t anycast, tsk_bool_t multicast, tsk_bool_t dnsserver, long if_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getbestsource</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac336e82f244c99e56b440b3b55c60c6a</anchor>
      <arglist>(const char *destination, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *source)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getaddrinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga36ace730e3924849994ea87548d495be</anchor>
      <arglist>(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tnet_freeaddrinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga18b2d2888f220d3117ee3b681c464922</anchor>
      <arglist>(struct addrinfo *ai)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getsockname</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaa1646c470ce392009805d7b46e5fe085</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr_storage *result)</arglist>
    </member>
    <member kind="function">
      <type>tnet_socket_type_t</type>
      <name>tnet_get_socket_type</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gac13e28b8f1c767306245bd3bf13f5e3c</anchor>
      <arglist>(tnet_fd_t fd)</arglist>
    </member>
    <member kind="function">
      <type>tnet_family_t</type>
      <name>tnet_get_family</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga00003bf6a8749b31d4b60b09cf5337a3</anchor>
      <arglist>(const char *host, tnet_port_t port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_get_sockip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga66eefd3c3bb0cc2c7a287374449b700e</anchor>
      <arglist>(const struct sockaddr *addr, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_get_peerip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga9a18f5ae763f323f38200e11b9ba369d</anchor>
      <arglist>(tnet_fd_t localFD, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_get_ip_n_port</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga7feb0af8302b35bf835036ab7a48c4e9</anchor>
      <arglist>(tnet_fd_t fd, tsk_bool_t getlocal, tnet_ip_t *ip, tnet_port_t *port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_getnameinfo</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga043fd7c4105a0d2014f3e0670b888d94</anchor>
      <arglist>(const struct sockaddr *sa, socklen_t salen, char *node, socklen_t nodelen, char *service, socklen_t servicelen, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_gethostname</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga906ade6f28556babf47cb08dc947abd1</anchor>
      <arglist>(tnet_host_t *result)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_waitUntil</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga3365d143e90fc4a1f83d330f489feb08</anchor>
      <arglist>(tnet_fd_t fd, long timeout, tsk_bool_t writable)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_joingroup6</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga39276ddf27fe08518cf2431ae46a2045</anchor>
      <arglist>(tnet_fd_t fd, const char *multiaddr, unsigned iface_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_leavegroup6</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaf54873a5fed045c9eff1200ebb368266</anchor>
      <arglist>(tnet_fd_t fd, const char *multiaddr, unsigned iface_index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_resolve</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga903b843f79e03448a90d70bdd92a4779</anchor>
      <arglist>(const char *fqdn, tnet_port_t port, tnet_socket_type_t type, tnet_ip_t *out_ip, tnet_port_t *out_port)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockaddrinfo_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaeb05ffebc3d308fd2ebb39de546c70a5</anchor>
      <arglist>(const char *host, tnet_port_t port, enum tnet_socket_type_e type, struct sockaddr_storage *ai_addr, int *ai_family, int *ai_socktype, int *ai_protocol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockaddr_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga0449294a58b8db4f81d835af2a535b90</anchor>
      <arglist>(const char *host, tnet_port_t port, tnet_socket_type_t type, struct sockaddr_storage *addr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_init</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga02e3ba851395a9688b964cc228b732a8</anchor>
      <arglist>(const char *host, tnet_port_t port, enum tnet_socket_type_e type, tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_set_mode</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gab9616a14cc2c468ad99b510e276abac0</anchor>
      <arglist>(tnet_fd_t fd, int nonBlocking)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_sendto</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga92fcf0392c9a93e2e578578479d4f35e</anchor>
      <arglist>(tnet_fd_t fd, const struct sockaddr *to, const void *buf, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_recvfrom</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gabbf0cddd16af300a66d0859ed79b3cda</anchor>
      <arglist>(tnet_fd_t fd, void *buf, tsk_size_t size, int flags, struct sockaddr *from)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tnet_sockfd_send</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga93356b04edf02a163bfaa421676f1efc</anchor>
      <arglist>(tnet_fd_t fd, const void *buf, tsk_size_t size, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_recv</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaba9d8246797b995ecfb81f42f1bd1b96</anchor>
      <arglist>(tnet_fd_t fd, void *buf, tsk_size_t size, int flags)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_connectto</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gafe03cf813e644a3d2cd2de8666d7f9c8</anchor>
      <arglist>(tnet_fd_t fd, const struct sockaddr_storage *to)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_listen</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>ga92a295abbf85a361ee5c609994e21f05</anchor>
      <arglist>(tnet_fd_t fd, int backlog)</arglist>
    </member>
    <member kind="function">
      <type>tnet_fd_t</type>
      <name>tnet_sockfd_accept</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gae4350aa61a296325ce5dfc34fae616f8</anchor>
      <arglist>(tnet_fd_t fd, struct sockaddr *addr, socklen_t *addrlen)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_close</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gad13808723477879ca310a70c527d850a</anchor>
      <arglist>(tnet_fd_t *fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_sockfd_shutdown</name>
      <anchorfile>group__tnet__utils__group.html</anchorfile>
      <anchor>gaba68ca319965780023e3209dae7167fa</anchor>
      <arglist>(tnet_fd_t fd)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tnet_turn_group</name>
    <title>TURN(draft-ietf-behave-turn-16) implementation.</title>
    <filename>group__tnet__turn__group.html</filename>
    <class kind="struct">tnet_turn_permission_s</class>
    <class kind="struct">tnet_turn_channel_binding_s</class>
    <class kind="struct">tnet_turn_allocation_s</class>
    <class kind="struct">tnet_turn_attribute_channelnum_s</class>
    <class kind="struct">tnet_turn_attribute_lifetime_s</class>
    <class kind="struct">tnet_turn_attribute_xpeer_addr_s</class>
    <class kind="struct">tnet_turn_attribute_data_s</class>
    <class kind="struct">tnet_turn_attribute_xrelayed_addr_s</class>
    <class kind="struct">tnet_turn_attribute_even_port_s</class>
    <class kind="struct">tnet_turn_attribute_reqtrans_s</class>
    <class kind="struct">tnet_turn_attribute_dontfrag_s</class>
    <class kind="struct">tnet_turn_attribute_restoken_s</class>
    <class kind="struct">tnet_turn_channel_data_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_PERMISSION_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae355f01c55abfcddc5e24b59acc170f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TNET_TURN_CHANBIND_TIMEOUT_DEFAULT</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8aa082bcb28e67d8ec00e95b2fe5406f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_permission_s</type>
      <name>tnet_turn_permission_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga022e47cd11dd0cd6b5985948e4f47421</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_channel_binding_s</type>
      <name>tnet_turn_channel_binding_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga869e95eb9bf494895d66ad7a68f39cd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_allocation_s</type>
      <name>tnet_turn_allocation_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga4962ff19fd67702f01a1b1c107b19a56</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_channelnum_s</type>
      <name>tnet_turn_attribute_channelnum_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaa77bcb197c9e99794d67fd934d8c50c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_lifetime_s</type>
      <name>tnet_turn_attribute_lifetime_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga26a916f263289acaad0a56b7e374d028</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_xpeer_addr_s</type>
      <name>tnet_turn_attribute_xpeer_addr_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaa7fa88cb2c5f7269b94a916b66083235</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_data_s</type>
      <name>tnet_turn_attribute_data_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaaeab89d23a21fcfdaec9920fea7af597</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_xrelayed_addr_s</type>
      <name>tnet_turn_attribute_xrelayed_addr_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga07d7abdfc37b58ca81964c22a5012e41</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_even_port_s</type>
      <name>tnet_turn_attribute_even_port_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaa895b29c22d94e8cfcad5530a7d3264b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_reqtrans_s</type>
      <name>tnet_turn_attribute_reqtrans_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga2ca2caa07d491a0d562e5b2e456d8f59</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_dontfrag_s</type>
      <name>tnet_turn_attribute_dontfrag_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gab703b2d48941e0a376c5c5d82d0c2b83</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tnet_turn_attribute_restoken_s</type>
      <name>tnet_turn_attribute_restoken_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga981d08f23d6a6c2f660e0b8631703093</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TNET_BEGIN_DECLS struct tnet_turn_channel_data_s</type>
      <name>tnet_turn_channel_data_t</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga4878f9790955d14d532e4de51ac0fa5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_t *</type>
      <name>tnet_turn_channel_binding_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga103e23a60bb64dd069e6ab091784a6d4</anchor>
      <arglist>(const tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_permission_t *</type>
      <name>tnet_turn_permission_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8b42fc6a1e3f09cef7c7bf7c681a8218</anchor>
      <arglist>(uint32_t timeout)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_t *</type>
      <name>tnet_turn_allocation_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8ab0618c59d331267df779a881eb5bab</anchor>
      <arglist>(tnet_fd_t fd, tnet_socket_type_t socket_type, const char *server_address, tnet_port_t server_port, const char *username, const char *password)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaddc8b26510c425a60140d25f131dcb85</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, tnet_stun_message_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_allocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae1e001971df0be2311a06976e2f33201</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga2fde61ef85bcab916a3c9ce62789bd7e</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_unallocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga54c0fc2d0586f3160ab4f804c8a3950e</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_channel_bind</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gacca17b6face01e3f046e75f0624545eb</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_channel_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga86f4cd5cd926f84b75a3f8c86bcc348f</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_sendindication</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga346d00b42c27cb7f9a367d790137a351</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_request_t *</type>
      <name>tnet_turn_create_request_permission</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gad8ed3de2531edf3185d41109d9411f2b</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_send_request</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaae576cf98cdc0ef40b0b3361327ad9cf</anchor>
      <arglist>(const tnet_nat_context_t *context, tnet_turn_allocation_t *allocation, tnet_turn_create_request_func funcptr,...)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_allocation_id_t</type>
      <name>tnet_turn_allocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gad66c1ecb8963678e0d8451fef43eafd0</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, const tnet_fd_t localFD, tnet_socket_type_t socket_type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_allocation_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8f8e9c71f4fcbddc9a95e7cf0ce333a6</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_unallocate</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gadfec48a9b5519234d73373a46b24606e</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_turn_allocation_t *allocation)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_channel_binding_id_t</type>
      <name>tnet_turn_channel_bind</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga3e6e2ae486927450c8a32d3000dd0e85</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_turn_allocation_t *allocation, struct sockaddr_storage *peer)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_channel_refresh</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga39d90117f000959af24720570c240b75</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_turn_channel_binding_t *channel_bind)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_channel_senddata</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga8623d3c7fec8d357ad29f76633643109</anchor>
      <arglist>(const struct tnet_nat_context_s *nat_context, const tnet_turn_channel_binding_t *channel_bind, const void *data, tsk_size_t size, int indication)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_add_permission</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gad49bc0f0bdd9832cf210825cc0b2156d</anchor>
      <arglist>(const tnet_nat_context_t *nat_context, tnet_turn_allocation_t *allocation, const char *ipaddress, uint32_t timeout)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_channelnum_t *</type>
      <name>tnet_turn_attribute_channelnum_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga407a64bdadf74e0cc58808f2f16de2fd</anchor>
      <arglist>(uint16_t number)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_lifetime_t *</type>
      <name>tnet_turn_attribute_lifetime_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga97892194ee263338e36c11d02d3c5ea2</anchor>
      <arglist>(uint32_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>tnet_turn_attribute_xpeer_addr_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gaf66bebd5bd8319c454a450dc092973c5</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_data_t *</type>
      <name>tnet_turn_attribute_data_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga6e749efbc0e6c2d181b7b5572dd4a84a</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_xrelayed_addr_t *</type>
      <name>tnet_turn_attribute_xrelayed_addr_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga27dd53bb4f5afd8fbe1b3c6450764d68</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_even_port_t *</type>
      <name>tnet_turn_attribute_even_port_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga9201f3b08080e3d9d73d91cce22fdb6b</anchor>
      <arglist>(unsigned R)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_reqtrans_t *</type>
      <name>tnet_turn_attribute_reqtrans_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga0f8e1e1c00b3c62c9c451ad2dbf8419e</anchor>
      <arglist>(tnet_proto_t protocol)</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_dontfrag_t *</type>
      <name>tnet_turn_attribute_dontfrag_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gab1baec0bd9b214f816737ccaf36846a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tnet_turn_attribute_restoken_t *</type>
      <name>tnet_turn_attribute_restoken_create</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga498409f84c06d567eeeeb913b01117c8</anchor>
      <arglist>(const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>tnet_stun_attribute_t *</type>
      <name>tnet_turn_attribute_deserialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>gae0fac81dd69579c064501d497cd03ea8</anchor>
      <arglist>(tnet_stun_attribute_type_t type, uint16_t length, const void *payload, tsk_size_t payload_size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tnet_turn_attribute_serialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga5c64288790f379c04d8aae14117e305c</anchor>
      <arglist>(const tnet_stun_attribute_t *attribute, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>tsk_buffer_t *</type>
      <name>tnet_turn_channel_data_serialize</name>
      <anchorfile>group__tnet__turn__group.html</anchorfile>
      <anchor>ga00439f15cae25f78c4fd515c2ad689fc</anchor>
      <arglist>(const tnet_turn_channel_data_t *message)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_address_s</name>
    <filename>structtnet__address__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_family_t</type>
      <name>family</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>aa9ac1283cbbf630bf8c60c5ef94f3efd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>unicast</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>abc788348c8761780614d2cc33a39f236</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>anycast</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>a829249b4ca698adfb1ace5bf60a8256e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>multicast</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>a47a0efb9804fcd897e6eb6f2ae307a44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>dnsserver</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>a9609c0c948615942826bd777e1397b7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ip</name>
      <anchorfile>structtnet__address__s.html</anchorfile>
      <anchor>afbc356cd0e25d1dbbece7c10fd025fa6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_ctx_s</name>
    <filename>structtnet__dhcp6__ctx__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>pen</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>ae2a65bb759a6405a5bf6e5de54b89d93</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>vendor_class_data</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>aab350594eb87cee0541230dca6312a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timeout</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>a053cdea1d85795444fe1aaa6b277a0ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>port_client</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>a834bf951592c252db10122108915b42a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>server_port</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>a35123c5d12252e9bbc6950e97753deb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_interfaces_L_t *</type>
      <name>interfaces</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>a8156a841e9546fd0b833dcdc5f23af32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtnet__dhcp6__ctx__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_duid_en_s</name>
    <filename>structtnet__dhcp6__duid__en__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP6_DUID</name>
      <anchorfile>structtnet__dhcp6__duid__en__s.html</anchorfile>
      <anchor>a7d5ca788939104cbabceb0d3f5c87f12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>en</name>
      <anchorfile>structtnet__dhcp6__duid__en__s.html</anchorfile>
      <anchor>a97a960f785c16a971f6c741af5c6ec2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>indentifier</name>
      <anchorfile>structtnet__dhcp6__duid__en__s.html</anchorfile>
      <anchor>a1c2883892c8c127410362f7d7cb9b7bb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_duid_ll_s</name>
    <filename>structtnet__dhcp6__duid__ll__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP6_DUID</name>
      <anchorfile>structtnet__dhcp6__duid__ll__s.html</anchorfile>
      <anchor>a7d5ca788939104cbabceb0d3f5c87f12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_hardware_type_t</type>
      <name>htype</name>
      <anchorfile>structtnet__dhcp6__duid__ll__s.html</anchorfile>
      <anchor>af11ee3ba27d63138ab8f3d9d83ff4e31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>address</name>
      <anchorfile>structtnet__dhcp6__duid__ll__s.html</anchorfile>
      <anchor>a0d802bc12d5854847214fa612961d6cc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_duid_llt_s</name>
    <filename>structtnet__dhcp6__duid__llt__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP6_DUID</name>
      <anchorfile>structtnet__dhcp6__duid__llt__s.html</anchorfile>
      <anchor>a7d5ca788939104cbabceb0d3f5c87f12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_hardware_type_t</type>
      <name>htype</name>
      <anchorfile>structtnet__dhcp6__duid__llt__s.html</anchorfile>
      <anchor>af11ee3ba27d63138ab8f3d9d83ff4e31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>time</name>
      <anchorfile>structtnet__dhcp6__duid__llt__s.html</anchorfile>
      <anchor>ae73654f333e4363463ad8c594eca1905</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>address</name>
      <anchorfile>structtnet__dhcp6__duid__llt__s.html</anchorfile>
      <anchor>a0d802bc12d5854847214fa612961d6cc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_duid_s</name>
    <filename>structtnet__dhcp6__duid__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp6__duid__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>initialized</name>
      <anchorfile>structtnet__dhcp6__duid__s.html</anchorfile>
      <anchor>adfdbb2f146565c87384f0090dba7a73e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp6_duid_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__dhcp6__duid__s.html</anchorfile>
      <anchor>ae0ae09e4fc714e06fdc68d480e758dff</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_message_s</name>
    <filename>structtnet__dhcp6__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp6__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp6_message_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__dhcp6__message__s.html</anchorfile>
      <anchor>a21e3b33868df6c3f411acbc6b4b8a170</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>transaction_id</name>
      <anchorfile>structtnet__dhcp6__message__s.html</anchorfile>
      <anchor>a87f9e7ddbb607ed91afc2e1c7c9a4a52</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp6_options_L_t *</type>
      <name>options</name>
      <anchorfile>structtnet__dhcp6__message__s.html</anchorfile>
      <anchor>a88b35352ad3437f608fef155b490245d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_option_data_s</name>
    <filename>structtnet__dhcp6__option__data__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp6__option__data__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_option_identifier_s</name>
    <filename>structtnet__dhcp6__option__identifier__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP6_OPTION_DATA</name>
      <anchorfile>structtnet__dhcp6__option__identifier__s.html</anchorfile>
      <anchor>a28e22ea1fe86a562264c53f323dea9ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp6_duid_t *</type>
      <name>duid</name>
      <anchorfile>structtnet__dhcp6__option__identifier__s.html</anchorfile>
      <anchor>a4b7a10003083e125dcd7c5df5d9da9b0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_option_orequest_s</name>
    <filename>structtnet__dhcp6__option__orequest__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP6_OPTION_DATA</name>
      <anchorfile>structtnet__dhcp6__option__orequest__s.html</anchorfile>
      <anchor>a28e22ea1fe86a562264c53f323dea9ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>codes</name>
      <anchorfile>structtnet__dhcp6__option__orequest__s.html</anchorfile>
      <anchor>ab146c1f395904867fc1f7b8e98dd04a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_option_s</name>
    <filename>structtnet__dhcp6__option__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp6__option__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp6_option_code_t</type>
      <name>code</name>
      <anchorfile>structtnet__dhcp6__option__s.html</anchorfile>
      <anchor>a7537e49643e085c23e4f9d9f6976d5b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>len</name>
      <anchorfile>structtnet__dhcp6__option__s.html</anchorfile>
      <anchor>a8aed22e2c7b283705ec82e0120515618</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp6_option_data_t *</type>
      <name>data</name>
      <anchorfile>structtnet__dhcp6__option__s.html</anchorfile>
      <anchor>a600e03a1fd89a07d22a9f21611282519</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp6_option_vendorclass_s</name>
    <filename>structtnet__dhcp6__option__vendorclass__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP6_OPTION_DATA</name>
      <anchorfile>structtnet__dhcp6__option__vendorclass__s.html</anchorfile>
      <anchor>a28e22ea1fe86a562264c53f323dea9ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>enterprise_number</name>
      <anchorfile>structtnet__dhcp6__option__vendorclass__s.html</anchorfile>
      <anchor>a5162ea2453bd20ec2797bafba71a9fdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>vendor_class_data</name>
      <anchorfile>structtnet__dhcp6__option__vendorclass__s.html</anchorfile>
      <anchor>a20c300f54f6b4dd9cafecd09a4380b15</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_ctx_s</name>
    <filename>structtnet__dhcp__ctx__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>vendor_id</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a73f47ae0f7d0ed50c56505b044790bb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>hostname</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>af203df082d5c6dcaa0c88b07cf86466d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>max_msg_size</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a8966178c69603c221293557d17f22920</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timeout</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a053cdea1d85795444fe1aaa6b277a0ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>port_client</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a834bf951592c252db10122108915b42a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>server_port</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a35123c5d12252e9bbc6950e97753deb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_interfaces_L_t *</type>
      <name>interfaces</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a8156a841e9546fd0b833dcdc5f23af32</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtnet__dhcp__ctx__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_message_s</name>
    <filename>structtnet__dhcp__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp_message_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>af4ceb1a7342479133b25455d3281b2ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp_message_op_t</type>
      <name>op</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>abbaa2226b43198cf6ceca447d386679d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_hardware_type_t</type>
      <name>htype</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>af11ee3ba27d63138ab8f3d9d83ff4e31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>hlen</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>aff25a69752648654e56c4ff3e0b1b226</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>hops</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a2719bad26c6e7de2bc08439cea7111ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>xid</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>ad031a042dfee0c8192ef5e606c7a1354</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>secs</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a3836e59e9ca2b5b77a89533388b6458f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>flags</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a1e87af3c18a2fd36c61faf89949bdc3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>ciaddr</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a0952ad95e626f53511afa5180222264b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>yiaddr</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a3cb44d0ce71c3b7201c22a41c00da379</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>siaddr</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>af938402cffa168e565c29cbc227a220a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>giaddr</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a2719c154fb9039aa55b367468df53de0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>chaddr</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a985fe29677d6c749d7471514cb744904</anchor>
      <arglist>[16]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>sname</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a197375c976d67dae5090e0065ead720c</anchor>
      <arglist>[64]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>file</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>a5c7003134e7a7486271d4f33c2e7b7de</anchor>
      <arglist>[128]</arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp_options_L_t *</type>
      <name>options</name>
      <anchorfile>structtnet__dhcp__message__s.html</anchorfile>
      <anchor>aaef79cd8df1bc83440d0b8652d0c9c4a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_option_dns_s</name>
    <filename>structtnet__dhcp__option__dns__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP_OPTION</name>
      <anchorfile>structtnet__dhcp__option__dns__s.html</anchorfile>
      <anchor>a76ae8d097b84b331a595990bed8b5c36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_strings_L_t *</type>
      <name>servers</name>
      <anchorfile>structtnet__dhcp__option__dns__s.html</anchorfile>
      <anchor>a34baae61a8169fa438d42973816211c6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_option_paramslist_s</name>
    <filename>structtnet__dhcp__option__paramslist__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP_OPTION</name>
      <anchorfile>structtnet__dhcp__option__paramslist__s.html</anchorfile>
      <anchor>a76ae8d097b84b331a595990bed8b5c36</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_option_s</name>
    <filename>structtnet__dhcp__option__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp__option__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>initialized</name>
      <anchorfile>structtnet__dhcp__option__s.html</anchorfile>
      <anchor>adfdbb2f146565c87384f0090dba7a73e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp_option_code_t</type>
      <name>code</name>
      <anchorfile>structtnet__dhcp__option__s.html</anchorfile>
      <anchor>af5b9e621afaa9b16052826373021e507</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>value</name>
      <anchorfile>structtnet__dhcp__option__s.html</anchorfile>
      <anchor>a9bcf56196da97bfeb9b5411f2a5e07f6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_option_sip_s</name>
    <filename>structtnet__dhcp__option__sip__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DHCP_OPTION</name>
      <anchorfile>structtnet__dhcp__option__sip__s.html</anchorfile>
      <anchor>a76ae8d097b84b331a595990bed8b5c36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_strings_L_t *</type>
      <name>servers</name>
      <anchorfile>structtnet__dhcp__option__sip__s.html</anchorfile>
      <anchor>a34baae61a8169fa438d42973816211c6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dhcp_params_s</name>
    <filename>structtnet__dhcp__params__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dhcp__params__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dhcp_option_code_t</type>
      <name>codes</name>
      <anchorfile>structtnet__dhcp__params__s.html</anchorfile>
      <anchor>a1be3fd6101ab9b0e039d4cf30e79cc46</anchor>
      <arglist>[TNET_DHCP_MAX_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>codes_count</name>
      <anchorfile>structtnet__dhcp__params__s.html</anchorfile>
      <anchor>a36227aefbb616902b06fbf9fb123123e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_a_s</name>
    <filename>structtnet__dns__a__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__a__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>address</name>
      <anchorfile>structtnet__dns__a__s.html</anchorfile>
      <anchor>a879a8cdf605d02f8af8b2e216b8764f2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_aaaa_s</name>
    <filename>structtnet__dns__aaaa__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__aaaa__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>address</name>
      <anchorfile>structtnet__dns__aaaa__s.html</anchorfile>
      <anchor>a879a8cdf605d02f8af8b2e216b8764f2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_cache_entry_s</name>
    <filename>structtnet__dns__cache__entry__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dns__cache__entry__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>qname</name>
      <anchorfile>structtnet__dns__cache__entry__s.html</anchorfile>
      <anchor>a5a317b1553299b3cbe3ef6d6d2edcffc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_qclass_t</type>
      <name>qclass</name>
      <anchorfile>structtnet__dns__cache__entry__s.html</anchorfile>
      <anchor>a15b4f97c9e5e000874a08a001eeb7733</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_qtype_t</type>
      <name>qtype</name>
      <anchorfile>structtnet__dns__cache__entry__s.html</anchorfile>
      <anchor>a3a9ecb9a634fe9bb31a40188e63a0370</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>epoch</name>
      <anchorfile>structtnet__dns__cache__entry__s.html</anchorfile>
      <anchor>a7d1c4feed1318aeb0b96f657d87cd1ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_response_t *</type>
      <name>response</name>
      <anchorfile>structtnet__dns__cache__entry__s.html</anchorfile>
      <anchor>ac9ec66f6ecc7aa0aeddcd20828199d68</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_cname_s</name>
    <filename>structtnet__dns__cname__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__cname__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cname</name>
      <anchorfile>structtnet__dns__cname__s.html</anchorfile>
      <anchor>a6b787eeec4924a37fc531cd8b950d9f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_ctx_s</name>
    <filename>structtnet__dns__ctx__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timeout</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a053cdea1d85795444fe1aaa6b277a0ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>recursion</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a97e982c6f7d2346cb1d95160a6667c3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>edns0</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>ae4004c3075864042e3cc5cfc0ca333b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>caching</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a3c98bef376fb846129f5ea3f78eddb98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>cache_ttl</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a4000992e7b3bb97836c9ab0c296c5c64</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>server_port</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a35123c5d12252e9bbc6950e97753deb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_cache_t *</type>
      <name>cache</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>adac05d42fe5d85b2b1b7eaddd61d89d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_addresses_L_t *</type>
      <name>servers</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>af91849b97bc1300e93e88f65a4ee44b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtnet__dns__ctx__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_message_s</name>
    <filename>structtnet__dns__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dns__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_dns_message_s::@0</type>
      <name>Header</name>
      <anchorfile>structtnet__dns__message__s.html</anchorfile>
      <anchor>a3fb15af2e8eecc6a97afbe7f5e27980f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_dns_message_s::@1</type>
      <name>Question</name>
      <anchorfile>structtnet__dns__message__s.html</anchorfile>
      <anchor>ab11a7098fcf552ad821a90a5ba6f4fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_rrs_L_t *</type>
      <name>Answers</name>
      <anchorfile>structtnet__dns__message__s.html</anchorfile>
      <anchor>a67f770a7758696b3e5a5809eff26fb91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_rrs_L_t *</type>
      <name>Authorities</name>
      <anchorfile>structtnet__dns__message__s.html</anchorfile>
      <anchor>a07b3c715e236c3e00b3db70b46857c8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_rrs_L_t *</type>
      <name>Additionals</name>
      <anchorfile>structtnet__dns__message__s.html</anchorfile>
      <anchor>a04fa4444757b7ded3f659e622b5a2924</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_mx_s</name>
    <filename>structtnet__dns__mx__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__mx__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>preference</name>
      <anchorfile>structtnet__dns__mx__s.html</anchorfile>
      <anchor>a0895a8ef7de652dbf7ac2ece95f267df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>exchange</name>
      <anchorfile>structtnet__dns__mx__s.html</anchorfile>
      <anchor>ae456886310fcf4a36c0d7a789ad3cb69</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_naptr_s</name>
    <filename>structtnet__dns__naptr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>order</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>afd05883aec8e6070b12674a77c541025</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>preference</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>a0895a8ef7de652dbf7ac2ece95f267df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>flags</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>a4db5c33b255239e4bc2f302bc34a5339</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>services</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>aa5bac2721b92fccd4e1ad17978c69ec4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>regexp</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>ac1d9b998a1dabf12492d943e3bb765bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>replacement</name>
      <anchorfile>structtnet__dns__naptr__s.html</anchorfile>
      <anchor>ac92b8d0fe1aa141a4ae8c7d0c0dae69b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_ns_s</name>
    <filename>structtnet__dns__ns__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__ns__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nsdname</name>
      <anchorfile>structtnet__dns__ns__s.html</anchorfile>
      <anchor>a1d173c3317758caf3501a48cb6c0cf48</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_opt_s</name>
    <filename>structtnet__dns__opt__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__opt__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_ptr_s</name>
    <filename>structtnet__dns__ptr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__ptr__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ptrdname</name>
      <anchorfile>structtnet__dns__ptr__s.html</anchorfile>
      <anchor>a243ff266ee3a887c216f55296768f10d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_rr_s</name>
    <filename>structtnet__dns__rr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>initialized</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>adfdbb2f146565c87384f0090dba7a73e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>a5ac083a645d964373f022d03df4849c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_qtype_t</type>
      <name>qtype</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>a3a9ecb9a634fe9bb31a40188e63a0370</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dns_qclass_t</type>
      <name>qclass</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>a15b4f97c9e5e000874a08a001eeb7733</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>ttl</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>adcbcef3ebe212c2b2b6c75f10d9186c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>rdlength</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>af06969277715677d7441c9b816daa2ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>rpdata</name>
      <anchorfile>structtnet__dns__rr__s.html</anchorfile>
      <anchor>a5c9a31069ba9dc307f5c3c383058873b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_soa_s</name>
    <filename>structtnet__dns__soa__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>mname</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>a8721eff2350ed6076b3b331690c9eb67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>rname</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>a4abc08d8bf8e71d81f1928b77675d9a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>serial</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>a1149ca4bd0659030412db8e77bebdf88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>refresh</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>a3185284e70c8e62827fa3c7932b82ce8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>retry</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>ae544e10bfc5a36197ae704603a9a6205</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>expire</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>ad2736d3cd8e2ca2d37da2b69cb721134</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>minimum</name>
      <anchorfile>structtnet__dns__soa__s.html</anchorfile>
      <anchor>a783317953fb3acf817a73a8af09d3636</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_srv_s</name>
    <filename>structtnet__dns__srv__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__srv__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>priority</name>
      <anchorfile>structtnet__dns__srv__s.html</anchorfile>
      <anchor>a0815784d41b3c13d42ce22367abfba1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>weight</name>
      <anchorfile>structtnet__dns__srv__s.html</anchorfile>
      <anchor>a3c304b42c785c4cc5fa46842e2845a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>port</name>
      <anchorfile>structtnet__dns__srv__s.html</anchorfile>
      <anchor>a8e0798404bf2cf5dabb84c5ba9a4f236</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>target</name>
      <anchorfile>structtnet__dns__srv__s.html</anchorfile>
      <anchor>a23b26cdb3a71f525caf03b57f68d47fa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dns_txt_s</name>
    <filename>structtnet__dns__txt__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_DECLARE_DNS_RR</name>
      <anchorfile>structtnet__dns__txt__s.html</anchorfile>
      <anchor>a5c892c7c008cc59b0067d98a30c533cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>txt_data</name>
      <anchorfile>structtnet__dns__txt__s.html</anchorfile>
      <anchor>aaa858f1e56c43a4400f94586c9ca8712</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_dtls_socket_s</name>
    <filename>structtnet__dtls__socket__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>fd</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a89776c821e7cb62a4acac46798a84137</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>verify_peer</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>ac3bbf4e3b1bc3ea9c47f0fc43969dac2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>use_srtp</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a07a7a29b74a7cecdc4392e6aa547005f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>handshake_completed</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a9d9b346395e6d39d82414e5dbbc99dac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dtls_setup_t</type>
      <name>setup</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a45067f39a61d7d389a19739fd1e59b99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_dtls_socket_s::@4</type>
      <name>cb</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>adba381cb3517a407757cb0bacf239fbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_dtls_socket_s::@5</type>
      <name>remote</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a2d62e3cc37a43c42b2e350fc1d70381b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_dtls_socket_s::@6</type>
      <name>local</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a654feace44e7c42b656ff47695fa3209</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtnet__dtls__socket__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_ice_action_s</name>
    <filename>structtnet__ice__action__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__ice__action__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_fsm_action_id</type>
      <name>id</name>
      <anchorfile>structtnet__ice__action__s.html</anchorfile>
      <anchor>afa188fbd86824e693af7d82baf1831a7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_ice_candidate_s</name>
    <filename>structtnet__ice__candidate__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_cand_type_t</type>
      <name>type_e</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>ab7208769aa8829731c5cfdc820849f1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>foundation</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a7dc7989fc6a4901626924310cebadbac</anchor>
      <arglist>[33]</arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>comp_id</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a30d7364b567d7c341a5dc3f602136363</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>transport_str</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a16edd675ed7c739a518c7df27ca57d82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>enum tnet_socket_type_e</type>
      <name>transport_e</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>ae4e142d80ab962928fc48710ebc9256e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>priority</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a90249de64da5ae5d7acd34da7ea1b857</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cand_type_str</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>abee5149305f31cc6ed5e356a5b9547f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ip_t</type>
      <name>connection_addr</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>ae862117942242057beb16b59e568fcd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>port</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a804003d8ce1732a48fc0a8d3f44fcc1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_params_L_t *</type>
      <name>extension_att_list</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a851450e616fa22e16448808be6e030a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_ice_jingle</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a2f30e8927ccd387faaf5628d115ae17c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_rtp</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a910d991f863a83bc7f53ef12bc174ce5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_video</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>aa32a2ad18730f160578396d76bc9cbb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>local_pref</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>ab9c182df2591c34c8185b98cb0360252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ufrag</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a3b4a16bb0327f02abc59505b25e77bb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>pwd</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a4be832ac49cb1384b79b9423028f1d54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_socket_s *</type>
      <name>socket</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>aa6ef92be590a80571ef2297ef5a3479d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_ice_candidate_s::@2</type>
      <name>stun</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>a52c3b5eb6826c5338d8b5e3c177a30fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>tostring</name>
      <anchorfile>structtnet__ice__candidate__s.html</anchorfile>
      <anchor>ac53a959d1e2819b4dae0c9b5d4f8f5d8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_ice_ctx_s</name>
    <filename>structtnet__ice__ctx__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_RUNNABLE</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>ae8fba71ac54cd6376ba9e5ed1cc87cf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_started</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>aceed659b1d0a7bdb1ddcc2480b6273f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_active</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>ac8e3cacbc0f857762970f6b9be72a4e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_callback_f</type>
      <name>callback</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a5fc2712f91a0b561cbd5232e8506c3cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>userdata</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a2aa76a7bfd06a16154eccdd30d6f6393</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>use_ipv6</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a558e28da1766e51b8cba545747aa9eed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>use_rtcp</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>ae3425b99ca7b44979bb44d6f46356238</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>use_rtcpmux</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a3ff7682ebd9100401e4344ddc38779e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_video</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>aa32a2ad18730f160578396d76bc9cbb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>unicast</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>ab849120f12135af90808a5a3cee69aac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>anycast</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>aee319295484f210ff8d76bd12c1c749d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>multicast</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a0721609fcddaa98a4c47a48b64faaabe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_controlling</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a505b4f2717219bc9a55dd48dc8753d8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_ice_jingle</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a2f30e8927ccd387faaf5628d115ae17c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>tie_breaker</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a5581ce626352db2070cae43032c963a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>concheck_timeout</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a35765e5bbc5be1762e99e6fd0b436793</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>rtp_callback_data</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a6d77826979298b328326b282bd787899</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_rtp_callback_f</type>
      <name>rtp_callback</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a43dfd07c697f57c396f8f04d542deeb6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ufrag</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a3b4a16bb0327f02abc59505b25e77bb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>pwd</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a4be832ac49cb1384b79b9423028f1d54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_timer_manager_handle_t *</type>
      <name>h_timer_mgr</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a5fd624501a8012544ccaf1bfbe9cfe6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_fsm_t *</type>
      <name>fsm</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a0cc2337becbdef2e7ea539f61019fdbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_candidates_L_t *</type>
      <name>candidates_local</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a6a3700734a198d6c04c5a7f428f66c35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_candidates_L_t *</type>
      <name>candidates_remote</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a87907f26f08ddd55e8d2bd65f7454ad4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_pairs_L_t *</type>
      <name>candidates_pairs</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a3f0d3e9fc285be5693fe2e5f14ae682c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>have_nominated_offer</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>ab820474c96a4e2752af92ae3c5352052</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>have_nominated_answer</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a78cff6175205e1c23e536a9d09220624</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>have_nominated_symetric</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a7bc095414b42e4af61747c3f4c037789</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>RTO</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>aebe0faf98d3286e1406eb88b460f0808</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>Rc</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a4df78cbd063bbc725d99e2f80b391cb1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_ice_ctx_s::@3</type>
      <name>stun</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a9c0864c5426d106b90ee56f76cd5e92e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtnet__ice__ctx__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_ice_event_s</name>
    <filename>structtnet__ice__event__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__ice__event__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_event_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__ice__event__s.html</anchorfile>
      <anchor>ad65eebeead91a113dfd8c7a5c8f82b0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>phrase</name>
      <anchorfile>structtnet__ice__event__s.html</anchorfile>
      <anchor>a601977f3c7b2d461e3723ca8ef7926f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_ice_action_s *</type>
      <name>action</name>
      <anchorfile>structtnet__ice__event__s.html</anchorfile>
      <anchor>a0ecb51cb1b029389ef4257b23dd9b075</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_ice_ctx_s *</type>
      <name>ctx</name>
      <anchorfile>structtnet__ice__event__s.html</anchorfile>
      <anchor>a5c998d86fc92f46033f738f1f761ef4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>userdata</name>
      <anchorfile>structtnet__ice__event__s.html</anchorfile>
      <anchor>a2aa76a7bfd06a16154eccdd30d6f6393</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_ice_pair_s</name>
    <filename>structtnet__ice__pair__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>priority</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>af43b7b00bdbaf3d0602086885a410899</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_pair_state_t</type>
      <name>state_offer</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>af268a8c411b9f018e0254ed4da093825</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ice_pair_state_t</type>
      <name>state_answer</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a220b0b99453f807075ad530c39756343</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_ice_jingle</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a2f30e8927ccd387faaf5628d115ae17c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>is_controlling</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a505b4f2717219bc9a55dd48dc8753d8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>tie_breaker</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a5581ce626352db2070cae43032c963a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_ice_candidate_s *</type>
      <name>candidate_offer</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a438bfe5089fc56f5297a5f40adc199be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_ice_candidate_s *</type>
      <name>candidate_answer</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>adbb3f8f093eed2e9995a74dbe3ada407</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_stun_message_s *</type>
      <name>last_request</name>
      <anchorfile>structtnet__ice__pair__s.html</anchorfile>
      <anchor>a9b53797dee1dd1569721b615a573aaab</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_interface_s</name>
    <filename>structtnet__interface__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__interface__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>index</name>
      <anchorfile>structtnet__interface__s.html</anchorfile>
      <anchor>a360ebf6209d16512186ca7adb893abbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>description</name>
      <anchorfile>structtnet__interface__s.html</anchorfile>
      <anchor>a8444d6e0dfe2bbab0b5e7b24308f1559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>mac_address</name>
      <anchorfile>structtnet__interface__s.html</anchorfile>
      <anchor>a2571db5ff6bc43c1d6c4f3b9c8aefd87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>mac_address_length</name>
      <anchorfile>structtnet__interface__s.html</anchorfile>
      <anchor>a9d62b42964509e35907e16d57ab116a5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_nat_context_s</name>
    <filename>structtnet__nat__context__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_socket_type_t</type>
      <name>socket_type</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a27db52c3553ad7f3a50a3f846128f5f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>username</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a9b20c006bd90a09e1465fb668700e81d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>password</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a59460a3ff2c12443d1022e5cc0fba85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>software</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>ab7dea44af93feeae91fbc350466cd75b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>server_address</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>afbcb0d71ddd626fabdbf6646e7db8b99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>server_port</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a35123c5d12252e9bbc6950e97753deb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>RTO</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>aebe0faf98d3286e1406eb88b460f0808</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>Rc</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a4df78cbd063bbc725d99e2f80b391cb1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>enable_dontfrag</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>ad92b1c73d8fc7717638c6a6785b0e1b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>enable_integrity</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a55e74a4b337b7b26c1b9edee06fec693</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>enable_evenport</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a43b8469c663698dd9c1c409de1c2452b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>enable_fingerprint</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>aaf103f3994e9b9c4b13612755ccb61d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>use_dnsquery</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>a37ce943df13721eabe344a56caab6135</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_allocations_L_t *</type>
      <name>allocations</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>ab1bc1c207c419d587ddc9272a48c84ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_bindings_L_t *</type>
      <name>stun_bindings</name>
      <anchorfile>structtnet__nat__context__s.html</anchorfile>
      <anchor>ac49251b98f3dc1c92f5d2293e9badb8c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_socket_s</name>
    <filename>structtnet__socket__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_socket_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>ae2a94a75704f25f33d3f032e34feb205</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>fd</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>a89776c821e7cb62a4acac46798a84137</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_ip_t</type>
      <name>ip</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>a520b46ef780bbe6f719ca59c3bfecd74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>port</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>a8e0798404bf2cf5dabb84c5ba9a4f236</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_tls_socket_handle_t *</type>
      <name>tlshandle</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>afb9a3122a38768304a5134beb27e1298</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_dtls_socket_handle_t *</type>
      <name>dtlshandle</name>
      <anchorfile>structtnet__socket__s.html</anchorfile>
      <anchor>aa0089985f6797dd810fd77090fbdd2b2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_altserver_s</name>
    <filename>structtnet__stun__attribute__altserver__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__altserver__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_addr_family_t</type>
      <name>family</name>
      <anchorfile>structtnet__stun__attribute__altserver__s.html</anchorfile>
      <anchor>a49f24db0b2265a9d515199d570dd4e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>port</name>
      <anchorfile>structtnet__stun__attribute__altserver__s.html</anchorfile>
      <anchor>a8e0798404bf2cf5dabb84c5ba9a4f236</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>server</name>
      <anchorfile>structtnet__stun__attribute__altserver__s.html</anchorfile>
      <anchor>af4d154cbfef4303a47869a02cd589624</anchor>
      <arglist>[128]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_errorcode_s</name>
    <filename>structtnet__stun__attribute__errorcode__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__errorcode__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>_class</name>
      <anchorfile>structtnet__stun__attribute__errorcode__s.html</anchorfile>
      <anchor>a1e336cb34c9f32b8d3b30ef50e4d5212</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>number</name>
      <anchorfile>structtnet__stun__attribute__errorcode__s.html</anchorfile>
      <anchor>af57eaec6f3dec6de717735725c9908a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>reason_phrase</name>
      <anchorfile>structtnet__stun__attribute__errorcode__s.html</anchorfile>
      <anchor>a18bf51f0b416cc27103a7a47db3442de</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_fingerprint_s</name>
    <filename>structtnet__stun__attribute__fingerprint__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__fingerprint__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__fingerprint__s.html</anchorfile>
      <anchor>ae7f66047e6e39ba2bb6af8b95f00d1dd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_ice_controlled_s</name>
    <filename>structtnet__stun__attribute__ice__controlled__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__ice__controlled__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__ice__controlled__s.html</anchorfile>
      <anchor>a4e630859cc0e2a22bd6acf39a6a8e218</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_ice_controlling_s</name>
    <filename>structtnet__stun__attribute__ice__controlling__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__ice__controlling__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__ice__controlling__s.html</anchorfile>
      <anchor>a4e630859cc0e2a22bd6acf39a6a8e218</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_ice_priority_s</name>
    <filename>structtnet__stun__attribute__ice__priority__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__ice__priority__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__ice__priority__s.html</anchorfile>
      <anchor>ae7f66047e6e39ba2bb6af8b95f00d1dd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_ice_use_candidate_s</name>
    <filename>structtnet__stun__attribute__ice__use__candidate__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__ice__use__candidate__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_integrity_s</name>
    <filename>structtnet__stun__attribute__integrity__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__integrity__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_sha1digest_t</type>
      <name>sha1digest</name>
      <anchorfile>structtnet__stun__attribute__integrity__s.html</anchorfile>
      <anchor>a7c7001f4d433128aec28db91a69bc8ed</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_mapped_addr_s</name>
    <filename>structtnet__stun__attribute__mapped__addr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__mapped__addr__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_addr_family_t</type>
      <name>family</name>
      <anchorfile>structtnet__stun__attribute__mapped__addr__s.html</anchorfile>
      <anchor>a49f24db0b2265a9d515199d570dd4e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>port</name>
      <anchorfile>structtnet__stun__attribute__mapped__addr__s.html</anchorfile>
      <anchor>a8e0798404bf2cf5dabb84c5ba9a4f236</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>address</name>
      <anchorfile>structtnet__stun__attribute__mapped__addr__s.html</anchorfile>
      <anchor>ae4879450c8041e3ab7fd9d958557f826</anchor>
      <arglist>[16]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_nonce_s</name>
    <filename>structtnet__stun__attribute__nonce__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__nonce__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__nonce__s.html</anchorfile>
      <anchor>a4e9aec275e566b978a3ccb4e043d8c61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_realm_s</name>
    <filename>structtnet__stun__attribute__realm__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__realm__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__realm__s.html</anchorfile>
      <anchor>a4e9aec275e566b978a3ccb4e043d8c61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_s</name>
    <filename>structtnet__stun__attribute__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__stun__attribute__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_attribute_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__stun__attribute__s.html</anchorfile>
      <anchor>a612b04c5b268170d0fb2ac0ef564ed83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>length</name>
      <anchorfile>structtnet__stun__attribute__s.html</anchorfile>
      <anchor>a1892eba2086d12ac2b09005aeb09ea3b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_software_s</name>
    <filename>structtnet__stun__attribute__software__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__software__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__software__s.html</anchorfile>
      <anchor>a4e9aec275e566b978a3ccb4e043d8c61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_unknowns_s</name>
    <filename>structtnet__stun__attribute__unknowns__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__unknowns__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__unknowns__s.html</anchorfile>
      <anchor>a9bcf56196da97bfeb9b5411f2a5e07f6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_username_s</name>
    <filename>structtnet__stun__attribute__username__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__username__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>structtnet__stun__attribute__username__s.html</anchorfile>
      <anchor>a4e9aec275e566b978a3ccb4e043d8c61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_attribute_xmapped_addr_s</name>
    <filename>structtnet__stun__attribute__xmapped__addr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__stun__attribute__xmapped__addr__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_addr_family_t</type>
      <name>family</name>
      <anchorfile>structtnet__stun__attribute__xmapped__addr__s.html</anchorfile>
      <anchor>a49f24db0b2265a9d515199d570dd4e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>xport</name>
      <anchorfile>structtnet__stun__attribute__xmapped__addr__s.html</anchorfile>
      <anchor>ac021791c2ad249b108a5155d4c252a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>xaddress</name>
      <anchorfile>structtnet__stun__attribute__xmapped__addr__s.html</anchorfile>
      <anchor>ac365f9f3b4cedc84224afd91b2597ccb</anchor>
      <arglist>[16]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_binding_s</name>
    <filename>structtnet__stun__binding__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_binding_id_t</type>
      <name>id</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a68541ba67a916edaa34f4da31009b97f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>username</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a9b20c006bd90a09e1465fb668700e81d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>password</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a59460a3ff2c12443d1022e5cc0fba85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>realm</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a78e609bc32bd5a9820209fe62b39963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nonce</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a814af5af8d9ea73885c8216c0e547e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>software</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>ab7dea44af93feeae91fbc350466cd75b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>localFD</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a2f7fda9a08aade674a360b301268cd92</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_socket_type_t</type>
      <name>socket_type</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a27db52c3553ad7f3a50a3f846128f5f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct sockaddr_storage</type>
      <name>server</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>acbcb80535e518b7b9f636338f948b88c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_attribute_mapped_addr_t *</type>
      <name>maddr</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a1bb895a0f6e6fbe93ac0bc346bc52526</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_attribute_xmapped_addr_t *</type>
      <name>xmaddr</name>
      <anchorfile>structtnet__stun__binding__s.html</anchorfile>
      <anchor>a5405872105ef7f32cabddc425993f69a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_stun_message_s</name>
    <filename>structtnet__stun__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_message_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>adc708b7d327e9a9f6e5d1a9f16fe1749</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>length</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a1892eba2086d12ac2b09005aeb09ea3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>cookie</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>af9c930e4206148386b1d991e325a132d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_transacid_t</type>
      <name>transaction_id</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>ab4cd124218c6c7c7ddf534b043600832</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>fingerprint</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a493cb3f41b0f9ef0178425b3fbde3ab1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>integrity</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a84c2663b2f0442ae8a425bc5f30bda7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>dontfrag</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>ab49b896e3ea7fc8cf5f432b267defcb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>nointegrity</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a5c9b21c563ac7ab9fda61a7e93a34250</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>username</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a9b20c006bd90a09e1465fb668700e81d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>password</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a59460a3ff2c12443d1022e5cc0fba85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>realm</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a78e609bc32bd5a9820209fe62b39963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nonce</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>a814af5af8d9ea73885c8216c0e547e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_attributes_L_t *</type>
      <name>attributes</name>
      <anchorfile>structtnet__stun__message__s.html</anchorfile>
      <anchor>ae438e8c15b79d8a020645d347bfa1b07</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_tls_socket_s</name>
    <filename>structtnet__tls__socket__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__tls__socket__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>fd</name>
      <anchorfile>structtnet__tls__socket__s.html</anchorfile>
      <anchor>a89776c821e7cb62a4acac46798a84137</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtnet__tls__socket__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_transport_event_s</name>
    <filename>structtnet__transport__event__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_transport_event_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>a312995fc2313cbfd77bf91982e33fb20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>data</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>a735984d41155bc1032e09bece8f8d66d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>size</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>a2af13611e30c0884ab2e1837c4da8282</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>callback_data</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>a7322346207ed833d5550334332388ff9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>local_fd</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>afa7f2e7bc012cbcc028e247c726043fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct sockaddr_storage</type>
      <name>remote_addr</name>
      <anchorfile>structtnet__transport__event__s.html</anchorfile>
      <anchor>ae0831b58d62cf11710b2638982259cfb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_transport_s</name>
    <filename>structtnet__transport__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_RUNNABLE</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>ae8fba71ac54cd6376ba9e5ed1cc87cf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_socket_type_t</type>
      <name>type</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>ae2a94a75704f25f33d3f032e34feb205</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>local_ip</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a39812ff38e55c55387e819985e9f0d20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>local_host</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a5581c3fd932c186d16c55dd83499788b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>req_local_port</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>af89530ad71276773e469c5e90ae18d6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_port_t</type>
      <name>bind_local_port</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>ae57f3ae4b492ff17c9aed39764016540</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_nat_context_handle_t *</type>
      <name>natt_ctx</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a4e40b6b4f2c827184bbfca2abafb683b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_socket_t *</type>
      <name>master</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a451156669819ac6a2cad58a45835c391</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_object_t *</type>
      <name>context</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a59bd43e8ef51cf37b1a9f3fc448a061e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>prepared</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>ae5b63fb7d1ac17bfe0f40c1358e2ced2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>mainThreadId</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a6588fc232f5001c84152098b2fe7695a</anchor>
      <arglist>[1]</arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>description</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a8444d6e0dfe2bbab0b5e7b24308f1559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_transport_cb_f</type>
      <name>callback</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>ac61a9065fbee4489fa84180893b95ccf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>callback_data</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a7322346207ed833d5550334332388ff9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_transport_s::@7</type>
      <name>tls</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>aa9c46ec1eddafe5714af85e8e3e57169</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_transport_s::@8</type>
      <name>dtls</name>
      <anchorfile>structtnet__transport__s.html</anchorfile>
      <anchor>a72dd4024a92a92e594c56275f2b3d01a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_allocation_s</name>
    <filename>structtnet__turn__allocation__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_allocation_id_t</type>
      <name>id</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a03aaf241a003d900555e758014c83005</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>relay_address</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>ad328f54cea8a7008376efcb17f465d43</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_attribute_mapped_addr_t *</type>
      <name>maddr</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a1bb895a0f6e6fbe93ac0bc346bc52526</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_attribute_xmapped_addr_t *</type>
      <name>xmaddr</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a5405872105ef7f32cabddc425993f69a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>localFD</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a2f7fda9a08aade674a360b301268cd92</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_socket_type_t</type>
      <name>socket_type</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a27db52c3553ad7f3a50a3f846128f5f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct sockaddr_storage</type>
      <name>server</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>acbcb80535e518b7b9f636338f948b88c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>username</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a9b20c006bd90a09e1465fb668700e81d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>password</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a59460a3ff2c12443d1022e5cc0fba85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>realm</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a78e609bc32bd5a9820209fe62b39963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nonce</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a814af5af8d9ea73885c8216c0e547e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>timeout</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>ab5627d8d8b095c198e2523c44ca380ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>software</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>ab7dea44af93feeae91fbc350466cd75b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_channel_bindings_L_t *</type>
      <name>channel_bindings</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>ac6f07a52b24bd7a43b9cef1654b7c2cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_permissions_L_t *</type>
      <name>permissions</name>
      <anchorfile>structtnet__turn__allocation__s.html</anchorfile>
      <anchor>a9c680afbc7b9cda04536e1a7a4d25a11</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_channelnum_s</name>
    <filename>structtnet__turn__attribute__channelnum__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__channelnum__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>number</name>
      <anchorfile>structtnet__turn__attribute__channelnum__s.html</anchorfile>
      <anchor>adf69c46b3dd2878c66bab78eddc40b04</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>rffu</name>
      <anchorfile>structtnet__turn__attribute__channelnum__s.html</anchorfile>
      <anchor>a423dae59c5f12250dfcf3cc92aa1dd06</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_data_s</name>
    <filename>structtnet__turn__attribute__data__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__data__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>value</name>
      <anchorfile>structtnet__turn__attribute__data__s.html</anchorfile>
      <anchor>a9bcf56196da97bfeb9b5411f2a5e07f6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_dontfrag_s</name>
    <filename>structtnet__turn__attribute__dontfrag__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__dontfrag__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_even_port_s</name>
    <filename>structtnet__turn__attribute__even__port__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__even__port__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>R</name>
      <anchorfile>structtnet__turn__attribute__even__port__s.html</anchorfile>
      <anchor>ad6607ab22ad9d395773e899a3594bad8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>rffu</name>
      <anchorfile>structtnet__turn__attribute__even__port__s.html</anchorfile>
      <anchor>a80faf1f85f4fda0fad2c1c69903c33dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_lifetime_s</name>
    <filename>structtnet__turn__attribute__lifetime__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__lifetime__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>value</name>
      <anchorfile>structtnet__turn__attribute__lifetime__s.html</anchorfile>
      <anchor>ae7f66047e6e39ba2bb6af8b95f00d1dd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_reqtrans_s</name>
    <filename>structtnet__turn__attribute__reqtrans__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__reqtrans__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_proto_t</type>
      <name>protocol</name>
      <anchorfile>structtnet__turn__attribute__reqtrans__s.html</anchorfile>
      <anchor>a8710e9ef6114f67bcc2651eca8a2c7fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>rffu</name>
      <anchorfile>structtnet__turn__attribute__reqtrans__s.html</anchorfile>
      <anchor>a05893d2921be797904cdd2bf010deef8</anchor>
      <arglist>[3]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_restoken_s</name>
    <filename>structtnet__turn__attribute__restoken__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__restoken__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>value</name>
      <anchorfile>structtnet__turn__attribute__restoken__s.html</anchorfile>
      <anchor>ac7a8beb4da4fc8eb8acfec13711a134f</anchor>
      <arglist>[8]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_xpeer_addr_s</name>
    <filename>structtnet__turn__attribute__xpeer__addr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__xpeer__addr__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_addr_family_t</type>
      <name>family</name>
      <anchorfile>structtnet__turn__attribute__xpeer__addr__s.html</anchorfile>
      <anchor>a49f24db0b2265a9d515199d570dd4e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>xport</name>
      <anchorfile>structtnet__turn__attribute__xpeer__addr__s.html</anchorfile>
      <anchor>ac021791c2ad249b108a5155d4c252a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>xaddress</name>
      <anchorfile>structtnet__turn__attribute__xpeer__addr__s.html</anchorfile>
      <anchor>ac365f9f3b4cedc84224afd91b2597ccb</anchor>
      <arglist>[16]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_attribute_xrelayed_addr_s</name>
    <filename>structtnet__turn__attribute__xrelayed__addr__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TNET_STUN_DECLARE_ATTRIBUTE</name>
      <anchorfile>structtnet__turn__attribute__xrelayed__addr__s.html</anchorfile>
      <anchor>a1c1589f58715cc05bb80c012d5979003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_stun_addr_family_t</type>
      <name>family</name>
      <anchorfile>structtnet__turn__attribute__xrelayed__addr__s.html</anchorfile>
      <anchor>a49f24db0b2265a9d515199d570dd4e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>xport</name>
      <anchorfile>structtnet__turn__attribute__xrelayed__addr__s.html</anchorfile>
      <anchor>ac021791c2ad249b108a5155d4c252a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>xaddress</name>
      <anchorfile>structtnet__turn__attribute__xrelayed__addr__s.html</anchorfile>
      <anchor>ac365f9f3b4cedc84224afd91b2597ccb</anchor>
      <arglist>[16]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_channel_binding_s</name>
    <filename>structtnet__turn__channel__binding__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__turn__channel__binding__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_channel_binding_id_t</type>
      <name>id</name>
      <anchorfile>structtnet__turn__channel__binding__s.html</anchorfile>
      <anchor>a273d32d05d21a77ad9228c9d1c74c83d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tnet_turn_allocation_s *</type>
      <name>allocation</name>
      <anchorfile>structtnet__turn__channel__binding__s.html</anchorfile>
      <anchor>a2c19d1b38ea7789b9f45212624c9dfbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>xpeer</name>
      <anchorfile>structtnet__turn__channel__binding__s.html</anchorfile>
      <anchor>af8cae1f1a500edee2f605e5f0396def4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>timeout</name>
      <anchorfile>structtnet__turn__channel__binding__s.html</anchorfile>
      <anchor>ab5627d8d8b095c198e2523c44ca380ac</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_channel_data_s</name>
    <filename>structtnet__turn__channel__data__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__turn__channel__data__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>chanel_number</name>
      <anchorfile>structtnet__turn__channel__data__s.html</anchorfile>
      <anchor>aa8c7009bf6b107e3990e62487e1cc7b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>length</name>
      <anchorfile>structtnet__turn__channel__data__s.html</anchorfile>
      <anchor>a1892eba2086d12ac2b09005aeb09ea3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>data</name>
      <anchorfile>structtnet__turn__channel__data__s.html</anchorfile>
      <anchor>a735984d41155bc1032e09bece8f8d66d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tnet_turn_permission_s</name>
    <filename>structtnet__turn__permission__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtnet__turn__permission__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_turn_attribute_xpeer_addr_t *</type>
      <name>xpeer</name>
      <anchorfile>structtnet__turn__permission__s.html</anchorfile>
      <anchor>af8cae1f1a500edee2f605e5f0396def4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>timeout</name>
      <anchorfile>structtnet__turn__permission__s.html</anchorfile>
      <anchor>ab5627d8d8b095c198e2523c44ca380ac</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0</name>
    <path>C:/Projects/doubango/branches/2.0/</path>
    <filename>dir_23c6058911ec3d6519846acb77914e60.html</filename>
    <dir>C:/Projects/doubango/branches/2.0/doubango</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches</name>
    <path>C:/Projects/doubango/branches/</path>
    <filename>dir_48ccd1e4df20782306b24b8e85052973.html</filename>
    <dir>C:/Projects/doubango/branches/2.0</dir>
  </compound>
  <compound kind="dir">
    <name>C:</name>
    <path>C:/</path>
    <filename>dir_e6bb53534ac0e427887cf7a94c0c004e.html</filename>
    <dir>C:/Projects</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp/</path>
    <filename>dir_9305dd5224cc9eb1b74ff189f58e640e.html</filename>
    <file>tnet_dhcp.c</file>
    <file>tnet_dhcp.h</file>
    <file>tnet_dhcp_message.c</file>
    <file>tnet_dhcp_message.h</file>
    <file>tnet_dhcp_option.c</file>
    <file>tnet_dhcp_option.h</file>
    <file>tnet_dhcp_option_sip.c</file>
    <file>tnet_dhcp_option_sip.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6/</path>
    <filename>dir_eda36b283219a502fc4de8db340bc374.html</filename>
    <file>tnet_dhcp6.c</file>
    <file>tnet_dhcp6.h</file>
    <file>tnet_dhcp6_duid.c</file>
    <file>tnet_dhcp6_duid.h</file>
    <file>tnet_dhcp6_message.c</file>
    <file>tnet_dhcp6_message.h</file>
    <file>tnet_dhcp6_option.c</file>
    <file>tnet_dhcp6_option.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns/</path>
    <filename>dir_a3af63e0f7dbe4aa673b61a1a0d562c7.html</filename>
    <file>tnet_dns.c</file>
    <file>tnet_dns.h</file>
    <file>tnet_dns_a.c</file>
    <file>tnet_dns_a.h</file>
    <file>tnet_dns_aaaa.c</file>
    <file>tnet_dns_aaaa.h</file>
    <file>tnet_dns_cname.c</file>
    <file>tnet_dns_cname.h</file>
    <file>tnet_dns_message.c</file>
    <file>tnet_dns_message.h</file>
    <file>tnet_dns_mx.c</file>
    <file>tnet_dns_mx.h</file>
    <file>tnet_dns_naptr.c</file>
    <file>tnet_dns_naptr.h</file>
    <file>tnet_dns_ns.c</file>
    <file>tnet_dns_ns.h</file>
    <file>tnet_dns_opt.c</file>
    <file>tnet_dns_opt.h</file>
    <file>tnet_dns_ptr.c</file>
    <file>tnet_dns_ptr.h</file>
    <file>tnet_dns_regexp.c</file>
    <file>tnet_dns_regexp.h</file>
    <file>tnet_dns_resolvconf.c</file>
    <file>tnet_dns_resolvconf.h</file>
    <file>tnet_dns_rr.c</file>
    <file>tnet_dns_rr.h</file>
    <file>tnet_dns_soa.c</file>
    <file>tnet_dns_soa.h</file>
    <file>tnet_dns_srv.c</file>
    <file>tnet_dns_srv.h</file>
    <file>tnet_dns_txt.c</file>
    <file>tnet_dns_txt.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/</path>
    <filename>dir_794c5f3a5871fccbd9cb0144b8a918bb.html</filename>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango</name>
    <path>C:/Projects/doubango/</path>
    <filename>dir_61caa8c9c937953110cbb688168e4704.html</filename>
    <dir>C:/Projects/doubango/branches</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice/</path>
    <filename>dir_7326b0f9d77a7a21c8e0c28502dde551.html</filename>
    <file>tnet_ice.c</file>
    <file>tnet_ice.h</file>
    <file>tnet_ice_candidate.c</file>
    <file>tnet_ice_candidate.h</file>
    <file>tnet_ice_ctx.c</file>
    <file>tnet_ice_ctx.h</file>
    <file>tnet_ice_event.c</file>
    <file>tnet_ice_event.h</file>
    <file>tnet_ice_pair.c</file>
    <file>tnet_ice_pair.h</file>
    <file>tnet_ice_utils.c</file>
    <file>tnet_ice_utils.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects</name>
    <path>C:/Projects/</path>
    <filename>dir_e27f7acb6c354cea3d58335de45d9d79.html</filename>
    <dir>C:/Projects/doubango</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/</path>
    <filename>dir_dc29ffbc5ac5e8d81dc8895f583aaf87.html</filename>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp</dir>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dhcp6</dir>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/dns</dir>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/ice</dir>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun</dir>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls</dir>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn</dir>
    <file>tinynet.h</file>
    <file>tinynet_config.h</file>
    <file>tnet.c</file>
    <file>tnet.h</file>
    <file>tnet_auth.c</file>
    <file>tnet_auth.h</file>
    <file>tnet_endianness.c</file>
    <file>tnet_endianness.h</file>
    <file>tnet_hardwares.h</file>
    <file>tnet_nat.c</file>
    <file>tnet_nat.h</file>
    <file>tnet_poll.c</file>
    <file>tnet_poll.h</file>
    <file>tnet_proto.h</file>
    <file>tnet_socket.c</file>
    <file>tnet_socket.h</file>
    <file>tnet_transport.c</file>
    <file>tnet_transport.h</file>
    <file>tnet_transport_cfsocket.c</file>
    <file>tnet_transport_poll.c</file>
    <file>tnet_transport_win32.c</file>
    <file>tnet_types.h</file>
    <file>tnet_utils.c</file>
    <file>tnet_utils.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/stun/</path>
    <filename>dir_a019e49c2167974a97bc9931fdf595ee.html</filename>
    <file>tnet_stun.c</file>
    <file>tnet_stun.h</file>
    <file>tnet_stun_attribute.c</file>
    <file>tnet_stun_attribute.h</file>
    <file>tnet_stun_message.c</file>
    <file>tnet_stun_message.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/</path>
    <filename>dir_a96db63cb14938079dd27e1b49770c9d.html</filename>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/tls/</path>
    <filename>dir_6ce67df634907730bdb278977e2dcad6.html</filename>
    <file>tnet_dtls.c</file>
    <file>tnet_dtls.h</file>
    <file>tnet_tls.c</file>
    <file>tnet_tls.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyNET/src/turn/</path>
    <filename>dir_a624ddd92b7822975b22b7ff89b4a0f7.html</filename>
    <file>tnet_turn.c</file>
    <file>tnet_turn.h</file>
    <file>tnet_turn_attribute.c</file>
    <file>tnet_turn_attribute.h</file>
    <file>tnet_turn_message.c</file>
    <file>tnet_turn_message.h</file>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>tinyNET API Overview</title>
    <filename>index</filename>
  </compound>
</tagfile>
