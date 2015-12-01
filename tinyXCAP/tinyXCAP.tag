<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>tinyXCAP API Overview</title>
    <filename>index</filename>
  </compound>
  <compound kind="file">
    <name>tinyxcap.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/</path>
    <filename>tinyxcap_8h</filename>
    <includes id="txcap_8h" name="txcap.h" local="yes" imported="no">txcap.h</includes>
    <includes id="txcap__selector_8h" name="txcap_selector.h" local="yes" imported="no">tinyXCAP/txcap_selector.h</includes>
    <includes id="txcap__document_8h" name="txcap_document.h" local="yes" imported="no">tinyXCAP/txcap_document.h</includes>
    <includes id="txcap__node_8h" name="txcap_node.h" local="yes" imported="no">tinyXCAP/txcap_node.h</includes>
    <includes id="txcap__action_8h" name="txcap_action.h" local="yes" imported="no">tinyXCAP/txcap_action.h</includes>
  </compound>
  <compound kind="file">
    <name>txcap_action.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/tinyXCAP/</path>
    <filename>txcap__action_8h</filename>
    <includes id="tinyxcap__config_8h" name="tinyxcap_config.h" local="yes" imported="no">tinyXCAP_config.h</includes>
    <includes id="txcap_8h" name="txcap.h" local="yes" imported="no">txcap.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_OPTION</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga7913c72583d400da51dea9f46bed553b</anchor>
      <arglist>(ID_INT, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_HEADER</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>gaf422679f169f916bdf2c786e4171622e</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_PAYLOAD</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga566653198a295d9d9854c35974c91e1a</anchor>
      <arglist>(PAY_PTR, PAY_SIZE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_SELECTOR</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga599c6c0389addf0e0d9a14bdbcef017e</anchor>
      <arglist>(AUID_STR,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_REQUEST_URI</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga17133536f59ded5dadd169d1673dd689</anchor>
      <arglist>(URI_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_NULL</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga0f062a39307c1230628d37b0f7476346</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_create_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga3d7b7194078364d1c43c9fa5f09ca40e</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_create_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga36013b774b75655eeaea1e859e33b502</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_create_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga0cecb0adbc797545bc6cc28e9d4ce5a3</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_replace_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga375d103ff56afadc8032b1f6b38ab249</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_replace_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga7eb8bddb5492761b82380ab0d1233c9a</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_replace_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga4d7f98bd61b8d241ba2b9c83dc009092</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_fetch_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga0966cf2d592efecd10a8bd2d2b023c58</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_fetch_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga6a4b9b3f428324216b672749cbdb0848</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_fetch_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga39870b7b6921d35742dcc98646d4b7a9</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_delete_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga362281093747e882ca8ba5bcb2e6cdcb</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_delete_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga19f261014954506058725a05d1e9b017</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_delete_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga6e4d15799c832261e9c41031c025e6cd</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="typedef">
      <type>TXCAP_BEGIN_DECLS enum txcap_action_type_e</type>
      <name>txcap_action_type_t</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>ab48d23353e6f7381766c3946b2de3270</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum txcap_action_target_e</type>
      <name>txcap_action_target_t</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>ae3f5f7f277fdca11b03434d38e07fd97</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum txcap_action_option_e</type>
      <name>txcap_action_option_t</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>ae404e3c23783c1135c34611dd1aaae5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum txcap_action_param_type_e</type>
      <name>txcap_action_param_type_t</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1b16090f879765af9a99da1913630692</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_action_type_e</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a869c3d8e91c06dca9ee38627c1e6a061</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atp_create</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a869c3d8e91c06dca9ee38627c1e6a061a72d2b146a899620bd26d67a6c6aa2f1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atp_replace</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a869c3d8e91c06dca9ee38627c1e6a061ae45e64e30950b7f798928712913ff3a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atp_fetch</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a869c3d8e91c06dca9ee38627c1e6a061a057a25f2090691fab0b292a864677d0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atp_delete</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a869c3d8e91c06dca9ee38627c1e6a061ad129ae80b7fefb6e2ffc58f5e4cef131</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_action_target_e</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>add5f633a6cef7c7fc4fb8965e4b2cf80</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atg_element</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>add5f633a6cef7c7fc4fb8965e4b2cf80a8da7d39587d683dc05cda3fc3792f1b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atg_document</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>add5f633a6cef7c7fc4fb8965e4b2cf80a8a707aef10e6e876a14f2000d3ef0258</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_atg_attribute</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>add5f633a6cef7c7fc4fb8965e4b2cf80a3d059d0d3e3853ecb520655d09cecc99</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_action_option_e</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a126e90bef3485fff033d7b88dfd99e2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_ACTION_OPTION_TIMEOUT</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a126e90bef3485fff033d7b88dfd99e2ea70ea20575574fbf26d9951dffe22cbf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_action_param_type_e</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_apt_null</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64a47c417669f41161d365286f7e45d25de</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_apt_option</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64a10c263268793862616555b9c38614ce5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_apt_header</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64a2f7ddd46fb8ffc93b6120df12629d911</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_apt_payload</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64a5a5fba1f015e425ed37e9866e70c3418</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_apt_selector</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64a37f1f75361f13404e9a61eb79aa7e5b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>txcap_apt_urlstring</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a1633fe947eb619c2ba67afd392eb9e64a12a850282b584ee5e144248546cc0f4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API int</type>
      <name>txcap_action_perform</name>
      <anchorfile>txcap__action_8h.html</anchorfile>
      <anchor>a655931ab014e9888031fb3aad8586081</anchor>
      <arglist>(txcap_stack_handle_t *stack, txcap_action_type_t type, txcap_action_target_t target,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_auid.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/tinyXCAP/</path>
    <filename>txcap__auid_8h</filename>
    <includes id="tinyxcap__config_8h" name="tinyxcap_config.h" local="yes" imported="no">tinyXCAP_config.h</includes>
    <class kind="struct">txcap_auid_s</class>
    <member kind="typedef">
      <type>TXCAP_BEGIN_DECLS enum txcap_auid_type_e</type>
      <name>txcap_auid_type_t</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ac55e909f277ee04a3cd3025e083f1677</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct txcap_auid_s</type>
      <name>txcap_auid_t</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>aee2bc25ba241afd2b3dc6eed47beb0d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>txcap_auids_L_t</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>a8d38563121a3f7e851793a32ce676c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_auid_type_e</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_dummy</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7a83fbdc787144805954ee333730e38ccc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_ietf_xcap_caps</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7a7a7ac0fa9baeae911c17307e883f41ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_ietf_resource_lists</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7aba2b3ecac6065390d403535140cacb68</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_ietf_rls_services</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7a38c4d8c903a4542e5013290f7b358551</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_ietf_pres_rules</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7a132c8ce10f982b0008d28290d702bb3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_ietf_directory</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7a150d134a12d307e50d9a65f3c3bf13d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_oma_conv_history</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7afd81a6a8939d732945e00048e9a3791b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_oma_pres_rules</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7ae21c0cac19bc8240be43186b6132fe6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_oma_directory</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7a085b33d7b7d1f557e13fbeb590b1996f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_oma_deferred_list</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7ab46b2ecbe522026131f0a6399d8b428b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_oma_pres_content</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7ab60346a7c4d1577c5e372f7412d06524</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tauid_oma_shared_groups</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ada31da1147a49bd5601065fa248e75c7ae54cb67102623d7b3651387f67a7ad86</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_auid_register</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>a07698ba0b25c8b167d2e6e8f48b559c0</anchor>
      <arglist>(txcap_auids_L_t *auids, const char *id, const char *mime_type, const char *ns, const char *document_name, tsk_bool_t is_global)</arglist>
    </member>
    <member kind="function">
      <type>txcap_auid_t *</type>
      <name>txcap_auid_get_by_id</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>ad053aef245824c96128d24bc9750d41e</anchor>
      <arglist>(txcap_auids_L_t *auids, const char *id)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_auids_init</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>a3cdcbe236d5444b5fdac8017a7963e46</anchor>
      <arglist>(txcap_auids_L_t **auids)</arglist>
    </member>
    <member kind="variable">
      <type>TINYXCAP_GEXTERN const tsk_object_def_t *</type>
      <name>txcap_auid_def_t</name>
      <anchorfile>txcap__auid_8h.html</anchorfile>
      <anchor>a18eb1bd014bb725bdf463e5b0c008fd5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_document.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/tinyXCAP/</path>
    <filename>txcap__document_8h</filename>
    <includes id="tinyxcap__config_8h" name="tinyxcap_config.h" local="yes" imported="no">tinyXCAP_config.h</includes>
    <includes id="txcap_8h" name="txcap.h" local="yes" imported="no">txcap.h</includes>
    <member kind="function">
      <type>TXCAP_BEGIN_DECLS TINYXCAP_API char *</type>
      <name>txcap_selector_get_document</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga75307ec551003312807af27cc5b09da1</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id)</arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API char *</type>
      <name>txcap_selector_get_document_2</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gac0bc0eefb98384d7c31b6a828ace4389</anchor>
      <arglist>(const char *xcap_root, const char *auid_id, const char *xui, const char *doc_name)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_node.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/tinyXCAP/</path>
    <filename>txcap__node_8h</filename>
    <includes id="tinyxcap__config_8h" name="tinyxcap_config.h" local="yes" imported="no">tinyXCAP_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_NAME</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga5160eb295db11e8557086fc42263e48f</anchor>
      <arglist>(QNAME_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_POS</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga7ccf665a941bf545ee608f912877d3c6</anchor>
      <arglist>(QNAME_STR, POS_UINT)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_ATTRIBUTE</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga1fbeb0d3296fde62814890dfaeb9c872</anchor>
      <arglist>(QNAME_STR, ATT_QNAME_STR, ATT_VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_POS_ATTRIBUTE</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga8e139963afdf60b0746c164d8ab0426c</anchor>
      <arglist>(QNAME_STR, POS_UINT, ATT_QNAME_STR, ATT_VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_NAMESPACE</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gaa0afbb9dde8ceb5e441c3e09aa83af8d</anchor>
      <arglist>(PREFIX_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_NULL</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga84757d87777f1fe02533c1445b218f04</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>TXCAP_BEGIN_DECLS enum txcap_selector_param_type_e</type>
      <name>txcap_selector_param_type_t</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>ae02376f17a8e4c8531dc2b7b04301312</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_selector_param_type_e</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_node_null</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73ba9aaf9106b4a0778f7e8bfcf574ea71c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_node_name</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73ba8154a49d6b733938038617e81d30cf49</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_node_pos</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73ba7a43e7c458fe6d20945f935b0de3b1cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_node_attribute</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73ba73310258fe05cdb12e694f8c5248bca5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_node_pos_n_attribute</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73ba03a5dd7115e29ba09703bb30b3e97b02</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_node_namespace</name>
      <anchorfile>txcap__node_8h.html</anchorfile>
      <anchor>a3b2a79d6e26bbf71f002f0dcc5afa73bafc9543a00bf44f7ce80dc2dd184b103d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API char *</type>
      <name>txcap_selector_get_node</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga1e2d23fa5fb817509b923f91400e63d7</anchor>
      <arglist>(const char *auid_id,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API char *</type>
      <name>txcap_selector_get_node_2</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga468ee48d2d553b8f56cbc62807239271</anchor>
      <arglist>(const char *auid_id, va_list *app)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_selector.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/tinyXCAP/</path>
    <filename>txcap__selector_8h</filename>
    <includes id="tinyxcap__config_8h" name="tinyxcap_config.h" local="yes" imported="no">tinyXCAP_config.h</includes>
    <includes id="txcap_8h" name="txcap.h" local="yes" imported="no">txcap.h</includes>
    <member kind="function">
      <type>TXCAP_BEGIN_DECLS TINYXCAP_API char *</type>
      <name>txcap_selector_get_url</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gabd7ca01b3076ebcfe9457ca3e565759e</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tinyxcap_config.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/</path>
    <filename>tinyxcap__config_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TINYXCAP_API</name>
      <anchorfile>tinyxcap__config_8h.html</anchorfile>
      <anchor>a54664825f55f2bf2a8e6feb9152e41dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYXCAP_GEXTERN</name>
      <anchorfile>tinyxcap__config_8h.html</anchorfile>
      <anchor>ad1dd8024d491d87ade682e7f4b09ec0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_BEGIN_DECLS</name>
      <anchorfile>tinyxcap__config_8h.html</anchorfile>
      <anchor>a9c75269aa7b9b18f6648a928f58b5b11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_END_DECLS</name>
      <anchorfile>tinyxcap__config_8h.html</anchorfile>
      <anchor>a263ab01cd338dc458fc637d1d4ea6e61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap.h</name>
    <path>C:/Projects/Doubango/tinyXCAP/include/</path>
    <filename>txcap_8h</filename>
    <includes id="tinyxcap__config_8h" name="tinyxcap_config.h" local="yes" imported="no">tinyXCAP_config.h</includes>
    <includes id="txcap__auid_8h" name="txcap_auid.h" local="yes" imported="no">tinyXCAP/txcap_auid.h</includes>
    <class kind="struct">txcap_stack_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_OPTION</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga9fa6d7713160de9afe77d210c5a7f605</anchor>
      <arglist>(ID_ENUM, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_PASSWORD</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gae376cf7191b894f99f2ef10f3baf4567</anchor>
      <arglist>(PASSWORD_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_XUI</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gac6736cdf9fc3fc3aadd0e1877dac9713</anchor>
      <arglist>(XUI_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_HEADER</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga62f11e3b0ef1cccc10a7808277a82c72</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_CONTEXT</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga0cb211757e5d6fdc3252cb2c16c7ab1f</anchor>
      <arglist>(CTX_PTR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_AUID</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga13a1de981cf3827e7a59a8eea5c9dcc8</anchor>
      <arglist>(ID_STR, MIME_TYPE_STR, NS_STR, DOC_NAME_STR, IS_GLOBAL_BOOL)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_NULL</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga3442fc62fb3de2d42e51cc6c7fa0d479</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>TXCAP_BEGIN_DECLS enum txcap_stack_option_e</type>
      <name>txcap_stack_option_t</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>af9bafb5b6c4395b7be962e7ac3de2f32</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum txcap_stack_param_type_e</type>
      <name>txcap_stack_param_type_t</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a7dd685cef440db49713a90dbecbbc399</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct txcap_stack_s</type>
      <name>txcap_stack_t</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a34af5d5ddecab024bbc063db570af6f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>txcap_stack_handle_t</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>aa41f6bc3290abf8b95a36c2422abaff9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_stack_option_e</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_STACK_OPTION_TIMEOUT</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65a180506fc290244e63cebb8fb88cf2987</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_STACK_OPTION_TTL</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65ab0cc54bd353ce0d95506d235d9c37780</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_STACK_OPTION_PASSWORD</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65a60df8aca6d453c625e515416a441fcbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_STACK_OPTION_XUI</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65a6b42f00f3d0e5b1a0d0a408b0ea18b3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_STACK_OPTION_LOCAL_IP</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65ae9cc2a54dac5e62c6207cf14a57750f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TXCAP_STACK_OPTION_LOCAL_PORT</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2ccc19e57676e593f4174cc2250d5e65abbece683a7c8f113bbc3b912e415fec3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>txcap_stack_param_type_e</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2977725773e5d560f43a662c2175fcd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_null</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2977725773e5d560f43a662c2175fcd6adc4f3307e7e19f155a02fd0ed62c8656</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_option</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2977725773e5d560f43a662c2175fcd6a33114dc48aa36d03cebfe20cf9a78079</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_header</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2977725773e5d560f43a662c2175fcd6ad01d9651df7986377e37029cb6f208c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_context</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2977725773e5d560f43a662c2175fcd6abdb3a3e3ace37b8937f589ac470ad5a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>xcapp_auid</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>a2977725773e5d560f43a662c2175fcd6a3db7f3148df94fb3281729599262721d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API txcap_stack_handle_t *</type>
      <name>txcap_stack_create</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gae20920e45078b18fc76c52c63931b2e9</anchor>
      <arglist>(thttp_stack_callback_f callback, const char *xui, const char *password, const char *xcap_root,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API int</type>
      <name>txcap_stack_start</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gaf0ce688bb71c94dc40623efab777e483</anchor>
      <arglist>(txcap_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API int</type>
      <name>txcap_stack_set</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gad8917c9fa5e12bd84e42b55538a24544</anchor>
      <arglist>(txcap_stack_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>TINYXCAP_API int</type>
      <name>txcap_stack_stop</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gabbef76dfa185d22149e259e2e11d165b</anchor>
      <arglist>(txcap_stack_handle_t *self)</arglist>
    </member>
    <member kind="variable">
      <type>TINYXCAP_GEXTERN const tsk_object_def_t *</type>
      <name>txcap_stack_def_t</name>
      <anchorfile>txcap_8h.html</anchorfile>
      <anchor>ab627bc7b38b1ba6e9618aaa7b59f6a32</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap.c</name>
    <path>C:/Projects/Doubango/tinyXCAP/src/</path>
    <filename>txcap_8c</filename>
    <includes id="txcap_8h" name="txcap.h" local="yes" imported="no">txcap.h</includes>
    <member kind="function">
      <type>int</type>
      <name>__txcap_stack_set</name>
      <anchorfile>txcap_8c.html</anchorfile>
      <anchor>ad045b2ab5910762e5b93325ee9bda55c</anchor>
      <arglist>(txcap_stack_t *self, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>txcap_stack_handle_t *</type>
      <name>txcap_stack_create</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gae20920e45078b18fc76c52c63931b2e9</anchor>
      <arglist>(thttp_stack_callback_f callback, const char *xui, const char *password, const char *xcap_root,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_stack_start</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gaf0ce688bb71c94dc40623efab777e483</anchor>
      <arglist>(txcap_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_stack_set</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gad8917c9fa5e12bd84e42b55538a24544</anchor>
      <arglist>(txcap_stack_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_stack_stop</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gabbef76dfa185d22149e259e2e11d165b</anchor>
      <arglist>(txcap_stack_handle_t *self)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>txcap_stack_def_t</name>
      <anchorfile>txcap_8c.html</anchorfile>
      <anchor>a8d4fe39e7adb8cf4f930eec2ec240207</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_action.c</name>
    <path>C:/Projects/Doubango/tinyXCAP/src/</path>
    <filename>txcap__action_8c</filename>
    <includes id="txcap__action_8h" name="txcap_action.h" local="yes" imported="no">tinyXCAP/txcap_action.h</includes>
    <includes id="txcap__selector_8h" name="txcap_selector.h" local="yes" imported="no">tinyXCAP/txcap_selector.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_MIME_TYPE_ELEMENT</name>
      <anchorfile>txcap__action_8c.html</anchorfile>
      <anchor>a26a0d0db583aa97ddc3e354a47589e9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_MIME_TYPE_ATTRIBUTE</name>
      <anchorfile>txcap__action_8c.html</anchorfile>
      <anchor>af0b508eccdcef6075c564c4885a6248c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_MIME_TYPE_NS</name>
      <anchorfile>txcap__action_8c.html</anchorfile>
      <anchor>ab4215e7b8a28b326b032a52a28cecbe8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_MIME_TYPE_ERROR</name>
      <anchorfile>txcap__action_8c.html</anchorfile>
      <anchor>ad1e4e28139aa9ffdf9b7d7f0eefc8f4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>__txcap_selector_get_url</name>
      <anchorfile>txcap__action_8c.html</anchorfile>
      <anchor>a3f6e3e9f7a99f4fe99b929dd4c805435</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_action_perform</name>
      <anchorfile>txcap__action_8c.html</anchorfile>
      <anchor>a53557ca13669c8d518bcf55e493147fe</anchor>
      <arglist>(txcap_stack_handle_t *stack, txcap_action_type_t type, txcap_action_target_t target,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_auid.c</name>
    <path>C:/Projects/Doubango/tinyXCAP/src/</path>
    <filename>txcap__auid_8c</filename>
    <includes id="txcap__auid_8h" name="txcap_auid.h" local="yes" imported="no">tinyXCAP/txcap_auid.h</includes>
    <class kind="struct">auid_s</class>
    <member kind="typedef">
      <type>struct auid_s</type>
      <name>auid_t</name>
      <anchorfile>txcap__auid_8c.html</anchorfile>
      <anchor>a76340f60ec9c92fc02dbadd86908f7e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>txcap_auid_t *</type>
      <name>txcap_auid_create</name>
      <anchorfile>txcap__auid_8c.html</anchorfile>
      <anchor>a6b0802fefabdcad4dbddb263bf95796a</anchor>
      <arglist>(txcap_auid_type_t type, const char *id, const char *mime_type, const char *ns, const char *document_name, tsk_bool_t is_global)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_auids_init</name>
      <anchorfile>txcap__auid_8c.html</anchorfile>
      <anchor>a3cdcbe236d5444b5fdac8017a7963e46</anchor>
      <arglist>(txcap_auids_L_t **auids)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_auid_register</name>
      <anchorfile>txcap__auid_8c.html</anchorfile>
      <anchor>a07698ba0b25c8b167d2e6e8f48b559c0</anchor>
      <arglist>(txcap_auids_L_t *auids, const char *id, const char *mime_type, const char *ns, const char *document_name, tsk_bool_t is_global)</arglist>
    </member>
    <member kind="function">
      <type>txcap_auid_t *</type>
      <name>txcap_auid_get_by_id</name>
      <anchorfile>txcap__auid_8c.html</anchorfile>
      <anchor>ad053aef245824c96128d24bc9750d41e</anchor>
      <arglist>(txcap_auids_L_t *auids, const char *id)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>txcap_auid_def_t</name>
      <anchorfile>txcap__auid_8c.html</anchorfile>
      <anchor>a04578a3b21a025b918ab33400fdb6a34</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_document.c</name>
    <path>C:/Projects/Doubango/tinyXCAP/src/</path>
    <filename>txcap__document_8c</filename>
    <includes id="txcap__document_8h" name="txcap_document.h" local="yes" imported="no">tinyXCAP/txcap_document.h</includes>
    <includes id="txcap__auid_8h" name="txcap_auid.h" local="yes" imported="no">tinyXCAP/txcap_auid.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_document</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga75307ec551003312807af27cc5b09da1</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_document_2</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gac0bc0eefb98384d7c31b6a828ace4389</anchor>
      <arglist>(const char *xcap_root, const char *auid_id, const char *xui, const char *doc_name)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_node.c</name>
    <path>C:/Projects/Doubango/tinyXCAP/src/</path>
    <filename>txcap__node_8c</filename>
    <includes id="txcap__node_8h" name="txcap_node.h" local="yes" imported="no">tinyXCAP/txcap_node.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_node</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga1e2d23fa5fb817509b923f91400e63d7</anchor>
      <arglist>(const char *auid_id,...)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_node_2</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga468ee48d2d553b8f56cbc62807239271</anchor>
      <arglist>(const char *auid_id, va_list *app)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>txcap_selector.c</name>
    <path>C:/Projects/Doubango/tinyXCAP/src/</path>
    <filename>txcap__selector_8c</filename>
    <includes id="txcap__selector_8h" name="txcap_selector.h" local="yes" imported="no">tinyXCAP/txcap_selector.h</includes>
    <includes id="txcap__document_8h" name="txcap_document.h" local="yes" imported="no">tinyXCAP/txcap_document.h</includes>
    <includes id="txcap__node_8h" name="txcap_node.h" local="yes" imported="no">tinyXCAP/txcap_node.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>__txcap_selector_get_url</name>
      <anchorfile>txcap__selector_8c.html</anchorfile>
      <anchor>a3f6e3e9f7a99f4fe99b929dd4c805435</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_url</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gabd7ca01b3076ebcfe9457ca3e565759e</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id,...)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>txcap_stack_group</name>
    <title>XCAP stack</title>
    <filename>group__txcap__stack__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_OPTION</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga9fa6d7713160de9afe77d210c5a7f605</anchor>
      <arglist>(ID_ENUM, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_PASSWORD</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gae376cf7191b894f99f2ef10f3baf4567</anchor>
      <arglist>(PASSWORD_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_XUI</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gac6736cdf9fc3fc3aadd0e1877dac9713</anchor>
      <arglist>(XUI_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_HEADER</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga62f11e3b0ef1cccc10a7808277a82c72</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_CONTEXT</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga0cb211757e5d6fdc3252cb2c16c7ab1f</anchor>
      <arglist>(CTX_PTR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_AUID</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga13a1de981cf3827e7a59a8eea5c9dcc8</anchor>
      <arglist>(ID_STR, MIME_TYPE_STR, NS_STR, DOC_NAME_STR, IS_GLOBAL_BOOL)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_STACK_SET_NULL</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>ga3442fc62fb3de2d42e51cc6c7fa0d479</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>txcap_stack_handle_t *</type>
      <name>txcap_stack_create</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gae20920e45078b18fc76c52c63931b2e9</anchor>
      <arglist>(thttp_stack_callback_f callback, const char *xui, const char *password, const char *xcap_root,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_stack_start</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gaf0ce688bb71c94dc40623efab777e483</anchor>
      <arglist>(txcap_stack_handle_t *self)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_stack_set</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gad8917c9fa5e12bd84e42b55538a24544</anchor>
      <arglist>(txcap_stack_handle_t *self,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>txcap_stack_stop</name>
      <anchorfile>group__txcap__stack__group.html</anchorfile>
      <anchor>gabbef76dfa185d22149e259e2e11d165b</anchor>
      <arglist>(txcap_stack_handle_t *self)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>txcap_action_group</name>
    <title>XDMC (Sending Requests)</title>
    <filename>group__txcap__action__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_OPTION</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga7913c72583d400da51dea9f46bed553b</anchor>
      <arglist>(ID_INT, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_HEADER</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>gaf422679f169f916bdf2c786e4171622e</anchor>
      <arglist>(NAME_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_PAYLOAD</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga566653198a295d9d9854c35974c91e1a</anchor>
      <arglist>(PAY_PTR, PAY_SIZE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_SELECTOR</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga599c6c0389addf0e0d9a14bdbcef017e</anchor>
      <arglist>(AUID_STR,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_REQUEST_URI</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga17133536f59ded5dadd169d1673dd689</anchor>
      <arglist>(URI_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_ACTION_SET_NULL</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga0f062a39307c1230628d37b0f7476346</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_create_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga3d7b7194078364d1c43c9fa5f09ca40e</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_create_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga36013b774b75655eeaea1e859e33b502</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_create_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga0cecb0adbc797545bc6cc28e9d4ce5a3</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_replace_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga375d103ff56afadc8032b1f6b38ab249</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_replace_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga7eb8bddb5492761b82380ab0d1233c9a</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_replace_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga4d7f98bd61b8d241ba2b9c83dc009092</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_fetch_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga0966cf2d592efecd10a8bd2d2b023c58</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_fetch_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga6a4b9b3f428324216b672749cbdb0848</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_fetch_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga39870b7b6921d35742dcc98646d4b7a9</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_delete_element</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga362281093747e882ca8ba5bcb2e6cdcb</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_delete_document</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga19f261014954506058725a05d1e9b017</anchor>
      <arglist>(stack,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>txcap_action_delete_attribute</name>
      <anchorfile>group__txcap__action__group.html</anchorfile>
      <anchor>ga6e4d15799c832261e9c41031c025e6cd</anchor>
      <arglist>(stack,...)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>txcap_selector_group</name>
    <title>XCAP URL selector</title>
    <filename>group__txcap__selector__group.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_NAME</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga5160eb295db11e8557086fc42263e48f</anchor>
      <arglist>(QNAME_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_POS</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga7ccf665a941bf545ee608f912877d3c6</anchor>
      <arglist>(QNAME_STR, POS_UINT)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_ATTRIBUTE</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga1fbeb0d3296fde62814890dfaeb9c872</anchor>
      <arglist>(QNAME_STR, ATT_QNAME_STR, ATT_VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_POS_ATTRIBUTE</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga8e139963afdf60b0746c164d8ab0426c</anchor>
      <arglist>(QNAME_STR, POS_UINT, ATT_QNAME_STR, ATT_VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_NAMESPACE</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gaa0afbb9dde8ceb5e441c3e09aa83af8d</anchor>
      <arglist>(PREFIX_STR, VALUE_STR)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TXCAP_SELECTOR_NODE_SET_NULL</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga84757d87777f1fe02533c1445b218f04</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_document</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga75307ec551003312807af27cc5b09da1</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_document_2</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gac0bc0eefb98384d7c31b6a828ace4389</anchor>
      <arglist>(const char *xcap_root, const char *auid_id, const char *xui, const char *doc_name)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_node</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga1e2d23fa5fb817509b923f91400e63d7</anchor>
      <arglist>(const char *auid_id,...)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_node_2</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>ga468ee48d2d553b8f56cbc62807239271</anchor>
      <arglist>(const char *auid_id, va_list *app)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>txcap_selector_get_url</name>
      <anchorfile>group__txcap__selector__group.html</anchorfile>
      <anchor>gabd7ca01b3076ebcfe9457ca3e565759e</anchor>
      <arglist>(const txcap_stack_handle_t *stack, const char *auid_id,...)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>auid_s</name>
    <filename>structauid__s.html</filename>
    <member kind="variable">
      <type>txcap_auid_type_t</type>
      <name>type</name>
      <anchorfile>structauid__s.html</anchorfile>
      <anchor>a3e4bffb785139534ee1f160685887c0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>id</name>
      <anchorfile>structauid__s.html</anchorfile>
      <anchor>aeffa2f0815ce90fecbda9aac199143db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>mime_type</name>
      <anchorfile>structauid__s.html</anchorfile>
      <anchor>a12139285594e136f3f40ec07d2fdc5c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>ns</name>
      <anchorfile>structauid__s.html</anchorfile>
      <anchor>a2ec6583af2612eed6944f95f0431eee8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>document_name</name>
      <anchorfile>structauid__s.html</anchorfile>
      <anchor>a7a57829128684dadc8df9382d33ce133</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>global</name>
      <anchorfile>structauid__s.html</anchorfile>
      <anchor>a30fb50f2a8e3336473796ead281a0f58</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>txcap_auid_s</name>
    <filename>structtxcap__auid__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>txcap_auid_type_t</type>
      <name>type</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>a3e4bffb785139534ee1f160685887c0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>id</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>aecb3b0d045ada529257a2fbf8f829599</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>mime_type</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>acda933efdc515b632bbace5658402427</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ns</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>a5fc7288f3742da2ab55985a1836d01c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>document_name</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>a8542617c3b9083b8b73abfd4ddea94e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_bool_t</type>
      <name>global</name>
      <anchorfile>structtxcap__auid__s.html</anchorfile>
      <anchor>a30fb50f2a8e3336473796ead281a0f58</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>txcap_stack_s</name>
    <filename>structtxcap__stack__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>xui</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a51bd5af50dd722b9d7147e0e8d75b7d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>password</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a59460a3ff2c12443d1022e5cc0fba85c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>xcap_root</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a727097ccf0d9a6c57d1d7480e1c34afa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_session_handle_t *</type>
      <name>http_session</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>afbbb143ae1207cd710c136527425dbe2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>thttp_stack_handle_t *</type>
      <name>http_stack</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a3a2f37bda797ff6851be6004dfc581a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_options_L_t *</type>
      <name>options</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>af5b6cc93a0b124046788312d128c9754</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>context</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a3f18b45f62968a5b7281db2feefd275c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>txcap_auids_L_t *</type>
      <name>auids</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a7a5cf9a37082230b18fdf2ccb0ae9f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtxcap__stack__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
