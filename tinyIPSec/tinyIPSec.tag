<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>index.doxy</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/</path>
    <filename>index_8doxy</filename>
  </compound>
  <compound kind="file">
    <name>tinyipsec_config.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/src/</path>
    <filename>tinyipsec__config_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TINYIPSEC_API</name>
      <anchorfile>tinyipsec__config_8h.html</anchorfile>
      <anchor>abef456bec9fbd947edd788ecf9838e6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYIPSEC_GEXTERN</name>
      <anchorfile>tinyipsec__config_8h.html</anchorfile>
      <anchor>ab70c3788fe63d08227fc5078ed53d7c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_BEGIN_DECLS</name>
      <anchorfile>tinyipsec__config_8h.html</anchorfile>
      <anchor>aaf1df1f70c5ceeac797c273b24e9e4fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_END_DECLS</name>
      <anchorfile>tinyipsec__config_8h.html</anchorfile>
      <anchor>a64796bed0fbed09967e22706cc312955</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tipsec.c</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/src/</path>
    <filename>tipsec_8c</filename>
    <includes id="tipsec_8h" name="tipsec.h" local="yes" imported="no">tipsec.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MAX_PLUGINS</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a51b3a9bd0bc40391022e69b14e419203</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_ctx_create</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a472c7e57352115a0ca0ff252afe0f50f</anchor>
      <arglist>(tipsec_ipproto_t ipproto, tsk_bool_t use_ipv6, tipsec_mode_t mode, tipsec_ealg_t ealg, tipsec_alg_t alg, tipsec_proto_t protocol, tipsec_ctx_t **pp_ctx)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_ctx_start</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a192333c618b2e8296a6486a1b26fa958</anchor>
      <arglist>(tipsec_ctx_t *p_ctx)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_ctx_set_local</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>af0e788c0d1569210093e6041aa4a4f93</anchor>
      <arglist>(tipsec_ctx_t *p_ctx, const char *addr_local, const char *addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_ctx_set_keys</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>ad7c0204aab8bcd52b778488d72a22ea6</anchor>
      <arglist>(tipsec_ctx_t *p_ctx, const tipsec_key_t *ik, const tipsec_key_t *ck)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_ctx_set_remote</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a05e544c50a8b364eaf003355ae482f1c</anchor>
      <arglist>(tipsec_ctx_t *p_ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_ctx_stop</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a769276133705f850d13a61b79a1d03ab</anchor>
      <arglist>(tipsec_ctx_t *p_ctx)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_plugin_register_static</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a46d95fbe620252695b97603174352761</anchor>
      <arglist>(const tipsec_plugin_def_t *pc_plugin)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_plugin_unregister_static</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a95335368c34451402e08939fb070b9a4</anchor>
      <arglist>(const tipsec_plugin_def_t *pc_plugin)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_plugin_register_file</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a954f7a388fbfe53cd0ffa2abca0e1fc5</anchor>
      <arglist>(const char *pc_filepath, struct tsk_plugin_s **pp_plugin)</arglist>
    </member>
    <member kind="function">
      <type>tipsec_error_t</type>
      <name>tipsec_plugin_unregister_file</name>
      <anchorfile>tipsec_8c.html</anchorfile>
      <anchor>a212e3094c19623ba3c4b1c7c4be80e33</anchor>
      <arglist>(struct tsk_plugin_s *p_plugin)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tipsec.h</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/src/</path>
    <filename>tipsec_8h</filename>
    <includes id="tinyipsec__config_8h" name="tinyipsec_config.h" local="yes" imported="no">tinyipsec_config.h</includes>
    <class kind="struct">tipsec_ctx_t</class>
    <class kind="struct">tipsec_plugin_def_t</class>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_CTX</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a467d82c35835872fc50c7b2ae47dcb16</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IPPROTO_FROM_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad7e053ddddfa157ded9b8086c431e55d</anchor>
      <arglist>(str_ipproto)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IPPROTO_TO_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a5623528f51d639631b0c7a701f38fa19</anchor>
      <arglist>(enum_ipproto)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MODE_FROM_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a491a2258010a1cce1635a509689a4eb7</anchor>
      <arglist>(str_mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MODE_TO_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a112c545fce64b33ac14dc3c3e70a6681</anchor>
      <arglist>(enum_mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_EALG_FROM_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a109cc06bd1b098fe5336794739748182</anchor>
      <arglist>(str_ealg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_EALG_TO_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad927e77abab9f8be5dc0477944a93e6e</anchor>
      <arglist>(enum_ealg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_ALG_FROM_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a76b577fb409243a693670168adf8cada</anchor>
      <arglist>(str_alg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_ALG_TO_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad783707af5efdd61a4c5d700f9b5e76c</anchor>
      <arglist>(enum_alg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_PROTOCOL_FROM_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a618150192c484e2e0b6937e52685e0e4</anchor>
      <arglist>(str_protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_PROTOCOL_TO_STR</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>aea16489666722ddcb91514253a01ed98</anchor>
      <arglist>(enum_protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_KEY_LEN</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a851fbd8d4f91dcda203bfe9856aaf23b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IK_LEN</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a0b023400d28a9cdd733bde20894dc83a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_CK_LEN</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a6ebefb2b9ed5e9c7962cecd5a9403f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_DECLARE_CTX</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a3cb3844fc43696dab7710e0c56d298e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>tipsec_lifetime_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a5e3f91a70997490f4ac4d98d96ee1adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>tipsec_spi_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a12b094dc84f16b83bcdb31052d6b4a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>tipsec_port_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a492784849740492d00c74b4a20a7a0b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>tipsec_key_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>aaba5a3f465f81050641b9f260f30ee0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_mode_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae63d06d1f17b104f8456f9dc6b0e6581</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_mode_trans</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae63d06d1f17b104f8456f9dc6b0e6581a5070f31ffc2f78752669e7bcbd02373d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_mode_tun</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae63d06d1f17b104f8456f9dc6b0e6581a80c88b03059157ab4f292467cd278c02</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_proto_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae3b87d54510581d86e6a4b4f2aa4f618</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_proto_ah</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae3b87d54510581d86e6a4b4f2aa4f618ae8fb5edbefa2b3226d4ae0377b1897c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_proto_esp</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae3b87d54510581d86e6a4b4f2aa4f618afce46e75b07ee1e99d2ff123d7338884</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_proto_both</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae3b87d54510581d86e6a4b4f2aa4f618ae64bca52e88845d6667c957d6e53b558</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_ipproto_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad615cb265ccbd49ee5997ece4846bc10</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_ipproto_udp</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad615cb265ccbd49ee5997ece4846bc10a40d9be45aca21c0b02829bbff52f07f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_ipproto_tcp</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad615cb265ccbd49ee5997ece4846bc10a8fac86b43cd6ee6178e0c6ad0e437517</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_ipproto_icmp</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad615cb265ccbd49ee5997ece4846bc10a14834c72e0712c1deba3c82669796c0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_alg_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a411329fc5c5ea00d5da90a5816df3b6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_alg_hmac_md5_96</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a411329fc5c5ea00d5da90a5816df3b6aa35c2790382b4a9cafc7dd77fd5cdc667</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_alg_hmac_sha_1_96</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a411329fc5c5ea00d5da90a5816df3b6aa72532a50164cdda02949285a56fa764e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_ealg_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a6b2ac4404ef962aece5dcd0213b01fb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_ealg_des_ede3_cbc</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a6b2ac4404ef962aece5dcd0213b01fb3aa1d2444af224e69b4b90fc5382b42846</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_ealg_aes</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a6b2ac4404ef962aece5dcd0213b01fb3a65d3f0dfd8a6d4174581f2cf1577add8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_ealg_null</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a6b2ac4404ef962aece5dcd0213b01fb3a302bf256e0637ae86f30827e60351c45</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_state_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad1c8935dac8d9f968ba7e63fb49d532d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_state_initial</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad1c8935dac8d9f968ba7e63fb49d532da71c49fc1ffada66c77fb542cac09167f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_state_inbound</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad1c8935dac8d9f968ba7e63fb49d532da33449eeadbf722c3baf78e5dfafb87bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_state_full</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad1c8935dac8d9f968ba7e63fb49d532da6ff4ee972e987fcf37bc1cd4fd906c2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_state_active</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ad1c8935dac8d9f968ba7e63fb49d532da3383c9cdaba4bb36e199e09328b05d7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_error_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_success</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073a599826fab10354915b93da7b87cfa563</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_invalid_param</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073ae73d28f507b88504ed0e45b6837b2470</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_invalid_state</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073adf76c386ea59bac74a9980535eb3ee34</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_access_violation</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073ae908b6afcbb11496d4120b0153b47041</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_permission_denied</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073ac6802cafc46607cb9118a9b2d3e8114b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_outofmemory</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073aca4704f36f371d51a466cb1ac263ef64</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_outofbound</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073a153e148252b8c9ec5acd198400bfb862</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_notfound</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073aa503579c4f98cf422dd4e781f7ac1ee0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_notimplemented</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073a69c97d09dd04ae572d52351956d3ce18</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_error_sys</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>af8fd353df1dede1a22c968b23534e073a439fccb3281f391c22b11d7dfea412fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>tipsec_impl_type_t</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a34f2495a9c2ceb2f2561b3f5d9b84313</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_impl_type_xp</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a34f2495a9c2ceb2f2561b3f5d9b84313aae3cf250d96df0d5ef8094f433126113</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_impl_type_vista</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a34f2495a9c2ceb2f2561b3f5d9b84313ad754dedb8acbe8466e757e6840d39a5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tipsec_impl_type_ltools</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a34f2495a9c2ceb2f2561b3f5d9b84313af7073d4f0a269216d8affc59df25f90e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_ctx_create</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a849b504b5f082252d0ac82b44af39dc9</anchor>
      <arglist>(tipsec_ipproto_t ipproto, tsk_bool_t use_ipv6, tipsec_mode_t mode, tipsec_ealg_t ealg, tipsec_alg_t alg, tipsec_proto_t protocol, tipsec_ctx_t **pp_ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_ctx_start</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a028ce54aeea7f8a0dbf4ea43a052062d</anchor>
      <arglist>(tipsec_ctx_t *p_ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_ctx_set_local</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a678503380a9c943805b99ac14cfacabf</anchor>
      <arglist>(tipsec_ctx_t *p_ctx, const char *addr_local, const char *addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_ctx_set_keys</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a4739c235bb860bd6539b73173b0a28a5</anchor>
      <arglist>(tipsec_ctx_t *p_ctx, const tipsec_key_t *ik, const tipsec_key_t *ck)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_ctx_set_remote</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a20fd2042742be4f02d7e024f448040b8</anchor>
      <arglist>(tipsec_ctx_t *p_ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_ctx_stop</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a7e184a4dad7b6ebc7bfed7ec037e3840</anchor>
      <arglist>(tipsec_ctx_t *p_ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_plugin_register_static</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ac9c6c6dfe29d06dbe0bc359ec6ac2b6c</anchor>
      <arglist>(const tipsec_plugin_def_t *pc_plugin)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_plugin_unregister_static</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>ae3d5ab2c43373258a2eec32c603ed29c</anchor>
      <arglist>(const tipsec_plugin_def_t *pc_plugin)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_plugin_register_file</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a2e672ee63edfc9d12e7d8180aa8d21db</anchor>
      <arglist>(const char *pc_filepath, struct tsk_plugin_s **pp_plugin)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_error_t</type>
      <name>tipsec_plugin_unregister_file</name>
      <anchorfile>tipsec_8h.html</anchorfile>
      <anchor>a5bde9e371f6d1e037d558ad4ccfa24eb</anchor>
      <arglist>(struct tsk_plugin_s *pp_plugin)</arglist>
    </member>
  <compound kind="struct">
    <name>tipsec_ctx_t</name>
    <filename>tipsec_8h.html</filename>
    <anchor>structtipsec__ctx__t</anchor>
  </compound>
  <compound kind="page">
    <name>_Page_Main_Overview</name>
    <title>Overview</title>
    <filename>__page__main__overview</filename>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_Intro</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_SecAgree</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_SecAgree_CallFlow</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_SecAgree_SipMessages</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_IPSecTools</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_IPSecAPI</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_IPSecAPI_LoadPlugin</docanchor>
    <docanchor file="__page__main__overview">_Anchor_TIPSec_Overview_IPSecAPI_Client</docanchor>
  </compound>
  <compound kind="page">
    <name>_Page_Main_FAQ</name>
    <title>(FAQ)</title>
    <filename>__page__main__f_a_q</filename>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_Client</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_Samples</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_Stable</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_Systems</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_Logs</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_ReportIssues</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_CheckSAs</docanchor>
    <docanchor file="__page__main__f_a_q">_Anchor_TIPSec_FAQ_Error5</docanchor>
  </compound>
  <compound kind="page">
    <name>_Page_Main_Medium_Level_API_Overview</name>
    <title>Medium level API (C++)</title>
    <filename>__page__main__medium__level__a_p_i__overview</filename>
  </compound>
  <compound kind="struct">
    <name>tipsec_plugin_def_t</name>
    <filename>structtipsec__plugin__def__t.html</filename>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>objdef</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>a5f6e72d7234576ccdb239af8b52916c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>enum tipsec_impl_type_e</type>
      <name>type</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>a9d9af76cbd445816c3c21931bbb7d1e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>desc</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>a437cdaf21a6304cf4ed8ac70419ff3d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_error_t(*</type>
      <name>init</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>ae5970f1bb28c562410dc92066b89efb2</anchor>
      <arglist>)(tipsec_ctx_t *)</arglist>
    </member>
    <member kind="variable">
      <type>tipsec_error_t(*</type>
      <name>set_local</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>afb4bc03bdf636587ffdc6adfecb1f7bf</anchor>
      <arglist>)(tipsec_ctx_t *, const char *addr_local, const char *addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)</arglist>
    </member>
    <member kind="variable">
      <type>tipsec_error_t(*</type>
      <name>set_remote</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>aa8f93a2570da1538c6247181b704f73e</anchor>
      <arglist>)(tipsec_ctx_t *, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)</arglist>
    </member>
    <member kind="variable">
      <type>tipsec_error_t(*</type>
      <name>set_keys</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>a9b73bc2ea371b044abd629811abd5ca3</anchor>
      <arglist>)(tipsec_ctx_t *, const tipsec_key_t *ik, const tipsec_key_t *ck)</arglist>
    </member>
    <member kind="variable">
      <type>tipsec_error_t(*</type>
      <name>start</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>aa07c642db9c45a3875ae1ed9e722787f</anchor>
      <arglist>)(tipsec_ctx_t *)</arglist>
    </member>
    <member kind="variable">
      <type>tipsec_error_t(*</type>
      <name>stop</name>
      <anchorfile>structtipsec__plugin__def__t.html</anchorfile>
      <anchor>aa8ff7af3adca6eafbc66cb5e57c58ed9</anchor>
      <arglist>)(tipsec_ctx_t *)</arglist>
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
    <name>C:/Projects/doubango/branches/2.0/doubango</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/</path>
    <filename>dir_794c5f3a5871fccbd9cb0144b8a918bb.html</filename>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango</name>
    <path>C:/Projects/doubango/</path>
    <filename>dir_61caa8c9c937953110cbb688168e4704.html</filename>
    <dir>C:/Projects/doubango/branches</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects</name>
    <path>C:/Projects/</path>
    <filename>dir_e27f7acb6c354cea3d58335de45d9d79.html</filename>
    <dir>C:/Projects/doubango</dir>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/src</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/src/</path>
    <filename>dir_469d5fc4ef0c7e6f06fee4947bf796da.html</filename>
    <file>tinyipsec_config.h</file>
    <file>tipsec.c</file>
    <file>tipsec.h</file>
  </compound>
  <compound kind="dir">
    <name>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec</name>
    <path>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/</path>
    <filename>dir_9c132a6bd8a33b79da9c96611c75368b.html</filename>
    <dir>C:/Projects/doubango/branches/2.0/doubango/tinyIPSec/src</dir>
    <file>index.doxy</file>
  </compound>
</tagfile>
