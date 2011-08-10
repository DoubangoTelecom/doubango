<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>tinySigComp API Overview</title>
    <filename>index</filename>
  </compound>
  <compound kind="file">
    <name>adler32.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>adler32_8c</filename>
    <includes id="zlib_8h" name="zlib.h" local="yes" imported="no">zlib.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ZLIB_INTERNAL</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a59d4d7ef4d73655c759211ec33a5d44f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BASE</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a79bcfb6bde984f42d1124b068a509af7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NMAX</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a5de5d183f9a6a8d53316f743e1ca6dc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO1</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a465ff70ce96dfc2e84b0e87548b4ecb4</anchor>
      <arglist>(buf, i)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO2</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a3d7044f8ea7e75164fe5108048fd87eb</anchor>
      <arglist>(buf, i)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO4</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>aef9bafc8b3d89e98b6e26320f99b9e31</anchor>
      <arglist>(buf, i)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO8</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a9aafc447614bf5c4dc0d484aba9edb89</anchor>
      <arglist>(buf, i)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO16</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a6912c3e78e2b797f42f214d1b508aa0c</anchor>
      <arglist>(buf)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOD</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>aa1e26f19f1e6cf348e41511e7db90881</anchor>
      <arglist>(a)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOD4</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>ad7b4cfdfbc3e12bf00d27e3375d196d4</anchor>
      <arglist>(a)</arglist>
    </member>
    <member kind="function">
      <type>uLong ZEXPORT</type>
      <name>adler32</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>a86607743a4b76949b24cf5cc2f01a40d</anchor>
      <arglist>(uLong adler, const Bytef *buf, uInt len)</arglist>
    </member>
    <member kind="function">
      <type>uLong ZEXPORT</type>
      <name>adler32_combine</name>
      <anchorfile>adler32_8c.html</anchorfile>
      <anchor>af4a8b45f615e831c56b08da530870e59</anchor>
      <arglist>(uLong adler1, uLong adler2, z_off_t len2)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>compress.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>compress_8c</filename>
    <includes id="zlib_8h" name="zlib.h" local="yes" imported="no">zlib.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ZLIB_INTERNAL</name>
      <anchorfile>compress_8c.html</anchorfile>
      <anchor>a59d4d7ef4d73655c759211ec33a5d44f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>compress2</name>
      <anchorfile>compress_8c.html</anchorfile>
      <anchor>aede7e438c033a969a1a40ba6b44f91f9</anchor>
      <arglist>(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>compress</name>
      <anchorfile>compress_8c.html</anchorfile>
      <anchor>aef315743418d760a360e1be293d27cd1</anchor>
      <arglist>(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)</arglist>
    </member>
    <member kind="function">
      <type>uLong ZEXPORT</type>
      <name>compressBound</name>
      <anchorfile>compress_8c.html</anchorfile>
      <anchor>a6bfd92b4426ff6008c841c2cc6f8fed3</anchor>
      <arglist>(uLong sourceLen)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>deflate.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>deflate_8c</filename>
    <includes id="deflate_8h" name="deflate.h" local="yes" imported="no">deflate.h</includes>
    <class kind="struct">config_s</class>
    <class kind="struct">static_tree_desc_s</class>
    <member kind="define">
      <type>#define</type>
      <name>NIL</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a263efce0e5cda1d387a1a43f94715445</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOO_FAR</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>abeff1f2c9d15b7bddefe44a4360821b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_LOOKAHEAD</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>aab627bcc7d38e658a93d395c50180cb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EQUAL</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a214c717b2e51e1993a749ac99df7de58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UPDATE_HASH</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a44b283365d6ca10b7028e639c4a10e67</anchor>
      <arglist>(s, h, c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INSERT_STRING</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a52d7ca8e864a07c0ba4a25d04588168b</anchor>
      <arglist>(s, str, match_head)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLEAR_HASH</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>af328dc05f080f25747abf0edd75b6251</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>check_match</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a4041db99b23ca08963d3e5c284d837d6</anchor>
      <arglist>(s, start, match, length)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FLUSH_BLOCK_ONLY</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>afbe439cc64ad2722568bcf5eb12f06e0</anchor>
      <arglist>(s, eof)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FLUSH_BLOCK</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a60091ec200ac7904b7d4e34563a8f421</anchor>
      <arglist>(s, eof)</arglist>
    </member>
    <member kind="typedef">
      <type>block_state compress_func</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a031e5ebda987f48c0ae613d46b8ac406</anchor>
      <arglist>((deflate_state *s, int flush))</arglist>
    </member>
    <member kind="typedef">
      <type>struct config_s</type>
      <name>config</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a4473b5227787415097004fd39f55185e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>block_state</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a1048c01fb24f2195a6b9a2a7c12e9454</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>need_more</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a1048c01fb24f2195a6b9a2a7c12e9454aa05dd4e7c2f875eee52a6568c244c1e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>block_done</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a1048c01fb24f2195a6b9a2a7c12e9454a94dca976f7dcfb0605e086068f09daa1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>finish_started</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a1048c01fb24f2195a6b9a2a7c12e9454a5ff14f05c1ba145fc4da40fb50211140</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>finish_done</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a1048c01fb24f2195a6b9a2a7c12e9454af1f0edbb0d9d8272a762de3b7be0b50c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>local void fill_window</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a39020875dba6274d4890d928ece7af4d</anchor>
      <arglist>((deflate_state *s))</arglist>
    </member>
    <member kind="function">
      <type>local block_state deflate_stored</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a8c5f4f16898c33f6c80e2c03a8f41f8a</anchor>
      <arglist>((deflate_state *s, int flush))</arglist>
    </member>
    <member kind="function">
      <type>local void putShortMSB</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a7a385b4ab82557d539fe9a6059caf4d7</anchor>
      <arglist>((deflate_state *s, uInt b))</arglist>
    </member>
    <member kind="function">
      <type>local void flush_pending</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a7c2e426a5f352f7e8c43ef7beec5138f</anchor>
      <arglist>((z_streamp strm))</arglist>
    </member>
    <member kind="function">
      <type>local int read_buf</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a89775aad02a4843022d7e83a3301239b</anchor>
      <arglist>((z_streamp strm, Bytef *buf, unsigned size))</arglist>
    </member>
    <member kind="function">
      <type>local uInt longest_match</type>
      <name>OF</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>aafad197bf6ab131a1734f6c8150b162b</anchor>
      <arglist>((deflate_state *s, IPos cur_match))</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateInit_</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a0e6601591faa9573f27340153b0f3296</anchor>
      <arglist>(z_streamp strm, int level, const char *version, int stream_size)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateInit2_</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>ae501d2862c68d17b909d6f1c9264815c</anchor>
      <arglist>(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char *version, int stream_size)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateSetDictionary</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a4e2855f986abd56d4515e9bb09986a5b</anchor>
      <arglist>(z_streamp strm, const Bytef *dictionary, uInt dictLength)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateReset</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a4f17a4d330d76066b4b26c27f7cde5dc</anchor>
      <arglist>(z_streamp strm)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateSetHeader</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a0bb9d562cacbe698297de30cdeeb2153</anchor>
      <arglist>(z_streamp strm, gz_headerp head)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflatePrime</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>aa8ca15b272bfde8fd23ab658d789aa87</anchor>
      <arglist>(z_streamp strm, int bits, int value)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateParams</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a6be3d26b7e317aa4db6c7870b1911d4d</anchor>
      <arglist>(z_streamp strm, int level, int strategy)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateTune</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>aa2f2f8ee11441c80c35f035ad7efbb8e</anchor>
      <arglist>(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain)</arglist>
    </member>
    <member kind="function">
      <type>uLong ZEXPORT</type>
      <name>deflateBound</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a746d0203373d0033ab69055f72a370e8</anchor>
      <arglist>(z_streamp strm, uLong sourceLen)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>putShortMSB</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a6db0a0a840addf113cb217c4aa1c0384</anchor>
      <arglist>(deflate_state *s, uInt b)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>flush_pending</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>ae9a3263767d40c4f7443bf07c31e0119</anchor>
      <arglist>(z_streamp strm)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflate</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>aedba3a94d6e827d61b660443ae5b9f09</anchor>
      <arglist>(z_streamp strm, int flush)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateEnd</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>ada68769789b1d84de7e708d6e848a075</anchor>
      <arglist>(z_streamp strm)</arglist>
    </member>
    <member kind="function">
      <type>int ZEXPORT</type>
      <name>deflateCopy</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a496430688ab9f4179689c883a6776b02</anchor>
      <arglist>(z_streamp dest, z_streamp source)</arglist>
    </member>
    <member kind="function">
      <type>local int</type>
      <name>read_buf</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a30b7fcd3e6bea023c1c00eafad492639</anchor>
      <arglist>(z_streamp strm, Bytef *buf, unsigned size)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>lm_init</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>ab197bb7bbeee75dcfccbfa8d55cae272</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local uInt</type>
      <name>longest_match</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a9c89f33a3fab1b87063832eeda415389</anchor>
      <arglist>(deflate_state *s, IPos cur_match)</arglist>
    </member>
    <member kind="function">
      <type>local uInt</type>
      <name>longest_match_fast</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>ada5e12c38f51cac74465ef4af16e46ed</anchor>
      <arglist>(deflate_state *s, IPos cur_match)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>fill_window</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a6cf3c43817cac5f5c422e94b2a57ea49</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local block_state</type>
      <name>deflate_stored</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a67569326587f77f823522212036fc2f9</anchor>
      <arglist>(deflate_state *s, int flush)</arglist>
    </member>
    <member kind="function">
      <type>local block_state</type>
      <name>deflate_fast</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a1ee6259c01fb1c8cd3ecc551a8fe7192</anchor>
      <arglist>(deflate_state *s, int flush)</arglist>
    </member>
    <member kind="function">
      <type>local block_state</type>
      <name>deflate_slow</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a9fc7e4969451bb185e77d530c376f79f</anchor>
      <arglist>(deflate_state *s, int flush)</arglist>
    </member>
    <member kind="variable">
      <type>const char</type>
      <name>deflate_copyright</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a75f4ce56f376b4ec7efc61fab510c8e7</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>local const config</type>
      <name>configuration_table</name>
      <anchorfile>deflate_8c.html</anchorfile>
      <anchor>a9d8772078a7001d0dc9bd34789631322</anchor>
      <arglist>[10]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>deflate.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>deflate_8h</filename>
    <includes id="zutil_8h" name="zutil.h" local="yes" imported="no">zutil.h</includes>
    <class kind="struct">ct_data_s</class>
    <class kind="struct">tree_desc_s</class>
    <class kind="struct">internal_state</class>
    <member kind="define">
      <type>#define</type>
      <name>LENGTH_CODES</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a8dd3b78c666b6592944b2acf1706e611</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LITERALS</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a3f59145500c9de746e7dc49fb98c29ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>L_CODES</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a2b0cc00cc6a7d521fba8d208a1cae477</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>D_CODES</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a8798a9a526623abbd88fedb0727e21aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BL_CODES</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a9e19158a493307d4f211cdf223da8319</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HEAP_SIZE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a1b45302695680930829cac31d65e41e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_BITS</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a51c241e7ad52441b8d4c1fd5f498b3b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INIT_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a96cb576b9a3b5f8cb90bd4bb7ee449d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXTRA_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>affc01bd472ba5f5ad1519c40e20a3778</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NAME_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>acec08b85e51457d214b759d489785e99</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COMMENT_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a06c47936258e98a0c4ca38dd31a5f6c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HCRC_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a09dffa22705cbe81ca94b8b379d43682</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BUSY_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a4821f69a5605c2618cd4dc4d3f60979c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FINISH_STATE</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a0474cba74b039aa45fe9412c1de86bed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Freq</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a8bab2775fa423836b05f407d2f8bf6e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Code</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a068e76e6034d06450385bc98b69ea53e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Dad</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>ad76da95d6074d29bacb152bc46d189d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Len</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a4490e644c06783cc69ad87c121e39e94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>max_insert_length</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a0aa718c141a5a98e84262fd9fc042dbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>put_byte</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a662a0774c0de95a9f1309019d52e914b</anchor>
      <arglist>(s, c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_LOOKAHEAD</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>aab627bcc7d38e658a93d395c50180cb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_DIST</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a1bac515806be1e59d7306b4126b0cca4</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>d_code</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a578b4336ef92d19278e0a0d95699def5</anchor>
      <arglist>(dist)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tr_tally_lit</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>af3b11322da0fb4ec60a5ccc28e2554df</anchor>
      <arglist>(s, c, flush)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_tr_tally_dist</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a68f55cdd396ad603d9f0b01afdbdf592</anchor>
      <arglist>(s, distance, length, flush)</arglist>
    </member>
    <member kind="typedef">
      <type>struct ct_data_s</type>
      <name>ct_data</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a0a4b5df3b2254c627b8a59e038963118</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct static_tree_desc_s</type>
      <name>static_tree_desc</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a905910aabf6aabbe79ff0832e4a664cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tree_desc_s</type>
      <name>tree_desc</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>ae978a66d78f0d68f945f48951f9151ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ush</type>
      <name>Pos</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a80a8610ae675eaeead82013bacde10a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Pos FAR</type>
      <name>Posf</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a7fc422ecb86afdd5f330dad6f7c1136b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned</type>
      <name>IPos</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>ada696b787a46d01634b6365afcec1a9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct internal_state</type>
      <name>deflate_state</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>aeaabc84f4830ec10deb9b64bd719617a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void _tr_init</type>
      <name>OF</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a7421434e97aacf6ea4ddd238d18bf9c8</anchor>
      <arglist>((deflate_state *s))</arglist>
    </member>
    <member kind="function">
      <type>int _tr_tally</type>
      <name>OF</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>ad2b29e535c13f6bfd762db8d1dd3bb3f</anchor>
      <arglist>((deflate_state *s, unsigned dist, unsigned lc))</arglist>
    </member>
    <member kind="function">
      <type>void _tr_flush_block</type>
      <name>OF</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>af8aa63aa73ba838890d15dddb92e2360</anchor>
      <arglist>((deflate_state *s, charf *buf, ulg stored_len, int eof))</arglist>
    </member>
    <member kind="variable">
      <type>uch</type>
      <name>_length_code</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>a034daec5591675c08dd7dde5a753a8da</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>uch</type>
      <name>_dist_code</name>
      <anchorfile>deflate_8h.html</anchorfile>
      <anchor>af81d7b42325e3ddbcf2cca9436d0b310</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp_8c</filename>
    <includes id="tcomp_8h" name="tcomp.h" local="yes" imported="no">tcomp.h</includes>
  </compound>
  <compound kind="file">
    <name>tcomp.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
  </compound>
  <compound kind="file">
    <name>tcomp_buffer.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__buffer_8c</filename>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <class kind="struct">tcomp_buffer_s</class>
    <member kind="define">
      <type>#define</type>
      <name>PRIME_1</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>ac795fa13b7b06403d272208285faa4c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIME_2</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>af979ca8ff762895d3699e3f91a7a7ac8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_buffer_s</type>
      <name>tcomp_buffer_t</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a00631535fe1bb8a1c3e4bcea429462ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_buffer_handle_t *</type>
      <name>tcomp_buffer_create</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>ade874ca0fc008d19c557492cd92aa33d</anchor>
      <arglist>(const void *data, tsk_size_t len)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_buffer_handle_t *</type>
      <name>tcomp_buffer_create_null</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>aa5cfdc24ecf030dd0ffb5f07289de0e5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_equals</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>ac6aa7f6b24a791e898c1fc616c345ab0</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle1, const tcomp_buffer_handle_t *handle2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_startsWith</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a2142e972cc4cb5f534b43b72ee733513</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle1, const tcomp_buffer_handle_t *handle2)</arglist>
    </member>
    <member kind="function">
      <type>const uint8_t *</type>
      <name>tcomp_buffer_getReadOnlyBufferAtPos</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a9fba8be7ab9dc38279c3b14254d325f1</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle, tsk_size_t position)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>tcomp_buffer_getBufferAtPos</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a1757d7fa138bae185bd36fa6ce56b8eb</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle, tsk_size_t position)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_buffer_getSize</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a959ada505a0d1b73a3b020cb8bbe01d6</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_buffer_getRemainingBits</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>ae869eb4293f721e25f47c1be0a037713</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>tcomp_buffer_readBytes</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>ac047c83dc6a29bdf1e7e9c60b4c0d280</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t length)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_buffer_readLsbToMsb</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a0b432914f2af2cf3d1d04b32989536c8</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t length)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_buffer_readMsbToLsb</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>aaad8d6dcaf4c6ffa957b038bca55a5a5</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_discardBits</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>aa1eb82e5dc1dcf5132ead711f36d5b7d</anchor>
      <arglist>(tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_discardLastBytes</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a3776db46a6db1354e8f4d005ee29df55</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, uint16_t count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_allocBuff</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a1e73d9b8ed8fda1f167d4d951e1e8f26</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_referenceBuff</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>abad7889e4712317b5e8bfa3fbd8c3a29</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, uint8_t *externalBuff, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_appendBuff</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a019e73ec35a3d0a5cb0d3dadf8f62df6</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_removeBuff</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a35a7ef87468f1df6edf45476b01c2a09</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t pos, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_freeBuff</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a6784139de22f5733384ea1fa2be25a74</anchor>
      <arglist>(tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t *</type>
      <name>tcomp_buffer_getIndexBytes</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a91b80cc3c954d9aa5296683dbd66993e</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t *</type>
      <name>tcomp_buffer_getIndexBits</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a0c7e69c96f0d8ef6512c04a4e7ca376b</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>tcomp_buffer_getP_BIT</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a791d3f1c107a65c2fde0ad782315a5a6</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>uint64_t</type>
      <name>tcomp_buffer_createHash</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a9a02469eaa630285bb3482abceadb8ec</anchor>
      <arglist>(const void *data, tsk_size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_nprint</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>ae530f6d538bfcdbc73dd49d5bff31135</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_reset</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a885d54447fb62c4223b212f386d304e2</anchor>
      <arglist>(tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_buffer_def_t</name>
      <anchorfile>tcomp__buffer_8c.html</anchorfile>
      <anchor>a2c49d9356221b178839dc2892a3da27d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_buffer.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__buffer_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_P_BIT_MSB_TO_LSB</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>afb63054a518d87c6fffcfd9b7c89b5c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_P_BIT_LSB_TO_MSB</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a7b99c2f2efb0bd844d6a8ab097a84f03</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_buffer_getReadOnlyBuffer</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>aa5e27561b89a1c2692891f1a6ffd8f1e</anchor>
      <arglist>(buffer)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_buffer_getBuffer</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ad7f83692c850f186a3d1a8bcfb54b9a5</anchor>
      <arglist>(handle)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_buffer_print</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a59d7a079dafc1a5f7383653f0215d5e3</anchor>
      <arglist>(handle)</arglist>
    </member>
    <member kind="typedef">
      <type>void</type>
      <name>tcomp_buffer_handle_t</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>aeb155cac21d622093c9ea495b9b8e144</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_buffer_handle_t *</type>
      <name>tcomp_buffer_create</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ade874ca0fc008d19c557492cd92aa33d</anchor>
      <arglist>(const void *data, tsk_size_t len)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_buffer_handle_t *</type>
      <name>tcomp_buffer_create_null</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>aa5cfdc24ecf030dd0ffb5f07289de0e5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_equals</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ac6aa7f6b24a791e898c1fc616c345ab0</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle1, const tcomp_buffer_handle_t *handle2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_startsWith</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a2142e972cc4cb5f534b43b72ee733513</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle1, const tcomp_buffer_handle_t *handle2)</arglist>
    </member>
    <member kind="function">
      <type>const uint8_t *</type>
      <name>tcomp_buffer_getReadOnlyBufferAtPos</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a9fba8be7ab9dc38279c3b14254d325f1</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle, tsk_size_t position)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API uint8_t *</type>
      <name>tcomp_buffer_getBufferAtPos</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ac35fd2027e83860b72cf70126d5c5365</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle, tsk_size_t position)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API tsk_size_t</type>
      <name>tcomp_buffer_getSize</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a808b0666fcc1e51130d596face2e6c4a</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_buffer_getRemainingBits</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ae869eb4293f721e25f47c1be0a037713</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>tcomp_buffer_readBytes</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a46837a573c6da78391d789121d735c1e</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_buffer_readLsbToMsb</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a0b432914f2af2cf3d1d04b32989536c8</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t length)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_buffer_readMsbToLsb</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>aaad8d6dcaf4c6ffa957b038bca55a5a5</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_discardBits</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>aa1eb82e5dc1dcf5132ead711f36d5b7d</anchor>
      <arglist>(tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_discardLastBytes</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a3776db46a6db1354e8f4d005ee29df55</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, uint16_t count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_allocBuff</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a1e73d9b8ed8fda1f167d4d951e1e8f26</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_referenceBuff</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>abad7889e4712317b5e8bfa3fbd8c3a29</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, uint8_t *externalBuff, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_appendBuff</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a019e73ec35a3d0a5cb0d3dadf8f62df6</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, const void *data, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_buffer_removeBuff</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a35a7ef87468f1df6edf45476b01c2a09</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t pos, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_freeBuff</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a6784139de22f5733384ea1fa2be25a74</anchor>
      <arglist>(tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t *</type>
      <name>tcomp_buffer_getIndexBytes</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a91b80cc3c954d9aa5296683dbd66993e</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t *</type>
      <name>tcomp_buffer_getIndexBits</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a0c7e69c96f0d8ef6512c04a4e7ca376b</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>tcomp_buffer_getP_BIT</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a791d3f1c107a65c2fde0ad782315a5a6</anchor>
      <arglist>(const tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>uint64_t</type>
      <name>tcomp_buffer_createHash</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a9a02469eaa630285bb3482abceadb8ec</anchor>
      <arglist>(const void *data, tsk_size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_nprint</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ae530f6d538bfcdbc73dd49d5bff31135</anchor>
      <arglist>(tcomp_buffer_handle_t *handle, tsk_size_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_buffer_reset</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>a885d54447fb62c4223b212f386d304e2</anchor>
      <arglist>(tcomp_buffer_handle_t *handle)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_buffer_def_t</name>
      <anchorfile>tcomp__buffer_8h.html</anchorfile>
      <anchor>ab999d1d75af8f413a39fc9f6a7bc6eb5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compartment.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compartment_8c</filename>
    <includes id="tcomp__compartment_8h" name="tcomp_compartment.h" local="yes" imported="no">tcomp_compartment.h</includes>
    <member kind="function">
      <type>tcomp_compartment_t *</type>
      <name>tcomp_compartment_create</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a6b54e7390394503148bea84d5e9cd52e</anchor>
      <arglist>(uint64_t id, uint16_t sigCompParameters)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_setRemoteParams</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a1e68190c2c83dba80461cbc81ec6463a</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_params_t *lpParams)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_setReqFeedback</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a9dd145ec13b7edfd3c28caf08971adad</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_buffer_handle_t *feedback)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_setRetFeedback</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>ad1011202565be1ddb9a2f65fbf3da366</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_buffer_handle_t *feedback)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_clearStates</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a0b9915ceb760196e51bc0ffa1f33ed48</anchor>
      <arglist>(tcomp_compartment_t *compartment)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeStateByPriority</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a00e53894db779de7ee81bd956b56e481</anchor>
      <arglist>(tcomp_compartment_t *compartment)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeState</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a4e1a157aaadda59389637c197407e918</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeStates</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a7e1a55b8673488be557da7f4b03c81c2</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_tempstate_to_free_t **tempStates, uint8_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_addState</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a8dadab6275e5e795cc5113411ef8082e</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_compartment_findState</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>ab72144795fc6997fcf2103b893ea17c8</anchor>
      <arglist>(tcomp_compartment_t *compartment, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeGhostState</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a2afc3585b73665d6dddf93e9889b19e1</anchor>
      <arglist>(tcomp_compartment_t *compartment)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_addNack</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a5c182d465dd341481e4b74ee77acbc9d</anchor>
      <arglist>(tcomp_compartment_t *compartment, const uint8_t nackId[TSK_SHA1_DIGEST_SIZE])</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_compartment_hasNack</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>a17e4a4d6725f159e090c5b0a861146ae</anchor>
      <arglist>(tcomp_compartment_t *compartment, const tcomp_buffer_handle_t *nackId)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_compartment_def_t</name>
      <anchorfile>tcomp__compartment_8c.html</anchorfile>
      <anchor>af774aca2c83810a86731d25cf82bbd07</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compartment.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compartment_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__types_8h" name="tcomp_types.h" local="yes" imported="no">tcomp_types.h</includes>
    <includes id="tcomp__params_8h" name="tcomp_params.h" local="yes" imported="no">tcomp_params.h</includes>
    <includes id="tcomp__compressordata_8h" name="tcomp_compressordata.h" local="yes" imported="no">tcomp_compressordata.h</includes>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <class kind="struct">tcomp_compartment_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_compartment_s</type>
      <name>tcomp_compartment_t</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a8f70456ed6f078247d22edbbcfe41adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_compartment_t *</type>
      <name>tcomp_compartment_create</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a6b54e7390394503148bea84d5e9cd52e</anchor>
      <arglist>(uint64_t id, uint16_t sigCompParameters)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_setRemoteParams</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a1e68190c2c83dba80461cbc81ec6463a</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_params_t *lpParams)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_setReqFeedback</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a9dd145ec13b7edfd3c28caf08971adad</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_buffer_handle_t *feedback)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_setRetFeedback</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>ad1011202565be1ddb9a2f65fbf3da366</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_buffer_handle_t *feedback)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_clearStates</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a0b9915ceb760196e51bc0ffa1f33ed48</anchor>
      <arglist>(tcomp_compartment_t *compartment)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeStateByPriority</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a00e53894db779de7ee81bd956b56e481</anchor>
      <arglist>(tcomp_compartment_t *compartment)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeState</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a4e1a157aaadda59389637c197407e918</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeStates</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a7e1a55b8673488be557da7f4b03c81c2</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_tempstate_to_free_t **tempStates, uint8_t size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_addState</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a8dadab6275e5e795cc5113411ef8082e</anchor>
      <arglist>(tcomp_compartment_t *compartment, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_compartment_findState</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>ab72144795fc6997fcf2103b893ea17c8</anchor>
      <arglist>(tcomp_compartment_t *compartment, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_freeGhostState</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a2afc3585b73665d6dddf93e9889b19e1</anchor>
      <arglist>(tcomp_compartment_t *compartment)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_compartment_addNack</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a5c182d465dd341481e4b74ee77acbc9d</anchor>
      <arglist>(tcomp_compartment_t *compartment, const uint8_t nackId[TSK_SHA1_DIGEST_SIZE])</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_compartment_hasNack</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a17e4a4d6725f159e090c5b0a861146ae</anchor>
      <arglist>(tcomp_compartment_t *compartment, const tcomp_buffer_handle_t *nackId)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_compartment_def_t</name>
      <anchorfile>tcomp__compartment_8h.html</anchorfile>
      <anchor>a7fd4a96decbd8c096b50785b5b74d19d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressor.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressor_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__compartment_8h" name="tcomp_compartment.h" local="yes" imported="no">tcomp_compartment.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_COMPRESSOR_COMPRESS_F</name>
      <anchorfile>tcomp__compressor_8h.html</anchorfile>
      <anchor>ab529ce900a984f595f961d6fb2b9e565</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="typedef">
      <type>tsk_bool_t(*</type>
      <name>tcomp_compressor_compress_f</name>
      <anchorfile>tcomp__compressor_8h.html</anchorfile>
      <anchor>a89e8cf822600cb829f045cc6da482710</anchor>
      <arglist>)(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressor_deflate.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressor__deflate_8c</filename>
    <includes id="tcomp__compressor__deflate_8h" name="tcomp_compressor_deflate.h" local="yes" imported="no">tcomp_compressor_deflate.h</includes>
    <includes id="tcomp__deflatedata_8h" name="tcomp_deflatedata.h" local="yes" imported="no">tcomp_deflatedata.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_MIN</name>
      <anchorfile>tcomp__compressor__deflate_8c.html</anchorfile>
      <anchor>ac287da0fb72036ee49a6a6d7f457cc79</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_OUTPUT_BUFFER_AT</name>
      <anchorfile>tcomp__compressor__deflate_8c.html</anchorfile>
      <anchor>a76f3d65a88b375c145f2f313adf6a449</anchor>
      <arglist>(position)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_compressor_deflate_compress</name>
      <anchorfile>tcomp__compressor__deflate_8c.html</anchorfile>
      <anchor>adefbb1a9f55fc924b6dda8ebc75292f0</anchor>
      <arglist>(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressor_deflate.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressor__deflate_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__compartment_8h" name="tcomp_compartment.h" local="yes" imported="no">tcomp_compartment.h</includes>
    <member kind="function">
      <type>TCOMP_BEGIN_DECLS tsk_bool_t</type>
      <name>tcomp_compressor_deflate_compress</name>
      <anchorfile>tcomp__compressor__deflate_8h.html</anchorfile>
      <anchor>a40af6c9124355c4ff307be664c331b62</anchor>
      <arglist>(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressor_dummy.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressor__dummy_8c</filename>
    <includes id="tcomp__compressor__dummy_8h" name="tcomp_compressor_dummy.h" local="yes" imported="no">tcomp_compressor_dummy.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>UNCOMPRESSED_BYTECODE_LENGTH</name>
      <anchorfile>tcomp__compressor__dummy_8c.html</anchorfile>
      <anchor>a7a8c0e6248ad14bc6615489014489f6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UNCOMPRESSED_BYTECODE_DESTINATION_CODE</name>
      <anchorfile>tcomp__compressor__dummy_8c.html</anchorfile>
      <anchor>a85cae11bb40fafaf6013352ec2d26f90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DUMMYCOMPRESSOR_UNCOMPRESSED_BYTECODE</name>
      <anchorfile>tcomp__compressor__dummy_8c.html</anchorfile>
      <anchor>a68c31c7fb503b833ce15873496d6d6b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_compressor_dummy_compress</name>
      <anchorfile>tcomp__compressor__dummy_8c.html</anchorfile>
      <anchor>aaf174210ca73b13b364f0c72a2b35f32</anchor>
      <arglist>(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressor_dummy.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressor__dummy_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__compartment_8h" name="tcomp_compartment.h" local="yes" imported="no">tcomp_compartment.h</includes>
    <member kind="function">
      <type>TCOMP_BEGIN_DECLS tsk_bool_t</type>
      <name>tcomp_compressor_dummy_compress</name>
      <anchorfile>tcomp__compressor__dummy_8h.html</anchorfile>
      <anchor>afe355c7530573a73b869c5378e116a36</anchor>
      <arglist>(tcomp_compartment_t *lpCompartment, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressordata.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressordata_8c</filename>
  </compound>
  <compound kind="file">
    <name>tcomp_compressordata.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressordata_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_DECLARE_COMPRESSORDATA</name>
      <anchorfile>tcomp__compressordata_8h.html</anchorfile>
      <anchor>a8df9e4af38727aa7790a1a646efd206c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>tcomp_xxx_freeGhostState</name>
      <anchorfile>tcomp__compressordata_8h.html</anchorfile>
      <anchor>a0881ddad615d5b5444cd8c8bae4efd76</anchor>
      <arglist>)(tcomp_compressordata_t *data)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>tcomp_xxx_ackGhost</name>
      <anchorfile>tcomp__compressordata_8h.html</anchorfile>
      <anchor>aa6c12669c69b66fc068f2e411b112a04</anchor>
      <arglist>)(tcomp_compressordata_t *data, const tcomp_buffer_handle_t *stateid)</arglist>
    </member>
    <member kind="variable">
      <type>TCOMP_BEGIN_DECLS typedef void</type>
      <name>tcomp_compressordata_t</name>
      <anchorfile>tcomp__compressordata_8h.html</anchorfile>
      <anchor>ac8063895d2b39f7377af53f087437a5b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressordisp.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressordisp_8c</filename>
    <includes id="tcomp__compressordisp_8h" name="tcomp_compressordisp.h" local="yes" imported="no">tcomp_compressordisp.h</includes>
    <includes id="tcomp__compressor__dummy_8h" name="tcomp_compressor_dummy.h" local="yes" imported="no">tcomp_compressor_dummy.h</includes>
    <includes id="tcomp__compressor__deflate_8h" name="tcomp_compressor_deflate.h" local="yes" imported="no">tcomp_compressor_deflate.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_NACK_SUPPORTED</name>
      <anchorfile>tcomp__compressordisp_8c.html</anchorfile>
      <anchor>a38e55ba6ae6fbbe1c82e843a7430ed4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_compressordisp_t *</type>
      <name>tcomp_compressordisp_create</name>
      <anchorfile>tcomp__compressordisp_8c.html</anchorfile>
      <anchor>a65a06e5aa5ac1f93484be2543c5fd655</anchor>
      <arglist>(const tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_compressordisp_compress</name>
      <anchorfile>tcomp__compressordisp_8c.html</anchorfile>
      <anchor>aec43191d86a10978161c029f18e5f981</anchor>
      <arglist>(tcomp_compressordisp_t *dispatcher, uint64_t compartmentId, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_compressordisp_addCompressor</name>
      <anchorfile>tcomp__compressordisp_8c.html</anchorfile>
      <anchor>aa29cb2515cfea50662f3b548d4754738</anchor>
      <arglist>(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_compressordisp_removeCompressor</name>
      <anchorfile>tcomp__compressordisp_8c.html</anchorfile>
      <anchor>ace1899914be8d3bb6795ab5d1a99cb3e</anchor>
      <arglist>(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_compressordisp_def_t</name>
      <anchorfile>tcomp__compressordisp_8c.html</anchorfile>
      <anchor>aad9a1d5ae9edb5d8fe3eeeabe92220c9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_compressordisp.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__compressordisp_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__statehandler_8h" name="tcomp_statehandler.h" local="yes" imported="no">tcomp_statehandler.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <includes id="tcomp__types_8h" name="tcomp_types.h" local="yes" imported="no">tcomp_types.h</includes>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <includes id="tcomp__compressor_8h" name="tcomp_compressor.h" local="yes" imported="no">tcomp_compressor.h</includes>
    <class kind="struct">tcomp_compressordisp_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_MAX_COMPRESSORS</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>a8d2e7bb50fd6bbdc19ed27a1e846fca5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_compressordisp_s</type>
      <name>tcomp_compressordisp_t</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>a23efaf69247656c591bee204825088dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_compressordisp_t *</type>
      <name>tcomp_compressordisp_create</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>a65a06e5aa5ac1f93484be2543c5fd655</anchor>
      <arglist>(const tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_compressordisp_compress</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>aec43191d86a10978161c029f18e5f981</anchor>
      <arglist>(tcomp_compressordisp_t *dispatcher, uint64_t compartmentId, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t *output_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_compressordisp_addCompressor</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>aa29cb2515cfea50662f3b548d4754738</anchor>
      <arglist>(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_compressordisp_removeCompressor</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>ace1899914be8d3bb6795ab5d1a99cb3e</anchor>
      <arglist>(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_compressordisp_def_t</name>
      <anchorfile>tcomp__compressordisp_8h.html</anchorfile>
      <anchor>a92cc61440724991756c773342fccd17e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_decompressordisp.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__decompressordisp_8c</filename>
    <includes id="tcomp__decompressordisp_8h" name="tcomp_decompressordisp.h" local="yes" imported="no">tcomp_decompressordisp.h</includes>
    <includes id="tcomp__message_8h" name="tcomp_message.h" local="yes" imported="no">tcomp_message.h</includes>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_MAX_STREAM_BUFFER_SIZE</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>aa4a8fdb851b4620b67a1d52d65441ed1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_NACK_SUPPORTED</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>ad278be58a5d53380479e7e53c1ec01bf</anchor>
      <arglist>(dispatcher)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_stream_buffer_t *</type>
      <name>tcomp_stream_buffer_create</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a998e223ef468fc09a9f8613c5d5d0854</anchor>
      <arglist>(uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_decompressordisp_t *</type>
      <name>tcomp_decompressordisp_create</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a22004ec43206b3803b3f6d978b058f0a</anchor>
      <arglist>(const tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_decompress</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a74fb7a3ff671e4576c0fb818b7760dc0</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, const void *input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_getNextMessage</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>aebe1904896d38c815f464353e6f5d8a0</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_internalDecompress</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a7194a6ba35ad1935b76fce25db19ec28</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, const void *input_ptr, const tsk_size_t input_size, tcomp_result_t **lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_appendStream</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a91fd6f2e49b3cbe6fb2a574c20448d79</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, const void *input_ptr, tsk_size_t input_size, uint64_t streamId)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_getNextStreamMsg</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a1d16a0b0eb86e5538cd37aaf8214aa04</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, uint64_t streamId, uint16_t *discard_count, tsk_size_t *size)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_decompressordisp_def_t</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a1782b0cf481d122093472b147fb944b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_stream_buffer_def_t</name>
      <anchorfile>tcomp__decompressordisp_8c.html</anchorfile>
      <anchor>a94e7516c75f7a0076067b7d2640cccc3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_decompressordisp.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__decompressordisp_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__statehandler_8h" name="tcomp_statehandler.h" local="yes" imported="no">tcomp_statehandler.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <includes id="tcomp__types_8h" name="tcomp_types.h" local="yes" imported="no">tcomp_types.h</includes>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <class kind="struct">tcomp_stream_buffer_s</class>
    <class kind="struct">tcomp_decompressordisp_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_stream_buffer_s</type>
      <name>tcomp_stream_buffer_t</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a657b520236f10cc7eb86124b11f0a768</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_decompressordisp_s</type>
      <name>tcomp_decompressordisp_t</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a02011c3f486e9780d2fb6191279631d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_decompressordisp_t *</type>
      <name>tcomp_decompressordisp_create</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a22004ec43206b3803b3f6d978b058f0a</anchor>
      <arglist>(const tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_decompress</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a74fb7a3ff671e4576c0fb818b7760dc0</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, const void *input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_getNextMessage</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>aebe1904896d38c815f464353e6f5d8a0</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_internalDecompress</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a7194a6ba35ad1935b76fce25db19ec28</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, const void *input_ptr, const tsk_size_t input_size, tcomp_result_t **lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_appendStream</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a91fd6f2e49b3cbe6fb2a574c20448d79</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, const void *input_ptr, tsk_size_t input_size, uint64_t streamId)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_decompressordisp_getNextStreamMsg</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a1d16a0b0eb86e5538cd37aaf8214aa04</anchor>
      <arglist>(tcomp_decompressordisp_t *dispatcher, uint64_t streamId, uint16_t *discard_count, tsk_size_t *size)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_stream_buffer_def_t</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a74fb91763a99373e6338f2e473f0637c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_decompressordisp_def_t</name>
      <anchorfile>tcomp__decompressordisp_8h.html</anchorfile>
      <anchor>a263d9f0ed06601f9458d841e0abf47b4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_deflatedata.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__deflatedata_8c</filename>
    <includes id="tcomp__deflatedata_8h" name="tcomp_deflatedata.h" local="yes" imported="no">tcomp_deflatedata.h</includes>
    <member kind="function">
      <type>tcomp_deflatedata_t *</type>
      <name>tcomp_deflatedata_create_2</name>
      <anchorfile>tcomp__deflatedata_8c.html</anchorfile>
      <anchor>acc107c5868ee319ab31775516a7b1008</anchor>
      <arglist>(tsk_bool_t isStream, int z_level, int z_windowBits)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_deflatedata_t *</type>
      <name>tcomp_deflatedata_create</name>
      <anchorfile>tcomp__deflatedata_8c.html</anchorfile>
      <anchor>a613938dc503712a88a29682f1b3332d3</anchor>
      <arglist>(tsk_bool_t isStream)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_isStateful</name>
      <anchorfile>tcomp__deflatedata_8c.html</anchorfile>
      <anchor>a6f6fa6262b170743244a1905866e7a49</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_deflatedata_def_t</name>
      <anchorfile>tcomp__deflatedata_8c.html</anchorfile>
      <anchor>ae43e14c938ca02c0945e781d71943e6c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_deflatedata.ghost.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__deflatedata_8ghost_8c</filename>
    <includes id="tcomp__deflatedata_8h" name="tcomp_deflatedata.h" local="yes" imported="no">tcomp_deflatedata.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_CB_START_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>af61dd76b0f070596ff477fa5fb11ff95</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_DMS_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>aa0e73f1c8b5cc6579fe838c8f896bfe1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_0x0005_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a1361b59108a666ce5794c1fdcbf69cdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_CB_END_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a438bef24bb5c09d4cf891a28e973378a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_HASH_LEN_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>aaefce17480e56589a8bd181a4073cb0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_SMS_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>ad79d54e5e2d4e9e37b4d5ba2302edc0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_CPB_DMS_SMS_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>ad8b17d69112d417446dee27863e30350</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_VERSION_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a8909b9553131b824cc647ae858eb0da0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_BYTECODE1_SIZE</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>adb3ba3e24b34c2226b1a867158733d68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_STATE_ADDRESS</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>aa3cb78c7b5d91c728d09c65fe9dd4d35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_STATE_INSTRUCTION</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>add6e53f528b0b7d81b7dbc774e173609</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_STATE_MIN_ACCESS_LEN</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a97b586450efa48aa8cd9f16a2d599b37</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_STATE_RETENTION_PRIORITY</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a4dfae63f9636d012a8b45851e8a971e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOSTVALUE_AT</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a2cdf0b5cd6985cf45e788e85f640386a</anchor>
      <arglist>(position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_DICT_CODE_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>aae1d230748b69f7026584ec75e890296</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_DICT_WORDS_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a7393f41cb8c02bfa84f55105cca5954c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_FIXME2_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a4dbfaf358c10a950c233f8c06cc68cc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_DEFLATE_BYTECODE_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a56dbe0e0d362dd7fbdffea9758c19887</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOST_INPUT_INDEX</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>addd836add0b7388c1c95dcf2dcd8d07e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GHOSTVALUE_AT</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a2cdf0b5cd6985cf45e788e85f640386a</anchor>
      <arglist>(position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZBUFF_LEN</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a587b1133cca8e91689f275380d6310a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_createGhost</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a685b9a7b997d9aeca3174530211711c7</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, uint16_t state_length, tcomp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_ackGhost</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>aadc5f39e93866e0cb2cd3b29bb99d12e</anchor>
      <arglist>(tcomp_compressordata_t *data, const tcomp_buffer_handle_t *stateid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_updateGhost</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a0c7dceae6a0f82652892d7d5c53da363</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, const uint8_t *input_ptr, tsk_size_t input_size)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t *</type>
      <name>tcomp_deflatedata_getGhostCopyOffset</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>acd4ef201aad5c16cc01703429e152509</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_freeGhostState</name>
      <anchorfile>tcomp__deflatedata_8ghost_8c.html</anchorfile>
      <anchor>a5706f951dba8f9b03a2bddfa24be5264</anchor>
      <arglist>(tcomp_compressordata_t *data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_deflatedata.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__deflatedata_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__params_8h" name="tcomp_params.h" local="yes" imported="no">tcomp_params.h</includes>
    <includes id="tcomp__state_8h" name="tcomp_state.h" local="yes" imported="no">tcomp_state.h</includes>
    <includes id="tcomp__compressordata_8h" name="tcomp_compressordata.h" local="yes" imported="no">tcomp_compressordata.h</includes>
    <includes id="zlib_8h" name="zlib.h" local="yes" imported="no">zlib.h</includes>
    <class kind="struct">tcomp_deflateStream_s</class>
    <class kind="struct">tcomp_deflatedata_s</class>
    <member kind="define">
      <type>#define</type>
      <name>USE_DICTS_FOR_COMPRESSION</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>adc85d4d5b991939e39bf3fcd2b41e0e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_DEFAULT_WINDOW_BITS</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a3ffc2b8c8034c6e80474b0ad988a4ab6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_DECOMPRESSION_PTR_INDEX</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>aecf738b0f582de9a6f93a0d807750040</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_UDVM_CIRCULAR_START_INDEX</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a297a076f50ac3d10649b26b083b6a4d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_SIP_DICT_ONLY</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a8a677017c1bf51b65dc49b541c46242c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_PRES_DICT_ONLY</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>ab7869b22a284727e3bafa890de3f08f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_SIP_PRES_DICTS</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a56e78baa3f167e1222521a92b5411470</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_NO_DICT</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a7b3a9bb16609d4529913a50ccd1e8117</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_FIXME_DICT</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a70b3741fac20f597294e67ec88c1ffd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_BYTECODE_DESTINATION_START</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>ac519f027665fcff17a6adcf26ce13e87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_BYTECODE_DESTINATION_CODE</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a1bfccabe5485ea3856fb661a360fcdcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATE_BYTECODE_LEN</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a5301d24aea35ac0ae5b82c8b209e7c96</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFLATEDATA_DEFLATE_BYTECODE</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a8ad70b4284af8f191f3e033784726a3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_deflateStream_s</type>
      <name>tcomp_deflateStream_t</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>afadd5ec8fe340558dde8db99700bcbab</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_deflatedata_s</type>
      <name>tcomp_deflatedata_t</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a51603f0230b2555a35c740b64d95b7d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflateStream_end</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a1bdeb3020a7e79ac2e0752c3c9048de9</anchor>
      <arglist>(tcomp_deflateStream_t *stream)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflateStream_copy</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a807b1c81bc1f376a5464a6e8e13f766d</anchor>
      <arglist>(tcomp_deflateStream_t *stream, tcomp_deflateStream_t *source)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_deflatedata_t *</type>
      <name>tcomp_deflatedata_create</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a613938dc503712a88a29682f1b3332d3</anchor>
      <arglist>(tsk_bool_t isStream)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_freeGhostState</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a84abeb10334414bf025282d103f56c3e</anchor>
      <arglist>(tcomp_compressordata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_ackGhost</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a9ee70414f50b4c064e5502da176c7396</anchor>
      <arglist>(tcomp_compressordata_t *deflatedata, const tcomp_buffer_handle_t *stateid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_createGhost</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a416082cf8161cfd26293df7273300e7e</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, uint16_t state_len, tcomp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_updateGhost</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a0c7dceae6a0f82652892d7d5c53da363</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, const uint8_t *input_ptr, tsk_size_t input_size)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t *</type>
      <name>tcomp_deflatedata_getGhostCopyOffset</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>acd4ef201aad5c16cc01703429e152509</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zReset</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a87d68a52cbaa9a6500d00dbfad52cf16</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zCompress</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a38a642737f96d3f6bb81eb8025136d25</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, const void *in, tsk_size_t inLen, void *out, tsk_size_t *outLen, tsk_bool_t *stateChanged)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_deflatedata_zGetWindowBits</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>ac1db80d4be4336cd73f03ad7d0de84ec</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_zSetWindowBits</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a0ad0c57a166fbf0700f2b4a0203b6581</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, int windowSize)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_isStateful</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a6f6fa6262b170743244a1905866e7a49</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zInit</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a279cfe52515a1de61335c1c2b5f3a64a</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zUnInit</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a20c3dbe17e832df89b6ed65a49a56ef4</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_deflatedata_def_t</name>
      <anchorfile>tcomp__deflatedata_8h.html</anchorfile>
      <anchor>a99d44b87f2fddf3a15186dd64b7da860</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_deflatedata.zlib.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__deflatedata_8zlib_8c</filename>
    <includes id="tcomp__deflatedata_8h" name="tcomp_deflatedata.h" local="yes" imported="no">tcomp_deflatedata.h</includes>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflateStream_end</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a1bdeb3020a7e79ac2e0752c3c9048de9</anchor>
      <arglist>(tcomp_deflateStream_t *stream)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflateStream_copy</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a807b1c81bc1f376a5464a6e8e13f766d</anchor>
      <arglist>(tcomp_deflateStream_t *stream, tcomp_deflateStream_t *source)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zInit</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a279cfe52515a1de61335c1c2b5f3a64a</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zUnInit</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a20c3dbe17e832df89b6ed65a49a56ef4</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zReset</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a87d68a52cbaa9a6500d00dbfad52cf16</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_deflatedata_zCompress</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a38a642737f96d3f6bb81eb8025136d25</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, const void *in, tsk_size_t inLen, void *out, tsk_size_t *outLen, tsk_bool_t *stateChanged)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_deflatedata_zGetWindowBits</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>ac1db80d4be4336cd73f03ad7d0de84ec</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_deflatedata_zSetWindowBits</name>
      <anchorfile>tcomp__deflatedata_8zlib_8c.html</anchorfile>
      <anchor>a0ad0c57a166fbf0700f2b4a0203b6581</anchor>
      <arglist>(tcomp_deflatedata_t *deflatedata, int windowSize)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_dicts.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__dicts_8c</filename>
    <includes id="tcomp__dicts_8h" name="tcomp_dicts.h" local="yes" imported="no">tcomp_dicts.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <includes id="tcomp__rfc3485__dictionary__sip_8h" name="tcomp_rfc3485_dictionary_sip.h" local="yes" imported="no">tcomp_rfc3485_dictionary_sip.h</includes>
    <includes id="tcomp__rfc5112__dictionary__presence_8h" name="tcomp_rfc5112_dictionary_presence.h" local="yes" imported="no">tcomp_rfc5112_dictionary_presence.h</includes>
    <member kind="function">
      <type>tcomp_dictionary_t *</type>
      <name>tcomp_dicts_create_presence_dict</name>
      <anchorfile>tcomp__dicts_8c.html</anchorfile>
      <anchor>a252a72a1fd88d9e77ca46bdbe214be95</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tcomp_dictionary_t *</type>
      <name>tcomp_dicts_create_sip_dict</name>
      <anchorfile>tcomp__dicts_8c.html</anchorfile>
      <anchor>a1810e66603f634d79120eb9da1400c05</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_dicts.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__dicts_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__state_8h" name="tcomp_state.h" local="yes" imported="no">tcomp_state.h</includes>
    <member kind="function">
      <type>TCOMP_BEGIN_DECLS tcomp_dictionary_t *</type>
      <name>tcomp_dicts_create_presence_dict</name>
      <anchorfile>tcomp__dicts_8h.html</anchorfile>
      <anchor>af8882efbf74466dba5054791cdbefc8b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tcomp_dictionary_t *</type>
      <name>tcomp_dicts_create_sip_dict</name>
      <anchorfile>tcomp__dicts_8h.html</anchorfile>
      <anchor>a1810e66603f634d79120eb9da1400c05</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_headers_index.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__headers__index_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_UDVM_MEMORY_SIZE_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>abf894a2433372417a5c7a0efdfbd1480</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_CYCLES_PER_BIT_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>a38cd1a41feaf84267fd1465126c48037</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_SIGCOMP_VERSION_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>af7b0bf249f78db457bc9533990706a6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_PARTIAL_STATE_ID_LENGTH_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>a1f45d23641e4a86d78fbf1a915c62bf1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_STATE_LENGTH_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>ab110c964556fb631d3bbf083ff646cd1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_RESERVED_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>abdec1536990849d8cc41c21ca7b02304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_BYTE_COPY_LEFT_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>ab63df45b63d7806ffd038bf04b9f6d17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_BYTE_COPY_RIGHT_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>af5b2a6c77994d109361982281ad0291d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_INPUT_BIT_ORDER_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>a6de42eba0b2b39a807587b94380558e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_STACK_LOCATION_INDEX</name>
      <anchorfile>tcomp__headers__index_8h.html</anchorfile>
      <anchor>a81e96e7c20757d40ddfad35eeef1a64f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_instructions.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__instructions_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__DECOMPRESSION_FAILURE</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a87890ef162bb7da42999de8b1a72fca3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__AND</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a27a17cdb472ce8e876387431f73ca656</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__OR</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a00a24090e81502dbb3d4ee509d8302e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__NOT</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>afdcaf9184d7aa4b98de6bd10f199add5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__LSHIFT</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a64d3e419cb78210ede1d0b7bbd9623ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__RSHIFT</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a3ffa5903dceacc4f09715cb930e02e09</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__ADD</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ab8ae80ceba9ef28562ca3f2bffbea1a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__SUBTRACT</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a6db9d9fe9eed72dfdfce6a79810fac35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__MULTIPLY</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a1bcaa1fbb2b217cf397ba6d09c1582bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__DIVIDE</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a2b693d9c2ab15facf911738ca9bed972</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__REMAINDER</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a8e39b4c47a3d72e0893e1d493db88ca1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__SORT_ASCENDING</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ac907e45ab8fe6ff439df738d6cb684f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__SORT_DESCENDING</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ac7b53c7dcce4a218c7a2561d7f74c091</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__SHA_1</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>aeebaac7790287b4ca2c7c6437a3756af</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__LOAD</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a1c880ec11b24aaf59298dd70dc5d5f89</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__MULTILOAD</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a34380db329ac1d5d549d6eb30b08ee18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__PUSH</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a07241036c93c177f20013d2393e1671f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__POP</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a7a714f6c0bdbc646d4c922ed6652e2f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__COPY</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a8abff2a63e3a52f58de1f56a47299599</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__COPY_LITERAL</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a136de87e16c30224139398d073b5bb7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__COPY_OFFSET</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a5f3ebdfe6019b02af70be90a4a2775f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__MEMSET</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>afc29b053d19c8ae01b856027b5f81445</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__JUMP</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ae87e9b20f08e39fd00061ef631950d70</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__COMPARE</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>aeb39a13beb297b6bf0e68698fc365086</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__CALL</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a784926453fbb97c8c32eff977c373785</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__RETURN</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>af516a83772c6aa1bf49fe0f3333eebc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__SWITCH</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ad94373cd714f35ca82d5a0e1aeddd86b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__CRC</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>abc6c90cbd81d69f0ff19ab7eec62e22e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__INPUT_BYTES</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a35d4e22f76b18c463f0f1a3a47c36c65</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__INPUT_BITS</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a0d83ae2bad089ac7183a83796cb98606</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__INPUT_HUFFMAN</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a4d446dcafd6b3abf66e61cc3a16f94e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__STATE_ACCESS</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ac4bc989b264267efc3075e76b6d69fa1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__STATE_CREATE</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a3fe3316151b39751baa3beb780654763</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__STATE_FREE</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a0ca162ec138dc61d67d25ae029c18505</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__OUTPUT</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>ab85ecc3de9b346085c55964af528c2f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_INST__END_MESSAGE</name>
      <anchorfile>tcomp__instructions_8h.html</anchorfile>
      <anchor>a9d8a67afca5cd30b9141865525df3cc1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_manager.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__manager_8c</filename>
    <includes id="tcomp__manager_8h" name="tcomp_manager.h" local="yes" imported="no">tcomp_manager.h</includes>
    <includes id="tcomp__compressordisp_8h" name="tcomp_compressordisp.h" local="yes" imported="no">tcomp_compressordisp.h</includes>
    <includes id="tcomp__decompressordisp_8h" name="tcomp_decompressordisp.h" local="yes" imported="no">tcomp_decompressordisp.h</includes>
    <includes id="tcomp__statehandler_8h" name="tcomp_statehandler.h" local="yes" imported="no">tcomp_statehandler.h</includes>
    <class kind="struct">tcomp_manager_s</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_DMS</name>
      <anchorfile>tcomp__manager_8c.html</anchorfile>
      <anchor>a10acc378faf5282b218afbf5ab398074</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_SMS</name>
      <anchorfile>tcomp__manager_8c.html</anchorfile>
      <anchor>ae490c67aa3f2d2ea3e4febda1bceea4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_CPB</name>
      <anchorfile>tcomp__manager_8c.html</anchorfile>
      <anchor>a593b66191765907a760012dd7c1cfb90</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_manager_s</type>
      <name>tcomp_manager_t</name>
      <anchorfile>tcomp__manager_8c.html</anchorfile>
      <anchor>a2fdb922c98c0aa355479d03e8378ba61</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_manager_handle_t *</type>
      <name>tcomp_manager_create</name>
      <anchorfile>tcomp__manager_8c.html</anchorfile>
      <anchor>a0a40ece41efdc3a5a8594a1b62cea56a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_manager_compress</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga5760b64e23e067e209a18a8480688493</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t output_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_manager_decompress</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga821375d468c0baa3581a6d19ecb76c2f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_manager_getNextStreamMessage</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac075422d0661cad2559083e6a700b78f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_manager_provideCompartmentId</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gaaa9bcc86de2877a1fd8070154985d83f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_manager_closeCompartment</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gad7a281742f5bde5ce1f7a25071f38944</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setDecompression_Memory_Size</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac19567fc1105a2c6e6458354f95b61d1</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint32_t dms)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setState_Memory_Size</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gabc8ca6c18fe8574f2d87380c68d5b6a5</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint32_t sms)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setCycles_Per_Bit</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga8b5db5469830967d9fe3bedbe07425ef</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint8_t cpb)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setSigComp_Version</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gad98c7dc09af9202e928a4754fc30f257</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint8_t version)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_addCompressor</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga4da0272938736caa99f328aea5fd7b21</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_removeCompressor</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac8f21e909564d587f73e1402d07eb453</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_addSipSdpDictionary</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga8e18b881be5fcfb412fb71fdaeb82e58</anchor>
      <arglist>(tcomp_manager_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_addPresenceDictionary</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga6093219506324383c563ae8f4caece46</anchor>
      <arglist>(tcomp_manager_handle_t *handle)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_manager_def_t</name>
      <anchorfile>tcomp__manager_8c.html</anchorfile>
      <anchor>ae27284b8b16fc1ba5ee706bedf9ad7c6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_manager.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__manager_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <includes id="tcomp__compressor_8h" name="tcomp_compressor.h" local="yes" imported="no">tcomp_compressor.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_manager_compressUDP</name>
      <anchorfile>tcomp__manager_8h.html</anchorfile>
      <anchor>a570970dc506d989d79071a7c2f5e07c4</anchor>
      <arglist>(handle, compartmentId, compartmentIdSize, input_ptr, input_size, output_ptr, output_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_manager_compressTCP</name>
      <anchorfile>tcomp__manager_8h.html</anchorfile>
      <anchor>ab619ca0e78051260d92fabce070350b2</anchor>
      <arglist>(handle, compartmentId, compartmentIdSize, input_ptr, input_size, output_ptr, output_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_manager_compressSCTP</name>
      <anchorfile>tcomp__manager_8h.html</anchorfile>
      <anchor>a0036702a43866b46d1916c42ebfc1a81</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API tcomp_manager_handle_t *</type>
      <name>tcomp_manager_create</name>
      <anchorfile>tcomp__manager_8h.html</anchorfile>
      <anchor>a86df1206e8a33b741a2aca708449ad53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API tsk_size_t</type>
      <name>tcomp_manager_compress</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga5760b64e23e067e209a18a8480688493</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t output_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API tsk_size_t</type>
      <name>tcomp_manager_decompress</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga821375d468c0baa3581a6d19ecb76c2f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API tsk_size_t</type>
      <name>tcomp_manager_getNextStreamMessage</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac075422d0661cad2559083e6a700b78f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API void</type>
      <name>tcomp_manager_provideCompartmentId</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gaaa9bcc86de2877a1fd8070154985d83f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API void</type>
      <name>tcomp_manager_closeCompartment</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gad7a281742f5bde5ce1f7a25071f38944</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_setDecompression_Memory_Size</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac19567fc1105a2c6e6458354f95b61d1</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint32_t dms)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_setState_Memory_Size</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gabc8ca6c18fe8574f2d87380c68d5b6a5</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint32_t sms)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_setCycles_Per_Bit</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga8b5db5469830967d9fe3bedbe07425ef</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint8_t cpb)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_setSigComp_Version</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gad98c7dc09af9202e928a4754fc30f257</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint8_t version)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_addCompressor</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga4da0272938736caa99f328aea5fd7b21</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_removeCompressor</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac8f21e909564d587f73e1402d07eb453</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_addSipSdpDictionary</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga8e18b881be5fcfb412fb71fdaeb82e58</anchor>
      <arglist>(tcomp_manager_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API int</type>
      <name>tcomp_manager_addPresenceDictionary</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga6093219506324383c563ae8f4caece46</anchor>
      <arglist>(tcomp_manager_handle_t *handle)</arglist>
    </member>
    <member kind="variable">
      <type>TCOMP_BEGIN_DECLS typedef void</type>
      <name>tcomp_manager_handle_t</name>
      <anchorfile>tcomp__manager_8h.html</anchorfile>
      <anchor>aa64cf234c2c21da1dee31e1379b6927a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_manager_def_t</name>
      <anchorfile>tcomp__manager_8h.html</anchorfile>
      <anchor>a8560164a3e44ab1834bd769b469b99af</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_message.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__message_8c</filename>
    <includes id="tcomp__message_8h" name="tcomp_message.h" local="yes" imported="no">tcomp_message.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MIN_LEN</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>aa69f8dba9add1a52ce3cf7bcfafcfda0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HEADER_GET_LEN</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a4921bb648bbd9a43c57c0253aa1245aa</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HEADER_GET_T</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a400ebf84dd990a146672f23703e7e929</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HEADER_IS_VALID</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>af39b225d6159e5dae8e7e87e26b0d59f</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HEADER_GET_DEST_VALUE</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a7bbced8470292d5082f874e1eb2deefc</anchor>
      <arglist>(destination)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HEADER_GET_STATE_LENGTH</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a00a05d029bd609d85057149244d367de</anchor>
      <arglist>(length)</arglist>
    </member>
    <member kind="function">
      <type>tcomp_message_t *</type>
      <name>tcomp_message_create</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>af26d2b9d3125210beed8323fce43953b</anchor>
      <arglist>(const void *input_ptr, tsk_size_t input_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initFeedbackItem</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a3e63e4c08ae337e106f4287534c4ab3d</anchor>
      <arglist>(tcomp_message_t *message, uint8_t **start_ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initStateId</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a0285ac7ac39e094b4f8ca95e78683774</anchor>
      <arglist>(tcomp_message_t *message, uint8_t **start_ptr, uint8_t state_len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initStateful</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a2a3548b1d814d6a0ebceac5322ffd124</anchor>
      <arglist>(tcomp_message_t *message, uint8_t **start_ptr, uint8_t *end_ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initStateless</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a150b495ddba63a17a67750ef84724a66</anchor>
      <arglist>(tcomp_message_t *message, uint8_t **start_ptr, uint8_t *end_ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initNack</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a7266ef0e2119275eed84d10a75eb5e3e</anchor>
      <arglist>(tcomp_message_t *message, uint8_t **start_ptr, uint8_t *end_ptr)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_message_def_t</name>
      <anchorfile>tcomp__message_8c.html</anchorfile>
      <anchor>a07e24e80145c143d56ae9b2b1d8752a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_message.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__message_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <includes id="tcomp__nackinfo_8h" name="tcomp_nackinfo.h" local="yes" imported="no">tcomp_nackinfo.h</includes>
    <class kind="struct">tcomp_message_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_message_s</type>
      <name>tcomp_message_t</name>
      <anchorfile>tcomp__message_8h.html</anchorfile>
      <anchor>a5b830f1437c6fa70288a90f2b45f7603</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_message_t *</type>
      <name>tcomp_message_create</name>
      <anchorfile>tcomp__message_8h.html</anchorfile>
      <anchor>af26d2b9d3125210beed8323fce43953b</anchor>
      <arglist>(const void *input_ptr, tsk_size_t input_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_message_def_t</name>
      <anchorfile>tcomp__message_8h.html</anchorfile>
      <anchor>aac047410ea3d49e2b2734895872b5ff3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_nack_codes.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__nack__codes_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>NACK_STATE_NOT_FOUND</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a132d6009341781111040a2ff44eca186</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_CYCLES_EXHAUSTED</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a12de9059500da9cdc50dded34e429ccf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_USER_REQUESTED</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a9ce6e9b059b8a30ba43d1eea343fe4dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_SEGFAULT</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>aca772e3713b6b18347505558c6c23578</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_TOO_MANY_STATE_REQUESTS</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a47ef11a53de6f61f0620faafbae181db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INVALID_STATE_ID_LENGTH</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a07817a5622f000dec7c65b5b5e9c9f66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INVALID_STATE_PRIORITY</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>acaa79bcea57de39f556720dec4672ba6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_OUTPUT_OVERFLOW</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a441cda20cade4376bb125245fb12356a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_STACK_UNDERFLOW</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a02a4cbf10336792a107dc442f4e4c136</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_BAD_INPUT_BITORDER</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>af516c66cb1d615f1e9e6f7ed8505e096</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_DIV_BY_ZERO</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>acdb06f37c8d5e1ec55fa8b6079b0a02b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_SWITCH_VALUE_TOO_HIGH</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a2ae7fb183dfa36b2199bc24d9e415d27</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_TOO_MANY_BITS_REQUESTED</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a795c409a358eee9b6b9f4683c5e60e8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INVALID_OPERAND</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>af115710c2db389d60caa817920aff076</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_HUFFMAN_NO_MATCH</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a43b660ff844d06fef8ec56d1ccce3fe1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_MESSAGE_TOO_SHORT</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>ac0a1f67963f89adab0198d9309a38cc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INVALID_CODE_LOCATION</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>af731179ac19a8910ecd232a48a1a5b2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_BYTECODES_TOO_LARGE</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a63be220eddfc73fe58bb7fb10a843601</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INVALID_OPCODE</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a51f8e2f525b1f894239a3413cabacc47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INVALID_STATE_PROBE</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>ae957d38af5fe3b8ba8d7161a08f7ded7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_ID_NOT_UNIQUE</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>aa2b88c2a62edd1dc21a3ff5e8ea169ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_MULTILOAD_OVERWRITTEN</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>aef41697967b938070221a24b966ac5cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_STATE_TOO_SHORT</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a160ac42931b810b56de01aab88abf5a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_INTERNAL_ERROR</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>a065b44c6b3f81d9bae36bffcb0f2a281</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_FRAMING_ERROR</name>
      <anchorfile>tcomp__nack__codes_8h.html</anchorfile>
      <anchor>aedc7714f5968c6499ebb8916ad4e2d2b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_nackinfo.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__nackinfo_8c</filename>
    <includes id="tcomp__nackinfo_8h" name="tcomp_nackinfo.h" local="yes" imported="no">tcomp_nackinfo.h</includes>
    <member kind="function">
      <type>tcomp_nackinfo_t *</type>
      <name>tcomp_nackinfo_create</name>
      <anchorfile>tcomp__nackinfo_8c.html</anchorfile>
      <anchor>a15aeba0ea0ff51dfbcdf3425161980de</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_nackinfo_def_t</name>
      <anchorfile>tcomp__nackinfo_8c.html</anchorfile>
      <anchor>a03d15007180cb15e356f06663a80d1fd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_nackinfo.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__nackinfo_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <class kind="struct">tcomp_nackinfo_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_nackinfo_s</type>
      <name>tcomp_nackinfo_t</name>
      <anchorfile>tcomp__nackinfo_8h.html</anchorfile>
      <anchor>a5e4c47adb9df673b58c56dfe100d421f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_nackinfo_t *</type>
      <name>tcomp_nackinfo_create</name>
      <anchorfile>tcomp__nackinfo_8h.html</anchorfile>
      <anchor>a15aeba0ea0ff51dfbcdf3425161980de</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_nackinfo_def_t</name>
      <anchorfile>tcomp__nackinfo_8h.html</anchorfile>
      <anchor>a02525ad190981cd6558e1cbd8f23f011</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_operands.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__operands_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>UDVM_OPERAND__LITERAL</name>
      <anchorfile>tcomp__operands_8h.html</anchorfile>
      <anchor>a6736d29fc06229e081898fd9947d2afb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UDVM_OPERAND__REFERENCE</name>
      <anchorfile>tcomp__operands_8h.html</anchorfile>
      <anchor>a9ccda951c305c748e532b1c015d9a7fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UDVM_OPERAND__MULTITYPE</name>
      <anchorfile>tcomp__operands_8h.html</anchorfile>
      <anchor>a4364310bc0f39292cebd64c53a465030</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UDVM_OPERAND__ADDRESS</name>
      <anchorfile>tcomp__operands_8h.html</anchorfile>
      <anchor>a0bab53a3ef7427a869a258e39a3a0f44</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_params.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__params_8c</filename>
    <includes id="tcomp__params_8h" name="tcomp_params.h" local="yes" imported="no">tcomp_params.h</includes>
    <member kind="function">
      <type>tcomp_params_t *</type>
      <name>tcomp_params_create</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>aff79a390a86e82736c90ec7ddc8ef7c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_params_hasCpbDmsSms</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>ace18c1d1e6806a82d6b158b46f52cecd</anchor>
      <arglist>(tcomp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setCpbCode</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a87c3e1ad7e61f83516de439c8a186fc2</anchor>
      <arglist>(tcomp_params_t *params, uint8_t cpbCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_params_setCpbValue</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a81af4bc5d519490c717c741ab23480ce</anchor>
      <arglist>(tcomp_params_t *params, uint8_t cpbValue)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setDmsCode</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a2acf1afd3d2e8359cbcbc771791e002b</anchor>
      <arglist>(tcomp_params_t *params, uint8_t dmsCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_params_setDmsValue</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a738f4e766ee0a8590623934563a9f03f</anchor>
      <arglist>(tcomp_params_t *params, uint32_t dmsValue)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setSmsCode</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>aa3b641a96b532be62f6d0d33d1ef673f</anchor>
      <arglist>(tcomp_params_t *params, uint8_t smsCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_params_setSmsValue</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a4e2638b63b326c87cd420b5339246620</anchor>
      <arglist>(tcomp_params_t *params, uint32_t smsValue)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_params_getParameters</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a636c5177d93ba294666f5dbaf34c7eb9</anchor>
      <arglist>(tcomp_params_t *params)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setParameters</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a08f982ab89d188f14fa91746e01019ca</anchor>
      <arglist>(tcomp_params_t *params, uint16_t sigCompParameters)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_reset</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a283700e42a675ffd9ee4debf31ddab49</anchor>
      <arglist>(tcomp_params_t *params)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_params_def_t</name>
      <anchorfile>tcomp__params_8c.html</anchorfile>
      <anchor>a9226f7f4b7cc832652edf20fd8ee7966</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_params.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__params_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__types_8h" name="tcomp_types.h" local="yes" imported="no">tcomp_types.h</includes>
    <class kind="struct">tcomp_params_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_params_s</type>
      <name>tcomp_params_t</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>a52cddb2c0e03536034bb1b5ac879bf9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_params_t *</type>
      <name>tcomp_params_create</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>aff79a390a86e82736c90ec7ddc8ef7c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_params_hasCpbDmsSms</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>ac51d8dcb90bf338377f9475215b5ada7</anchor>
      <arglist>(tcomp_params_t *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setCpbCode</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>acd7851a904ac910ad85647c365287162</anchor>
      <arglist>(tcomp_params_t *, uint8_t _cpbCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_params_setCpbValue</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>ab8061cf4c8ea6aa7c10a06e95faa74b7</anchor>
      <arglist>(tcomp_params_t *, uint8_t _cpbValue)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setDmsCode</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>a1d5891be3add4a0027e5e106f7b25b01</anchor>
      <arglist>(tcomp_params_t *, uint8_t _dmsCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_params_setDmsValue</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>aaad79730b8375681195cc0fec83f92b9</anchor>
      <arglist>(tcomp_params_t *, uint32_t _dmsValue)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setSmsCode</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>a19edf4c10c8e772f158865e36b0fcd57</anchor>
      <arglist>(tcomp_params_t *, uint8_t _smsCode)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_params_setSmsValue</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>a068f841b1d144c6ee8a4c723f42f8285</anchor>
      <arglist>(tcomp_params_t *, uint32_t _smsValue)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_params_getParameters</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>add392eb935355e8d7a14c5974087a052</anchor>
      <arglist>(tcomp_params_t *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_setParameters</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>ab65938f2959229825dc6b9a6f45282bf</anchor>
      <arglist>(tcomp_params_t *, uint16_t sigCompParameters)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_params_reset</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>ac06b28bfef8e088704a4f789f91117d1</anchor>
      <arglist>(tcomp_params_t *)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_params_def_t</name>
      <anchorfile>tcomp__params_8h.html</anchorfile>
      <anchor>a3600f7ce67b12c71981e3544c89828eb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_reqfeed.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__reqfeed_8c</filename>
    <includes id="tcomp__reqfeed_8h" name="tcomp_reqfeed.h" local="yes" imported="no">tcomp_reqfeed.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <member kind="function">
      <type>tcomp_reqfeed_t *</type>
      <name>tcomp_reqfeed_create</name>
      <anchorfile>tcomp__reqfeed_8c.html</anchorfile>
      <anchor>aa42a159dddace38dd8ac5f3c1ee62653</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_reqfeed_reset</name>
      <anchorfile>tcomp__reqfeed_8c.html</anchorfile>
      <anchor>a6dee2bd65e673c13673a8d3d58eac0c2</anchor>
      <arglist>(tcomp_reqfeed_t *feedback)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_reqfeed_def_t</name>
      <anchorfile>tcomp__reqfeed_8c.html</anchorfile>
      <anchor>aab91e963c2a0c9364106a93d616d574c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_reqfeed.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__reqfeed_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <class kind="struct">tcomp_reqfeed_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_reqfeed_s</type>
      <name>tcomp_reqfeed_t</name>
      <anchorfile>tcomp__reqfeed_8h.html</anchorfile>
      <anchor>ad58d4c79312b5df8e65938b5797a9d8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_reqfeed_t *</type>
      <name>tcomp_reqfeed_create</name>
      <anchorfile>tcomp__reqfeed_8h.html</anchorfile>
      <anchor>aa42a159dddace38dd8ac5f3c1ee62653</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_reqfeed_reset</name>
      <anchorfile>tcomp__reqfeed_8h.html</anchorfile>
      <anchor>ae376cd64cdad8d81ef03e57a5a149346</anchor>
      <arglist>(tcomp_reqfeed_t *)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_reqfeed_def_t</name>
      <anchorfile>tcomp__reqfeed_8h.html</anchorfile>
      <anchor>ae1b641281723464ea33c07c917cd0352</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_result.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__result_8c</filename>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <member kind="function">
      <type>tcomp_result_t *</type>
      <name>tcomp_result_create</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a75b4f33c065c8a33bff2884a603f30b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tcomp_tempstate_to_free_t *</type>
      <name>tcomp_tempstate_to_free_create</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a0595a1b80463b9ba61c9b4c756ff8cfc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_tcomp_result_reset</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>ae25b615864914c068829dedac9768661</anchor>
      <arglist>(tcomp_result_t *result, tsk_bool_t isDestructor, tsk_bool_t isResetOutput)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_result_setOutputBuffer</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a4cd1aeca4164979683ae0b316da7c59f</anchor>
      <arglist>(tcomp_result_t *result, void *output_ptr, tsk_size_t output_size, tsk_bool_t isStream, uint64_t streamId)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_result_setCompartmentId</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>abd157db3d43502310150ac5ba8cbafd1</anchor>
      <arglist>(tcomp_result_t *result, const void *id, tsk_size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_result_addTempStateToCreate</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a4fb2e9f9d04f140435ce7a4863ec972e</anchor>
      <arglist>(tcomp_result_t *result, tcomp_state_t *lpState)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>tcomp_result_getTempStatesToCreateSize</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a30558503dcf2f741715a3d25502866d2</anchor>
      <arglist>(const tcomp_result_t *result)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_result_addTempStateToFree</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a49a90ce9760b500e40dee74760e2c59b</anchor>
      <arglist>(tcomp_result_t *result, tcomp_tempstate_to_free_t *lpDesc)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>tcomp_result_getTempStatesToFreeSize</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a76ee9b657de6b5384bc9b47c17bafa96</anchor>
      <arglist>(const tcomp_result_t *result)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_result_def_t</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>a3818667591ab0a590b7881bf0a793042</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_tempstate_to_free_def_t</name>
      <anchorfile>tcomp__result_8c.html</anchorfile>
      <anchor>aaaa7af0f2d5c7821d28103e6fd49cd44</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_result.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__result_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__state_8h" name="tcomp_state.h" local="yes" imported="no">tcomp_state.h</includes>
    <includes id="tcomp__reqfeed_8h" name="tcomp_reqfeed.h" local="yes" imported="no">tcomp_reqfeed.h</includes>
    <includes id="tcomp__params_8h" name="tcomp_params.h" local="yes" imported="no">tcomp_params.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <class kind="struct">tcomp_tempstate_to_free_s</class>
    <class kind="struct">tcomp_result_s</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TEMP_SATES</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>aac492d4225f8ee67192a144defd3f00e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_result_reset</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a059ee5d3c8e030734fba4cd2ff80d9e5</anchor>
      <arglist>(result)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_result_setOutputUDPBuffer</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>ae29d72c3b8a2e72b1f753bf78e4fb83a</anchor>
      <arglist>(result, output_ptr, output_size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_result_setOutputTCPBuffer</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a2d2b3608d8cdbbfd9b0fff7c85d9c04c</anchor>
      <arglist>(result, output_ptr, output_size, streamId)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_result_setOutputSCTPBuffer</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>adea3ef453be58d89425b37213a234e15</anchor>
      <arglist>(result, output_ptr, output_size)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_tempstate_to_free_s</type>
      <name>tcomp_tempstate_to_free_t</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>ab2631fa5e7686e230adfcc7316b22f16</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_result_s</type>
      <name>tcomp_result_t</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a82894c74b022fc398e00ddfea27f7b27</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API tcomp_result_t *</type>
      <name>tcomp_result_create</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a70e8b8c9e34d7320be8a0d927d65037d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tcomp_tempstate_to_free_t *</type>
      <name>tcomp_tempstate_to_free_create</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a0595a1b80463b9ba61c9b4c756ff8cfc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_tcomp_result_reset</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a8a62f531a26334f397679729b754f1aa</anchor>
      <arglist>(tcomp_result_t *result, int isDestructor, int isResetOutput)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API void</type>
      <name>tcomp_result_setOutputBuffer</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>ac78b2a60778579be64188d753f34cd8f</anchor>
      <arglist>(tcomp_result_t *result, void *output_ptr, tsk_size_t output_size, tsk_bool_t isStream, uint64_t streamId)</arglist>
    </member>
    <member kind="function">
      <type>TINYSIGCOMP_API void</type>
      <name>tcomp_result_setCompartmentId</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a830d13abdb0fd5c862aae953c365471b</anchor>
      <arglist>(tcomp_result_t *result, const void *id, tsk_size_t len)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_result_addTempStateToCreate</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a4fb2e9f9d04f140435ce7a4863ec972e</anchor>
      <arglist>(tcomp_result_t *result, tcomp_state_t *lpState)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>tcomp_result_getTempStatesToCreateSize</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a30558503dcf2f741715a3d25502866d2</anchor>
      <arglist>(const tcomp_result_t *result)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_result_addTempStateToFree</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a49a90ce9760b500e40dee74760e2c59b</anchor>
      <arglist>(tcomp_result_t *result, tcomp_tempstate_to_free_t *lpDesc)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>tcomp_result_getTempStatesToFreeSize</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>a76ee9b657de6b5384bc9b47c17bafa96</anchor>
      <arglist>(const tcomp_result_t *result)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_tempstate_to_free_def_t</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>ae5134c3378c12c8958abc01627e5dac6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_result_def_t</name>
      <anchorfile>tcomp__result_8h.html</anchorfile>
      <anchor>aed43d77f3b1024e3b8f2f2621a6aac9c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_rfc3485_dictionary_sip.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__rfc3485__dictionary__sip_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_VALUE_LENGTH</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>a46e6c1833a325b412f1b90d9d3cff42f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_STATE_ADDRESS</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>ae789106efa08941eee61853b1b895e1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_STATE_INSTRUCTION</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>a9b51704ff3f8bad008f4946276859c7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_MINIMUM_ACCESS_LENGTH</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>aec7741538222b69014f4943c368d6452</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_IDENTIFIER_LENGTH</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>add0319732919cb3200aa30c9c9761b1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_IDENTIFIER</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>a1d974cbbdbfccaf0ca462557cbff48fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC3485_DICTIONARY_SIP_VALUE</name>
      <anchorfile>tcomp__rfc3485__dictionary__sip_8h.html</anchorfile>
      <anchor>afd9f736f60d149d4f6c7b6fe658415dd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_rfc5049_sip.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__rfc5049__sip_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>SIP_RFC5049_DECOMPRESSION_MEMORY_SIZE</name>
      <anchorfile>tcomp__rfc5049__sip_8h.html</anchorfile>
      <anchor>afea228c70438633401ce75af876c204b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIP_RFC5049_STATE_MEMORY_SIZE</name>
      <anchorfile>tcomp__rfc5049__sip_8h.html</anchorfile>
      <anchor>ac46426f92491fd0f3ec4ab081c692620</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIP_RFC5049_CYCLES_PER_BIT</name>
      <anchorfile>tcomp__rfc5049__sip_8h.html</anchorfile>
      <anchor>a6b85c26b54ca1af4e56dc8060f905a91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIP_RFC5049_SIGCOMP_VERSION</name>
      <anchorfile>tcomp__rfc5049__sip_8h.html</anchorfile>
      <anchor>a2a7c63da9d90ca86d8c5240246d8d5a1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_rfc5112_dictionary_presence.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__rfc5112__dictionary__presence_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>a416058ff6652546d05c7f3474bebb9e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_STATE_ADDRESS</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>aff4f353ba71c9a524f06b16c1791d55f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_STATE_INSTRUCTION</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>aa38255635e19a159e4d3bbdfa45e2903</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_MINIMUM_ACCESS_LENGTH</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>ad1a4dda419eeb89b26fd7f4bd843d920</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_IDENTIFIER_LENGTH</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>afe38ec4bf3e263e04138502ccacbddd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_IDENTIFIER</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>a68b9a76e5d681e316a61acb216a13027</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RFC5112_DICTIONARY_PRESENCE_VALUE</name>
      <anchorfile>tcomp__rfc5112__dictionary__presence_8h.html</anchorfile>
      <anchor>af209ef460fd56bfddd2c961b8636d948</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_state.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__state_8c</filename>
    <includes id="tcomp__state_8h" name="tcomp_state.h" local="yes" imported="no">tcomp_state.h</includes>
    <member kind="function">
      <type>tcomp_state_t *</type>
      <name>tcomp_state_create</name>
      <anchorfile>tcomp__state_8c.html</anchorfile>
      <anchor>ab1d9b808b98cc614722e1b9f05c11aba</anchor>
      <arglist>(uint16_t length, uint16_t address, uint16_t instruction, uint16_t minimum_access_length, uint16_t retention_priority)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_state_equals</name>
      <anchorfile>tcomp__state_8c.html</anchorfile>
      <anchor>a3d3fd6c9177e3b1eb636c659d003bf79</anchor>
      <arglist>(const tcomp_state_t *state1, const tcomp_state_t *state2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_state_makeValid</name>
      <anchorfile>tcomp__state_8c.html</anchorfile>
      <anchor>a03e88223cd551855cec74d9d52846aec</anchor>
      <arglist>(tcomp_state_t *state)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_state_def_t</name>
      <anchorfile>tcomp__state_8c.html</anchorfile>
      <anchor>a1c5967de31ecec8879549467ef3a18bb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_state.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__state_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <class kind="struct">tcomp_state_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_PARTIAL_ID_LEN_CODE</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>aee3d499dc9e1ff74689cc26ae06a0b15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_PARTIAL_ID_LEN_VALUE</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>a5f3ebc3b848b5430dbdfb91a4e3e39cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_GET_STATE_SIZE</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>ae43ac0c95eed7ad26bebf7d6bef9c889</anchor>
      <arglist>(state)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_state_s</type>
      <name>tcomp_state_t</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>a67df0787edba76e7ab0814bb350e7f77</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tcomp_state_t</type>
      <name>tcomp_dictionary_t</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>ab386576fd112e3d2faa72357ec0402fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_state_t *</type>
      <name>tcomp_state_create</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>ab1d9b808b98cc614722e1b9f05c11aba</anchor>
      <arglist>(uint16_t length, uint16_t address, uint16_t instruction, uint16_t minimum_access_length, uint16_t retention_priority)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_state_equals</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>a3d3fd6c9177e3b1eb636c659d003bf79</anchor>
      <arglist>(const tcomp_state_t *state1, const tcomp_state_t *state2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_state_makeValid</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>aa2181d1c22e09ed60a4d0afdb5716615</anchor>
      <arglist>(tcomp_state_t *)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_state_def_t</name>
      <anchorfile>tcomp__state_8h.html</anchorfile>
      <anchor>acd942cf8e1e7ebbd2ff9cada79e0e627</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_statehandler.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__statehandler_8c</filename>
    <includes id="tcomp__statehandler_8h" name="tcomp_statehandler.h" local="yes" imported="no">tcomp_statehandler.h</includes>
    <includes id="tcomp__rfc5049__sip_8h" name="tcomp_rfc5049_sip.h" local="yes" imported="no">tcomp_rfc5049_sip.h</includes>
    <includes id="tcomp__nack__codes_8h" name="tcomp_nack_codes.h" local="yes" imported="no">tcomp_nack_codes.h</includes>
    <includes id="tcomp__dicts_8h" name="tcomp_dicts.h" local="yes" imported="no">tcomp_dicts.h</includes>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="function">
      <type>tcomp_statehandler_t *</type>
      <name>tcomp_statehandler_create</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>aba10fe6bff0862b9f3064901cfe10a46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tcomp_compartment_t *</type>
      <name>tcomp_statehandler_getCompartment</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a797699092db01a2f4db2f2f260053746</anchor>
      <arglist>(const tcomp_statehandler_t *statehandler, uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_statehandler_deleteCompartment</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a5800845e0c96c2f1be8b3f415a7537d8</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_statehandler_compartmentExist</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a39aac43417c1c111a74679a118a58cba</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_statehandler_findState</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a73c0a026931ce0b721e1dc28943cf186</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_statehandler_handleResult</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a3c3b5ef6aada3d28e6f095f9de5c56d1</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, tcomp_result_t **lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_statehandler_handleNack</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a5ad50cd29378cf5e0412913a5b28aead</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, const tcomp_nackinfo_t *nackinfo)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_statehandler_addSipSdpDictionary</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a6c37d108c757397a4d45b26012d7da7c</anchor>
      <arglist>(tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_statehandler_addPresenceDictionary</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>af8f13459ff48ebc861124c087d40bd7b</anchor>
      <arglist>(tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_statehandler_def_t</name>
      <anchorfile>tcomp__statehandler_8c.html</anchorfile>
      <anchor>a824d0d2dee7bf65b58b11882fb3c2ad0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_statehandler.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__statehandler_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__types_8h" name="tcomp_types.h" local="yes" imported="no">tcomp_types.h</includes>
    <includes id="tcomp__params_8h" name="tcomp_params.h" local="yes" imported="no">tcomp_params.h</includes>
    <includes id="tcomp__nackinfo_8h" name="tcomp_nackinfo.h" local="yes" imported="no">tcomp_nackinfo.h</includes>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <includes id="tcomp__buffer_8h" name="tcomp_buffer.h" local="yes" imported="no">tcomp_buffer.h</includes>
    <includes id="tcomp__compartment_8h" name="tcomp_compartment.h" local="yes" imported="no">tcomp_compartment.h</includes>
    <includes id="tcomp__state_8h" name="tcomp_state.h" local="yes" imported="no">tcomp_state.h</includes>
    <class kind="struct">tcomp_statehandler_s</class>
    <member kind="typedef">
      <type>TCOMP_BEGIN_DECLS struct tcomp_statehandler_s</type>
      <name>tcomp_statehandler_t</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>acbeadf12b871d633340f8db0d4bab6b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_statehandler_t *</type>
      <name>tcomp_statehandler_create</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>aba10fe6bff0862b9f3064901cfe10a46</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tcomp_compartment_t *</type>
      <name>tcomp_statehandler_getCompartment</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a797699092db01a2f4db2f2f260053746</anchor>
      <arglist>(const tcomp_statehandler_t *statehandler, uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_statehandler_deleteCompartment</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a5800845e0c96c2f1be8b3f415a7537d8</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_statehandler_compartmentExist</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a39aac43417c1c111a74679a118a58cba</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, uint64_t id)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_statehandler_findState</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a73c0a026931ce0b721e1dc28943cf186</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, const tcomp_buffer_handle_t *partial_identifier, tcomp_state_t **lpState)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_statehandler_handleResult</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a3c3b5ef6aada3d28e6f095f9de5c56d1</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, tcomp_result_t **lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_statehandler_handleNack</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a28e289f7334f2b4541f20f9bb9f217cb</anchor>
      <arglist>(tcomp_statehandler_t *statehandler, const tcomp_nackinfo_t *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_statehandler_addSipSdpDictionary</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a6c37d108c757397a4d45b26012d7da7c</anchor>
      <arglist>(tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_statehandler_addPresenceDictionary</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>af8f13459ff48ebc861124c087d40bd7b</anchor>
      <arglist>(tcomp_statehandler_t *statehandler)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_statehandler_def_t</name>
      <anchorfile>tcomp__statehandler_8h.html</anchorfile>
      <anchor>a1f7c842c3c935a11555a7b5b550b9a04</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_types.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__types_8h</filename>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tcomp_states_L_t</name>
      <anchorfile>tcomp__types_8h.html</anchorfile>
      <anchor>ac33ce77066e58224e9fb6cc906f0b882</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tcomp_dictionaries_L_t</name>
      <anchorfile>tcomp__types_8h.html</anchorfile>
      <anchor>ab08d4225211c6ff4181d4edbb3ff5a51</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tcomp_compartments_L_t</name>
      <anchorfile>tcomp__types_8h.html</anchorfile>
      <anchor>a60f2876ffe7c49071a549062dca38f9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tsk_list_t</type>
      <name>tcomp_stream_buffer_L_t</name>
      <anchorfile>tcomp__types_8h.html</anchorfile>
      <anchor>a57014d6cc41fd5ede1999c7260f8156b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TCOMP_BEGIN_DECLS typedef tsk_list_t</type>
      <name>tcomp_buffers_L_t</name>
      <anchorfile>tcomp__types_8h.html</anchorfile>
      <anchor>a527910dae1c173fcb2209bfc3a032d93</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.bytecopy.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8bytecopy_8c</filename>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_MEMORY_REGISTERS_PTR</name>
      <anchorfile>tcomp__udvm_8bytecopy_8c.html</anchorfile>
      <anchor>a61a91f9f6e2ce5e778dd238243704b87</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_bytecopy_self</name>
      <anchorfile>tcomp__udvm_8bytecopy_8c.html</anchorfile>
      <anchor>a7105b1d9f78119c8f98335bc9fab46e5</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t *destination, uint16_t source, uint16_t tsk_size_tocopy)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_bytecopy_to</name>
      <anchorfile>tcomp__udvm_8bytecopy_8c.html</anchorfile>
      <anchor>a6d635123e7327e51fc053467668f89da</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t destination, const uint8_t *source, uint16_t tsk_size_tocopy)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_bytecopy_from</name>
      <anchorfile>tcomp__udvm_8bytecopy_8c.html</anchorfile>
      <anchor>aa4424706a97d3b07ce20c447d6b7512a</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint8_t *destination, uint16_t source, uint16_t tsk_size_tocopy)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8c</filename>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_HEADER_RESERVED_SIZE</name>
      <anchorfile>tcomp__udvm_8c.html</anchorfile>
      <anchor>a4b56d9e243e2bf97293429a64d250454</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_udvm_t *</type>
      <name>tcomp_udvm_create</name>
      <anchorfile>tcomp__udvm_8c.html</anchorfile>
      <anchor>a16df46fa6fa56dc28b12c3c3d2c7f4bd</anchor>
      <arglist>(tcomp_message_t *_sigCompMessage, tcomp_statehandler_t *stateHandler, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_udvm_decompress</name>
      <anchorfile>tcomp__udvm_8c.html</anchorfile>
      <anchor>a02b9bb3cf2d898d80a37eab23cc24c31</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="variable">
      <type>const tsk_object_def_t *</type>
      <name>tcomp_udvm_def_t</name>
      <anchorfile>tcomp__udvm_8c.html</anchorfile>
      <anchor>af00e8ac0e2de8a88b47db6c6ce183ed7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <includes id="tcomp__message_8h" name="tcomp_message.h" local="yes" imported="no">tcomp_message.h</includes>
    <includes id="tcomp__result_8h" name="tcomp_result.h" local="yes" imported="no">tcomp_result.h</includes>
    <includes id="tcomp__headers__index_8h" name="tcomp_headers_index.h" local="yes" imported="no">tcomp_headers_index.h</includes>
    <includes id="tcomp__nack__codes_8h" name="tcomp_nack_codes.h" local="yes" imported="no">tcomp_nack_codes.h</includes>
    <includes id="tcomp__statehandler_8h" name="tcomp_statehandler.h" local="yes" imported="no">tcomp_statehandler.h</includes>
    <includes id="tcomp__instructions_8h" name="tcomp_instructions.h" local="yes" imported="no">tcomp_instructions.h</includes>
    <class kind="struct">tcomp_udvm_s</class>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_SET_2BYTES_VAL</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>adb9b204c8be94eeba4976b43e28fa508</anchor>
      <arglist>(position, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_GET_2BYTES_VAL</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>abc0f3fc7d0eb8eada7ca448e8d45374b</anchor>
      <arglist>(position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_DEBUG_PRINT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a5032ec60358f97e2c6c28025e9c1bc10</anchor>
      <arglist>(size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_GET_SIZE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a7bf31af34af1e2ada0514173e59b2bf8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_GET_BUFFER</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ab30dc9d6a107a65876a4ea16a8fa9de5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_GET_BUFFER_AT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>af784d748dab947aab792481b4b24bb13</anchor>
      <arglist>(position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_udvm_createNackInfo2</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>abd366e5f7766aefe241b3a03a1baa2d8</anchor>
      <arglist>(udvm, reasonCode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tcomp_udvm_createNackInfo3</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ac232aae773eb94b7c1789912b17dc0fe</anchor>
      <arglist>(udvm, reasonCode, lpDetails)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_EXEC_INST__PUSH2</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>af332769e4abbe65f37848ff4ff6657a3</anchor>
      <arglist>(udvm)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_EXEC_INST__POP2</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>af2cbccff0846a365d0c3f7c2618c3519</anchor>
      <arglist>(udvm)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_UDVM_EXEC_INST__JUMP2</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a2169ba3289879baf5639ed677df5f3e4</anchor>
      <arglist>(udvm)</arglist>
    </member>
    <member kind="typedef">
      <type>struct tcomp_udvm_s</type>
      <name>tcomp_udvm_t</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a81a140165274fcf18781e1e0550d6ce1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tcomp_udvm_t *</type>
      <name>tcomp_udvm_create</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a16df46fa6fa56dc28b12c3c3d2c7f4bd</anchor>
      <arglist>(tcomp_message_t *_sigCompMessage, tcomp_statehandler_t *stateHandler, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>tcomp_udvm_decompress</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a02b9bb3cf2d898d80a37eab23cc24c31</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_literal_param</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aba3b7b5e835a6b0b94d1e7431fdc9721</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_reference_param</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>af8457bd036ee98ae3eb88194b7e99a81</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_multitype_param</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a28aa1c73369d612a37aadb899f1c40ce</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_address_param</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a9127af6e5fed8885a8423e8aca9c9d8c</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t memory_address_of_instruction)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_bytecopy_self</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a7105b1d9f78119c8f98335bc9fab46e5</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t *destination, uint16_t source, uint16_t tsk_size_tocopy)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_bytecopy_to</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a6d635123e7327e51fc053467668f89da</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t destination, const uint8_t *source, uint16_t tsk_size_tocopy)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_bytecopy_from</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aa4424706a97d3b07ce20c447d6b7512a</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint8_t *destination, uint16_t source, uint16_t tsk_size_tocopy)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_byteCopy_TempStates</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a4c7c5fef14331240f01333f65fa26d0c</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_createTempState</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a201b466c9f6fdd801b95d85217f28616</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority, int end_msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_udvm_createNackInfo</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aa3bdee3985967a5386d64edf78e418fb</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint8_t reasonCode, tcomp_buffer_handle_t *lpDetails, int16_t memory_address_of_instruction)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__DECOMPRESSION_FAILURE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a180ab76639971685afcd85b1a10babac</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__AND</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a7648d6e68144c12bb243a34fd267f56e</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__OR</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ab4da42951faf649460335e6886742852</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__NOT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>acdcf12c69913e80cf161395ecc4a33dd</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__LSHIFT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aba3e450e86a072d53ce906b5ea41f97f</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__RSHIFT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a86538de0deb2b2dc310913919d380b4b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__ADD</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a3899de24f5a3dea0f3796917e0d83676</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SUBTRACT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ac133aab55e2826b388af11fc8a84e964</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__MULTIPLY</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a992361183a482a98374152a74f023752</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__DIVIDE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a18dcd9621e72255eb23f48a3762d9a20</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__REMAINDER</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a6501f4e5ac88d2313261dc20527d9365</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SORT_ASCENDING</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a0e4ecc7b1d6c800e0e39f6bd3b341162</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SORT_DESCENDING</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ae1a039751a45ea6724d4c67763894df6</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SHA_1</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a34da436d0a62982ecc8e5ef01dea79da</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__LOAD</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aff253dc2b735dc84f0a99ce9d3d52a9b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address, uint16_t value)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__MULTILOAD</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>afcd6abbc82e6f26ce30e85a08b48eb0b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address, uint16_t n)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__PUSH</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a87c852814a930fd041c7cfd3e6860def</anchor>
      <arglist>(tcomp_udvm_t *udvm, int16_t value)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__POP</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a12d539688e57b4c2d557b15a8c91da19</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t *value)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COPY</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a6c838f51794fe192e8ebbea09eff8c58</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COPY_LITERAL</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a4179a80e7d1d8f24c2321326e24d2cd5</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COPY_OFFSET</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a6c9526e3f681b45e5a3613628cb57bfd</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t offset, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__MEMSET</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a32f882639d587723a0f580c6e0688a3c</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address, uint16_t length, uint16_t start_value, uint16_t offset)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__JUMP</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a238b31bc4cf045aa805fa8f7f4a2825c</anchor>
      <arglist>(tcomp_udvm_t *udvm, int16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COMPARE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aa30109fa08fcff9efc60ffbac386d580</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t value_1, uint16_t value_2, uint16_t address_1, uint16_t address_2, uint16_t address_3)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__CALL</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ab29b849f7f4fe1d6bbb690732b943284</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__RETURN</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a86006ae64fdd37c8149f0fd3b12035cc</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SWITCH</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ae6af049d3325ec85c50496b7ba443ff3</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t n, uint16_t j)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__CRC</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ad71e921644a616fc8c8ea4bb939f0029</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t value, uint16_t position, uint16_t length, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__INPUT_BYTES</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a1687f0bee1f33eb024d61289af9d485b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__INPUT_BITS</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a63a6dea457a13925869b30338f867285</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__INPUT_HUFFMAN</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ab157ab26ab4a822aa64b04e9c2ea562f</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t destination, uint16_t address, uint16_t n)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__STATE_ACCESS</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>ada10a2b2a97b0c5345a01f9c3c3ffa22</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length, uint16_t state_begin, uint16_t state_length, uint16_t state_address, uint16_t state_instruction)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__STATE_CREATE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a5997c6bb91c7d21a1fa75c8d6c531c96</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__STATE_FREE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>a53169151997ac68e1fa04a491f8ab703</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__OUTPUT</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aff3958d7201fa9c46cb5ca48a2181a39</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t output_start, uint16_t output_length)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__END_MESSAGE</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>aec57780e7fade6291b3baeb867471f7b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t requested_feedback_location, uint16_t returned_parameters_location, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)</arglist>
    </member>
    <member kind="variable">
      <type>TINYSIGCOMP_GEXTERN const tsk_object_def_t *</type>
      <name>tcomp_udvm_def_t</name>
      <anchorfile>tcomp__udvm_8h.html</anchorfile>
      <anchor>acd10495089ab209d659e9c240dc25a0a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.instructions.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8instructions_8c</filename>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <class kind="struct">IndexValuePair_s</class>
    <member kind="define">
      <type>#define</type>
      <name>F_BIT_MSB_TO_LSB</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a37bd4434265e0d4d32c34dc0bf1c0e74</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F_BIT_LSB_TO_MSB</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a7d2f1327f1e70b45cd57593731cad238</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>H_BIT_MSB_TO_LSB</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a979a627a246411f6f43bf52c79c4d126</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>H_BIT_LSB_TO_MSB</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a2a31b69aacaab5f8179b4b7bf10de238</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CEILLINGLOG2</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ac8ecaf840dd84f010e9d34d30af8beb7</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CONSUME_CYCLES</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>aef3a50cd7dadc677353232158a907001</anchor>
      <arglist>(cycles)</arglist>
    </member>
    <member kind="typedef">
      <type>struct IndexValuePair_s</type>
      <name>IndexValuePair_t</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a314ecebee88e0aee0cea83588efbb741</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__DECOMPRESSION_FAILURE</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a180ab76639971685afcd85b1a10babac</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__AND</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a7648d6e68144c12bb243a34fd267f56e</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__OR</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ab4da42951faf649460335e6886742852</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__NOT</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>acdcf12c69913e80cf161395ecc4a33dd</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__LSHIFT</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>aba3e450e86a072d53ce906b5ea41f97f</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__RSHIFT</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a86538de0deb2b2dc310913919d380b4b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__ADD</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a3899de24f5a3dea0f3796917e0d83676</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SUBTRACT</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ac133aab55e2826b388af11fc8a84e964</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__MULTIPLY</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a992361183a482a98374152a74f023752</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__DIVIDE</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a18dcd9621e72255eb23f48a3762d9a20</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__REMAINDER</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a6501f4e5ac88d2313261dc20527d9365</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t operand_1, uint16_t operand_2)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SORT_ASCENDING</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a0e4ecc7b1d6c800e0e39f6bd3b341162</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SORT_DESCENDING</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ae1a039751a45ea6724d4c67763894df6</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t start, uint16_t n, uint16_t k)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SHA_1</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a34da436d0a62982ecc8e5ef01dea79da</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__LOAD</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>aff253dc2b735dc84f0a99ce9d3d52a9b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address, uint16_t value)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__MULTILOAD</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>afcd6abbc82e6f26ce30e85a08b48eb0b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address, uint16_t n)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__PUSH</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a87c852814a930fd041c7cfd3e6860def</anchor>
      <arglist>(tcomp_udvm_t *udvm, int16_t value)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__POP</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a12d539688e57b4c2d557b15a8c91da19</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t *value)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COPY</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a6c838f51794fe192e8ebbea09eff8c58</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COPY_LITERAL</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a4179a80e7d1d8f24c2321326e24d2cd5</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t position, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COPY_OFFSET</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a6c9526e3f681b45e5a3613628cb57bfd</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t offset, uint16_t length, uint16_t destination)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__MEMSET</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a32f882639d587723a0f580c6e0688a3c</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address, uint16_t length, uint16_t start_value, uint16_t offset)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__JUMP</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a238b31bc4cf045aa805fa8f7f4a2825c</anchor>
      <arglist>(tcomp_udvm_t *udvm, int16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__COMPARE</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>aa30109fa08fcff9efc60ffbac386d580</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t value_1, uint16_t value_2, uint16_t address_1, uint16_t address_2, uint16_t address_3)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__CALL</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ab29b849f7f4fe1d6bbb690732b943284</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__RETURN</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a86006ae64fdd37c8149f0fd3b12035cc</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__SWITCH</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ae6af049d3325ec85c50496b7ba443ff3</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t n, uint16_t j)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__CRC</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ad71e921644a616fc8c8ea4bb939f0029</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t value, uint16_t position, uint16_t length, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__INPUT_BYTES</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a1687f0bee1f33eb024d61289af9d485b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__INPUT_BITS</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a63a6dea457a13925869b30338f867285</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t length, uint16_t destination, uint16_t address)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__INPUT_HUFFMAN</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ab157ab26ab4a822aa64b04e9c2ea562f</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t destination, uint16_t address, uint16_t n)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__STATE_ACCESS</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>ada10a2b2a97b0c5345a01f9c3c3ffa22</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length, uint16_t state_begin, uint16_t state_length, uint16_t state_address, uint16_t state_instruction)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__STATE_CREATE</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a5997c6bb91c7d21a1fa75c8d6c531c96</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__STATE_FREE</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>a53169151997ac68e1fa04a491f8ab703</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t partial_identifier_start, uint16_t partial_identifier_length)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__OUTPUT</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>aff3958d7201fa9c46cb5ca48a2181a39</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t output_start, uint16_t output_length)</arglist>
    </member>
    <member kind="function">
      <type>tsk_bool_t</type>
      <name>TCOMP_UDVM_EXEC_INST__END_MESSAGE</name>
      <anchorfile>tcomp__udvm_8instructions_8c.html</anchorfile>
      <anchor>aec57780e7fade6291b3baeb867471f7b</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t requested_feedback_location, uint16_t returned_parameters_location, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.nack.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8nack_8c</filename>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>NACK_HEADER_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>a15ec29509a4a28ad21db5e2bc373e091</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_VERSION_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>af41d439880d0eee887dd37527348b6e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_REASON_CODE_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>a16f9f852d995a0197f36936dd62dd0a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_OPCODE_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>ae16d49f1dc390062a54a45670976dc2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_PC_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>a8db7152c921e099beea80560ae6f0ab2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_SHA1_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>a0215f1bb730f208a9de8211ca6dc1f76</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_DETAILS_INDEX</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>a5141e54ef81314259c1009aa1d0a3208</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_udvm_createNackInfo</name>
      <anchorfile>tcomp__udvm_8nack_8c.html</anchorfile>
      <anchor>aa3bdee3985967a5386d64edf78e418fb</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint8_t reasonCode, tcomp_buffer_handle_t *lpDetails, int16_t memory_address_of_instruction)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.operands.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8operands_8c</filename>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_literal_param</name>
      <anchorfile>tcomp__udvm_8operands_8c.html</anchorfile>
      <anchor>aba3b7b5e835a6b0b94d1e7431fdc9721</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_reference_param</name>
      <anchorfile>tcomp__udvm_8operands_8c.html</anchorfile>
      <anchor>af8457bd036ee98ae3eb88194b7e99a81</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_multitype_param</name>
      <anchorfile>tcomp__udvm_8operands_8c.html</anchorfile>
      <anchor>a28aa1c73369d612a37aadb899f1c40ce</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>tcomp_udvm_opget_address_param</name>
      <anchorfile>tcomp__udvm_8operands_8c.html</anchorfile>
      <anchor>a9127af6e5fed8885a8423e8aca9c9d8c</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t memory_address_of_instruction)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tcomp_udvm.statemanagment.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tcomp__udvm_8statemanagment_8c</filename>
    <includes id="tcomp__udvm_8h" name="tcomp_udvm.h" local="yes" imported="no">tcomp_udvm.h</includes>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_byteCopy_TempStates</name>
      <anchorfile>tcomp__udvm_8statemanagment_8c.html</anchorfile>
      <anchor>a4c7c5fef14331240f01333f65fa26d0c</anchor>
      <arglist>(tcomp_udvm_t *udvm)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_udvm_createTempState</name>
      <anchorfile>tcomp__udvm_8statemanagment_8c.html</anchorfile>
      <anchor>a201b466c9f6fdd801b95d85217f28616</anchor>
      <arglist>(tcomp_udvm_t *udvm, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority, int end_msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tinysigcomp_config.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>tinysigcomp__config_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>TINYSIGCOMP_API</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>a91f4e14a57c6fa32103d9f2724d7f7b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TINYSIGCOMP_GEXTERN</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>af0b0a913503d1a7ccd5ea2d0b080b80c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_BEGIN_DECLS</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>a8f9ace67902587dbb2d57ca365576af8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TCOMP_END_DECLS</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>a31258ce4dc26ff064422a1f004f89ef8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FORCE_STATIC</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>aff8ec2db7e49184b95d7bdc50523a308</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NO_GZIP</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>a27b69f7a184c1d379f4b610518c17852</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_VERSION</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>aa3dd4ff77b198f4c85a9126eb19e0d79</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NACK_MAX_HISTORY_SIZE</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>a272992e8952cd316ffac7e3810822e55</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_ONLY_ACKED_STATES</name>
      <anchorfile>tinysigcomp__config_8h.html</anchorfile>
      <anchor>a12ac66d9032d10004ec49c4b4d5e4383</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>trees.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>trees_8c</filename>
    <includes id="deflate_8h" name="deflate.h" local="yes" imported="no">deflate.h</includes>
    <class kind="struct">static_tree_desc_s</class>
    <member kind="define">
      <type>#define</type>
      <name>MAX_BL_BITS</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a8f9fea0ddde71a75c6caf59ba993ca4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>END_BLOCK</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a1e33ae802796dae8694948b87ac62d61</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REP_3_6</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a5d39bc5068c496733ec81b9d5543db40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REPZ_3_10</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a39f77126525d41c0155506e942365da8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REPZ_11_138</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a350267bb8ca6269fac9ba06057336788</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Buf_size</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a0c0213b942f7535235cf515e8fe23bf9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DIST_CODE_LEN</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a368ebabef641e688c8f848e76b704500</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>send_code</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a46e5de3915e644d07a6ff571e7db7bfa</anchor>
      <arglist>(s, c, tree)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>put_short</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ae76cb23c4bc3d3f80e7a7917dd538c4e</anchor>
      <arglist>(s, w)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>send_bits</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>aea5290b53438097651a8d1c7098aa5ef</anchor>
      <arglist>(s, value, length)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SMALLEST</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ad14f32cbc4e0d9dfc99f6a3897ee4607</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pqremove</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>abf2ba789ae874d21ce278885758c4dcc</anchor>
      <arglist>(s, tree, top)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>smaller</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a78d1ce835d96eb110f23c580ad7fab7c</anchor>
      <arglist>(tree, n, m, depth)</arglist>
    </member>
    <member kind="function">
      <type>local void tr_static_init</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>abe64a455dd65fe0a67e6321ee371fcd2</anchor>
      <arglist>((void))</arglist>
    </member>
    <member kind="function">
      <type>local void init_block</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>af75a7af96f53e3152d3f50ca52a7853d</anchor>
      <arglist>((deflate_state *s))</arglist>
    </member>
    <member kind="function">
      <type>local void pqdownheap</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ab92e26169a5759daa001ec2e76a2643a</anchor>
      <arglist>((deflate_state *s, ct_data *tree, int k))</arglist>
    </member>
    <member kind="function">
      <type>local void gen_bitlen</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>af9351d2c46b32565d5cf1e7bc1ef5141</anchor>
      <arglist>((deflate_state *s, tree_desc *desc))</arglist>
    </member>
    <member kind="function">
      <type>local void gen_codes</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a239c1b73bfca5de0e5bc1e16672baa96</anchor>
      <arglist>((ct_data *tree, int max_code, ushf *bl_count))</arglist>
    </member>
    <member kind="function">
      <type>local void scan_tree</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ad3c361cdfe3d2a57c27124471e633d72</anchor>
      <arglist>((deflate_state *s, ct_data *tree, int max_code))</arglist>
    </member>
    <member kind="function">
      <type>local void send_all_trees</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a6e8841ba44352e17c6d8d42fda0a58db</anchor>
      <arglist>((deflate_state *s, int lcodes, int dcodes, int blcodes))</arglist>
    </member>
    <member kind="function">
      <type>local void compress_block</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ac7450586e0f5715477279373e4020183</anchor>
      <arglist>((deflate_state *s, ct_data *ltree, ct_data *dtree))</arglist>
    </member>
    <member kind="function">
      <type>local unsigned bi_reverse</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a6ebe72dd3504aba5d2eef6d4465534f5</anchor>
      <arglist>((unsigned value, int length))</arglist>
    </member>
    <member kind="function">
      <type>local void copy_block</type>
      <name>OF</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a7872aa8a931e8bd1b0e34904bae32e16</anchor>
      <arglist>((deflate_state *s, charf *buf, unsigned len, int header))</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>tr_static_init</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>af0327179e9998353dddfef79251847d4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_tr_init</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a339961134b155099c972524109f9119b</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>init_block</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ac5265fdf07ca6c4cc57769e2fb546d55</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>pqdownheap</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a44f0c568451fbfd68a418eaddea09982</anchor>
      <arglist>(deflate_state *s, ct_data *tree, int k)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>gen_bitlen</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a1ad07677feac279d0f161fea3891af92</anchor>
      <arglist>(deflate_state *s, tree_desc *desc)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>gen_codes</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a599e4c4281ef721bd56222d9f01dec7c</anchor>
      <arglist>(ct_data *tree, int max_code, ushf *bl_count)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>build_tree</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ae24585b9d3230020126f1c8ae8f240f9</anchor>
      <arglist>(deflate_state *s, tree_desc *desc)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>scan_tree</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a7a7678fb5fcf9024b76f5ce22abce082</anchor>
      <arglist>(deflate_state *s, ct_data *tree, int max_code)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>send_tree</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a66b34bd277c8c1de59110a448a55b467</anchor>
      <arglist>(deflate_state *s, ct_data *tree, int max_code)</arglist>
    </member>
    <member kind="function">
      <type>local int</type>
      <name>build_bl_tree</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>aef05b1c8b7f7ef2902111a8f415ead66</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>send_all_trees</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a8e697cc571f3acaa017cbd15bc352fe5</anchor>
      <arglist>(deflate_state *s, int lcodes, int dcodes, int blcodes)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_tr_stored_block</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a9a6836d79cbfac6ba37a99d940201794</anchor>
      <arglist>(deflate_state *s, charf *buf, ulg stored_len, int eof)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_tr_align</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a728f046b1fdbb36085e368a92a0d6738</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_tr_flush_block</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a4dca532d73a1df8ac5af0cf3ae3b079b</anchor>
      <arglist>(deflate_state *s, charf *buf, ulg stored_len, int eof)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>_tr_tally</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a3307d897f4939a67623f4f5120bd2ec2</anchor>
      <arglist>(deflate_state *s, unsigned dist, unsigned lc)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>compress_block</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a507733a2d56ecdb7b6d7b169f241d8ea</anchor>
      <arglist>(deflate_state *s, ct_data *ltree, ct_data *dtree)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>set_data_type</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a0a027841f3e3745808bcf5d05287385e</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local unsigned</type>
      <name>bi_reverse</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ac59b08248ea9953b87f375717ab8524a</anchor>
      <arglist>(unsigned code, int len)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>bi_flush</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ab3b46873b1a7611ee2ca275cfd1c8f79</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>bi_windup</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a8532fd6b0e6cd8b6a633fe249ac4353c</anchor>
      <arglist>(deflate_state *s)</arglist>
    </member>
    <member kind="function">
      <type>local void</type>
      <name>copy_block</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a1f3d6aa6a0147df6a48af35bcb5719d6</anchor>
      <arglist>(deflate_state *s, charf *buf, unsigned len, int header)</arglist>
    </member>
    <member kind="variable">
      <type>local const int</type>
      <name>extra_lbits</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a805b183476b3962fc40c42137cdea492</anchor>
      <arglist>[LENGTH_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local const int</type>
      <name>extra_dbits</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a1b86eb7d51abdc18b72241d84013d548</anchor>
      <arglist>[D_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local const int</type>
      <name>extra_blbits</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>adcbeeb62f98f141503fc258c65b17204</anchor>
      <arglist>[BL_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local const uch</type>
      <name>bl_order</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a719d14ec6023beb7ec40af2e05b5b453</anchor>
      <arglist>[BL_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local ct_data</type>
      <name>static_ltree</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a4cd63132278f666b275275d6ba45cca3</anchor>
      <arglist>[L_CODES+2]</arglist>
    </member>
    <member kind="variable">
      <type>local ct_data</type>
      <name>static_dtree</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a0ddc0ba781501da71ec8d54256819740</anchor>
      <arglist>[D_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>uch</type>
      <name>_dist_code</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a9e0f7ed4c0868ed23dd3093176fb6d5b</anchor>
      <arglist>[DIST_CODE_LEN]</arglist>
    </member>
    <member kind="variable">
      <type>uch</type>
      <name>_length_code</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a970df3a5fad079dfda5bbc4cb6d11b42</anchor>
      <arglist>[MAX_MATCH-MIN_MATCH+1]</arglist>
    </member>
    <member kind="variable">
      <type>local int</type>
      <name>base_length</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a8d04da861714d277b95e3b45f97f4e10</anchor>
      <arglist>[LENGTH_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local int</type>
      <name>base_dist</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>af61be67e8bcd86435b28d981d652a6ec</anchor>
      <arglist>[D_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local static_tree_desc</type>
      <name>static_l_desc</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>ad553480a12b19dfb3b08e200e19a1375</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>local static_tree_desc</type>
      <name>static_d_desc</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a716c27cf743b894c43da63a9f5e0b0c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>local static_tree_desc</type>
      <name>static_bl_desc</name>
      <anchorfile>trees_8c.html</anchorfile>
      <anchor>a5a995d10bfad4ef5f1faaac496a8aa64</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>trees.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>trees_8h</filename>
    <member kind="variable">
      <type>local const ct_data</type>
      <name>static_ltree</name>
      <anchorfile>trees_8h.html</anchorfile>
      <anchor>a6768b3bbdf10808bdddd44b13349c5a2</anchor>
      <arglist>[L_CODES+2]</arglist>
    </member>
    <member kind="variable">
      <type>local const ct_data</type>
      <name>static_dtree</name>
      <anchorfile>trees_8h.html</anchorfile>
      <anchor>a32b77d04af5dc26c5312766355386cec</anchor>
      <arglist>[D_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>const uch</type>
      <name>_dist_code</name>
      <anchorfile>trees_8h.html</anchorfile>
      <anchor>a4b07c410c27f6182d3b8404b3ae824c8</anchor>
      <arglist>[DIST_CODE_LEN]</arglist>
    </member>
    <member kind="variable">
      <type>const uch</type>
      <name>_length_code</name>
      <anchorfile>trees_8h.html</anchorfile>
      <anchor>a438c19d3b28bf2456a1876d3e4f02a00</anchor>
      <arglist>[MAX_MATCH-MIN_MATCH+1]</arglist>
    </member>
    <member kind="variable">
      <type>local const int</type>
      <name>base_length</name>
      <anchorfile>trees_8h.html</anchorfile>
      <anchor>ac443b72d29eda7b511a746eab5411199</anchor>
      <arglist>[LENGTH_CODES]</arglist>
    </member>
    <member kind="variable">
      <type>local const int</type>
      <name>base_dist</name>
      <anchorfile>trees_8h.html</anchorfile>
      <anchor>a8aa132d4ec076aeec45b2fe616ce9db9</anchor>
      <arglist>[D_CODES]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>zconf.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>zconf_8h</filename>
    <includes id="tinysigcomp__config_8h" name="tinysigcomp_config.h" local="yes" imported="no">tinysigcomp_config.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>const</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a2c212835823e3c54a8ab6d95c652660e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_MEM_LEVEL</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a8e9fefb3d20386851ca693946127ab2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_WBITS</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>abd2f406ac1f9c49236376115d78ccb5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OF</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>aa6c3c093d0900e1d96d07c91b53c1b5d</anchor>
      <arglist>(args)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZEXTERN</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a49b7f5eccfcc99e53178581632bc9589</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZEXPORT</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a181415878f03b95a7094309d824a14b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZEXPORTVA</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>af38d1853df635eee27f1a02f96457b22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FAR</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>aef060b3456fdcc093a7210a762d5f2ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SEEK_SET</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a0d112bae8fd35be772185b6ec6bcbe64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SEEK_CUR</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a4c8d0b76b470ba65a43ca46a88320f39</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SEEK_END</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>ad2a2e6c114780c3071efd24f16c7f7d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>z_off_t</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a8cf4661a88ec12823d7cbdd3fa629eb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>Byte</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>ae3a497195d617519e5353ea7b417940f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>uInt</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a87d141052bcd5ec8a80812a565c70369</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long</type>
      <name>uLong</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>acd2a5701a3aecf6700d2c66c606ecb40</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Byte FAR</type>
      <name>Bytef</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>aeb722a888064be47e12d05f692e0f407</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>char FAR</type>
      <name>charf</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a0f3f92a9c02f6f688bf081aabf230212</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int FAR</type>
      <name>intf</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>aa857123283d7630b35e9b1d427dd6438</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uInt FAR</type>
      <name>uIntf</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>adddbe74608d318334285e01f8a56fa5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uLong FAR</type>
      <name>uLongf</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a0426a5a0ed418ec4ae15af8281c64254</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Byte const *</type>
      <name>voidpc</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a7db54413d7060e4b57868c2b23c0ec1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Byte FAR *</type>
      <name>voidpf</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>a04c731495379021454c8e61439dc88ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Byte *</type>
      <name>voidp</name>
      <anchorfile>zconf_8h.html</anchorfile>
      <anchor>aa3397ff439b5e076528b04bc9b8b086a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>zlib.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>zlib_8h</filename>
    <includes id="zconf_8h" name="zconf.h" local="yes" imported="no">zconf.h</includes>
    <class kind="struct">z_stream_s</class>
    <class kind="struct">gz_header_s</class>
    <class kind="struct">internal_state</class>
    <member kind="define">
      <type>#define</type>
      <name>ZLIB_VERSION</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a51dc9dc65e483b3222c1f0f6a87eb76e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZLIB_VERNUM</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a6413cdfb4cdc0b5a09cf9b5f3f8f48ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_NO_FLUSH</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ac3b62e37b9922d9496014515acd6083a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_PARTIAL_FLUSH</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>adbd49089a5611593f05ac80c9e12cc7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_SYNC_FLUSH</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aa32ca175522e0ef314f4dc0b0196bada</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_FULL_FLUSH</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aed4fbb038c3973aacf52fccffbc44b53</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_FINISH</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a876a626ba14eb12eea4bbf6823b6ca50</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_BLOCK</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a73e5f4c24156c63ba863014396685062</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_OK</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a8cb802b6a11e926478dc6547c514c65c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_STREAM_END</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a7d94a4a852228a899864a0053162e7a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_NEED_DICT</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a32916ba675b2d06463da2576b6a49d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_ERRNO</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a3c66d64c427c0337c64e578537478737</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_STREAM_ERROR</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aad97cde83a9a4bb77f5c3b03bb68e483</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_DATA_ERROR</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a9eda83d428a71752d148f38b5dbe04f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_MEM_ERROR</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ae2be6be871e377c6e2c25a7296ce320c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_BUF_ERROR</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a425e2f0664e1b1f75bf2bffab88d353d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_VERSION_ERROR</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a7634cdd55f955ee1be99c065c6977371</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_NO_COMPRESSION</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a496a4523776bcfa14568cd083b59f24c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_BEST_SPEED</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ad5222ae19b77c6e83815d07ca5d03fd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_BEST_COMPRESSION</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a8db83edd9dea51f8cf0afd1727838b95</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_DEFAULT_COMPRESSION</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>af5387baee05124298e431ebe6fe96b17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_FILTERED</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>af01ccb5a9db8d415cde987d9553ad200</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_HUFFMAN_ONLY</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a1eec3d3720ba91fd54fa08d2757c1785</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_RLE</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>af52c1220e7193d2da78332d75f9251ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_FIXED</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aaa5c996fbc985d5ec43f99112c0ac4a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_DEFAULT_STRATEGY</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a580d7c2527e653ddd783109271750849</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_BINARY</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a2bfe22ed89d3553335d18a4c3cab67af</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_TEXT</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ac5c9021ac9271248094c6b93c61e1f02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_ASCII</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a9a852d9163cdb10687403d4e79dda06e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_UNKNOWN</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a4be54d9d68d40adb73f87492284cc290</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_DEFLATED</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a27a96226f3dc88e4743131c632adf32f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z_NULL</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ad15afa2776b41b2f4d8582dd152bd736</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>zlib_version</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a1f228220c3e6935eb171761c61487059</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>deflateInit</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ae7440c1c5a8becba6dbf410275dc0a5c</anchor>
      <arglist>(strm, level)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>inflateInit</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ad7c6797b618699f70f61323c5184f26e</anchor>
      <arglist>(strm)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>deflateInit2</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a59b575a521efe7ea0d3b9dd3c8d315c1</anchor>
      <arglist>(strm, level, method, windowBits, memLevel, strategy)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>inflateInit2</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a611fc206e69f34e812ca8b590982fdd5</anchor>
      <arglist>(strm, windowBits)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>inflateBackInit</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a9253571ea0fc77cc53c330c0411b8c19</anchor>
      <arglist>(strm, windowBits, window)</arglist>
    </member>
    <member kind="typedef">
      <type>voidpf alloc_func</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a8614d49a5dd53697f4689073c0dbecbb</anchor>
      <arglist>((voidpf opaque, uInt items, uInt size))</arglist>
    </member>
    <member kind="typedef">
      <type>struct z_stream_s</type>
      <name>z_stream</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>afa60092f4e0b9bc4f23b41c6930463f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>z_stream FAR *</type>
      <name>z_streamp</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a6ede25c84d3c9f8632305866acffb1d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct gz_header_s</type>
      <name>gz_header</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a2c394ffb61e707fba3c6b1a36704b305</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>gz_header FAR *</type>
      <name>gz_headerp</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a40e9dcc294796d99b25e98fb06477fc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>voidp</type>
      <name>gzFile</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>acd0143ddd532551631ecc7093fd7adfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN const char *ZEXPORT zlibVersion</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ab1bbaa8881cd84bb4d6c306e3a315241</anchor>
      <arglist>((void))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflate</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a2e6d9561156aac56578c2f1434dc2595</anchor>
      <arglist>((z_streamp strm, int flush))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateEnd</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aaf595349427894f1dbaff9b509791256</anchor>
      <arglist>((z_streamp strm))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateSetDictionary</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>af4ceec9e4a1b0328f7bb73a6f423d2c0</anchor>
      <arglist>((z_streamp strm, const Bytef *dictionary, uIntdictLength))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateCopy</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ae1a33ab7164b2fbab7683c4bea4c50c4</anchor>
      <arglist>((z_streamp dest, z_streamp source))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateParams</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a35cdc947fd49a1bc22481945422bb97e</anchor>
      <arglist>((z_streamp strm, int level, int strategy))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateTune</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a769be8bd85feda1b2dcb502abc010b23</anchor>
      <arglist>((z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN uLong ZEXPORT deflateBound</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ab8bfcdd5ca05f2544694149a771644fa</anchor>
      <arglist>((z_streamp strm, uLong sourceLen))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflatePrime</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a4891be63c26cb4dc463c8b2fba7e5ae2</anchor>
      <arglist>((z_streamp strm, int bits, int value))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateSetHeader</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a77ea653416e54d2d8415fecdcfd2472c</anchor>
      <arglist>((z_streamp strm, gz_headerp head))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT inflateBack</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a8df5cf36f8df181376ad722b2c13c8c2</anchor>
      <arglist>((z_streamp strm, in_func in, void FAR *in_desc, out_func out, void FAR *out_desc))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT compress</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a85df695aadb45de9c814f7a11f74705c</anchor>
      <arglist>((Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT compress2</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aa38fec9fd654c448681296d8bfee871a</anchor>
      <arglist>((Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN uLong ZEXPORT compressBound</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a4d4f28ef1b8cbd7884aa62fc9f7ab22b</anchor>
      <arglist>((uLong sourceLen))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN gzFile ZEXPORT gzopen</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aefa7cf8ffe7d0612cce1682e8eddc033</anchor>
      <arglist>((const char *path, const char *mode))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN gzFile ZEXPORT gzdopen</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a60335f5e60905450d17d415a99db1d0e</anchor>
      <arglist>((int fd, const char *mode))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzsetparams</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a4bf8da17c7ec075cfbe8ba5340897cfd</anchor>
      <arglist>((gzFile file, int level, int strategy))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzread</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>afe848f00a307b58004de5a0ed5c7a98d</anchor>
      <arglist>((gzFile file, voidp buf, unsigned len))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzwrite</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ab8124cca4e0111abc3a089ac1fc53ecf</anchor>
      <arglist>((gzFile file, voidpc buf, unsigned len))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORTVA gzprintf</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aa16722ec242cc8e52e5a9bcb07f6681a</anchor>
      <arglist>((gzFile file, const char *format,...))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzputs</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>afce6ce4068efaf8d5035a06a2a8ef8d4</anchor>
      <arglist>((gzFile file, const char *s))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN char *ZEXPORT gzgets</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aeb249b54a8b291ffedd96a0f3309d17b</anchor>
      <arglist>((gzFile file, char *buf, int len))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzputc</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a71bf5da80bd11fd776592fc319d6305c</anchor>
      <arglist>((gzFile file, int c))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzgetc</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a200aee6968107e38df3efcf80e9b696f</anchor>
      <arglist>((gzFile file))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzungetc</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a731d6b0156abd7f9b38ccb8d1841f371</anchor>
      <arglist>((int c, gzFile file))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT gzflush</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a1f60ebb9d0a34dc0246d08f8c17892e4</anchor>
      <arglist>((gzFile file, int flush))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN z_off_t ZEXPORT gzseek</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a036580ed18d33791beb30eee4a381233</anchor>
      <arglist>((gzFile file, z_off_t offset, int whence))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN const char *ZEXPORT gzerror</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a8719df7ce51af4f5f1b2ed8e8454a56c</anchor>
      <arglist>((gzFile file, int *errnum))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN uLong ZEXPORT adler32</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a58d297b6efb8e8abf76451b5be91eac9</anchor>
      <arglist>((uLong adler, const Bytef *buf, uInt len))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN uLong ZEXPORT adler32_combine</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aac77e0399fd64cf9e6467ff38b29bf80</anchor>
      <arglist>((uLong adler1, uLong adler2, z_off_t len2))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN uLong ZEXPORT crc32</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a6fd2dcbf92d74eb1debc04f1ad4a7064</anchor>
      <arglist>((uLong crc, const Bytef *buf, uInt len))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN uLong ZEXPORT crc32_combine</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a35b6e16ca9bee651ef93d8c6aeadee77</anchor>
      <arglist>((uLong crc1, uLong crc2, z_off_t len2))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateInit_</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aee7a3a2881b7bfd06796c01120975964</anchor>
      <arglist>((z_streamp strm, int level, const char *version, int stream_size))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT inflateInit_</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a6fd21cd770ef0aeb9689108a28c17848</anchor>
      <arglist>((z_streamp strm, const char *version, int stream_size))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT deflateInit2_</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>ad6065a31da6d85fc705e644f578e7493</anchor>
      <arglist>((z_streamp strm, intlevel, intmethod, int windowBits, int memLevel, int strategy, const char *version, int stream_size))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT inflateInit2_</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a2f12a7cc3ddf8face3b53e6a7df08f24</anchor>
      <arglist>((z_streamp strm, intwindowBits, const char *version, int stream_size))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT inflateBackInit_</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a740e2f0087d5e04edd8d19fd124e0114</anchor>
      <arglist>((z_streamp strm, int windowBits, unsigned char FAR *window, const char *version, int stream_size))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN const char *ZEXPORT zError</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>aeb600ab48f5eefd73b51e2e9d94d40cf</anchor>
      <arglist>((int))</arglist>
    </member>
    <member kind="function">
      <type>ZEXTERN int ZEXPORT inflateSyncPoint</type>
      <name>OF</name>
      <anchorfile>zlib_8h.html</anchorfile>
      <anchor>a2e797b0d60f7efb11e86f4161207d166</anchor>
      <arglist>((z_streamp z))</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>zutil.c</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>zutil_8c</filename>
    <includes id="zutil_8h" name="zutil.h" local="yes" imported="no">zutil.h</includes>
    <class kind="struct">internal_state</class>
    <member kind="function">
      <type>const char *ZEXPORT</type>
      <name>zlibVersion</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>a705cd706dfe534aa9161a253a1c9a81a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>uLong ZEXPORT</type>
      <name>zlibCompileFlags</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>a864d90914dbbc92e246503b147b553eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *ZEXPORT</type>
      <name>zError</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>a43d0ecf9c894ba2ed44845a6ffc36209</anchor>
      <arglist>(int err)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>zmemcpy</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>aaeaf4c4bfd0bbb073ce42fe420bb828a</anchor>
      <arglist>(Bytef *dest, const Bytef *source, uInt len)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>zmemcmp</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>a6590fb2f333a07dc593b873440d7c6ab</anchor>
      <arglist>(Bytef *s1, const Bytef *s2, uInt len) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>zmemzero</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>ae92187bde62d7c4e84b231576d291881</anchor>
      <arglist>(Bytef *dest, uInt len)</arglist>
    </member>
    <member kind="function">
      <type>voidp malloc</type>
      <name>OF</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>add8b03ae5c2c602b728ebb3b21931371</anchor>
      <arglist>((uInt size))</arglist>
    </member>
    <member kind="function">
      <type>voidp calloc</type>
      <name>OF</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>a2be754c5363f22e1db3e80698a568ead</anchor>
      <arglist>((uInt items, uInt size))</arglist>
    </member>
    <member kind="function">
      <type>void free</type>
      <name>OF</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>a8f8149fe81b707d93e4b55388d11a430</anchor>
      <arglist>((voidpf ptr))</arglist>
    </member>
    <member kind="function">
      <type>voidpf</type>
      <name>zcalloc</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>acd6de6ee77b7c44353e61511b6eb1d90</anchor>
      <arglist>(voidpf opaque, unsigned items, unsigned size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>zcfree</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>ac5167b77f4a79cb68ba27c598b52485e</anchor>
      <arglist>(voidpf opaque, voidpf ptr)</arglist>
    </member>
    <member kind="variable">
      <type>const char *const</type>
      <name>z_errmsg</name>
      <anchorfile>zutil_8c.html</anchorfile>
      <anchor>ace9cfadbe3490be7fea6728608c4428d</anchor>
      <arglist>[10]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>zutil.h</name>
    <path>C:/Projects/Doubango/tinySIGCOMP/src/</path>
    <filename>zutil_8h</filename>
    <includes id="zlib_8h" name="zlib.h" local="yes" imported="no">zlib.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ZLIB_INTERNAL</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a59d4d7ef4d73655c759211ec33a5d44f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>local</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a08023ea6765c99d60a6a3840cd07156e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_MSG</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a66466516a4a71f3c78ccc40d203c92e5</anchor>
      <arglist>(err)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERR_RETURN</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a0fcc2fe26110263e812ce50919b6a5c1</anchor>
      <arglist>(strm, err)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_WBITS</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>aef084f32d9ca8b9273a1c826b89b979a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEF_MEM_LEVEL</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a19c8e60fa32d4ddf303ce988ba97af61</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STORED_BLOCK</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a9f650bc9c1fbd9499098e3a70d295b8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STATIC_TREES</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a0f63a9090acfe4f653f117b638b289d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DYN_TREES</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a40a71de753262f20a0bcacc2304e7b19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_MATCH</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a7984ceb1ff48ac91172976b91ed43f60</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_MATCH</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ab7a9188dce25dac8d3a970c01b89b0c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRESET_DICT</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a1156d73d88538d122acfa852b95492bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OS_CODE</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a919da7d9b61966c3af796ab42e618bef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F_OPEN</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a7d40e22cfc0ad4087bf371d61290a0f5</anchor>
      <arglist>(name, mode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Assert</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a5c8d8d889189d1461d10df831f3cd40e</anchor>
      <arglist>(cond, msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Trace</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a93bbe97269cf8304b452a4d472dba191</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Tracev</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a67eab10382e740297e8a59d69f85464e</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Tracevv</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ae65182ff511f0b12345c20b6533f32d9</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Tracec</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a15ce9096835263938f62694aee7a0a99</anchor>
      <arglist>(c, x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Tracecv</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a54a66ee51c61d67607b64678b055af6b</anchor>
      <arglist>(c, x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZALLOC</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a39679d03501d691bdfc62a3324cb9548</anchor>
      <arglist>(strm, items, size)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ZFREE</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a71d7c4f186ae3a055367c92db5a8ab18</anchor>
      <arglist>(strm, addr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRY_FREE</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a8a5cef3599266dcaa129ed959a42bcf6</anchor>
      <arglist>(s, p)</arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>uch</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>af3307af5922c72924a837559c801a8a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uch FAR</type>
      <name>uchf</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>aaa6fdf02a2fdaf3731f17a19920ddb0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned short</type>
      <name>ush</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a3754180d606d4ed15468d15d9665aa2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ush FAR</type>
      <name>ushf</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ab854e4722acf30ada8a7a71b58d1b238</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long</type>
      <name>ulg</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>abd6f60bf9450af2ecb94097a32c19a64</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void zmemcpy</type>
      <name>OF</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ac40b32ad09bcfb95fa13459dd0bd44fd</anchor>
      <arglist>((Bytef *dest, const Bytef *source, uInt len))</arglist>
    </member>
    <member kind="function">
      <type>int zmemcmp</type>
      <name>OF</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ab54c0c502433f939a51beae56b93c690</anchor>
      <arglist>((const Bytef *s1, const Bytef *s2, uInt len))</arglist>
    </member>
    <member kind="function">
      <type>void zmemzero</type>
      <name>OF</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ac1e8b0d84844c91143ab66679c2a0441</anchor>
      <arglist>((Bytef *dest, uInt len))</arglist>
    </member>
    <member kind="function">
      <type>voidpf zcalloc</type>
      <name>OF</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a8ddde111a12dcffe2ab5854a44f9fb23</anchor>
      <arglist>((voidpf opaque, unsigned items, unsigned size))</arglist>
    </member>
    <member kind="function">
      <type>void zcfree</type>
      <name>OF</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>a49e951898640714825f7fbde87a1e05b</anchor>
      <arglist>((voidpf opaque, voidpf ptr))</arglist>
    </member>
    <member kind="variable">
      <type>const char *const</type>
      <name>z_errmsg</name>
      <anchorfile>zutil_8h.html</anchorfile>
      <anchor>ace9cfadbe3490be7fea6728608c4428d</anchor>
      <arglist>[10]</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>tcomp_udp_compression_page</name>
    <title>SigComp UDP compression</title>
    <filename>tcomp_udp_compression_page</filename>
  </compound>
  <compound kind="page">
    <name>tcomp_udp_decompression_page</name>
    <title>SigComp UDP decompression</title>
    <filename>tcomp_udp_decompression_page</filename>
  </compound>
  <compound kind="group">
    <name>tcomp_manager_group</name>
    <title>SigComp manager.</title>
    <filename>group__tcomp__manager__group.html</filename>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_manager_compress</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga5760b64e23e067e209a18a8480688493</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize, const void *input_ptr, tsk_size_t input_size, void *output_ptr, tsk_size_t output_size, tsk_bool_t stream)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_manager_decompress</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga821375d468c0baa3581a6d19ecb76c2f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *input_ptr, tsk_size_t input_size, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>tsk_size_t</type>
      <name>tcomp_manager_getNextStreamMessage</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac075422d0661cad2559083e6a700b78f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_manager_provideCompartmentId</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gaaa9bcc86de2877a1fd8070154985d83f</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_result_t *lpResult)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tcomp_manager_closeCompartment</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gad7a281742f5bde5ce1f7a25071f38944</anchor>
      <arglist>(tcomp_manager_handle_t *handle, const void *compartmentId, tsk_size_t compartmentIdSize)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setDecompression_Memory_Size</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac19567fc1105a2c6e6458354f95b61d1</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint32_t dms)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setState_Memory_Size</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gabc8ca6c18fe8574f2d87380c68d5b6a5</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint32_t sms)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setCycles_Per_Bit</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga8b5db5469830967d9fe3bedbe07425ef</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint8_t cpb)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_setSigComp_Version</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gad98c7dc09af9202e928a4754fc30f257</anchor>
      <arglist>(tcomp_manager_handle_t *handle, uint8_t version)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_addCompressor</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga4da0272938736caa99f328aea5fd7b21</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_removeCompressor</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>gac8f21e909564d587f73e1402d07eb453</anchor>
      <arglist>(tcomp_manager_handle_t *handle, tcomp_compressor_compress_f compressor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_addSipSdpDictionary</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga8e18b881be5fcfb412fb71fdaeb82e58</anchor>
      <arglist>(tcomp_manager_handle_t *handle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tcomp_manager_addPresenceDictionary</name>
      <anchorfile>group__tcomp__manager__group.html</anchorfile>
      <anchor>ga6093219506324383c563ae8f4caece46</anchor>
      <arglist>(tcomp_manager_handle_t *handle)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>config_s</name>
    <filename>structconfig__s.html</filename>
    <member kind="variable">
      <type>ush</type>
      <name>good_length</name>
      <anchorfile>structconfig__s.html</anchorfile>
      <anchor>a12e4b2a1125383087b30cb6522b71197</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>max_lazy</name>
      <anchorfile>structconfig__s.html</anchorfile>
      <anchor>a0910f67b3c94cbb754281a3dcdbb86b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>nice_length</name>
      <anchorfile>structconfig__s.html</anchorfile>
      <anchor>afc59200fff87f1713bc9b36660fbc88e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>max_chain</name>
      <anchorfile>structconfig__s.html</anchorfile>
      <anchor>a8da0c5aeae38cf413453b4e2bafd57e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>compress_func</type>
      <name>func</name>
      <anchorfile>structconfig__s.html</anchorfile>
      <anchor>aa35582a2ad4899fa71fd73d730682d98</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ct_data_s</name>
    <filename>structct__data__s.html</filename>
    <member kind="variable">
      <type>union ct_data_s::@0</type>
      <name>fc</name>
      <anchorfile>structct__data__s.html</anchorfile>
      <anchor>aa2b16a21e6fd8013b418fb713126ac4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>freq</name>
      <anchorfile>unionct__data__s_1_1@0.html</anchorfile>
      <anchor>a149c8e4e0d6521908212876304180082</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>code</name>
      <anchorfile>unionct__data__s_1_1@0.html</anchorfile>
      <anchor>aae706e2e35581fc2749c42c4061172c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>union ct_data_s::@1</type>
      <name>dl</name>
      <anchorfile>structct__data__s.html</anchorfile>
      <anchor>a0ee8aa4a315dabf19b88b68f3017fbde</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>dad</name>
      <anchorfile>unionct__data__s_1_1@1.html</anchorfile>
      <anchor>aa141db0795588eea4ebed74db9f9e7bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>len</name>
      <anchorfile>unionct__data__s_1_1@1.html</anchorfile>
      <anchor>ae3d331541f37d03a7a6b51c6ccabac9e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>gz_header_s</name>
    <filename>structgz__header__s.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>text</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>acb727b700580681a32b288d9ebc9b762</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uLong</type>
      <name>time</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a4d66380f18f556788b252ca3bb0492a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>xflags</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a5890059f779baab7fc72bca5e3900552</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>os</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a0c98ab56e4779bc5f994fdff60723115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>extra</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a2b260b4b03b66af0a115704504632b68</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>extra_len</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a5ded8c4f212b31a3ad229496d74f1239</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>extra_max</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a8dcbcbd67c34445b41842b73a6d768f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>name</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>adbc87684a91f1ff13ca81f2202630b34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>name_max</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>af5b7779d893562b8e759efd582d4fcba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>comment</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>ac4e81bcead752082c53d69e098298de1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>comm_max</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a6cec1c0c3a9ac1b3c0a8975e382a2b7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>hcrc</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>ab54066d1aca7e674fbc6d5579cc48894</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>done</name>
      <anchorfile>structgz__header__s.html</anchorfile>
      <anchor>a5992b274cfdcacdbc1fa8347fd01ebde</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>IndexValuePair_s</name>
    <filename>struct_index_value_pair__s.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>index</name>
      <anchorfile>struct_index_value_pair__s.html</anchorfile>
      <anchor>a852b86a2eaee9852ada7a43e61e311a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>value</name>
      <anchorfile>struct_index_value_pair__s.html</anchorfile>
      <anchor>a900b69af7d674b8db3bceae754b4955a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>internal_state</name>
    <filename>structinternal__state.html</filename>
    <member kind="variable">
      <type>z_streamp</type>
      <name>strm</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a56100d9cd9199027a4ef8ea99b8165fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>status</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a6e27f49150e9a14580fb313cc2777e00</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>pending_buf</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>add8dff920bf5363d3293c060ccb4132b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ulg</type>
      <name>pending_buf_size</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a8ef2d4721624a3781154877f4d576674</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>pending_out</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a197dc2c83bdda00d19744269e48f3277</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>pending</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>affd92665b1d074510ee8cfa69ae42265</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>wrap</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a49468a16361d11e78ebbfc41f2a5aa6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>gz_headerp</type>
      <name>gzhead</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ae4ff80e9adb2d87f1eb7f1c8194add53</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>gzindex</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a17f00293243666fb41e774190d970599</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Byte</type>
      <name>method</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a41227eae8a136d330560407e787f2351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>last_flush</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ab050e3e068eb034e21daa13f3d3e024e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>w_size</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a5b830ab1319a58a0ef52f902355049de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>w_bits</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ac85d3640b140f458974c8b771fbdda04</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>w_mask</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a38b550157a9bb295a6704e7e5698e2f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>window</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a20e3092f0de3f83b9315e5fc3d96abf1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ulg</type>
      <name>window_size</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a71025fb69e44e6fdb31e37cfeaaa6ab2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Posf *</type>
      <name>prev</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a801108600ea34ac568dece2e7be61600</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Posf *</type>
      <name>head</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a643ef758b8821126405cab7927c23328</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>ins_h</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a5f63de75e2e8ff3b68f02f718e23ec5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>hash_size</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>af257a6025a926c1ac782286f66d95a1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>hash_bits</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a722089d43eaa7cae28a259c12979e14d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>hash_mask</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ae5b4314e1a5171e5873f70fe018af7b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>hash_shift</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a9ec0ba01a1f947808ca393e4be491fa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long</type>
      <name>block_start</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a8d5f86eed0d6045ca005fe96021deef4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>match_length</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a9d18e9a0d290188df6b22c839d7b784c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>IPos</type>
      <name>prev_match</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a85a4927e76db64dd8ba5ce0da33d7304</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>match_available</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a45892abd08471a3734d78fed86e658e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>strstart</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a2aefcb07963d0ec89075ad73d284be11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>match_start</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>acd379fdd3b23383d35bff8ab081d071d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>lookahead</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ae4449747880a57274043a10cd719c617</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>prev_length</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a8c8621137ed382bdbec5a3bc213e7ade</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>max_chain_length</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a0cb72c41aa2f20d5d5da95099afd5373</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>max_lazy_match</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a05713b0262180deedb5501958802abe0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>level</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>acf4d33ee4cff36f69b924471174dcb11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>strategy</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>acc89aafdc65ecd15f010b7c6fc295522</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>good_match</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ae99f64e663d0cf8ccd04b82891865ef7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>nice_match</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a477f14aaa7517ee48d0762b6339e404e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct ct_data_s</type>
      <name>dyn_ltree</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ad516f2f1ec9134632f06790d23e2c96c</anchor>
      <arglist>[HEAP_SIZE]</arglist>
    </member>
    <member kind="variable">
      <type>struct ct_data_s</type>
      <name>dyn_dtree</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a9b02fe2ee922fa2378deae53cfd1bd83</anchor>
      <arglist>[2 *D_CODES+1]</arglist>
    </member>
    <member kind="variable">
      <type>struct ct_data_s</type>
      <name>bl_tree</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a709654afea8a59be597cba0d71803a8c</anchor>
      <arglist>[2 *BL_CODES+1]</arglist>
    </member>
    <member kind="variable">
      <type>struct tree_desc_s</type>
      <name>l_desc</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>acf7b1d7f022417907661d37363028305</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tree_desc_s</type>
      <name>d_desc</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a271f5aff94bc69bc031b8b9676645aa9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct tree_desc_s</type>
      <name>bl_desc</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>aa9db1fdbde1efc7bd07eb2f5b95a4a60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>bl_count</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a9e049d031155d0cd7a8dd46d4204481a</anchor>
      <arglist>[MAX_BITS+1]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>heap</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ad89d220326d796f308c1d8493dba3c94</anchor>
      <arglist>[2 *L_CODES+1]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>heap_len</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a2fba52115b6829135ff5c64eaea555a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>heap_max</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a204e9ef382f520484601f5a469fc6dbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uch</type>
      <name>depth</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a933b166cb8c680e0f1534b8b3f8d0b7b</anchor>
      <arglist>[2 *L_CODES+1]</arglist>
    </member>
    <member kind="variable">
      <type>uchf *</type>
      <name>l_buf</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>afc108fba69847767d47ad330d9637ce3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>lit_bufsize</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ac6395f717442666b3042d10c46bb9cab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>last_lit</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ac5a9280972e00453bef446a5fa345f18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ushf *</type>
      <name>d_buf</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>aa02fc8ecb80a7056c744105d9ede8f92</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ulg</type>
      <name>opt_len</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ae9197d310941ebc96f196d73ccde6764</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ulg</type>
      <name>static_len</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a5fb6007de6c68144d7d58c5fec3c3cc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>matches</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>accbd185909002743df649629bec6e515</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>last_eob_len</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>ac3da39014207f30bffca111b3fa8c1b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ush</type>
      <name>bi_buf</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a166a631734f09a3fe307f5447f8dfe39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>bi_valid</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>afd3692889fd372df24d04adf0ea527df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>dummy</name>
      <anchorfile>structinternal__state.html</anchorfile>
      <anchor>a7c1d654b7b6114d7a0abc8d351dd1bcd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>static_tree_desc_s</name>
    <filename>structstatic__tree__desc__s.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>dummy</name>
      <anchorfile>structstatic__tree__desc__s.html</anchorfile>
      <anchor>a7c1d654b7b6114d7a0abc8d351dd1bcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const ct_data *</type>
      <name>static_tree</name>
      <anchorfile>structstatic__tree__desc__s.html</anchorfile>
      <anchor>ab685d5fd60b3dadeca7f768aa15ed032</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const intf *</type>
      <name>extra_bits</name>
      <anchorfile>structstatic__tree__desc__s.html</anchorfile>
      <anchor>afa68649c85f1bb485ba4627b325bb15d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>extra_base</name>
      <anchorfile>structstatic__tree__desc__s.html</anchorfile>
      <anchor>ab00260e2abef7dc2d6bb39a107b34c0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>elems</name>
      <anchorfile>structstatic__tree__desc__s.html</anchorfile>
      <anchor>a92ca0166a5b1b021bc3b3e17af819be7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>max_length</name>
      <anchorfile>structstatic__tree__desc__s.html</anchorfile>
      <anchor>ad888a988cf5dcbcdbd97ad63dd32df45</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_buffer_s</name>
    <filename>structtcomp__buffer__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>size</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a2af13611e30c0884ab2e1837c4da8282</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>lpbuffer</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a4e6a9aee7494cddfc58fe7300c1f247a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>index_bytes</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a8a0be01bb7eaa5a3c3497f232be8e1e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>index_bits</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a6c3818beae16f622b3f4bb524cdb6d85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>owner</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a63128a957ecaae32439e46f59e55043b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>P_BIT</name>
      <anchorfile>structtcomp__buffer__s.html</anchorfile>
      <anchor>a9a806c5ee63a025854af6eba58ea3e8c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_compartment_s</name>
    <filename>structtcomp__compartment__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>identifier</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>ad1813f451cfaf8c9700e6329c5ea2f2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_states_L_t *</type>
      <name>local_states</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>af6d33af050833129953760f824168f1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_params_t *</type>
      <name>remote_parameters</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a7b6120c10f2abea33a08675b54b83214</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_params_t *</type>
      <name>local_parameters</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a4dfe3e8067c9ca543cce4a7f4a923bdb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>total_memory_size</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a790e69895499c85ab7cc3fad46592431</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>total_memory_left</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a8773de221e5a7f5d1d81d543c5fa25ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>lpReqFeedback</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>aa0d31da910a22850932bbadbac0ee744</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>lpRetFeedback</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a75fae69d0a3406b3b56585f6a4679a74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TCOMP_DECLARE_COMPRESSORDATA</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a3522f99b579c5d1cbed2d604388b0991</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffers_L_t *</type>
      <name>nacks</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>affcc6a890ca01e44e4678c4ce238c4ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>nacks_history_count</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a3ec80f337c5e68d7a434d9ef49bc34c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__compartment__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_compressordisp_s</name>
    <filename>structtcomp__compressordisp__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__compressordisp__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_compressor_compress_f</type>
      <name>compressors</name>
      <anchorfile>structtcomp__compressordisp__s.html</anchorfile>
      <anchor>ab83049c7478d778de1bd99c08ac11cfa</anchor>
      <arglist>[TCOMP_MAX_COMPRESSORS]</arglist>
    </member>
    <member kind="variable">
      <type>const tcomp_statehandler_t *</type>
      <name>stateHandler</name>
      <anchorfile>structtcomp__compressordisp__s.html</anchorfile>
      <anchor>a377adfc9677811b73dbd9bd8bb87a21e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__compressordisp__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_decompressordisp_s</name>
    <filename>structtcomp__decompressordisp__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__decompressordisp__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const tcomp_statehandler_t *</type>
      <name>stateHandler</name>
      <anchorfile>structtcomp__decompressordisp__s.html</anchorfile>
      <anchor>a377adfc9677811b73dbd9bd8bb87a21e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_stream_buffer_L_t *</type>
      <name>streamBuffers</name>
      <anchorfile>structtcomp__decompressordisp__s.html</anchorfile>
      <anchor>a19007483d3ca8ea9762cabc14ad797ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__decompressordisp__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_deflatedata_s</name>
    <filename>structtcomp__deflatedata__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_deflateStream_t</type>
      <name>stream_1</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a6fe0d57e55ee24b973fe34309b7f8c9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_deflateStream_t</type>
      <name>stream_acked</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a9a176a077b51bd4c898ba0454a456cbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>zLevel</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>ad0303c014db686f5cd57c7c5e235a0c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>zWindowBits</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a2b4466742de3ed1fb9576b0025135a79</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>initialized</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a8ac8977c8c92fcb0ed6caa2e0d04accf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>ghost_copy_offset</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a1fa5028a49f5ddf9154170506681c397</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_state_t *</type>
      <name>ghostState</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a0869aba8dac1803d54d9151a66bdefcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>stream</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>aaccd30b07977c3f95bb3a6801b82da48</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__deflatedata__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_deflateStream_s</name>
    <filename>structtcomp__deflate_stream__s.html</filename>
    <member kind="variable">
      <type>z_stream</type>
      <name>zs</name>
      <anchorfile>structtcomp__deflate_stream__s.html</anchorfile>
      <anchor>a20a4a45dc1eba85dfdde4fea53e1a03e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>dataWaitingAck</name>
      <anchorfile>structtcomp__deflate_stream__s.html</anchorfile>
      <anchor>a783a2dfccc9dc5d7a1565bae6cbced7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>stateful</name>
      <anchorfile>structtcomp__deflate_stream__s.html</anchorfile>
      <anchor>a25e0ab97e1803bb2d7eaa1c40d45a382</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_manager_s</name>
    <filename>structtcomp__manager__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__manager__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_compressordisp_t *</type>
      <name>dispatcher_compressor</name>
      <anchorfile>structtcomp__manager__s.html</anchorfile>
      <anchor>ab4c1fc9443cf2ad610403a3f1b6d60ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_decompressordisp_t *</type>
      <name>dispatcher_decompressor</name>
      <anchorfile>structtcomp__manager__s.html</anchorfile>
      <anchor>aa0bb807172258930b96c6b0798c9afe8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_statehandler_t *</type>
      <name>stateHandler</name>
      <anchorfile>structtcomp__manager__s.html</anchorfile>
      <anchor>a754f0eb3a00277e15a50f7d31b4e9750</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__manager__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_message_s</name>
    <filename>structtcomp__message__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>headerSigComp</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a322a64dd37d9f21429f74e78537e7e0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>isOK</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>aaf906de830e4f3a60757ee41126a16ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>isNack</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a3bf509542952c540bf345d20fb49e6ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>stream_based</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>ad2395656563d506c75df9e586ae735ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const uint8_t *</type>
      <name>startPtr</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a30a781db0d3f736905fc09a49f7c1f4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>totalSize</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>ae723ec2f11cfe6af9b45ec15f2b30309</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tsk_size_t</type>
      <name>header_size</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>ae3015daaf39bc764154ea56e217b271f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>stateId</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>ac0522dc892c683457195c549c7c3c640</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>bytecodes_destination</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a78a5df9269ccf3bc12fe968f5b191220</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>remaining_sigcomp_buffer</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a91c75e56d0c23452345ce4d7c9603b09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>uploaded_UDVM_buffer</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a763e84969e8cf87ffcbb0775d721a9c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>ret_feedback_buffer</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a718e01a3640a8973a47524587c780377</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_nackinfo_t *</type>
      <name>nack_info</name>
      <anchorfile>structtcomp__message__s.html</anchorfile>
      <anchor>a6691355fd42b0f7f770e2cb36aa9eeac</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_nackinfo_s</name>
    <filename>structtcomp__nackinfo__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>version</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>ab22abc2906422da61885ac6c8e6a1a59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>reasonCode</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>a7456084b9e87c3c655d397fe3155ae31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>opcode</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>a5c1b56e6bccc2a95dbddf1a08e56e87d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>pc</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>af21633fb74a6aa87c93baf3caaa58864</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>sha1</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>a8091fffbbfb679a672b21a2edcf42cff</anchor>
      <arglist>[TSK_SHA1_DIGEST_SIZE]</arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>details</name>
      <anchorfile>structtcomp__nackinfo__s.html</anchorfile>
      <anchor>a84cb9e7a29b0903a71ebec069d57ea16</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_params_s</name>
    <filename>structtcomp__params__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>cpbCode</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>ac61d72ccdbffbc5f954a3e3bb4cd31cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>dmsCode</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a1466f673e7dec55e6bdaeb731b5d3412</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>smsCode</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a7803f989e38d38fb439d0e5deabaf045</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>cpbValue</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a2e3cc7088ab1d2dcc961c40516fee1e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>dmsValue</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a4044027f99e4d8a5c227d0d487eebbb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>smsValue</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a12146ce0b16a2a6a972659fc7624006d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>SigComp_version</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>a3b04b3885f0d0d56d9b6fad4e8cc5acf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffers_L_t *</type>
      <name>returnedStates</name>
      <anchorfile>structtcomp__params__s.html</anchorfile>
      <anchor>ab55ca50a7082b3e99205b6cc77aae36e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_reqfeed_s</name>
    <filename>structtcomp__reqfeed__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__reqfeed__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>Q</name>
      <anchorfile>structtcomp__reqfeed__s.html</anchorfile>
      <anchor>a8244ab842968c8fefa4318505dce2e2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>S</name>
      <anchorfile>structtcomp__reqfeed__s.html</anchorfile>
      <anchor>a144c5a43162033ca0ba61e313283eed8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>I</name>
      <anchorfile>structtcomp__reqfeed__s.html</anchorfile>
      <anchor>af09b20fe2ad9b430a57b013b0354f9ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>item</name>
      <anchorfile>structtcomp__reqfeed__s.html</anchorfile>
      <anchor>a9d757d371255e9179c6e44258a3946b1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_result_s</name>
    <filename>structtcomp__result__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>compartmentId</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a5a970fbb60df3ba85e39c754c14dd80e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_state_t *</type>
      <name>statesToCreate</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>aca240e0df8f6a752d9a492022d992271</anchor>
      <arglist>[MAX_TEMP_SATES]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>statesToCreateIndex</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a9c1ccf794337faa9786351ec3e0f466b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_tempstate_to_free_t *</type>
      <name>statesToFree</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>adee425d368286a498e26d4961be13fb6</anchor>
      <arglist>[MAX_TEMP_SATES]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>statesToFreeIndex</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a27b4bb07826f6da1c6f475ab10c1bb4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>streamId</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>ae5d6b8c950c0b947fc410a97ed35f7cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>isStreamBased</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a6e42073b59f96ae1d4282d1952bedaea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>output_buffer</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a9ec40e87d96bcd101755521b51cf5315</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_reqfeed_t *</type>
      <name>req_feedback</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a771db0f67529cfced8222261b053049c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_params_t *</type>
      <name>remote_parameters</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a7b6120c10f2abea33a08675b54b83214</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>ret_feedback</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a6fca330196fe190f48b8fbdfb932efc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>isNack</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a3bf509542952c540bf345d20fb49e6ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>nack_info</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>a817f183ef2c1d43cd77d70dd70406382</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>consumed_cycles</name>
      <anchorfile>structtcomp__result__s.html</anchorfile>
      <anchor>ae3ab31f83ca5587dd03acb2ade9b810f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_state_s</name>
    <filename>structtcomp__state__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>value</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>a3bc458bf1c685c1ff5016723ea76398c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>identifier</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>aae524b3dafa5945a516fcd14fde6012f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>length</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>a1892eba2086d12ac2b09005aeb09ea3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>address</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>a643d30df67a67c5915fde941c934f9f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>instruction</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>aa9a8e88ff3c973e84e7cb5c56d89f887</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>minimum_access_length</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>a8d8e3e7b3d7f8eae76daee0b97b3de67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>retention_priority</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>ac3f4a05b52c8aa07c35fc71c9eacf16b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__state__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_statehandler_s</name>
    <filename>structtcomp__statehandler__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_compartments_L_t *</type>
      <name>compartments</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>a8b073f4024b50ee6ca1bb6471ac22ab3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_params_t *</type>
      <name>sigcomp_parameters</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>adfd4241f436fa6ff6e9e8c5912660bee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_dictionaries_L_t *</type>
      <name>dictionaries</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>a9accdc69af30f1702933b8d960a578ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>hasSipSdpDictionary</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>a9ad6d032a969c355aae2286c4af0f6e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>hasPresenceDictionary</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>a3e0975416d218f88c8fb8ae321477425</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__statehandler__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_stream_buffer_s</name>
    <filename>structtcomp__stream__buffer__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__stream__buffer__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>id</name>
      <anchorfile>structtcomp__stream__buffer__s.html</anchorfile>
      <anchor>a7e290573ef1be67b92a2c745e3b00d1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>buffer</name>
      <anchorfile>structtcomp__stream__buffer__s.html</anchorfile>
      <anchor>afcde768381460aba9b960c1fd35d11ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_SAFEOBJ</name>
      <anchorfile>structtcomp__stream__buffer__s.html</anchorfile>
      <anchor>a9818e976fd73889027b42c32d628169f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_tempstate_to_free_s</name>
    <filename>structtcomp__tempstate__to__free__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__tempstate__to__free__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>partial_identifier_start</name>
      <anchorfile>structtcomp__tempstate__to__free__s.html</anchorfile>
      <anchor>af0daf53e33f69671e6570adfdfba82ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>partial_identifier_length</name>
      <anchorfile>structtcomp__tempstate__to__free__s.html</anchorfile>
      <anchor>ae427caba6fe52721f5f769b6320696d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>identifier</name>
      <anchorfile>structtcomp__tempstate__to__free__s.html</anchorfile>
      <anchor>aae524b3dafa5945a516fcd14fde6012f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tcomp_udvm_s</name>
    <filename>structtcomp__udvm__s.html</filename>
    <member kind="variable">
      <type></type>
      <name>TSK_DECLARE_OBJECT</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>a6cb2c811d40d14ceb34bd3b2e7dc5e6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned</type>
      <name>isOK</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>aaf906de830e4f3a60757ee41126a16ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_message_t *</type>
      <name>sigCompMessage</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>aa8a11b96f1dca369938fc65d46749922</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_statehandler_t *</type>
      <name>stateHandler</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>a754f0eb3a00277e15a50f7d31b4e9750</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_result_t *</type>
      <name>lpResult</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>abd6fbba2ff0803a1d518559492e4f387</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>maximum_UDVM_cycles</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>a00ebb3b1c30c33e4f6060fb3ffdc3d52</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>consumed_cycles</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>ae3ab31f83ca5587dd03acb2ade9b810f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>tcomp_buffer_handle_t *</type>
      <name>memory</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>ac10ddd456bab2902136a0f65aff558d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>executionPointer</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>a3d92f1ad667088add68916888ba25e27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>last_memory_address_of_instruction</name>
      <anchorfile>structtcomp__udvm__s.html</anchorfile>
      <anchor>a3140c25beb6e3887a9e5771b7cc97738</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tree_desc_s</name>
    <filename>structtree__desc__s.html</filename>
    <member kind="variable">
      <type>ct_data *</type>
      <name>dyn_tree</name>
      <anchorfile>structtree__desc__s.html</anchorfile>
      <anchor>ad116dfb62ac4b04ee9c1adf5d5d3209e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>max_code</name>
      <anchorfile>structtree__desc__s.html</anchorfile>
      <anchor>a4808321b49374743053b834929f16ba9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>static_tree_desc *</type>
      <name>stat_desc</name>
      <anchorfile>structtree__desc__s.html</anchorfile>
      <anchor>ae8e86e2a7bc63a34bcaf33b4aab76562</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>z_stream_s</name>
    <filename>structz__stream__s.html</filename>
    <member kind="variable">
      <type>Bytef *</type>
      <name>next_in</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a19880e07a77d10515147e23b6379dd49</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>avail_in</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a7eb50ccbfeb2368dd84c3a97f409692a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uLong</type>
      <name>total_in</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>aa301948b62e927cb5b34c7fa48b369e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bytef *</type>
      <name>next_out</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a6926e8fa9e456364c4a931da1508662e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uInt</type>
      <name>avail_out</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>aaf14708cee6cfc16ac27f3cff686f8d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uLong</type>
      <name>total_out</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>ace45f7b8af1035bc35ee7436fdd41081</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>msg</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a32d2f5216cddb59c7cc8fb2806a7e727</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>struct internal_state FAR *</type>
      <name>state</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>ac21f94cab4411ce1c77771033cff040a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>alloc_func</type>
      <name>zalloc</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a0b2f68e949d83a93be0e4d0b88148c80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>free_func</type>
      <name>zfree</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a7587bb1e332fc8f57ef74071eeec8070</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>voidpf</type>
      <name>opaque</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>afb43594753f721206f8751bdc6f311ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>data_type</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a47eb6f1029f8333d4e55b08296f900dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uLong</type>
      <name>adler</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>a74bb92610e11a5515a286f04f906d0db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uLong</type>
      <name>reserved</name>
      <anchorfile>structz__stream__s.html</anchorfile>
      <anchor>ae3bf2150b8009b089ae6878004146180</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
