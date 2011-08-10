<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>TinyHTTP API Overview</title>
    <filename>index</filename>
  </compound>
  <compound kind="file">
    <name>thttp.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/</path>
    <filename>thttp_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__event_8h" name="thttp_event.h" local="yes" imported="no">tinyhttp/thttp_event.h</includes>
    <includes id="thttp__session_8h" name="thttp_session.h" local="yes" imported="no">tinyhttp/thttp_session.h</includes>
    <class kind="struct">thttp_stack_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_STACK_SET_NULL</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>aa22aaebb83618128ec0704c0a33ceda1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_STACK_SET_LOCAL_IP</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>aa39965563df5974669824d7f747616f8</anchor>
      <arglist>(IP_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_STACK_SET_LOCAL_PORT</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a8ab31ae73d696f7a288deb807bb932e6</anchor>
      <arglist>(PORT_INT)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_STACK_SET_TLS_CERTS</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a2b1708f4a57a4f0f7ce44bf4fae6ebab</anchor>
      <arglist>(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_STACK_SET_USERDATA</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a6c61714a8e4f5962412e21022b726aa3</anchor>
      <arglist>(USERDATA_PTR)</arglist>
    </member>
    <member kind="typedef">
      <type>THTTP_BEGIN_DECLS enum thttp_stack_param_type_e</type>
      <name>thttp_stack_param_type_t</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>aa43a8bd7d848cc86857c75ea444b47a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_stack_s</type>
      <name>thttp_stack_t</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a6d44adbd32b3e94b88f726968b5aeb70</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_stack_param_type_e</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a777ab1dae7d35ae8e00c9440b3fda5d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_pname_null</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a777ab1dae7d35ae8e00c9440b3fda5d3a1d7408231cf27316c82c3ca6665bb560</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_stack_handle_t *</type>
      <name>thttp_stack_create</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga23f21d510973ba5761fd25286df19eef</anchor>
      <arglist>(thttp_stack_callback_f callback,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_stack_start</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga53cf350947bec4313d285a3b89df7f31</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_stack_set</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga6f67d03e0f023335b97788002fb247a2</anchor>
      <arglist>(thttp_stack_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const void *</type>
      <name>thttp_stack_get_userdata</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>gacf4a84954ca6c196f357cba4731e3ae3</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_stack_stop</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga46ca1b404d575a8b1f6c1bcea9cf9484</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_stack_def_t</name>
      <anchorfile>thttp_8h.html</anchorfile>
      <anchor>a95f5e2bebfd52c5688a6d02f77690175</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tinyhttp.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/</path>
    <filename>tinyhttp_8h</filename>
    <includes id="thttp_8h" name="thttp.h" local="yes" imported="no">thttp.h</includes>
    <includes id="thttp__action_8h" name="thttp_action.h" local="yes" imported="no">tinyhttp/thttp_action.h</includes>
    <includes id="thttp__parser__message_8h" name="thttp_parser_message.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_message.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <includes id="thttp__parser__header_8h" name="thttp_parser_header.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_header.h</includes>
    <includes id="thttp__header___dummy_8h" name="thttp_header_Dummy.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Dummy.h</includes>
    <includes id="thttp__header___e_tag_8h" name="thttp_header_ETag.h" local="yes" imported="no">tinyhttp/headers/thttp_header_ETag.h</includes>
  </compound>
  <compound kind="file">
    <name>thttp_auth.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/auth/</path>
    <filename>thttp__auth_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_NCOUNT_2_STRING</name>
      <anchorfile>thttp__auth_8h.html</anchorfile>
      <anchor>a108b41bf1d3e8f255a73f010235de2ea</anchor>
      <arglist>(nc_int32, nc_string)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API tsk_size_t</type>
      <name>thttp_auth_basic_response</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>gaf720869992f752dc684f3dc63f228be6</anchor>
      <arglist>(const char *userid, const char *password, char **response)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_auth_digest_HA1</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga512b7e39c4f78e196f225b6e75fc7551</anchor>
      <arglist>(const char *username, const char *realm, const char *password, tsk_md5string_t *ha1)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_auth_digest_HA1sess</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga82bbc1b3b5f7945349db1bf918c2a7cc</anchor>
      <arglist>(const char *username, const char *realm, const char *password, const char *nonce, const char *cnonce, tsk_md5string_t *ha1sess)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_auth_digest_HA2</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga35a96816fa3c4c48468187abc6ed36c3</anchor>
      <arglist>(const char *method, const char *url, const tsk_buffer_t *entity_body, const char *qop, tsk_md5string_t *ha2)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_auth_digest_response</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>gadfec81e484a75707d0319cbc9d1bb3ec</anchor>
      <arglist>(const tsk_md5string_t *ha1, const char *nonce, const nonce_count_t noncecount, const char *cnonce, const char *qop, const tsk_md5string_t *ha2, tsk_md5string_t *response)</arglist>
    </member>
    <member kind="variable">
      <type>THTTP_BEGIN_DECLS typedef char</type>
      <name>nonce_count_t</name>
      <anchorfile>thttp__auth_8h.html</anchorfile>
      <anchor>a6f072daaf74ecb5ac2ea368e885741ff</anchor>
      <arglist>[9]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_challenge.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/auth/</path>
    <filename>thttp__challenge_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <includes id="thttp__auth_8h" name="thttp_auth.h" local="yes" imported="no">tinyhttp/auth/thttp_auth.h</includes>
    <class kind="struct">thttp_challenge_s</class>
    <member kind="typedef">
      <type>THTTP_BEGIN_DECLS struct thttp_challenge_s</type>
      <name>thttp_challenge_t</name>
      <anchorfile>thttp__challenge_8h.html</anchorfile>
      <anchor>a18b8f955ae2aa14669f88e8a214687f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>thttp_challenges_L_t</name>
      <anchorfile>thttp__challenge_8h.html</anchorfile>
      <anchor>a95d31c3cc8a65c39df80c9e96c7b2df0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_challenge_update</name>
      <anchorfile>thttp__challenge_8h.html</anchorfile>
      <anchor>a3dfcdc7a0b7e13d2e1b8f34d85c0aa97</anchor>
      <arglist>(thttp_challenge_t *self, const char *scheme, const char *realm, const char *nonce, const char *opaque, const char *algorithm, const char *qop)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_t *</type>
      <name>thttp_challenge_create_header_authorization</name>
      <anchorfile>thttp__challenge_8h.html</anchorfile>
      <anchor>af754fbfa7db986e184bba843f946d47b</anchor>
      <arglist>(thttp_challenge_t *self, const char *username, const char *password, const thttp_request_t *request)</arglist>
    </member>
    <member kind="function">
      <type>thttp_challenge_t *</type>
      <name>thttp_challenge_create</name>
      <anchorfile>thttp__challenge_8h.html</anchorfile>
      <anchor>a318168bdbcafa58b5c669569df78675d</anchor>
      <arglist>(tsk_bool_t isproxy, const char *scheme, const char *realm, const char *nonce, const char *opaque, const char *algorithm, const char *qop)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_challenge_def_t</name>
      <anchorfile>thttp__challenge_8h.html</anchorfile>
      <anchor>a9b17030a9355e16ee5090751b92fb57f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <class kind="struct">thttp_header_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a868c72981915275b725b056cd8a3f7a7</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_PARAMS</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a6616d5c1b24d815837ec672e4176377a</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_VALUE_TOSTRING_F</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a8a42d2de0768b3671ba15b116082e8fc</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac340624692d0a7e2eae1aefcd9f563c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>thttp_header_value_tostring_f</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a08d2c90e143aef55226c37aa69c0371f</anchor>
      <arglist>)(const struct thttp_header_s *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_header_type_e</type>
      <name>thttp_header_type_t</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a51e8bf92fa5ed1d8f51415ef6f731ca0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_header_s</type>
      <name>thttp_header_t</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a9e194bb99d8904bc6df619eb50bda13a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>thttp_headers_L_t</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>af86b639ed08ea957275dce83d248325e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_header_type_e</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Authorization</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a770b789c54e20fbea69d133bec598384</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Content_Length</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a197a21f3a7e3f358a012b6331f0c3472</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Content_Type</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a43637319a23a9c01b51715915a41ba8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Dummy</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3ab19a252a14fc57329e6c2ff21e678f66</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_ETag</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a5fc20e555c17ac3e067c6e39bf9fdffd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Proxy_Authenticate</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a9ac8ce958e6850cca7d9fa4a878a1374</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Proxy_Authorization</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a8ae250e4da5f57fca5eb36f227409c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_Transfer_Encoding</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a396cb186e47c23c5e52be304b6a5bd47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_htype_WWW_Authenticate</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>ac740fe3f373d2a91913437e31e44d4b3a2a2fb50fc02e75f40fd4e36e2d5d68cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const char *</type>
      <name>thttp_header_get_name</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a06261a76820856a92828ff8abdc1b756</anchor>
      <arglist>(thttp_header_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const char *</type>
      <name>thttp_header_get_nameex</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a9cc8de7e894d5cea0306c1ebbc807177</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API char</type>
      <name>thttp_header_get_param_separator</name>
      <anchorfile>thttp__header_8h.html</anchorfile>
      <anchor>a4f9bf41358736bdb7d4f0566c68c36f5</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_header_serialize</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga7abd684222ab6096765addca79071561</anchor>
      <arglist>(const thttp_header_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API char *</type>
      <name>thttp_header_tostring</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga2ba34e8e33da57b8af99350c85768099</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API char *</type>
      <name>thttp_header_value_tostring</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga770293aff5d59b867680f5c099eee323</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Authorization.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___authorization_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_Authorization_s</class>
    <member kind="typedef">
      <type>THTTP_BEGIN_DECLS struct thttp_header_Authorization_s</type>
      <name>thttp_header_Authorization_t</name>
      <anchorfile>thttp__header___authorization_8h.html</anchorfile>
      <anchor>a054e46dd08b3bd0963391b3da50911ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>thttp_header_Authorization_t</type>
      <name>thttp_header_Proxy_Authorization_t</name>
      <anchorfile>thttp__header___authorization_8h.html</anchorfile>
      <anchor>a041db7f5305a5d40769d3d1654856b73</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_header_Authorization_t *</type>
      <name>thttp_header_Authorization_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>gaf62fc4d2fedd30ee7dc84856859e8a4f</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_header_Proxy_Authorization_t *</type>
      <name>thttp_header_Proxy_Authorization_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga2d423bf012c44446fbadc8f2e9e9c753</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Authorization_t *</type>
      <name>thttp_header_authorization_create</name>
      <anchorfile>thttp__header___authorization_8h.html</anchorfile>
      <anchor>a166164e6a4ed700ef370b5777aecde26</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_Authorization_def_t</name>
      <anchorfile>thttp__header___authorization_8h.html</anchorfile>
      <anchor>a806d394abc8668dad45d9f49e3d1334e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Content_Length.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___content___length_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_Content_Length_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_CONTENT_LENGTH_VA_ARGS</name>
      <anchorfile>thttp__header___content___length_8h.html</anchorfile>
      <anchor>a22456e388f139ec91728f982c966b147</anchor>
      <arglist>(length)</arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_header_Content_Length_s</type>
      <name>thttp_header_Content_Length_t</name>
      <anchorfile>thttp__header___content___length_8h.html</anchorfile>
      <anchor>a1a1e39d3d7b19fc6bc36913069c7f5c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Length_t *</type>
      <name>thttp_header_Content_Length_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga4c9a74cd97da944385e046590fa0c4c1</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_Content_Length_def_t</name>
      <anchorfile>thttp__header___content___length_8h.html</anchorfile>
      <anchor>afbf282ffb3a90006cc1a20948e93dbb6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Content_Type.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___content___type_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_Content_Type_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_CONTENT_TYPE_VA_ARGS</name>
      <anchorfile>thttp__header___content___type_8h.html</anchorfile>
      <anchor>aa106b22b9f042dec5f93b04bc17ebf10</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_header_Content_Type_s</type>
      <name>thttp_header_Content_Type_t</name>
      <anchorfile>thttp__header___content___type_8h.html</anchorfile>
      <anchor>ae7ac03f008d0057187041e2e8e93bd6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Type_t *</type>
      <name>thttp_header_Content_Type_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga88c3c565650876a99c75c7503333cced</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_Content_Type_def_t</name>
      <anchorfile>thttp__header___content___type_8h.html</anchorfile>
      <anchor>aae2f5aaccb96719f11770748caa192b6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Dummy.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___dummy_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_Dummy_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_DUMMY_VA_ARGS</name>
      <anchorfile>thttp__header___dummy_8h.html</anchorfile>
      <anchor>ae5f98a8670248c7cd39ec1e889a01912</anchor>
      <arglist>(name, value)</arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_header_Dummy_s</type>
      <name>thttp_header_Dummy_t</name>
      <anchorfile>thttp__header___dummy_8h.html</anchorfile>
      <anchor>a5784b5077571d8fcd531790929706ae8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Dummy_t *</type>
      <name>thttp_header_Dummy_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>gaa51ce8cbaeba9ffaf24847f4ff6c6c63</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_Dummy_def_t</name>
      <anchorfile>thttp__header___dummy_8h.html</anchorfile>
      <anchor>a9f358237c9e4f069e59ef43611a70a52</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_ETag.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___e_tag_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_ETag_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_ETAG_VA_ARGS</name>
      <anchorfile>thttp__header___e_tag_8h.html</anchorfile>
      <anchor>a039bccad262e308c8b74b2541b6d8304</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_header_ETag_s</type>
      <name>thttp_header_ETag_t</name>
      <anchorfile>thttp__header___e_tag_8h.html</anchorfile>
      <anchor>a9fa8f6c90c1352f4161c632a3c51d228</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_ETag_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga8f80a83c38b4b3c23ef9adca526c2067</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_etag_create</name>
      <anchorfile>thttp__header___e_tag_8h.html</anchorfile>
      <anchor>a51972875d3d2702e9bb2d1cad9b45659</anchor>
      <arglist>(const char *value)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_etag_create_null</name>
      <anchorfile>thttp__header___e_tag_8h.html</anchorfile>
      <anchor>ad07c1d52806e9274dc962ec7db01af97</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_ETag_def_t</name>
      <anchorfile>thttp__header___e_tag_8h.html</anchorfile>
      <anchor>aa6c25aa22c1f30f0bc0d49db495e304b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Transfer_Encoding.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___transfer___encoding_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_Transfer_Encoding_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_HEADER_TRANSFER_ENCODING_VA_ARGS</name>
      <anchorfile>thttp__header___transfer___encoding_8h.html</anchorfile>
      <anchor>af3d45684db89cc6a76d82e56abf46bd6</anchor>
      <arglist>(encoding)</arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_header_Transfer_Encoding_s</type>
      <name>thttp_header_Transfer_Encoding_t</name>
      <anchorfile>thttp__header___transfer___encoding_8h.html</anchorfile>
      <anchor>a819c92eb51b3f91042057b99c1c33a78</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_Transfer_Encoding_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga16eb00aaf3ca02295c0c7dcd1f7c808d</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_transfer_encoding_create</name>
      <anchorfile>thttp__header___transfer___encoding_8h.html</anchorfile>
      <anchor>a153a7a9fd2726478bc543fffaab1bf2b</anchor>
      <arglist>(const char *encoding)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_transfer_encoding_create_null</name>
      <anchorfile>thttp__header___transfer___encoding_8h.html</anchorfile>
      <anchor>a660ce9c41e898c7a557faa7e66234f93</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_Transfer_Encoding_def_t</name>
      <anchorfile>thttp__header___transfer___encoding_8h.html</anchorfile>
      <anchor>a8451162b13619940f0a0b438c840fe61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_WWW_Authenticate.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/headers/</path>
    <filename>thttp__header___w_w_w___authenticate_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <class kind="struct">thttp_header_WWW_Authenticate_s</class>
    <member kind="typedef">
      <type>THTTP_BEGIN_DECLS struct thttp_header_WWW_Authenticate_s</type>
      <name>thttp_header_WWW_Authenticate_t</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8h.html</anchorfile>
      <anchor>aac1441a1db75b97f56be36469d797f7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>thttp_header_WWW_Authenticate_t</type>
      <name>thttp_header_Proxy_Authenticate_t</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8h.html</anchorfile>
      <anchor>ac37330aa4d069e45b8a98b10939723df</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_header_WWW_Authenticate_t *</type>
      <name>thttp_header_WWW_Authenticate_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga6ea6cdaa0f153de0f53b25d02a99778c</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_header_Proxy_Authenticate_t *</type>
      <name>thttp_header_Proxy_Authenticate_parse</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8h.html</anchorfile>
      <anchor>a6f4e9203753aa6bd36d40a677946569a</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_header_WWW_Authenticate_def_t</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8h.html</anchorfile>
      <anchor>a036a29510b2ecda5c1da6f664b8cc5b1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_parser_header.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/parsers/</path>
    <filename>thttp__parser__header_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <member kind="function">
      <type>THTTP_BEGIN_DECLS int</type>
      <name>thttp_header_parse</name>
      <anchorfile>thttp__parser__header_8h.html</anchorfile>
      <anchor>ab4743228cce981790ac00a1a39fde530</anchor>
      <arglist>(tsk_ragel_state_t *state, thttp_message_t *message)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_parser_message.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/parsers/</path>
    <filename>thttp__parser__message_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <member kind="function">
      <type>THTTP_BEGIN_DECLS TINYHTTP_API int</type>
      <name>thttp_message_parse</name>
      <anchorfile>thttp__parser__message_8h.html</anchorfile>
      <anchor>a568a948c7793fa708ba269a54448581f</anchor>
      <arglist>(tsk_ragel_state_t *state, thttp_message_t **result, tsk_bool_t extract_content)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_parser_url.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/parsers/</path>
    <filename>thttp__parser__url_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__url_8h" name="thttp_url.h" local="yes" imported="no">tinyhttp/thttp_url.h</includes>
    <member kind="function">
      <type>THTTP_BEGIN_DECLS TINYHTTP_API thttp_url_t *</type>
      <name>thttp_url_parse</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga2204b5747836c64bea185be649ce1470</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_action.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/</path>
    <filename>thttp__action_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__session_8h" name="thttp_session.h" local="yes" imported="no">tinyhttp/thttp_session.h</includes>
    <class kind="struct">thttp_action_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_INVALID_ID</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a20d504500fcc14c31fb22fef1e9a1250</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_INVALID_HANDLE</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a017a0448f203a91d8cf5bcc23bb74848</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_OPTION</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gafddb2b8c342d440cb0be8ae5f55714c0</anchor>
      <arglist>(ID_ENUM, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_HEADER</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga276d6118d0038e4bdab2a02027c53c4e</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_PAYLOAD</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gab54688c397a0df38f72e19d3e761513c</anchor>
      <arglist>(PAY_PTR, PAY_SIZE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_NULL</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga168e5e2a98540e218c15868f86cf494f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_CONNECT</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gad1a804068d4f970d35b62506405bb1b9</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_DELETE</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gae4dc4de16832e1756213d8f2a60c286d</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_GET</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga25a8af4242b64342c590ef5ee8db56b4</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_HEAD</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga659e821e8419c3046b89480cdd3cdaec</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_OPTIONS</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga6b5adbcce3f8d0bd215ff75f1c9f59cf</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_PATCH</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga95836da9e16ff5bb6ad7e7cea186d760</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_POST</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga34c6edcf5b07069d79ced7d2a99b3a2f</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_PUT</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gaa0ebc1d07393d144d54319044d33911a</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_TRACE</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga99cf9ae9b1e646d8d54dc07f841ae52a</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_action_option_e</type>
      <name>thttp_action_option_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a6373665289b9c54cd181e81ff55c7243</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_action_type_e</type>
      <name>thttp_action_type_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a0bd496f956529711c69a4b9b06179c04</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_action_param_type_e</type>
      <name>thttp_action_param_type_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a14aa9780ca4255c9c506d4c6e5959029</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_action_s</type>
      <name>thttp_action_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ae9349fa0651e1123667a9b3e61ac3dcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>thttp_action_handle_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>adb09dac17a8d26456e99473fbdbecd5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>thttp_actions_L_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a2daa7482248c60ac251b34539f206712</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_action_option_e</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ac7e8501a0bc472a5dbb2b0d35bc5fac9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>THTTP_ACTION_OPTION_TIMEOUT</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ac7e8501a0bc472a5dbb2b0d35bc5fac9ae29bc22ba4b0dbab91762df2498bec75</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_action_type_e</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_atype_o_request</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20da94dd5a15405a3e9670fd7ec4c9feffe3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_atype_i_message</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20da1526a13b8d09f2c59bd1c800305b6527</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_thttp_atype_closed</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20dacce4bfb9974aab252afda32c64eb2889</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_atype_error</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20da3cded72460652754f0f3ddd0780f4266</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_atype_close</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20da683a2f2397f380fc974edf986b26b9f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_atype_cancel</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad6ebb36157ee09773621e943b01ee20dadeeceaaa38136910d50512d0a514fb40</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_action_param_type_e</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad990d5d648fad6d0d72b341c47d69db1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_aptype_null</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad990d5d648fad6d0d72b341c47d69db1a5fe2b29cb022f67565f6ed610356e56d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_aptype_option</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad990d5d648fad6d0d72b341c47d69db1a588f77a0488f3e9389a47636118fd251</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_aptype_header</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad990d5d648fad6d0d72b341c47d69db1aee6021949ec5b5736b201a0b9c0a2546</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_aptype_payload</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>ad990d5d648fad6d0d72b341c47d69db1af479bb13b57cebbc5e59da1c5af89c5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_action_perform</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga36b1aa78a24305f56973f370f356236d</anchor>
      <arglist>(thttp_session_handle_t *session, const char *urlstring, const char *method,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_action_t *</type>
      <name>thttp_action_create</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a869d3344724238503662712499e750ea</anchor>
      <arglist>(thttp_action_type_t type, const char *urlstring, const char *method, va_list *app)</arglist>
    </member>
    <member kind="variable">
      <type>THTTP_BEGIN_DECLS typedef uint64_t</type>
      <name>thttp_action_id_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>a74615a300b43700d75bbf75dbd3fca66</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_action_def_t</name>
      <anchorfile>thttp__action_8h.html</anchorfile>
      <anchor>afdd97f7a8f3f6105422357dd67528bec</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_dialog.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/</path>
    <filename>thttp__dialog_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <class kind="struct">thttp_dialog_s</class>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>thttp_dialog_id_t</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>a57e73702af9b64aba5c0ff3c05579720</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_dialog_s</type>
      <name>thttp_dialog_t</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>a587ed7cde413d761276ff1156ee86c06</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>thttp_dialogs_L_t</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>aa7df434225327b7d3e00ee71a14fae5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_dialog_fsm_act</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>a6a81fe7b67b23d5f4c784e3b7584621d</anchor>
      <arglist>(thttp_dialog_t *self, tsk_fsm_action_id, const struct thttp_message_s *, const struct thttp_action_s *)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_dialog_t *</type>
      <name>thttp_dialog_new</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>a9aa550eb12714d4acc51bb40db0cfd35</anchor>
      <arglist>(struct thttp_session_s *session)</arglist>
    </member>
    <member kind="function">
      <type>thttp_dialog_t *</type>
      <name>thttp_dialog_get_oldest</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>a11a7f172eec35d75c54445562db2733e</anchor>
      <arglist>(thttp_dialogs_L_t *dialogs)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_dialog_def_t</name>
      <anchorfile>thttp__dialog_8h.html</anchorfile>
      <anchor>a8d5cfa0ffc8894fdf08b9ca63ad99693</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_event.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/</path>
    <filename>thttp__event_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__session_8h" name="thttp_session.h" local="yes" imported="no">tinyhttp/thttp_session.h</includes>
    <class kind="struct">thttp_event_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_EVENT</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a43116b34b6b0e891dc43f62042f22fc6</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_event_type_e</type>
      <name>thttp_event_type_t</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a7e3ffc2ecbf1689482b62c4ffe191de6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_event_s</type>
      <name>thttp_event_t</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a557fba33080480bef7f571730aef67f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>thttp_stack_callback_f</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a6928f5272b66dbe7301b45eae0b53002</anchor>
      <arglist>)(const thttp_event_t *httpevent)</arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_event_type_e</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a51c721cc0f4d910ee95ddeace0837bf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_event_message</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a51c721cc0f4d910ee95ddeace0837bf0a2e14763b19bef1bdcdb5822236e27821</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_event_auth_failed</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a51c721cc0f4d910ee95ddeace0837bf0a4d59f64b54e1cf6d0b4fcc0b5c5fc24f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_event_closed</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a51c721cc0f4d910ee95ddeace0837bf0a5b7b35a75468449e47b64012c3b6c7da</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_event_transport_error</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a51c721cc0f4d910ee95ddeace0837bf0aa4d012a82186a348afe99d67ff3bf731</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>thttp_event_t *</type>
      <name>thttp_event_create</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a957893a2f552054a69a7e63ab96ddb04</anchor>
      <arglist>(thttp_event_type_t type, const thttp_session_handle_t *session, const char *description, const thttp_message_t *message)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const void *</type>
      <name>thttp_event_def_t</name>
      <anchorfile>thttp__event_8h.html</anchorfile>
      <anchor>a5902c37ff7680aecd20346b580f1bb9c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_message.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/</path>
    <filename>thttp__message_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__url_8h" name="thttp_url.h" local="yes" imported="no">tinyhttp/thttp_url.h</includes>
    <includes id="thttp__header___content___length_8h" name="thttp_header_Content_Length.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Content_Length.h</includes>
    <includes id="thttp__header___content___type_8h" name="thttp_header_Content_Type.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Content_Type.h</includes>
    <class kind="struct">thttp_message_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_10</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga6fccc5ffca4ebf347df8399b2e963209</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_11</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga8155bb2f79c1fe2df0e2b2321025d192</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_20</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga796b8afa7aea730922da497f01fe1871</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_DEFAULT</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gacf15d8ae6e023b950943a9f6b7fd8c9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_IS_REQUEST</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaa9a2544392c121a7ff4e8ae169e8484f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_IS_RESPONSE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga52f49cde166aaca9f848361cb596f259</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga90c3419edd3fe5e9a628b80f5f35f2a1</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_AS_RESPONSE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac6f99e8cdc194ef64689b46cb1a35742</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_AS_REQUEST</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac831236cab706205d058e4428904b113</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_CODE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaca2eee1f3f53fb4ca5f80250a2cb92df</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_PHRASE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga651d0e661c88033b07a8a29cce10f1be</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_REQUEST_METHOD</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gafee4f7e440362a26739ee31806556213</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_REQUEST_URL</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3e3bdb1f4760ed4bda3ddeb780c0f653</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_CONTENT_LENGTH</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gab9825bef1b6278ceb242885043b2c177</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_CONTENT</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga57bcc0247fdfdc73b73a1655780a7129</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_HAS_CONTENT</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga365e5ba009877617b1cdf3d183bb1fd9</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>ae3b5033d849e708b7a0b97d34952f8fc</anchor>
      <arglist>(self, code)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_NXX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a641d321a01174ac61aed91bd50dfec1c</anchor>
      <arglist>(self, N)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_1XX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>aacd611cd6935f469dc6c555ec57d959c</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_2XX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>ad472dd9ace91c5afeedd5be289fcbdd7</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_3XX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a1bb593c25cdc33073522999490a205b1</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_4XX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>aab178a4a7270de04c6b0974d101c96bc</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_5XX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a57ef1489ee218761ab796dab2acfc51f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_6XX</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>abd83a5ef14e80dcf4ced825c5d78281d</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_IS_23456</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a6af504e12b000e93068c604a91b4a46c</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_ADD_HEADER</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>ad4e69079adee1808caa415e9fdd4b6dd</anchor>
      <arglist>(self, objdef,...)</arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_message_type_e</type>
      <name>thttp_message_type_t</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a843ab805f01eafc2570957d479777c7d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_message_s</type>
      <name>thttp_message_t</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>ae1a40b9ad336160d8ae5b510d079c7fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>thttp_message_t</type>
      <name>thttp_request_t</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a57b38051641d423a2cfc011d9b1cbd50</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>thttp_message_t</type>
      <name>thttp_response_t</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a71eaa24ef161dd63f934792444c9c0f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_message_type_e</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a47f7887c7582df2ae9e329dc76a126fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_unknown</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a47f7887c7582df2ae9e329dc76a126fda3e40a32359a3d8411b70412fb477d1ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_request</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a47f7887c7582df2ae9e329dc76a126fda587157828bb70e733106a050499b11d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_response</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a47f7887c7582df2ae9e329dc76a126fda39efe54fb19fd003cad37c88e0360eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_message_add_header</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga0067f283164eb98c6850328b39d5ff26</anchor>
      <arglist>(thttp_message_t *self, const thttp_header_t *hdr)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_message_add_headers</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga9d772552b9a892b718f0fd8aff3b86b2</anchor>
      <arglist>(thttp_message_t *self, const thttp_headers_L_t *headers)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_message_add_content</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga0fbe5b273002fc7b0bfaa4ce306508af</anchor>
      <arglist>(thttp_message_t *self, const char *content_type, const void *content, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_message_append_content</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gada8275938d66a7f7c77a18b40dae685e</anchor>
      <arglist>(thttp_message_t *self, const void *content, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const thttp_header_t *</type>
      <name>thttp_message_get_headerAt</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gacc210783cfa8c4f937430dd1f2508611</anchor>
      <arglist>(const thttp_message_t *self, thttp_header_type_t type, tsk_size_t index)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const thttp_header_t *</type>
      <name>thttp_message_get_header</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga423b5655de3f4ddff2a05a259ad7557f</anchor>
      <arglist>(const thttp_message_t *self, thttp_header_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const thttp_header_t *</type>
      <name>thttp_message_get_headerByName</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gadbe0434124bf2f50b12fdba10a44fb2b</anchor>
      <arglist>(const thttp_message_t *self, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_message_serialize</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3f87c7ab7eb5bb730f5b153fcc1dc1c0</anchor>
      <arglist>(const thttp_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API char *</type>
      <name>thttp_message_tostring</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga58993e3fc64c1116997758f6c4e468d1</anchor>
      <arglist>(const thttp_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_request_t *</type>
      <name>thttp_request_new</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga5dcd151de1e0ad4aec5e5212a506b889</anchor>
      <arglist>(const char *method, const thttp_url_t *request_url)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_response_t *</type>
      <name>thttp_response_new</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac011c57f8bd07ed2662bbc71b6e44a62</anchor>
      <arglist>(short status_code, const char *reason_phrase, const thttp_request_t *request)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_message_t *</type>
      <name>thttp_message_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3fad120325a24fcdab2c8a1aac655b84</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_request_t *</type>
      <name>thttp_request_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaa65be3fa6b585652c98594734b64efa8</anchor>
      <arglist>(const char *method, const thttp_url_t *url)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_message_def_t</name>
      <anchorfile>thttp__message_8h.html</anchorfile>
      <anchor>a8040423d31bfcfff0b6988678929f344</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_session.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/</path>
    <filename>thttp__session_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <includes id="thttp__challenge_8h" name="thttp_challenge.h" local="yes" imported="no">tinyhttp/auth/thttp_challenge.h</includes>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <includes id="thttp__dialog_8h" name="thttp_dialog.h" local="yes" imported="no">tinyhttp/thttp_dialog.h</includes>
    <class kind="struct">thttp_session_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_INVALID_ID</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ae558d602903bc13694ee6345de296c76</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_INVALID_HANDLE</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a592226c437d90cc444c3e36058c93192</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_OPTION</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga3b48b30caffdca4b22831e6beb10a196</anchor>
      <arglist>(ID_ENUM, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_CRED</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gaeeccefade84db508310850d9d0f6d368</anchor>
      <arglist>(USERNAME_STR, PASSWORD_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_HEADER</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga5ed06de61a21d4c422e4ec5d56ff121d</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_UNSET_HEADER</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga4c820f1027fb3479cfb4bf5d7dbf56fb</anchor>
      <arglist>(NAME_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_USERDATA</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga6252a61755fc2f58722844795ee98938</anchor>
      <arglist>(USERDATA_PTR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_NULL</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gad00fd2b6fdfc9f0fe5001aee6fbfdff7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>thttp_session_id_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>aa6372f0a6a5f36d90f0b7e10958cfcb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_session_option_e</type>
      <name>thttp_session_option_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>afca88a535a5b7b953dcdcb54b45f41b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_session_param_type_e</type>
      <name>thttp_session_param_type_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>acca573689cd64890cc8c9c41f6d53543</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_session_s</type>
      <name>thttp_session_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>acb50bccb108b39f53be974b1e5d89995</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>thttp_sessions_L_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a5454a641989a0b5aefff214ed0a8f41e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>thttp_session_handle_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a0697a651e7789539f8fa15c2ee3e8565</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>thttp_stack_handle_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a41c8dff11e4abd7d8043716a46db1060</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_session_option_e</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a693a3edc20cb070116e6ca8400b8e337</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>THTTP_SESSION_OPTION_TIMEOUT</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a693a3edc20cb070116e6ca8400b8e337a94cc57900b9735e4313e16c36cfb3416</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_session_param_type_e</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ac60e1a2d905c79691f843b2ff333c7e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>httpp_null</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ac60e1a2d905c79691f843b2ff333c7e3ad0712c5b56959908ae2d1d5687aaf142</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>httpp_option</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ac60e1a2d905c79691f843b2ff333c7e3a97698826573c58b30fe72710553c1bb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>httpp_cred</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ac60e1a2d905c79691f843b2ff333c7e3a03d31a322cd2b83a8ca8e0fb87233e62</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>httpp_header</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ac60e1a2d905c79691f843b2ff333c7e3a895def420a1dcc12fce22e87fa052878</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>httpp_userdata</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>ac60e1a2d905c79691f843b2ff333c7e3a067b4c74eaac026c6a463d4bb0994efc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_session_handle_t *</type>
      <name>thttp_session_create</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga4ea39662c02afa5c56439c4f087511e9</anchor>
      <arglist>(const thttp_stack_handle_t *stack,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_session_set</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gad32c0f75635447af16a761bdab0b89bc</anchor>
      <arglist>(thttp_session_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_session_id_t</type>
      <name>thttp_session_get_id</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga183dab1dec59d010e2efa1be4809e3c0</anchor>
      <arglist>(const thttp_session_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API const void *</type>
      <name>thttp_session_get_userdata</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga0f457e2b4f3d1cc43074230a174cca81</anchor>
      <arglist>(const thttp_session_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_session_closefd</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a996c48c3c855e394db760a92133644aa</anchor>
      <arglist>(thttp_session_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_update_challenges</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a91341cd284ebed49ece3d7e0c1ffac75</anchor>
      <arglist>(thttp_session_t *self, const thttp_response_t *response, tsk_bool_t answered)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_signal_closed</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a00500a6442e130d6f1a0ac29084cfa6e</anchor>
      <arglist>(thttp_session_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_signal_error</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a8d99c10a76adba8db5ae8a3f5a04ef0c</anchor>
      <arglist>(thttp_session_t *self)</arglist>
    </member>
    <member kind="function">
      <type>thttp_session_t *</type>
      <name>thttp_session_get_by_fd</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>a1e988df3ceff34ac789db8845416695a</anchor>
      <arglist>(thttp_sessions_L_t *sessions, tnet_fd_t fd)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_session_def_t</name>
      <anchorfile>thttp__session_8h.html</anchorfile>
      <anchor>aa76ab49cc2bf8b06a8710dd2363dd9bc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_url.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/tinyhttp/</path>
    <filename>thttp__url_8h</filename>
    <includes id="tinyhttp__config_8h" name="tinyhttp_config.h" local="yes" imported="no">tinyhttp_config.h</includes>
    <class kind="struct">thttp_url_s</class>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_URL_IS_SECURE</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a57cff1b0002ccece844698c368dc6977</anchor>
      <arglist>(url)</arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_url_type_e</type>
      <name>thttp_url_type_t</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a945d6b58ceb5915e0ae6599d908fa2f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum thttp_host_type_e</type>
      <name>thttp_host_type_t</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a3f252099597edf09599b4215afbd33ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct thttp_url_s</type>
      <name>thttp_url_t</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a952418bd9b2f4a4fe8005cf489de403a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_url_type_e</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>af22b11b0c965c8c7c48f518abc48461e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_url_unknown</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>af22b11b0c965c8c7c48f518abc48461eac6966438fbdde9d5b4002bbdc272e0e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_url_http</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>af22b11b0c965c8c7c48f518abc48461eae00ad6d35614bde9178c62d99d7371b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_url_https</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>af22b11b0c965c8c7c48f518abc48461ea7eb154f55d294931bc37ea828a61086c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>thttp_host_type_e</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a011593e62d5f5598b1b3a4cf772cbef4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_host_unknown</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a011593e62d5f5598b1b3a4cf772cbef4a7fe650cff2182f4d64de44fb24af16d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_host_hostname</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a011593e62d5f5598b1b3a4cf772cbef4ad30b915ca257e90c8684fab6b7d26902</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_host_ipv4</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a011593e62d5f5598b1b3a4cf772cbef4a1af01821ee02ff7cf2b5aba5d789c5a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>thttp_host_ipv6</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a011593e62d5f5598b1b3a4cf772cbef4abe089701023a2324b0000de5311bd0a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API int</type>
      <name>thttp_url_serialize</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga9b7274dffec6932a2851513b865226b8</anchor>
      <arglist>(const thttp_url_t *url, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API char *</type>
      <name>thttp_url_tostring</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>gadc56e46c5df2f68f9a359d65f566904e</anchor>
      <arglist>(const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API thttp_url_t *</type>
      <name>thttp_url_clone</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga164927101cd3c02cd641e1bea51b836a</anchor>
      <arglist>(const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>TINYHTTP_API tsk_bool_t</type>
      <name>thttp_url_isvalid</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>gabec149e64b6b9adffc609190be8c9c33</anchor>
      <arglist>(const char *urlstring)</arglist>
    </member>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_create</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga3326c658c6aa66b70472a129190a8f27</anchor>
      <arglist>(thttp_url_type_t type)</arglist>
    </member>
    <member kind="variable">
      <type>TINYHTTP_GEXTERN const tsk_object_def_t *</type>
      <name>thttp_url_def_t</name>
      <anchorfile>thttp__url_8h.html</anchorfile>
      <anchor>a6eec1868fa5fa2b26b2b837a7450f0f3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tinyhttp_config.h</name>
    <path>C:/Projects/Doubango/tinyHTTP/include/</path>
    <filename>tinyhttp__config_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TINYHTTP_API</name>
      <anchorfile>tinyhttp__config_8h.html</anchorfile>
      <anchor>ac042a298e1ceba9c523f9cbea33509b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYHTTP_GEXTERN</name>
      <anchorfile>tinyhttp__config_8h.html</anchorfile>
      <anchor>ab5292e1e342d7d2dad9961f02e0468ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_BEGIN_DECLS</name>
      <anchorfile>tinyhttp__config_8h.html</anchorfile>
      <anchor>a8ba9bcf4430a75f1160185b44b401c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_END_DECLS</name>
      <anchorfile>tinyhttp__config_8h.html</anchorfile>
      <anchor>a49350caf57b39001d75fd5b8e3dd70c8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_auth.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/auth/</path>
    <filename>thttp__auth_8c</filename>
    <includes id="thttp__auth_8h" name="thttp_auth.h" local="yes" imported="no">tinyhttp/auth/thttp_auth.h</includes>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>thttp_auth_basic_response</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>gaf720869992f752dc684f3dc63f228be6</anchor>
      <arglist>(const char *userid, const char *password, char **response)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_HA1</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga512b7e39c4f78e196f225b6e75fc7551</anchor>
      <arglist>(const char *username, const char *realm, const char *password, tsk_md5string_t *ha1)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_HA1sess</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga82bbc1b3b5f7945349db1bf918c2a7cc</anchor>
      <arglist>(const char *username, const char *realm, const char *password, const char *nonce, const char *cnonce, tsk_md5string_t *ha1sess)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_HA2</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga35a96816fa3c4c48468187abc6ed36c3</anchor>
      <arglist>(const char *method, const char *url, const tsk_buffer_t *entity_body, const char *qop, tsk_md5string_t *ha2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_response</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>gadfec81e484a75707d0319cbc9d1bb3ec</anchor>
      <arglist>(const tsk_md5string_t *ha1, const char *nonce, const nonce_count_t noncecount, const char *cnonce, const char *qop, const tsk_md5string_t *ha2, tsk_md5string_t *response)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_challenge.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/auth/</path>
    <filename>thttp__challenge_8c</filename>
    <includes id="thttp__challenge_8h" name="thttp_challenge.h" local="yes" imported="no">tinyhttp/auth/thttp_challenge.h</includes>
    <includes id="thttp_8h" name="thttp.h" local="yes" imported="no">thttp.h</includes>
    <includes id="thttp__header___authorization_8h" name="thttp_header_Authorization.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Authorization.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_CHALLENGE_IS_DIGEST</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a17266f53b47c6759209b024cfeec652e</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_CHALLENGE_IS_BASIC</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>affefab655d3dcf05e87102924c3426b9</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_CHALLENGE_IS_AKAv1</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a17c7cf7ae6a60a7ec4f737534ff78186</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_CHALLENGE_IS_AKAv2</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>ae62785164aa259e6ee525611fe749235</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_AUTH_COPY_VALUES</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a3bd5133626ec170c6a86d945a95c12aa</anchor>
      <arglist>(hdr)</arglist>
    </member>
    <member kind="function">
      <type>thttp_challenge_t *</type>
      <name>thttp_challenge_create</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a318168bdbcafa58b5c669569df78675d</anchor>
      <arglist>(tsk_bool_t isproxy, const char *scheme, const char *realm, const char *nonce, const char *opaque, const char *algorithm, const char *qop)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_challenge_reset_cnonce</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a9a8e025b746285d9fc92ce1075b98e2d</anchor>
      <arglist>(thttp_challenge_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_challenge_get_digest_response</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>ade573d9fc358704de4e1e7edb359bc76</anchor>
      <arglist>(thttp_challenge_t *self, const char *username, const char *password, const char *method, const char *uristring, const tsk_buffer_t *entity_body, char **response)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_challenge_update</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a3dfcdc7a0b7e13d2e1b8f34d85c0aa97</anchor>
      <arglist>(thttp_challenge_t *self, const char *scheme, const char *realm, const char *nonce, const char *opaque, const char *algorithm, const char *qop)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_t *</type>
      <name>thttp_challenge_create_header_authorization</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>af754fbfa7db986e184bba843f946d47b</anchor>
      <arglist>(thttp_challenge_t *self, const char *username, const char *password, const thttp_request_t *request)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_challenge_def_t</name>
      <anchorfile>thttp__challenge_8c.html</anchorfile>
      <anchor>a8de773d49fea958030eaf390c2eac874</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header_8c</filename>
    <includes id="thttp__header_8h" name="thttp_header.h" local="yes" imported="no">tinyhttp/headers/thttp_header.h</includes>
    <includes id="thttp__header___dummy_8h" name="thttp_header_Dummy.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Dummy.h</includes>
    <member kind="function">
      <type>const char *</type>
      <name>thttp_header_get_name</name>
      <anchorfile>thttp__header_8c.html</anchorfile>
      <anchor>aff5b23d3dee1ac20625a4d6adc2dabd8</anchor>
      <arglist>(thttp_header_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>thttp_header_get_nameex</name>
      <anchorfile>thttp__header_8c.html</anchorfile>
      <anchor>a65a286d7828397c6e1f335edf055b6f7</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>thttp_header_get_param_separator</name>
      <anchorfile>thttp__header_8c.html</anchorfile>
      <anchor>ab427239dde18b3463673e20585534e33</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_serialize</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga7abd684222ab6096765addca79071561</anchor>
      <arglist>(const thttp_header_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_header_tostring</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga2ba34e8e33da57b8af99350c85768099</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_header_value_tostring</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga770293aff5d59b867680f5c099eee323</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Authorization.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___authorization_8c</filename>
    <includes id="thttp__header___authorization_8h" name="thttp_header_Authorization.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Authorization.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="function">
      <type>thttp_header_Authorization_t *</type>
      <name>thttp_header_authorization_create</name>
      <anchorfile>thttp__header___authorization_8c.html</anchorfile>
      <anchor>a166164e6a4ed700ef370b5777aecde26</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_Authorization_tostring</name>
      <anchorfile>thttp__header___authorization_8c.html</anchorfile>
      <anchor>a7b352d0f3e061b2ba7fbcb8b859e6474</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Authorization_t *</type>
      <name>thttp_header_Authorization_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>gaf62fc4d2fedd30ee7dc84856859e8a4f</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Proxy_Authorization_t *</type>
      <name>thttp_header_Proxy_Authorization_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga2d423bf012c44446fbadc8f2e9e9c753</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_Authorization_def_t</name>
      <anchorfile>thttp__header___authorization_8c.html</anchorfile>
      <anchor>ae199e6ec32d09c9af2eb5885f96c6bb2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Content_Length.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___content___length_8c</filename>
    <includes id="thttp__header___content___length_8h" name="thttp_header_Content_Length.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Content_Length.h</includes>
    <member kind="function">
      <type>thttp_header_Content_Length_t *</type>
      <name>thttp_header_content_length_create</name>
      <anchorfile>thttp__header___content___length_8c.html</anchorfile>
      <anchor>a0c695121d4b61386a6ac1e9df931ba8f</anchor>
      <arglist>(uint32_t length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_Content_Length_tostring</name>
      <anchorfile>thttp__header___content___length_8c.html</anchorfile>
      <anchor>a78fbeaf931b0b78c46944584d40f361c</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Length_t *</type>
      <name>thttp_header_Content_Length_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga4c9a74cd97da944385e046590fa0c4c1</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_Content_Length_def_t</name>
      <anchorfile>thttp__header___content___length_8c.html</anchorfile>
      <anchor>a185d9b0e08ccec6ee26850e74e9d7c66</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Content_Type.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___content___type_8c</filename>
    <includes id="thttp__header___content___type_8h" name="thttp_header_Content_Type.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Content_Type.h</includes>
    <member kind="function">
      <type>thttp_header_Content_Type_t *</type>
      <name>thttp_header_content_type_create</name>
      <anchorfile>thttp__header___content___type_8c.html</anchorfile>
      <anchor>a805b4a1049b3760791cda03c19584c75</anchor>
      <arglist>(const char *type)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Type_t *</type>
      <name>thttp_header_content_type_create_null</name>
      <anchorfile>thttp__header___content___type_8c.html</anchorfile>
      <anchor>ae8b68771502f79c782af939bfda18b93</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_Content_Type_tostring</name>
      <anchorfile>thttp__header___content___type_8c.html</anchorfile>
      <anchor>a0b2beef56edbd1226b5a15411aeb43f4</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Type_t *</type>
      <name>thttp_header_Content_Type_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga88c3c565650876a99c75c7503333cced</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_Content_Type_def_t</name>
      <anchorfile>thttp__header___content___type_8c.html</anchorfile>
      <anchor>a911bb4b12f106ad2da6673169f6e37be</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Dummy.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___dummy_8c</filename>
    <includes id="thttp__header___dummy_8h" name="thttp_header_Dummy.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Dummy.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="function">
      <type>thttp_header_Dummy_t *</type>
      <name>thttp_header_dummy_create</name>
      <anchorfile>thttp__header___dummy_8c.html</anchorfile>
      <anchor>a260b69294cde8c14d02fcce86146de63</anchor>
      <arglist>(const char *name, const char *value)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Dummy_t *</type>
      <name>thttp_header_dummy_create_null</name>
      <anchorfile>thttp__header___dummy_8c.html</anchorfile>
      <anchor>a754efe688f5a1b19dbb715bce8468a9f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_Dummy_tostring</name>
      <anchorfile>thttp__header___dummy_8c.html</anchorfile>
      <anchor>af7f2542fcb0b5e94c55e24ad9a7983ec</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Dummy_t *</type>
      <name>thttp_header_Dummy_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>gaa51ce8cbaeba9ffaf24847f4ff6c6c63</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_Dummy_def_t</name>
      <anchorfile>thttp__header___dummy_8c.html</anchorfile>
      <anchor>a7a8a488e2f20e15dc52821d4f7900677</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_ETag.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___e_tag_8c</filename>
    <includes id="thttp__header___e_tag_8h" name="thttp_header_ETag.h" local="yes" imported="no">tinyhttp/headers/thttp_header_ETag.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_etag_create</name>
      <anchorfile>thttp__header___e_tag_8c.html</anchorfile>
      <anchor>a51972875d3d2702e9bb2d1cad9b45659</anchor>
      <arglist>(const char *value)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_etag_create_null</name>
      <anchorfile>thttp__header___e_tag_8c.html</anchorfile>
      <anchor>ad07c1d52806e9274dc962ec7db01af97</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_ETag_tostring</name>
      <anchorfile>thttp__header___e_tag_8c.html</anchorfile>
      <anchor>afb4b696e2d9abe37060e161791bb546a</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_ETag_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga8f80a83c38b4b3c23ef9adca526c2067</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_ETag_def_t</name>
      <anchorfile>thttp__header___e_tag_8c.html</anchorfile>
      <anchor>a6458161bdd34b5aa4035281a50addd8f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_Transfer_Encoding.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___transfer___encoding_8c</filename>
    <includes id="thttp__header___transfer___encoding_8h" name="thttp_header_Transfer_Encoding.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Transfer_Encoding.h</includes>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_transfer_encoding_create</name>
      <anchorfile>thttp__header___transfer___encoding_8c.html</anchorfile>
      <anchor>aa1db4ca84e99d39bbe1d5a7751c2dbb5</anchor>
      <arglist>(const char *encoding)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_transfer_encoding_create_null</name>
      <anchorfile>thttp__header___transfer___encoding_8c.html</anchorfile>
      <anchor>aebce8cb85bbc984054dc55535eff4ef0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_Transfer_Encoding_tostring</name>
      <anchorfile>thttp__header___transfer___encoding_8c.html</anchorfile>
      <anchor>a60865a3a13d8ebf0ac0c37c702ba5cb2</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_Transfer_Encoding_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga16eb00aaf3ca02295c0c7dcd1f7c808d</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_Transfer_Encoding_def_t</name>
      <anchorfile>thttp__header___transfer___encoding_8c.html</anchorfile>
      <anchor>adefb146cda0858b796412ae5af31bb86</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_header_WWW_Authenticate.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/headers/</path>
    <filename>thttp__header___w_w_w___authenticate_8c</filename>
    <includes id="thttp__header___w_w_w___authenticate_8h" name="thttp_header_WWW_Authenticate.h" local="yes" imported="no">tinyhttp/headers/thttp_header_WWW_Authenticate.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>prev_not_comma</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8c.html</anchorfile>
      <anchor>a0271fd9cc6bda8b3fde50745215bee42</anchor>
      <arglist>(p)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_WWW_Authenticate_t *</type>
      <name>thttp_header_www_authenticate_create</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8c.html</anchorfile>
      <anchor>abf3fc1eedfd316305b60c406f2b4a9a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_WWW_Authenticate_tostring</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8c.html</anchorfile>
      <anchor>a3191f98b29b0c55f0dbd8ee836da9b43</anchor>
      <arglist>(const thttp_header_t *header, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_WWW_Authenticate_t *</type>
      <name>thttp_header_WWW_Authenticate_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga6ea6cdaa0f153de0f53b25d02a99778c</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Proxy_Authenticate_t *</type>
      <name>thttp_header_Proxy_Authenticate_parse</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8c.html</anchorfile>
      <anchor>a617438484d983519ec1531e0f1912fce</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_header_WWW_Authenticate_def_t</name>
      <anchorfile>thttp__header___w_w_w___authenticate_8c.html</anchorfile>
      <anchor>a59a07c2bac40f845974d43ee9f3f5fe0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_parser_header.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/parsers/</path>
    <filename>thttp__parser__header_8c</filename>
    <includes id="thttp__parser__header_8h" name="thttp_parser_header.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_header.h</includes>
    <includes id="thttp__header___authorization_8h" name="thttp_header_Authorization.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Authorization.h</includes>
    <includes id="thttp__header___dummy_8h" name="thttp_header_Dummy.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Dummy.h</includes>
    <includes id="thttp__header___e_tag_8h" name="thttp_header_ETag.h" local="yes" imported="no">tinyhttp/headers/thttp_header_ETag.h</includes>
    <includes id="thttp__header___transfer___encoding_8h" name="thttp_header_Transfer_Encoding.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Transfer_Encoding.h</includes>
    <includes id="thttp__header___w_w_w___authenticate_8h" name="thttp_header_WWW_Authenticate.h" local="yes" imported="no">tinyhttp/headers/thttp_header_WWW_Authenticate.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ADD_HEADERS</name>
      <anchorfile>thttp__parser__header_8c.html</anchorfile>
      <anchor>a27be7662a4a3bc93b2472c1b7123f46c</anchor>
      <arglist>(headers)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADD_HEADER</name>
      <anchorfile>thttp__parser__header_8c.html</anchorfile>
      <anchor>a0137efa753e3b9c5c03d459ed14887f7</anchor>
      <arglist>(header)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_parse</name>
      <anchorfile>thttp__parser__header_8c.html</anchorfile>
      <anchor>ae52707f146ca8f1c20d2d170b6bd4e65</anchor>
      <arglist>(tsk_ragel_state_t *state, thttp_message_t *message)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_parser_message.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/parsers/</path>
    <filename>thttp__parser__message_8c</filename>
    <includes id="thttp__parser__message_8h" name="thttp_parser_message.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_message.h</includes>
    <includes id="thttp__parser__header_8h" name="thttp_parser_header.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_header.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_parse</name>
      <anchorfile>thttp__parser__message_8c.html</anchorfile>
      <anchor>aa58e2b6a672e557389465d5def68802e</anchor>
      <arglist>(tsk_ragel_state_t *state, thttp_message_t **result, tsk_bool_t extract_content)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_parser_url.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/parsers/</path>
    <filename>thttp__parser__url_8c</filename>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_parse</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga2204b5747836c64bea185be649ce1470</anchor>
      <arglist>(const char *urlstring, tsk_size_t length)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp_8c</filename>
    <includes id="thttp_8h" name="thttp.h" local="yes" imported="no">thttp.h</includes>
    <includes id="thttp__action_8h" name="thttp_action.h" local="yes" imported="no">tinyhttp/thttp_action.h</includes>
    <includes id="thttp__event_8h" name="thttp_event.h" local="yes" imported="no">tinyhttp/thttp_event.h</includes>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <includes id="thttp__parser__message_8h" name="thttp_parser_message.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_message.h</includes>
    <includes id="thttp__header___transfer___encoding_8h" name="thttp_header_Transfer_Encoding.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Transfer_Encoding.h</includes>
    <includes id="thttp__dialog_8h" name="thttp_dialog.h" local="yes" imported="no">tinyhttp/thttp_dialog.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MIN_STREAM_CHUNCK_SIZE</name>
      <anchorfile>thttp_8c.html</anchorfile>
      <anchor>a4fe6c3e978f2b8dd596315d8c714675f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>__thttp_stack_set</name>
      <anchorfile>thttp_8c.html</anchorfile>
      <anchor>a5094d9b95c2ce4945b1de1b5d6d2260e</anchor>
      <arglist>(thttp_stack_t *self, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>thttp_stack_handle_t *</type>
      <name>thttp_stack_create</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga23f21d510973ba5761fd25286df19eef</anchor>
      <arglist>(thttp_stack_callback_f callback,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_start</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga53cf350947bec4313d285a3b89df7f31</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_set</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga6f67d03e0f023335b97788002fb247a2</anchor>
      <arglist>(thttp_stack_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>const void *</type>
      <name>thttp_stack_get_userdata</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>gacf4a84954ca6c196f357cba4731e3ae3</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_stop</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga46ca1b404d575a8b1f6c1bcea9cf9484</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_alert</name>
      <anchorfile>thttp_8c.html</anchorfile>
      <anchor>ab532602d492266c03327a604daa7462e</anchor>
      <arglist>(const thttp_stack_t *self, const thttp_event_t *e)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_stack_def_t</name>
      <anchorfile>thttp_8c.html</anchorfile>
      <anchor>ab351f37e2977937fb0040e18630b06a4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_action.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp__action_8c</filename>
    <includes id="thttp__action_8h" name="thttp_action.h" local="yes" imported="no">tinyhttp/thttp_action.h</includes>
    <includes id="thttp_8h" name="thttp.h" local="yes" imported="no">thttp.h</includes>
    <member kind="function">
      <type>thttp_action_t *</type>
      <name>thttp_action_create</name>
      <anchorfile>thttp__action_8c.html</anchorfile>
      <anchor>a73543e652404a6e5f64ff18066468db8</anchor>
      <arglist>(thttp_action_type_t type, const char *urlstring, const char *method, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_action_perform</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga36b1aa78a24305f56973f370f356236d</anchor>
      <arglist>(thttp_session_handle_t *session, const char *urlstring, const char *method,...)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_action_def_t</name>
      <anchorfile>thttp__action_8c.html</anchorfile>
      <anchor>a3880c0dbf8e3b468c2813cf398e070ad</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_dialog.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp__dialog_8c</filename>
    <includes id="thttp__dialog_8h" name="thttp_dialog.h" local="yes" imported="no">tinyhttp/thttp_dialog.h</includes>
    <includes id="thttp_8h" name="thttp.h" local="yes" imported="no">thttp.h</includes>
    <includes id="thttp__action_8h" name="thttp_action.h" local="yes" imported="no">tinyhttp/thttp_action.h</includes>
    <includes id="thttp__session_8h" name="thttp_session.h" local="yes" imported="no">tinyhttp/thttp_session.h</includes>
    <includes id="thttp__url_8h" name="thttp_url.h" local="yes" imported="no">tinyhttp/thttp_url.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <includes id="thttp__header___dummy_8h" name="thttp_header_Dummy.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Dummy.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_STATE_MACHINE</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a81cfaffe67503aea261a80843624919e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_DESCRIPTION</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a7a8940a5d53df638ec52288b75989fc4</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_DIALOG_TRANSPORT_ERROR_CODE</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>aa32e20ee3bff57d5338833ef141907ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _fsm_action_e</type>
      <name>_fsm_action_t</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>afe20aca1bd90c447e89dfd4b30661acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _fsm_state_e</type>
      <name>_fsm_state_t</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ac06f5d03925a1287a4f89e0d57f347fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_fsm_action_e</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_request</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da2e7bfdefaa441cf97db0f8ccd0efbf43</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_close</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61dafa51b123b60fbdfe6b1c2265a912ad1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_message</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61daa5b7c1cbb6978522d70fcd9daa3c68ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_closed</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61dae0791cf072837e4754246ba7d8de9ece</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_action_error</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ad032147dfda9028e630a09a1c3bec61da8112fd037b6e1e00369c8412688c95ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_fsm_state_e</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_Started</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba76034bb262163ff2e14ebbdbe3b3b2ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_Transfering</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcba07dc0fcb6ff70d5b4480ab3c965e6e55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>_fsm_state_Terminated</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a92913a3e734277dfa8b96cdd97a9cbcbaeab8f5e66ccc9c9df9e4edd20400e037</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_send_request</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a508502e5b7d170d72fe0fd6572bfe4e7</anchor>
      <arglist>(thttp_dialog_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_update_timestamp</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a4022253c16e2be6003e1bc212814ffcd</anchor>
      <arglist>(thttp_dialog_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_OnTerminated</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>acc131be6c5a9a9b9f6580b2a0ca0918d</anchor>
      <arglist>(thttp_dialog_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_alert</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ab532602d492266c03327a604daa7462e</anchor>
      <arglist>(const thttp_stack_t *self, const thttp_event_t *e)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_Started_2_Transfering_X_request</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a4fec827477039cd133bb0ba55cd6e1b6</anchor>
      <arglist>(va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_Transfering_2_Transfering_X_401_407</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a00ab51fe52cf9ff43607376dd9b2f663</anchor>
      <arglist>(va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_Transfering_2_Transfering_X_1xx</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ac58361d510dad68eed62a8ccf8e3ba75</anchor>
      <arglist>(va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_Transfering_2_Terminated_X_message</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ab1728da2b2245a6768a00ce40e0861f7</anchor>
      <arglist>(va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_Any_2_Terminated_X_closed</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ae2b2c69ea17d9de8e454f914f94393bf</anchor>
      <arglist>(va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_Any_2_Terminated_X_Error</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a271bc2b8bacb15714584816fb587a8cb</anchor>
      <arglist>(va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>_fsm_cond_i_401_407</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a06b79f19a5f46a36b5b10c3f150dd32a</anchor>
      <arglist>(thttp_dialog_t *self, thttp_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>_fsm_cond_i_1xx</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a900dbaa94550c911cf068916d7949b52</anchor>
      <arglist>(thttp_dialog_t *self, thttp_message_t *message)</arglist>
    </member>
    <member kind="function">
      <type>thttp_dialog_t *</type>
      <name>thttp_dialog_create</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>ae36feb1433b9c3231537edb7cb386a97</anchor>
      <arglist>(struct thttp_session_s *session)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_dialog_fsm_act</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a75bbcbd775498c9c768e4d3536c7cc3b</anchor>
      <arglist>(thttp_dialog_t *self, tsk_fsm_action_id action_id, const thttp_message_t *message, const thttp_action_t *action)</arglist>
    </member>
    <member kind="function">
      <type>thttp_dialog_t *</type>
      <name>thttp_dialog_new</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a47c02f0e16ba68f049d5721c479b8de0</anchor>
      <arglist>(thttp_session_t *session)</arglist>
    </member>
    <member kind="function">
      <type>thttp_dialog_t *</type>
      <name>thttp_dialog_get_oldest</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a11a7f172eec35d75c54445562db2733e</anchor>
      <arglist>(thttp_dialogs_L_t *dialogs)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_dialog_def_t</name>
      <anchorfile>thttp__dialog_8c.html</anchorfile>
      <anchor>a9829fc6c800994cd8a137eb90597b47a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_event.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp__event_8c</filename>
    <includes id="thttp__event_8h" name="thttp_event.h" local="yes" imported="no">tinyhttp/thttp_event.h</includes>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <member kind="function">
      <type>thttp_event_t *</type>
      <name>thttp_event_create</name>
      <anchorfile>thttp__event_8c.html</anchorfile>
      <anchor>a957893a2f552054a69a7e63ab96ddb04</anchor>
      <arglist>(thttp_event_type_t type, const thttp_session_handle_t *session, const char *description, const thttp_message_t *message)</arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>thttp_event_def_t</name>
      <anchorfile>thttp__event_8c.html</anchorfile>
      <anchor>a80813987ea170771d264808d86bb3c03</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_message.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp__message_8c</filename>
    <includes id="thttp__message_8h" name="thttp_message.h" local="yes" imported="no">tinyhttp/thttp_message.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ADD_HEADER</name>
      <anchorfile>thttp__message_8c.html</anchorfile>
      <anchor>a04933dbce47a20696dc3fbdaeaacb126</anchor>
      <arglist>(type, field)</arglist>
    </member>
    <member kind="function">
      <type>thttp_message_t *</type>
      <name>thttp_message_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3fad120325a24fcdab2c8a1aac655b84</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>thttp_request_t *</type>
      <name>thttp_request_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaa65be3fa6b585652c98594734b64efa8</anchor>
      <arglist>(const char *method, const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>thttp_response_t *</type>
      <name>thttp_response_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga07f948cc1371f453a96c01a57dc0adbb</anchor>
      <arglist>(const thttp_request_t *request, short status_code, const char *reason_phrase)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_add_header</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga0067f283164eb98c6850328b39d5ff26</anchor>
      <arglist>(thttp_message_t *self, const thttp_header_t *hdr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_add_headers</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga9d772552b9a892b718f0fd8aff3b86b2</anchor>
      <arglist>(thttp_message_t *self, const thttp_headers_L_t *headers)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_add_content</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga0fbe5b273002fc7b0bfaa4ce306508af</anchor>
      <arglist>(thttp_message_t *self, const char *content_type, const void *content, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_append_content</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gada8275938d66a7f7c77a18b40dae685e</anchor>
      <arglist>(thttp_message_t *self, const void *content, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>const thttp_header_t *</type>
      <name>thttp_message_get_headerAt</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gacc210783cfa8c4f937430dd1f2508611</anchor>
      <arglist>(const thttp_message_t *self, thttp_header_type_t type, tsk_size_t index)</arglist>
    </member>
    <member kind="function">
      <type>const thttp_header_t *</type>
      <name>thttp_message_get_header</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga423b5655de3f4ddff2a05a259ad7557f</anchor>
      <arglist>(const thttp_message_t *self, thttp_header_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>const thttp_header_t *</type>
      <name>thttp_message_get_headerByName</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gadbe0434124bf2f50b12fdba10a44fb2b</anchor>
      <arglist>(const thttp_message_t *self, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_serialize</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3f87c7ab7eb5bb730f5b153fcc1dc1c0</anchor>
      <arglist>(const thttp_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_message_tostring</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga58993e3fc64c1116997758f6c4e468d1</anchor>
      <arglist>(const thttp_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>thttp_request_t *</type>
      <name>thttp_request_new</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga5dcd151de1e0ad4aec5e5212a506b889</anchor>
      <arglist>(const char *method, const thttp_url_t *request_url)</arglist>
    </member>
    <member kind="function">
      <type>thttp_response_t *</type>
      <name>thttp_response_new</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac011c57f8bd07ed2662bbc71b6e44a62</anchor>
      <arglist>(short status_code, const char *reason_phrase, const thttp_request_t *request)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_message_def_t</name>
      <anchorfile>thttp__message_8c.html</anchorfile>
      <anchor>a6d03014b06c22907fa7251aa74a04656</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_session.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp__session_8c</filename>
    <includes id="thttp__session_8h" name="thttp_session.h" local="yes" imported="no">tinyhttp/thttp_session.h</includes>
    <includes id="thttp_8h" name="thttp.h" local="yes" imported="no">thttp.h</includes>
    <includes id="thttp__action_8h" name="thttp_action.h" local="yes" imported="no">tinyhttp/thttp_action.h</includes>
    <includes id="thttp__header___dummy_8h" name="thttp_header_Dummy.h" local="yes" imported="no">tinyhttp/headers/thttp_header_Dummy.h</includes>
    <includes id="thttp__header___w_w_w___authenticate_8h" name="thttp_header_WWW_Authenticate.h" local="yes" imported="no">tinyhttp/headers/thttp_header_WWW_Authenticate.h</includes>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_signal</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>aca34bca93da39b305f8a1afc0bc91904</anchor>
      <arglist>(thttp_session_t *self, thttp_action_type_t atype)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>__thttp_session_set</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>a0b34d6e5d87a8694c3f251cabaa7133f</anchor>
      <arglist>(thttp_session_t *self, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>thttp_session_handle_t *</type>
      <name>thttp_session_create</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga4ea39662c02afa5c56439c4f087511e9</anchor>
      <arglist>(const thttp_stack_handle_t *stack,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_set</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gad32c0f75635447af16a761bdab0b89bc</anchor>
      <arglist>(thttp_session_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>thttp_session_id_t</type>
      <name>thttp_session_get_id</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga183dab1dec59d010e2efa1be4809e3c0</anchor>
      <arglist>(const thttp_session_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const void *</type>
      <name>thttp_session_get_userdata</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga0f457e2b4f3d1cc43074230a174cca81</anchor>
      <arglist>(const thttp_session_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_closefd</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>ab301eb511cb7e193cdae2d25dd007ca5</anchor>
      <arglist>(thttp_session_handle_t *_self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_update_challenges</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>a91341cd284ebed49ece3d7e0c1ffac75</anchor>
      <arglist>(thttp_session_t *self, const thttp_response_t *response, tsk_bool_t answered)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_signal_closed</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>a00500a6442e130d6f1a0ac29084cfa6e</anchor>
      <arglist>(thttp_session_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_signal_error</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>a8d99c10a76adba8db5ae8a3f5a04ef0c</anchor>
      <arglist>(thttp_session_t *self)</arglist>
    </member>
    <member kind="function">
      <type>thttp_session_t *</type>
      <name>thttp_session_get_by_fd</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>a1e988df3ceff34ac789db8845416695a</anchor>
      <arglist>(thttp_sessions_L_t *sessions, tnet_fd_t fd)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_session_def_t</name>
      <anchorfile>thttp__session_8c.html</anchorfile>
      <anchor>a1bb8f8df043cc568314c18977f4d8edc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thttp_url.c</name>
    <path>C:/Projects/Doubango/tinyHTTP/src/</path>
    <filename>thttp__url_8c</filename>
    <includes id="thttp__url_8h" name="thttp_url.h" local="yes" imported="no">tinyhttp/thttp_url.h</includes>
    <includes id="thttp__parser__url_8h" name="thttp_parser_url.h" local="yes" imported="no">tinyhttp/parsers/thttp_parser_url.h</includes>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_create</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga3326c658c6aa66b70472a129190a8f27</anchor>
      <arglist>(thttp_url_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_url_serialize</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga9b7274dffec6932a2851513b865226b8</anchor>
      <arglist>(const thttp_url_t *url, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_url_tostring</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>gadc56e46c5df2f68f9a359d65f566904e</anchor>
      <arglist>(const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_clone</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga164927101cd3c02cd641e1bea51b836a</anchor>
      <arglist>(const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>thttp_url_isvalid</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>gabec149e64b6b9adffc609190be8c9c33</anchor>
      <arglist>(const char *urlstring)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>thttp_url_def_t</name>
      <anchorfile>thttp__url_8c.html</anchorfile>
      <anchor>a98e11c7a8aa8410e4a955c4376d78cd4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_auth_group</name>
    <title>HTTP basic/digest authentication (RFC 2617)</title>
    <filename>group__thttp__auth__group.html</filename>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>thttp_auth_basic_response</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>gaf720869992f752dc684f3dc63f228be6</anchor>
      <arglist>(const char *userid, const char *password, char **response)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_HA1</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga512b7e39c4f78e196f225b6e75fc7551</anchor>
      <arglist>(const char *username, const char *realm, const char *password, tsk_md5string_t *ha1)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_HA1sess</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga82bbc1b3b5f7945349db1bf918c2a7cc</anchor>
      <arglist>(const char *username, const char *realm, const char *password, const char *nonce, const char *cnonce, tsk_md5string_t *ha1sess)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_HA2</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>ga35a96816fa3c4c48468187abc6ed36c3</anchor>
      <arglist>(const char *method, const char *url, const tsk_buffer_t *entity_body, const char *qop, tsk_md5string_t *ha2)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_auth_digest_response</name>
      <anchorfile>group__thttp__auth__group.html</anchorfile>
      <anchor>gadfec81e484a75707d0319cbc9d1bb3ec</anchor>
      <arglist>(const tsk_md5string_t *ha1, const char *nonce, const nonce_count_t noncecount, const char *cnonce, const char *qop, const tsk_md5string_t *ha2, tsk_md5string_t *response)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_header_group</name>
    <title>HTTP Headers</title>
    <filename>group__thttp__header__group.html</filename>
    <member kind="function">
      <type>int</type>
      <name>thttp_header_serialize</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga7abd684222ab6096765addca79071561</anchor>
      <arglist>(const thttp_header_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_header_tostring</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga2ba34e8e33da57b8af99350c85768099</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_header_value_tostring</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga770293aff5d59b867680f5c099eee323</anchor>
      <arglist>(const thttp_header_t *self)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Authorization_t *</type>
      <name>thttp_header_Authorization_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>gaf62fc4d2fedd30ee7dc84856859e8a4f</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Proxy_Authorization_t *</type>
      <name>thttp_header_Proxy_Authorization_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga2d423bf012c44446fbadc8f2e9e9c753</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Length_t *</type>
      <name>thttp_header_Content_Length_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga4c9a74cd97da944385e046590fa0c4c1</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Content_Type_t *</type>
      <name>thttp_header_Content_Type_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga88c3c565650876a99c75c7503333cced</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Dummy_t *</type>
      <name>thttp_header_Dummy_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>gaa51ce8cbaeba9ffaf24847f4ff6c6c63</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_ETag_t *</type>
      <name>thttp_header_ETag_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga8f80a83c38b4b3c23ef9adca526c2067</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_Transfer_Encoding_t *</type>
      <name>thttp_header_Transfer_Encoding_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga16eb00aaf3ca02295c0c7dcd1f7c808d</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>thttp_header_WWW_Authenticate_t *</type>
      <name>thttp_header_WWW_Authenticate_parse</name>
      <anchorfile>group__thttp__header__group.html</anchorfile>
      <anchor>ga6ea6cdaa0f153de0f53b25d02a99778c</anchor>
      <arglist>(const char *data, tsk_size_t size)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_stack_group</name>
    <title>HTTP/HTTPS stack</title>
    <filename>group__thttp__stack__group.html</filename>
    <member kind="function">
      <type>thttp_stack_handle_t *</type>
      <name>thttp_stack_create</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga23f21d510973ba5761fd25286df19eef</anchor>
      <arglist>(thttp_stack_callback_f callback,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_start</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga53cf350947bec4313d285a3b89df7f31</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_set</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga6f67d03e0f023335b97788002fb247a2</anchor>
      <arglist>(thttp_stack_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>const void *</type>
      <name>thttp_stack_get_userdata</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>gacf4a84954ca6c196f357cba4731e3ae3</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_stack_stop</name>
      <anchorfile>group__thttp__stack__group.html</anchorfile>
      <anchor>ga46ca1b404d575a8b1f6c1bcea9cf9484</anchor>
      <arglist>(thttp_stack_handle_t *self)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_action_group</name>
    <title>Sending Requests</title>
    <filename>group__thttp__action__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_OPTION</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gafddb2b8c342d440cb0be8ae5f55714c0</anchor>
      <arglist>(ID_ENUM, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_HEADER</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga276d6118d0038e4bdab2a02027c53c4e</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_PAYLOAD</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gab54688c397a0df38f72e19d3e761513c</anchor>
      <arglist>(PAY_PTR, PAY_SIZE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_ACTION_SET_NULL</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga168e5e2a98540e218c15868f86cf494f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_CONNECT</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gad1a804068d4f970d35b62506405bb1b9</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_DELETE</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gae4dc4de16832e1756213d8f2a60c286d</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_GET</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga25a8af4242b64342c590ef5ee8db56b4</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_HEAD</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga659e821e8419c3046b89480cdd3cdaec</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_OPTIONS</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga6b5adbcce3f8d0bd215ff75f1c9f59cf</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_PATCH</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga95836da9e16ff5bb6ad7e7cea186d760</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_POST</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga34c6edcf5b07069d79ced7d2a99b3a2f</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_PUT</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>gaa0ebc1d07393d144d54319044d33911a</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>thttp_action_TRACE</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga99cf9ae9b1e646d8d54dc07f841ae52a</anchor>
      <arglist>(session, urlstring,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_action_perform</name>
      <anchorfile>group__thttp__action__group.html</anchorfile>
      <anchor>ga36b1aa78a24305f56973f370f356236d</anchor>
      <arglist>(thttp_session_handle_t *session, const char *urlstring, const char *method,...)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_message_group</name>
    <title>HTTP Message</title>
    <filename>group__thttp__message__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_10</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga6fccc5ffca4ebf347df8399b2e963209</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_11</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga8155bb2f79c1fe2df0e2b2321025d192</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_20</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga796b8afa7aea730922da497f01fe1871</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_VERSION_DEFAULT</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gacf15d8ae6e023b950943a9f6b7fd8c9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_IS_REQUEST</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaa9a2544392c121a7ff4e8ae169e8484f</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_IS_RESPONSE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga52f49cde166aaca9f848361cb596f259</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga90c3419edd3fe5e9a628b80f5f35f2a1</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_AS_RESPONSE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac6f99e8cdc194ef64689b46cb1a35742</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_AS_REQUEST</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac831236cab706205d058e4428904b113</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_CODE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaca2eee1f3f53fb4ca5f80250a2cb92df</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_RESPONSE_PHRASE</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga651d0e661c88033b07a8a29cce10f1be</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_REQUEST_METHOD</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gafee4f7e440362a26739ee31806556213</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_REQUEST_URL</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3e3bdb1f4760ed4bda3ddeb780c0f653</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_CONTENT_LENGTH</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gab9825bef1b6278ceb242885043b2c177</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_CONTENT</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga57bcc0247fdfdc73b73a1655780a7129</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_MESSAGE_HAS_CONTENT</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga365e5ba009877617b1cdf3d183bb1fd9</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>thttp_message_t *</type>
      <name>thttp_message_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3fad120325a24fcdab2c8a1aac655b84</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>thttp_request_t *</type>
      <name>thttp_request_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gaa65be3fa6b585652c98594734b64efa8</anchor>
      <arglist>(const char *method, const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>thttp_response_t *</type>
      <name>thttp_response_create</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga07f948cc1371f453a96c01a57dc0adbb</anchor>
      <arglist>(const thttp_request_t *request, short status_code, const char *reason_phrase)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_add_header</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga0067f283164eb98c6850328b39d5ff26</anchor>
      <arglist>(thttp_message_t *self, const thttp_header_t *hdr)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_add_headers</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga9d772552b9a892b718f0fd8aff3b86b2</anchor>
      <arglist>(thttp_message_t *self, const thttp_headers_L_t *headers)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_add_content</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga0fbe5b273002fc7b0bfaa4ce306508af</anchor>
      <arglist>(thttp_message_t *self, const char *content_type, const void *content, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_append_content</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gada8275938d66a7f7c77a18b40dae685e</anchor>
      <arglist>(thttp_message_t *self, const void *content, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>const thttp_header_t *</type>
      <name>thttp_message_get_headerAt</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gacc210783cfa8c4f937430dd1f2508611</anchor>
      <arglist>(const thttp_message_t *self, thttp_header_type_t type, tsk_size_t index)</arglist>
    </member>
    <member kind="function">
      <type>const thttp_header_t *</type>
      <name>thttp_message_get_header</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga423b5655de3f4ddff2a05a259ad7557f</anchor>
      <arglist>(const thttp_message_t *self, thttp_header_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>const thttp_header_t *</type>
      <name>thttp_message_get_headerByName</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gadbe0434124bf2f50b12fdba10a44fb2b</anchor>
      <arglist>(const thttp_message_t *self, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_message_serialize</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga3f87c7ab7eb5bb730f5b153fcc1dc1c0</anchor>
      <arglist>(const thttp_message_t *self, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_message_tostring</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga58993e3fc64c1116997758f6c4e468d1</anchor>
      <arglist>(const thttp_message_t *self)</arglist>
    </member>
    <member kind="function">
      <type>thttp_request_t *</type>
      <name>thttp_request_new</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>ga5dcd151de1e0ad4aec5e5212a506b889</anchor>
      <arglist>(const char *method, const thttp_url_t *request_url)</arglist>
    </member>
    <member kind="function">
      <type>thttp_response_t *</type>
      <name>thttp_response_new</name>
      <anchorfile>group__thttp__message__group.html</anchorfile>
      <anchor>gac011c57f8bd07ed2662bbc71b6e44a62</anchor>
      <arglist>(short status_code, const char *reason_phrase, const thttp_request_t *request)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_session_group</name>
    <title>HTTP Session</title>
    <filename>group__thttp__session__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_OPTION</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga3b48b30caffdca4b22831e6beb10a196</anchor>
      <arglist>(ID_ENUM, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_CRED</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gaeeccefade84db508310850d9d0f6d368</anchor>
      <arglist>(USERNAME_STR, PASSWORD_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_HEADER</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga5ed06de61a21d4c422e4ec5d56ff121d</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_UNSET_HEADER</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga4c820f1027fb3479cfb4bf5d7dbf56fb</anchor>
      <arglist>(NAME_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_USERDATA</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga6252a61755fc2f58722844795ee98938</anchor>
      <arglist>(USERDATA_PTR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THTTP_SESSION_SET_NULL</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gad00fd2b6fdfc9f0fe5001aee6fbfdff7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>thttp_session_handle_t *</type>
      <name>thttp_session_create</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga4ea39662c02afa5c56439c4f087511e9</anchor>
      <arglist>(const thttp_stack_handle_t *stack,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_session_set</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>gad32c0f75635447af16a761bdab0b89bc</anchor>
      <arglist>(thttp_session_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>thttp_session_id_t</type>
      <name>thttp_session_get_id</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga183dab1dec59d010e2efa1be4809e3c0</anchor>
      <arglist>(const thttp_session_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>const void *</type>
      <name>thttp_session_get_userdata</name>
      <anchorfile>group__thttp__session__group.html</anchorfile>
      <anchor>ga0f457e2b4f3d1cc43074230a174cca81</anchor>
      <arglist>(const thttp_session_handle_t *self)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>thttp_url_group</name>
    <title>HTTP/HTTPS URL</title>
    <filename>group__thttp__url__group.html</filename>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_parse</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga2204b5747836c64bea185be649ce1470</anchor>
      <arglist>(const char *urlstring, tsk_size_t length)</arglist>
    </member>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_create</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga3326c658c6aa66b70472a129190a8f27</anchor>
      <arglist>(thttp_url_type_t type)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>thttp_url_serialize</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga9b7274dffec6932a2851513b865226b8</anchor>
      <arglist>(const thttp_url_t *url, tsk_buffer_t *output)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>thttp_url_tostring</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>gadc56e46c5df2f68f9a359d65f566904e</anchor>
      <arglist>(const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>thttp_url_t *</type>
      <name>thttp_url_clone</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>ga164927101cd3c02cd641e1bea51b836a</anchor>
      <arglist>(const thttp_url_t *url)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>thttp_url_isvalid</name>
      <anchorfile>group__thttp__url__group.html</anchorfile>
      <anchor>gabec149e64b6b9adffc609190be8c9c33</anchor>
      <arglist>(const char *urlstring)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_action_s</name>
    <filename>structthttp__action__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_action_type_t</type>
      <name>type</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>a9c8cae4b4529140c71b1b2afb27817ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>url</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>aec657bef26e5ff27b9442563a591d9ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>method</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>a8b44a03925325ceaf2de7b079b1541a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_options_L_t *</type>
      <name>options</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>af5b6cc93a0b124046788312d128c9754</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_params_L_t *</type>
      <name>headers</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>a3dde1aa67335fc69f6d10612ec3652e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>payload</name>
      <anchorfile>structthttp__action__s.html</anchorfile>
      <anchor>ac049a2d3c61157a2bf049e5fcbd927af</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_challenge_s</name>
    <filename>structthttp__challenge__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>isproxy</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>a5d83b60ca32847b481dfe71ab729913f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>scheme</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>ac386fe0902e2c7802ee782d8aaca75e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>realm</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>a78e609bc32bd5a9820209fe62b39963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nonce</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>a814af5af8d9ea73885c8216c0e547e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>opaque</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>adb63a38cb2e704dc0d28907cc4f956f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>algorithm</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>a76c514e8785ea8be81ca07a2a8438eea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>qop</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>a842bcfd3b1ed80eae2773a121e6e29fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_md5string_t</type>
      <name>cnonce</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>aa4216ef6fabd48adb79b7057ac02ed12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>nc</name>
      <anchorfile>structthttp__challenge__s.html</anchorfile>
      <anchor>aef55cf6d0fb0daedf7050d8f3af6cbf0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_dialog_s</name>
    <filename>structthttp__dialog__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_dialog_id_t</type>
      <name>id</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>a176867df914b58b1c9386bb39cec2d9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timestamp</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>a465bef81f6478756e5443025b1f2ddfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_fsm_t *</type>
      <name>fsm</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>a0cc2337becbdef2e7ea539f61019fdbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>buf</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>a5a7a25cfb97b3a5f42c044bedbeb6f77</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_session_s *</type>
      <name>session</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>ac63b8d9ecd3c8f8285253ac68118143b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_action_s *</type>
      <name>action</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>a08a6ff5782503236f0997fba1f5b2114</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>answered</name>
      <anchorfile>structthttp__dialog__s.html</anchorfile>
      <anchor>aa64370d30fb5dc136542677833fe57f0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_event_s</name>
    <filename>structthttp__event__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__event__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_event_type_t</type>
      <name>type</name>
      <anchorfile>structthttp__event__s.html</anchorfile>
      <anchor>add778238f719d384de831d0fc439c2b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const thttp_session_handle_t *</type>
      <name>session</name>
      <anchorfile>structthttp__event__s.html</anchorfile>
      <anchor>a3b381f4a3c8fca00afee8d8e0a6438ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>description</name>
      <anchorfile>structthttp__event__s.html</anchorfile>
      <anchor>a8444d6e0dfe2bbab0b5e7b24308f1559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_message_s *</type>
      <name>message</name>
      <anchorfile>structthttp__event__s.html</anchorfile>
      <anchor>aef824d18448deffcebe4655882856da5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_Authorization_s</name>
    <filename>structthttp__header___authorization__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>scheme</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>ac386fe0902e2c7802ee782d8aaca75e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>username</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a9b20c006bd90a09e1465fb668700e81d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>realm</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a78e609bc32bd5a9820209fe62b39963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nonce</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a814af5af8d9ea73885c8216c0e547e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>uri</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>af98b83cec5342c54027dd92f7357d6e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>response</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a11373f02c42587a1853ad6c8be5fe10e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>algorithm</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a76c514e8785ea8be81ca07a2a8438eea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>cnonce</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a3c0cf27b692a0e4435f63d1a208dddd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>opaque</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>adb63a38cb2e704dc0d28907cc4f956f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>qop</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>aa0f49f0469dc77817dad52c9f98b1892</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nc</name>
      <anchorfile>structthttp__header___authorization__s.html</anchorfile>
      <anchor>a274cafeb5e5151166584be1d8c8e80e3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_Content_Length_s</name>
    <filename>structthttp__header___content___length__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___content___length__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>length</name>
      <anchorfile>structthttp__header___content___length__s.html</anchorfile>
      <anchor>aebb70c2aab3407a9f05334c47131a43b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_Content_Type_s</name>
    <filename>structthttp__header___content___type__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___content___type__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>type</name>
      <anchorfile>structthttp__header___content___type__s.html</anchorfile>
      <anchor>a23506fc4821ab6d9671f3e6222591a96</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_Dummy_s</name>
    <filename>structthttp__header___dummy__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___dummy__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>name</name>
      <anchorfile>structthttp__header___dummy__s.html</anchorfile>
      <anchor>a5ac083a645d964373f022d03df4849c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>structthttp__header___dummy__s.html</anchorfile>
      <anchor>a4e9aec275e566b978a3ccb4e043d8c61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_ETag_s</name>
    <filename>structthttp__header___e_tag__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___e_tag__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>value</name>
      <anchorfile>structthttp__header___e_tag__s.html</anchorfile>
      <anchor>a4e9aec275e566b978a3ccb4e043d8c61</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>isWeak</name>
      <anchorfile>structthttp__header___e_tag__s.html</anchorfile>
      <anchor>a3a38986ca9f298cc1abb518762e5b4bd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_s</name>
    <filename>structthttp__header__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__header__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_header_type_t</type>
      <name>type</name>
      <anchorfile>structthttp__header__s.html</anchorfile>
      <anchor>a223b05b90b639020d5f35f1974099327</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_header_value_tostring_f</type>
      <name>tostring</name>
      <anchorfile>structthttp__header__s.html</anchorfile>
      <anchor>aad81e5d2c11ec9fef889b7eb8542c52b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_params_L_t *</type>
      <name>params</name>
      <anchorfile>structthttp__header__s.html</anchorfile>
      <anchor>a224e951a121249c4cd0d2356d3cce567</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_Transfer_Encoding_s</name>
    <filename>structthttp__header___transfer___encoding__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___transfer___encoding__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>encoding</name>
      <anchorfile>structthttp__header___transfer___encoding__s.html</anchorfile>
      <anchor>abfd0452ce0a5ba319ea4d624dc209250</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_header_WWW_Authenticate_s</name>
    <filename>structthttp__header___w_w_w___authenticate__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>THTTP_DECLARE_HEADER</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>a54cf1e6c501a1773be1a8cffaf0aa865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>scheme</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>ac386fe0902e2c7802ee782d8aaca75e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>realm</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>a78e609bc32bd5a9820209fe62b39963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>domain</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>ac7098a8cde5110cd169c9958dca2d4d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>nonce</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>a814af5af8d9ea73885c8216c0e547e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>opaque</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>adb63a38cb2e704dc0d28907cc4f956f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>stale</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>aa2a796ee3134061f458712d1066d938c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>algorithm</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>a76c514e8785ea8be81ca07a2a8438eea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>qop</name>
      <anchorfile>structthttp__header___w_w_w___authenticate__s.html</anchorfile>
      <anchor>aa0f49f0469dc77817dad52c9f98b1892</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_message_s</name>
    <filename>structthttp__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>http_version</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>aa91778dc72b55de92098694627097b76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_message_type_t</type>
      <name>type</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>ad23686d7e996aee3038028014f0667c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union thttp_message_s::@1</type>
      <name>line</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>a0cc007d506cba250d7b1ff9265a3df15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_message_s::@1::@2</type>
      <name>request</name>
      <anchorfile>unionthttp__message__s_1_1@1.html</anchorfile>
      <anchor>a4809242002ade00dae60a933658bb6ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>method</name>
      <anchorfile>structthttp__message__s_1_1@1_1_1@2.html</anchorfile>
      <anchor>aca2544ecce27ac7a7e973d466c3a8fd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_url_t *</type>
      <name>url</name>
      <anchorfile>structthttp__message__s_1_1@1_1_1@2.html</anchorfile>
      <anchor>af654eb7c68f01c8b20b7212663995f7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_message_s::@1::@3</type>
      <name>response</name>
      <anchorfile>unionthttp__message__s_1_1@1.html</anchorfile>
      <anchor>a6f0a95c62e035f77fd6594a32c7656bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>short</type>
      <name>status_code</name>
      <anchorfile>structthttp__message__s_1_1@1_1_1@3.html</anchorfile>
      <anchor>a88f8987abe9732d27f636e6d35f5637e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>reason_phrase</name>
      <anchorfile>structthttp__message__s_1_1@1_1_1@3.html</anchorfile>
      <anchor>a18bf51f0b416cc27103a7a47db3442de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_header_Content_Type_t *</type>
      <name>Content_Type</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>a5cb03e4c98a8e65d00f2972198cefbc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_header_Content_Length_t *</type>
      <name>Content_Length</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>af5f22bec3c5f03a92fdff64f0fe8f0e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_buffer_t *</type>
      <name>Content</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>addeaa2a4d824a2357757fc6871bc1089</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_headers_L_t *</type>
      <name>headers</name>
      <anchorfile>structthttp__message__s.html</anchorfile>
      <anchor>a08ec919af0234be02136ae14c56f1f19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_session_s</name>
    <filename>structthttp__session__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_session_id_t</type>
      <name>id</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a7f20b720ff6bde879723258ada96eddd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_stack_s *</type>
      <name>stack</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a7004efb524ffe11cdbd0e9ac3360abb6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>userdata</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a2aa76a7bfd06a16154eccdd30d6f6393</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_options_L_t *</type>
      <name>options</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>af5b6cc93a0b124046788312d128c9754</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_params_L_t *</type>
      <name>headers</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a3dde1aa67335fc69f6d10612ec3652e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_fd_t</type>
      <name>fd</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a89776c821e7cb62a4acac46798a84137</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_challenges_L_t *</type>
      <name>challenges</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>aa1cb22349d59d6e9afaa1afb15716ef0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_dialogs_L_t *</type>
      <name>dialogs</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a2d591ef8f0d5990d7bfa377c48c1a0bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_session_s::@4</type>
      <name>cred</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>ae617255706678a02d7aaf7198c3c1787</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>usename</name>
      <anchorfile>structthttp__session__s_1_1@4.html</anchorfile>
      <anchor>ac5a5d5f31fad8ad1a4a7d1fa2186c598</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>password</name>
      <anchorfile>structthttp__session__s_1_1@4.html</anchorfile>
      <anchor>a59460a3ff2c12443d1022e5cc0fba85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structthttp__session__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_stack_s</name>
    <filename>structthttp__stack__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>started</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a3e87319b1ebccbd0529514c298ec4fe2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_stack_callback_f</type>
      <name>callback</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>ab156f92e27ec8892a3e4fb8e8bebeda2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>local_ip</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a39812ff38e55c55387e819985e9f0d20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>local_port</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a009e2d58737d2223ce009dc0631e65dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tnet_transport_t *</type>
      <name>transport</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a4fe87d83913e898af8fe8e673f40b092</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct thttp_stack_s::@0</type>
      <name>tls</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a5065093a1d213d93d0ba91d114478b91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ca</name>
      <anchorfile>structthttp__stack__s_1_1@0.html</anchorfile>
      <anchor>aed75f3bb30e712d0a8ca4e18ac0d1582</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>pbk</name>
      <anchorfile>structthttp__stack__s_1_1@0.html</anchorfile>
      <anchor>a2bc102c261f5f1170825a6d7bfd11783</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>pvk</name>
      <anchorfile>structthttp__stack__s_1_1@0.html</anchorfile>
      <anchor>a7b558b120c654899dda385dd65db4c57</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_sessions_L_t *</type>
      <name>sessions</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a17cea6f5781e9952e1aab5f69e9cd00f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>userdata</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a2aa76a7bfd06a16154eccdd30d6f6393</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structthttp__stack__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_url_s</name>
    <filename>structthttp__url__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_url_type_t</type>
      <name>type</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>ac7445e1a58ebd7a61b6f9568df2a83e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>scheme</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>ac386fe0902e2c7802ee782d8aaca75e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>host</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>a1c2046dcb30a629d6d9f45ff8f403f12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>hpath</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>a5cb6d370959d3bf66de95a2ed31e5ad9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>search</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>aad4b53dc2fef51a522ec724c45532afb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_host_type_t</type>
      <name>host_type</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>a90c770b993bc162c6d29b1e07e518846</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>port</name>
      <anchorfile>structthttp__url__s.html</anchorfile>
      <anchor>a8e0798404bf2cf5dabb84c5ba9a4f236</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>thttp_url_t</name>
    <filename>structthttp__url__t.html</filename>
  </compound>
</tagfile>
