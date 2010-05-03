<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>TinyIPSec API Overview</title>
    <filename>index</filename>
  </compound>
  <compound kind="file">
    <name>tinyipsec_config.h</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
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
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec_8c</filename>
    <includes id="tipsec_8h" name="tipsec.h" local="yes" imported="no">tipsec.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec.h</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec_8h</filename>
    <includes id="tinyipsec__config_8h" name="tinyipsec_config.h" local="yes" imported="no">tinyIPSEC_config.h</includes>
    <includes id="tipsec__common_8h" name="tipsec_common.h" local="yes" imported="no">tipsec_common.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec_common.c</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__common_8c</filename>
    <includes id="tipsec__common_8h" name="tipsec_common.h" local="yes" imported="no">tipsec_common.h</includes>
    <member kind="function">
      <type>tipsec_context_t *</type>
      <name>tipsec_context_create</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>ad4e25e81e8ae133e43fdba8eedb30029</anchor>
      <arglist>(tipsec_ipproto_t ipproto, tsk_bool_t use_ipv6, tipsec_mode_t mode, tipsec_ealgorithm_t ealg, tipsec_algorithm_t alg, tipsec_protocol_t protocol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tipsec_start</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>affd11a7ac0bde42c42e204074df88498</anchor>
      <arglist>(tipsec_context_t *ctx)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tipsec_set_local</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>a928bf5d10f375dc9936e34e80229ce16</anchor>
      <arglist>(tipsec_context_t *ctx, const char *addr_local, const char *addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tipsec_set_keys</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>a7776b00550479080bf1120fff8ed5fd1</anchor>
      <arglist>(tipsec_context_t *ctx, const tipsec_key_t *ik, const tipsec_key_t *ck)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tipsec_set_remote</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>a20d8708dfb2dd9bdf0423e5fb945ba34</anchor>
      <arglist>(tipsec_context_t *ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tipsec_stop</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>acef726977f2a96380dbeee403f8ece02</anchor>
      <arglist>(tipsec_context_t *ctx)</arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>tipsec_context_def_t</name>
      <anchorfile>tipsec__common_8c.html</anchorfile>
      <anchor>ac9ef6d05704cf4b106c66124b3adcb80</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tipsec_common.h</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__common_8h</filename>
    <includes id="tinyipsec__config_8h" name="tinyipsec_config.h" local="yes" imported="no">tinyIPSEC_config.h</includes>
    <class kind="struct">tipsec_context_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_CONTEXT</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga031b539830dd0bba323f772aa8773158</anchor>
      <arglist>(ctx)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IPPROTO_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaa8aac71ce8c6de518f67923dff3c91ce</anchor>
      <arglist>(ipproto)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IPPROTO_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0559d76ead2418b1eebcb44eefaceddc</anchor>
      <arglist>(ipproto)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MODE_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga088685f515f05aa14d5e9d67e958d700</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MODE_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga3e94f6c1a82d4598abd088d1f71f204f</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_EALG_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaac76bbc14fcbb47116916220d8998068</anchor>
      <arglist>(ealg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_EALG_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga23e38a587b10d23aebb47fd650309b8a</anchor>
      <arglist>(ealg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_ALG_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0a49d87e54aabd4a168a281acf71a5ab</anchor>
      <arglist>(alg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_ALG_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga3d431568e9beaddf1885590b88422ae3</anchor>
      <arglist>(alg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_PROTOCOL_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga1f15db462110b18e6f953a55590a0be3</anchor>
      <arglist>(protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_PROTOCOL_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga27dc104c07bf731ec09ae9d841deb3a8</anchor>
      <arglist>(protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_KEY_LEN</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga851fbd8d4f91dcda203bfe9856aaf23b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IK_LEN</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0b023400d28a9cdd733bde20894dc83a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_CK_LEN</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga6ebefb2b9ed5e9c7962cecd5a9403f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYIPSEC_DECLARE_CONTEXT</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaf11d05f9c9fa26e24f7fcb02822992e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>tipsec_lifetime_t</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a5e3f91a70997490f4ac4d98d96ee1adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>tipsec_spi_t</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a12b094dc84f16b83bcdb31052d6b4a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>tipsec_port_t</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a492784849740492d00c74b4a20a7a0b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>tipsec_key_t</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>aaba5a3f465f81050641b9f260f30ee0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_mode_e</type>
      <name>tipsec_mode_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga877a3113645122f113e9399962a6ee04</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_protocol_e</type>
      <name>tipsec_protocol_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga06eccdaf53b7ed5c4836a57cd098a17f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_ipproto_e</type>
      <name>tipsec_ipproto_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga9750a5fce24a25fed8683cd5b74c4776</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_algorithm_e</type>
      <name>tipsec_algorithm_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga95027d339821caf4b3868c67f3dfc117</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_ealgorithm_e</type>
      <name>tipsec_ealgorithm_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga8b2c0135326a3e9f04ec4983c4747b6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_state_e</type>
      <name>tipsec_state_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga7145039e5ecc8b0f3d0329d991fad686</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tipsec_context_s</type>
      <name>tipsec_context_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0b8ffcf277be1447a551d18fbec2d96a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_mode_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga2e365e188f68a1791007ad68dfe7de2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>mode_trans</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga2e365e188f68a1791007ad68dfe7de2ea49ff719c96c4668d4303d2884413d87a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>mode_tun</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga2e365e188f68a1791007ad68dfe7de2ea68a35e9f2906a3e9018b925863d8de44</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_protocol_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaa2d750c8e3680f9ce2d2ab37972b9e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>proto_ah</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa2d750c8e3680f9ce2d2ab37972b9e33aa74c85f7f22556e53fe3805e3cabf69c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>proto_esp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa2d750c8e3680f9ce2d2ab37972b9e33a8d3f645c9bcb4aad8a6aef3d02a255c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>proto_both</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa2d750c8e3680f9ce2d2ab37972b9e33a54ed28109aff482fd675b96b83c1ca07</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_ipproto_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaa71e8cb8bf5fc3673e7a1585f9ba2521</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ipproto_udp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa71e8cb8bf5fc3673e7a1585f9ba2521a105b8570058e9003717bd56643af75b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ipproto_tcp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa71e8cb8bf5fc3673e7a1585f9ba2521a51d75c8309d93b4fd6c58b675510a1b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ipproto_icmp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa71e8cb8bf5fc3673e7a1585f9ba2521a0dd871bdd179315f39834d265df6e5ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_algorithm_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga529fade1455b4f029e61a882467b5021</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>algo_hmac_md5_96</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga529fade1455b4f029e61a882467b5021aa3bf68329aa5d57dcc9f7611968e205b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>algo_hmac_sha_1_96</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga529fade1455b4f029e61a882467b5021a860a6ba099cc0df925d1cd1725a0c6cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_ealgorithm_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga71634bfd364a00cb020b42790a4a4ae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ealg_des_ede3_cbc</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga71634bfd364a00cb020b42790a4a4ae3a26c3a0cf983ec250339ee9ac2fb2239e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ealg_aes</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga71634bfd364a00cb020b42790a4a4ae3ae5cdadedebba9cc34cae3aec3aa75778</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ealg_null</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga71634bfd364a00cb020b42790a4a4ae3abf16edc14198aa145add99b22a9fb6a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_state_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaf85b43ddf445b576d6ff125356bd652b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_initial</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652ba8f952da0293fceca1122e9fe917e5948</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_inbound</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652ba43d0c18ca65168ed7d227c55b9391d1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_full</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652bac1e4f14db80773ee398bca9292569d50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_active</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652ba7c7581232b6636c63abe1eb55d0a3d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API tipsec_context_t *</type>
      <name>tipsec_context_create</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a251b9f886032a9af83e1adfaa7dc87de</anchor>
      <arglist>(tipsec_ipproto_t ipproto, tsk_bool_t use_ipv6, tipsec_mode_t mode, tipsec_ealgorithm_t ealg, tipsec_algorithm_t alg, tipsec_protocol_t protocol)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API int</type>
      <name>tipsec_start</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>ab34dd23f5b4ba87cf7c1e2846b35b269</anchor>
      <arglist>(tipsec_context_t *ctx)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API int</type>
      <name>tipsec_set_local</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>acf143069e15b831ce1de8c80e4739fe0</anchor>
      <arglist>(tipsec_context_t *ctx, const char *addr_local, const char *addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API int</type>
      <name>tipsec_set_keys</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a137acab0153ebffa1b2726b7b92e6705</anchor>
      <arglist>(tipsec_context_t *ctx, const tipsec_key_t *ik, const tipsec_key_t *ck)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API int</type>
      <name>tipsec_set_remote</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a7e631003938572fd2c41d139a21cec2d</anchor>
      <arglist>(tipsec_context_t *ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)</arglist>
    </member>
    <member kind="function">
      <type>TINYIPSEC_API int</type>
      <name>tipsec_stop</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a060c0509d26fe1e224917e068959fc61</anchor>
      <arglist>(tipsec_context_t *ctx)</arglist>
    </member>
    <member kind="variable">
      <type>TINYIPSEC_GEXTERN const void *</type>
      <name>tipsec_context_def_t</name>
      <anchorfile>tipsec__common_8h.html</anchorfile>
      <anchor>a0f542fef286aafa5c9498661023f2b62</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tipsec_racoon.c</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__racoon_8c</filename>
    <includes id="tipsec__racoon_8h" name="tipsec_racoon.h" local="yes" imported="no">tipsec_racoon.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec_racoon.h</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__racoon_8h</filename>
    <includes id="tinyipsec__config_8h" name="tinyipsec_config.h" local="yes" imported="no">tinyIPSEC_config.h</includes>
    <includes id="tipsec__common_8h" name="tipsec_common.h" local="yes" imported="no">tipsec_common.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec_vista.c</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__vista_8c</filename>
    <includes id="tipsec__vista_8h" name="tipsec_vista.h" local="yes" imported="no">tipsec_vista.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec_vista.h</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__vista_8h</filename>
    <includes id="tinyipsec__config_8h" name="tinyipsec_config.h" local="yes" imported="no">tinyIPSEC_config.h</includes>
    <includes id="tipsec__common_8h" name="tipsec_common.h" local="yes" imported="no">tipsec_common.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec_xp.c</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__xp_8c</filename>
    <includes id="tipsec__xp_8h" name="tipsec_xp.h" local="yes" imported="no">tipsec_xp.h</includes>
  </compound>
  <compound kind="file">
    <name>tipsec_xp.h</name>
    <path>C:/Projects/Doubango/tinyIPSec/src/</path>
    <filename>tipsec__xp_8h</filename>
    <includes id="tinyipsec__config_8h" name="tinyipsec_config.h" local="yes" imported="no">tinyIPSEC_config.h</includes>
    <includes id="tipsec__common_8h" name="tipsec_common.h" local="yes" imported="no">tipsec_common.h</includes>
  </compound>
  <compound kind="group">
    <name>tipsec_common_group</name>
    <title>IPSec common.</title>
    <filename>group__tipsec__common__group.html</filename>
    <class kind="struct">tipsec_context_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_CONTEXT</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga031b539830dd0bba323f772aa8773158</anchor>
      <arglist>(ctx)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IPPROTO_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaa8aac71ce8c6de518f67923dff3c91ce</anchor>
      <arglist>(ipproto)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IPPROTO_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0559d76ead2418b1eebcb44eefaceddc</anchor>
      <arglist>(ipproto)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MODE_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga088685f515f05aa14d5e9d67e958d700</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_MODE_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga3e94f6c1a82d4598abd088d1f71f204f</anchor>
      <arglist>(mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_EALG_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaac76bbc14fcbb47116916220d8998068</anchor>
      <arglist>(ealg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_EALG_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga23e38a587b10d23aebb47fd650309b8a</anchor>
      <arglist>(ealg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_ALG_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0a49d87e54aabd4a168a281acf71a5ab</anchor>
      <arglist>(alg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_ALG_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga3d431568e9beaddf1885590b88422ae3</anchor>
      <arglist>(alg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_PROTOCOL_FROM_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga1f15db462110b18e6f953a55590a0be3</anchor>
      <arglist>(protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_PROTOCOL_TO_STR</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga27dc104c07bf731ec09ae9d841deb3a8</anchor>
      <arglist>(protocol)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_KEY_LEN</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga851fbd8d4f91dcda203bfe9856aaf23b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_IK_LEN</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0b023400d28a9cdd733bde20894dc83a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIPSEC_CK_LEN</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga6ebefb2b9ed5e9c7962cecd5a9403f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYIPSEC_DECLARE_CONTEXT</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaf11d05f9c9fa26e24f7fcb02822992e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_mode_e</type>
      <name>tipsec_mode_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga877a3113645122f113e9399962a6ee04</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_protocol_e</type>
      <name>tipsec_protocol_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga06eccdaf53b7ed5c4836a57cd098a17f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_ipproto_e</type>
      <name>tipsec_ipproto_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga9750a5fce24a25fed8683cd5b74c4776</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_algorithm_e</type>
      <name>tipsec_algorithm_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga95027d339821caf4b3868c67f3dfc117</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_ealgorithm_e</type>
      <name>tipsec_ealgorithm_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga8b2c0135326a3e9f04ec4983c4747b6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum tipsec_state_e</type>
      <name>tipsec_state_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga7145039e5ecc8b0f3d0329d991fad686</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tipsec_context_s</type>
      <name>tipsec_context_t</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga0b8ffcf277be1447a551d18fbec2d96a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_mode_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga2e365e188f68a1791007ad68dfe7de2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>mode_trans</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga2e365e188f68a1791007ad68dfe7de2ea49ff719c96c4668d4303d2884413d87a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>mode_tun</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga2e365e188f68a1791007ad68dfe7de2ea68a35e9f2906a3e9018b925863d8de44</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_protocol_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaa2d750c8e3680f9ce2d2ab37972b9e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>proto_ah</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa2d750c8e3680f9ce2d2ab37972b9e33aa74c85f7f22556e53fe3805e3cabf69c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>proto_esp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa2d750c8e3680f9ce2d2ab37972b9e33a8d3f645c9bcb4aad8a6aef3d02a255c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>proto_both</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa2d750c8e3680f9ce2d2ab37972b9e33a54ed28109aff482fd675b96b83c1ca07</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_ipproto_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaa71e8cb8bf5fc3673e7a1585f9ba2521</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ipproto_udp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa71e8cb8bf5fc3673e7a1585f9ba2521a105b8570058e9003717bd56643af75b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ipproto_tcp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa71e8cb8bf5fc3673e7a1585f9ba2521a51d75c8309d93b4fd6c58b675510a1b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ipproto_icmp</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaa71e8cb8bf5fc3673e7a1585f9ba2521a0dd871bdd179315f39834d265df6e5ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_algorithm_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga529fade1455b4f029e61a882467b5021</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>algo_hmac_md5_96</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga529fade1455b4f029e61a882467b5021aa3bf68329aa5d57dcc9f7611968e205b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>algo_hmac_sha_1_96</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga529fade1455b4f029e61a882467b5021a860a6ba099cc0df925d1cd1725a0c6cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_ealgorithm_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ga71634bfd364a00cb020b42790a4a4ae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ealg_des_ede3_cbc</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga71634bfd364a00cb020b42790a4a4ae3a26c3a0cf983ec250339ee9ac2fb2239e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ealg_aes</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga71634bfd364a00cb020b42790a4a4ae3ae5cdadedebba9cc34cae3aec3aa75778</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ealg_null</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gga71634bfd364a00cb020b42790a4a4ae3abf16edc14198aa145add99b22a9fb6a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>tipsec_state_e</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>gaf85b43ddf445b576d6ff125356bd652b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_initial</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652ba8f952da0293fceca1122e9fe917e5948</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_inbound</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652ba43d0c18ca65168ed7d227c55b9391d1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_full</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652bac1e4f14db80773ee398bca9292569d50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>state_active</name>
      <anchorfile>group__tipsec__common__group.html</anchorfile>
      <anchor>ggaf85b43ddf445b576d6ff125356bd652ba7c7581232b6636c63abe1eb55d0a3d5c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>tipsec_racoon_group</name>
    <title>IPSec racoon implementation (IPSec-tools).</title>
    <filename>group__tipsec__racoon__group.html</filename>
  </compound>
  <compound kind="group">
    <name>tipsec_vista_group</name>
    <title>IPSec Vista/7 implementation (WFP).</title>
    <filename>group__tipsec__vista__group.html</filename>
  </compound>
  <compound kind="group">
    <name>tipsec_xp_group</name>
    <title>IPSec XP implementation.</title>
    <filename>group__tipsec__xp__group.html</filename>
  </compound>
  <compound kind="struct">
    <name>tipsec_context_s</name>
    <filename>structtipsec__context__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>initialized</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a8ac8977c8c92fcb0ed6caa2e0d04accf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>started</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ab5202032c6f302c5af65f677e81f85ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_state_t</type>
      <name>state</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a2df38f03125e48e731fcb211b2048558</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>use_ipv6</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ae5095fe5b034d1f95b3a40f33c8859b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_ipproto_t</type>
      <name>ipproto</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>abd7d3a4162f66b9db9f804c6104af79c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_mode_t</type>
      <name>mode</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a56d1b9adaa7ba7f320fa329ceb3c8a63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_ealgorithm_t</type>
      <name>ealg</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a25f4a0fd55a15418ce390e8eda46efaf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_algorithm_t</type>
      <name>alg</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a697cdb7db7ecceaa5344678d964626a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_protocol_t</type>
      <name>protocol</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ae981aea3fc62cafbe8854831f6e07bdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>addr_remote</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a38b70b6c14625550d40696e30a1cdba8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_spi_t</type>
      <name>spi_pc</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ad42534d6f9918b2b76d54ddba141f6b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_spi_t</type>
      <name>spi_ps</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a1e6a1164174fb00ced5f7a0a5240311e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_port_t</type>
      <name>port_pc</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ac66f4a28b4adae716fb0eaf8084e3cec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_port_t</type>
      <name>port_ps</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a730ff7fa7683b47c0b7629a5edcb70e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>addr_local</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a811d6e6b4918e2d236eb921d4c49bda2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_spi_t</type>
      <name>spi_uc</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>afc536ff0c14d22abbe456ba5e40e2baf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_spi_t</type>
      <name>spi_us</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a3163c2199fd0b242546bb3d6d42ad779</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_port_t</type>
      <name>port_uc</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ac729f02f32c01188729b82d9e289dd3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_port_t</type>
      <name>port_us</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ad0412d958389edf7c73ec008fcf6674f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_key_t *</type>
      <name>ck</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a569633d677a9d05b76fb1a88b0b3f6e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_key_t *</type>
      <name>ik</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>ae9fefaf339adffbb94848b4b6a060c9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tipsec_lifetime_t</type>
      <name>lifetime</name>
      <anchorfile>structtipsec__context__s.html</anchorfile>
      <anchor>a95fe4be3dd2b115a2c9588815bddf7da</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
