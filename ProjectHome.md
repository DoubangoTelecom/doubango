<font color='green' size='3'>
<hr />
<strong>Doubango v2.x</strong><br />
<hr />
To all developers: Doubango v1.x is now deprecated. <br />
Please use version v2.x. The source code is under <strong>branches/2.0</strong><br />
</font>
<br />

<h2>Welcome to <a href='http://www.doubango.org'>doubango</a> project!</h2>
<p><a href='http://en.wikipedia.org/wiki/IP_Multimedia_Subsystem'>3GPP IMS</a> (IP Multimedia Subsystem) is the next generation network for delivering<br>
IP multimedia services. IMS is standardized by the 3rd Generation Partnership Project (3GPP). <br />
IMS services could be used over any type of network, such as <a href='http://en.wikipedia.org/wiki/3GPP_Long_Term_Evolution'>3GPP LTE</a>, GPRS, Wireless LAN, CDMA2000 or fixed line.<br>
</p>
<p>
<a href='http://www.doubango.org'>doubango</a> is an experimental, open source, 3GPP IMS/LTE framework for both embedded and desktop systems.<br />
The framework is written in ANSI-C to ease portability and has been carefully designed to efficiently work on embedded systems with limited memory and low computing power and to be extremely portable.</p>

<h2>Proof Of Concept</h2>
<table border='1'>
<tr><td><b>Client-side components</b></td></tr>
<tr>
<td><a href='http://code.google.com/p/boghe/'>Boghe</a></td>
<td>- SIP Video Client for Windows Phone 8 and Surface Pro<br /> - IMS/RCS Client for Windows XP, Vista, 7 and 8</td>
</tr>
<tr>
<td><a href='http://code.google.com/p/imsdroid/'>IMSDroid</a></td>
<td>SIP/IMS Client for Android</td>
</tr>
<tr>
<td><a href='http://code.google.com/p/idoubs/'>iDoubs</a></td>
<td>SIP/IMS VideoPhone for iOS (iPhone, iPad and iPod Touch) and MAC OS X</td>
</tr>

<tr><td><b>Server-side components</b></td></tr>
<tr>
<td><a href='http://webrtc2sip.org'>webrtc2sip</a></td>
<td>Smart SIP and Media Gateway to connect WebRTC endpoints to any SIP-legacy network</td>
</tr>
<tr>
<td><a href='http://code.google.com/p/telepresence/'>telepresence</a></td>
<td>the open source SIP TelePresence system with a porwerfull <a href='http://en.wikipedia.org/wiki/Multipoint_Control_Unit'>MCU</a>
</td>
</tr>
<tr>
<td><a href='http://code.google.com/p/flash2ims/'>Flash2IMS</a></td>
<td>Adobe® Flash® to SIP/IMS Gateway.</td>
</tr>
</table>

See also [webrtc4all](http://code.google.com/p/webrtc4all/), [sipML5](http://code.google.com/p/sipml5/) and [click2dial](https://code.google.com/p/click-2-dial/) components.

<h2>Features</h2>
<ul>
<blockquote><li>SIP (RFC 3261, 3GPP TS 24.229 Rel-9)</li>
<li>IMS-AKA (RFC 3310, 3GPP TS 33.203, 35.205, 35.206, 35.207, 35.208 and 35.909)</li>
<li>Private extension headers for 3GPP</li>
<li>IPv4/IPv6 dual stack</li>
<li>UDP, TCP and TLS</li>
<li>Service-Route Discovery (RFC 3608)</li>
<li>Proxy-CSCF discovery using DHCPv4/v6 or/and DNS NAPTR</li>
<li>SigComp (RFC 3320, 3485, 4077, 4464, 4465, 4896, 5049, 5112 and 1951)</li>
<li>IPSec</li>
<li>Security Agreement (RFC 3329)</li>
<li>NAT Traversal (STUN, TURN and ICE)</li>
<li>Preconditions (RFC 3312, 4032 and 5027)</li>
<li>SMS over IP (3GPP TS 23.038, 24.040, 24.011, 24.341 and 24.451)</li>
<li>ENUM (RFC 3761)</li>
<li>The tel URI for Telephone Numbers (RFC 3966)</li>
<li>SIP SIMPLE (Presence Subscription/Publication and Pager Mode IM)</li>
<li>MMTel (UNI)</li>
<li>SDP (RFC 3264)</li>
<li>(S)RTP and (S)RTCP</li>
<li>(S)AVP, (S)AVP(F) and UDP/TLS/RTP/(S)AVP(F) profiles</li>
<li>DTLS-SRTP(RFC 5763 and 5764)</li>
<li>XCAP (RFC 4825)</li>
<li>MSRP (RFC 4975 and 5547)</li>
<li>Audio/Video (AMR, Speex, G.729, G.722, Opus, G.711, GSM, iLBC, VP8, H.264, H.263, Theora, MP4V-ES...)</li>
<li>Full HD (1080p) video</li>
<li>T.140: Text Over IP (<a href='http://en.wikipedia.org/wiki/Text_over_IP'>http://en.wikipedia.org/wiki/Text_over_IP</a>)</li>
<li>...and many other features</li>
</ul></blockquote>

<h2>The One Voice Profile (4G/LTE)</h2>
<p><a href='http://www.doubango.org'>doubango</a> supports both Voice and SMS over LTE as defined by the <b>One Voice initiative</b>.<br />
For more information, please refer to <a href='http://news.vzw.com/OneVoiceProfile.pdf'>Voice over IMS profile V1.0.0(2009-11)</a>.<br>
<blockquote></p>
<img src='http://doubango.org/LTE_Architecture.png' alt='3GPP LTE Architecture' title='3GPP LTE Architecture' /></blockquote>

<h2>Multimedia Telephony (MMTel)</h2>
<ul>
<li>MMTel Supplementary Services (3GPP TS 24.173, 24607, 24.608, 24.607, 24.604, 24.611, 24.610, 24.606, 24.615 and 24.605)</li>
<li>Supplementary Service Configuration(3GPP TS 24.623)</li>
<li>Ad-Hoc Multi Party Conference (3GPP TS 24.605)</li>
<li>Communication Waiting(3GPP TS 24.615)</li>
<li>Message Waiting Indication(3GPP TS 24.606 and RFC 3842)</li>
<li>Originating/Terminating Identification Restriction(3GPP TS 24.607/608)</li>
<li>Communication Diversion(3GPP TS 24.604)</li>
<li>Communication Barring(3GPP TS 24.611)</li>
</ul>
<br>

<h2>Support</h2>
Please check our <a href='http://code.google.com/p/webrtc2sip/issues/list'>issue tracker</a> or <a href='https://groups.google.com/group/doubango'>developer group</a> if you have any problem.<br>
<br>
For more information, please visit <a href='http://www.doubango.org'><a href='http://www.doubango.org'>http://www.doubango.org</a></a>

<br />
<br />
<b>© 2010-2013 Doubango Telecom</b> <br />
<i>Inspiring the future</i>