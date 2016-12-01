/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file txcap.c
 * @brief RFC 4825 (XCAP) implementation.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#include "txcap.h"

#include "tinyhttp/thttp_url.h"

/**@defgroup txcap_stack_group XCAP stack
*/


/**@mainpage tinyXCAP API Overview
*
*
* <h1>16 XCAP</h1>
*
* The XCAP Framework is mainly based on RFC 4825 and uses tinyHTTP project. The framework can be used to implements advanced OMA functionalities such Enhanced Address Book, Presence Authorization Rules, Service Configuration …
* At startup the stack will load all supported AUIDs with their default values.
*
*
*
* <h2>16.1 Initialization</h2>
*
* As the XCAP stack depends on the HTTP/HTTPS stack (<a href="http://www.doubango.org/API/tinyHTTP/">tinyHTTP</a>) which uses the network library (<a href="http://www.doubango.org/API/tinyNET/"tinyNET</a>), you MUST call <a href="http://doubango.org/API/tinyNET/tnet_8c.html#affba6c2710347476f615b0135777c640"> tnet_startup()</a> before using any XCAP function (txcap_*). <br>
* <a href="http://doubango.org/API/tinyNET/tnet_8c.html#ac42b22a7ac5831f04326aee9de033c84"> tnet_cleanup()</a> is used to terminate use of network functions. <br>
* The example below demonstrates how to create and start a XCAP stack. In this example, the xcap-root URI is http://doubango.org:8080/services and the SIP AOR (used as XUI) is sip:bob@doubango.org.
*
* @code
txcap_stack_handle_t* stack = tsk_null;
int ret;

tnet_startup();

stack = txcap_stack_create(test_stack_callback, "sip:bob@doubango.org", "mysecret", "http://doubango.org:8080/services",

	// options
	TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_TIMEOUT, "6000"),

	// stack-level headers (not mandatory)
	TXCAP_STACK_SET_HEADER("Pragma", "No-Cache"),
	TXCAP_STACK_SET_HEADER("Connection", "Keep-Alive"),
	TXCAP_STACK_SET_HEADER("X-3GPP-Intended-Identity", "sip:bob@doubango.org"),
	TXCAP_STACK_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
	TXCAP_STACK_SET_NULL());

if((ret = txcap_stack_start(stack))){
	goto bail;
}

// …………

bail:
	TSK_OBJECT_SAFE_FREE(stack);
tnet_cleanup();

* @endcode
*
* The stack-level headers will be added in all outgoing requests.
* A stack is a well-defined object and must be destroyed by using @a TSK_OBJECT_SAFE_FREE() macro. The stack will be automatically stopped when destroyed.
*
*
*
* <h2>16.2	Application Unique ID (AUID) object</h2>
*
* An AUID object is defined by:
* -	An id (e.g. “xcap-caps”),
* -	A MIME-Type (e.g. “application/xcap-caps+xml”),
* -	A namespace (e.g. “urn:ietf:params:xml:ns:xcap-caps”),
* -	A document name (e.g. “index”), which defines the name of the default document associated with this AUID
* -	A scope (“global” or “users”)
*
* At startup, the stack will load all supported AUIDs with their default values. You can at any time change these values or register your own AUID object. The list of default AUIDs with their default values are shown in the next sections.
* When you are about to send a request, it’s not mandatory to use a registered AUID but it’s easier to generate the selector as all parameters are pre-configured.
*
*
*
* <h3>16.2.1	Default AUIDs</h3>
*
* The table below shows the default AUIDs as they are defined by the stack at startup.
* <TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139 BGCOLOR="#d9d9d9">
*
* 			<P STYLE="border: none; padding: 0in"><B>Id</B></P>
* 		</TD>
* 		<TD WIDTH=132 BGCOLOR="#d9d9d9">
* 			<P STYLE="border: none; padding: 0in"><B>MIME-Type</B></P>
* 		</TD>
* 		<TD WIDTH=120 BGCOLOR="#d9d9d9">
* 			<P STYLE="border: none; padding: 0in"><B>Namespace</B></P>
*
* 		</TD>
* 		<TD WIDTH=84 BGCOLOR="#d9d9d9">
* 			<P STYLE="border: none; padding: 0in"><B>Document </B>
* 			</P>
* 		</TD>
* 		<TD WIDTH=53 BGCOLOR="#d9d9d9">
* 			<P STYLE="border: none; padding: 0in"><B>Scope</B></P>
* 		</TD>
*
* 		<TD WIDTH=129 BGCOLOR="#d9d9d9">
* 			<P STYLE="border: none; padding: 0in"><B>Reference</B></P>
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">xcap-caps</P>
* 		</TD>
*
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/xcap-caps+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
* 			<P STYLE="border: none; padding: 0in">urn:ietf:params:xml:ns:xcap-caps</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P STYLE="border: none; padding: 0in">index</P>
*
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">global</P>
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">RFC
* 			4825 section 12.1</P>
* 		</TD>
* 	</TR>
*
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">resource-lists</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/resource-lists+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
*
* 			<P STYLE="border: none; padding: 0in">urn:ietf:params:xml:ns:resource-lists</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P STYLE="border: none; padding: 0in">index</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
*
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">RFC
* 			4826 section 3.4.1</P>
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">rls-services</P>
*
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/rls-services+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
* 			<P STYLE="border: none; padding: 0in">urn:ietf:params:xml:ns:rls-services&quot;</P>
* 		</TD>
* 		<TD WIDTH=84>
*
* 			<P STYLE="border: none; padding: 0in">index</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">RFC
* 			4826 section 4.4.1</P>
*
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">pres-rules</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/auth-policy+xml</P>
*
* 		</TD>
* 		<TD WIDTH=120>
* 			<P STYLE="border: none; padding: 0in">urn:ietf:params:xml:ns:pres-rules</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P STYLE="border: none; padding: 0in">index</P>
* 		</TD>
* 		<TD WIDTH=53>
*
* 			<P STYLE="border: none; padding: 0in">users</P>
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">RFC
* 			5025 section 9.1</P>
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
*
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">org.openmobilealliance.pres-rules</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/auth-policy+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
* 			<P STYLE="border: none; padding: 0in">urn:ietf:params:xml:ns:common-policy</P>
*
* 		</TD>
* 		<TD WIDTH=84>
* 			<P STYLE="border: none; padding: 0in">pres-rules</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
* 		</TD>
* 		<TD WIDTH=129>
*
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">[OMA-TS-Presence_SIMPLE_XDM-V1_1-20080627-A]
* 			section 5.1.1.2</P>
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">directory</P>
* 		</TD>
* 		<TD WIDTH=132>
*
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/directory+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
* 			<P STYLE="border: none; padding: 0in">urn:ietf:params:xml:ns:xcap-directory</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P STYLE="border: none; padding: 0in">directory.xml</P>
*
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">draft-garcia-simple-xcap-directory-00
* 			section 9.1</P>
* 		</TD>
* 	</TR>
*
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">org.openmobilealliance.xcap-directory</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/vnd.oma.xcap-directory+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
*
* 			<P STYLE="border: none; padding: 0in">urn:oma:xml:xdm:xcap-directory</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P STYLE="border: none; padding: 0in">directory.xml</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
*
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">[OMA-TS-XDM_Core-V1_1-20080627-A]
* 			section 6.7.2.1</P>
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">org.openmobilealliance.pres-content</P>
*
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/vnd.oma.pres-content+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">urn:oma:xml:prs:pres-content</P>
* 		</TD>
* 		<TD WIDTH=84>
*
* 			<P STYLE="border: none; padding: 0in">oma_status-icon/rcs_status_icon</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
* 		</TD>
* 		<TD WIDTH=129>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">[OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C]
* 			section 5.1.2</P>
*
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">org.openmobilealliance.conv-history</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="en-GB" STYLE="border: none; padding: 0in">application/vnd.oma.im.history-list+xml</P>
*
* 		</TD>
* 		<TD WIDTH=120>
* 			<P STYLE="border: none; padding: 0in">urn:oma:xml:im:history-list</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">conv-history</P>
* 		</TD>
* 		<TD WIDTH=53>
*
* 			<P STYLE="border: none; padding: 0in">users</P>
* 		</TD>
* 		<TD WIDTH=129>
* 			<P STYLE="border: none; padding: 0in">[OMA-TS-IM_XDM-V1_0-20070816-C]
* 			<SPAN LANG="fr-FR">section</SPAN>
* 			5.1.2</P>
* 		</TD>
* 	</TR>
*
* 	<TR VALIGN=TOP>
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">org.openmobilealliance.deferred-list</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="en-GB" STYLE="border: none; padding: 0in">application/vnd.oma.im.deferred-list+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
*
* 			<P STYLE="border: none; padding: 0in">urn:oma:xml:im:history-list</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">deferred-list</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P STYLE="border: none; padding: 0in">users</P>
*
* 		</TD>
* 		<TD WIDTH=129>
* 			<P STYLE="border: none; padding: 0in">[OMA-TS-IM_XDM-V1_0-20070816-C]
* 			<SPAN LANG="fr-FR">section</SPAN>
* 			5.2.2</P>
* 		</TD>
* 	</TR>
* 	<TR VALIGN=TOP>
*
* 		<TD WIDTH=139>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">org.openmobilealliance.group-usage-list</P>
* 		</TD>
* 		<TD WIDTH=132>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">application/vnd.oma.group-usage-list+xml</P>
* 		</TD>
* 		<TD WIDTH=120>
* 			<PRE CLASS="western" STYLE="margin-bottom: 0.2in; border: none; padding: 0in">rn:ietf:params:xml:ns:resource-lists</PRE><P STYLE="border: none; padding: 0in">
*
* 			&nbsp;</P>
* 		</TD>
* 		<TD WIDTH=84>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">index</P>
* 		</TD>
* 		<TD WIDTH=53>
* 			<P LANG="fr-FR" STYLE="border: none; padding: 0in">users</P>
* 		</TD>
*
* 		<TD WIDTH=129>
* 			<P STYLE="border: none; padding: 0in">[OMA-TS-XDM_Shared-V1_1-20080627-A]
* 			subclause 5.2.2</P>
* 		</TD>
* 	</TR>
* </TABLE>
*
*
* <h3>16.2.2	AUID registration</h3>
*
* === The code below shows how to register two AUIDs. If the AUID object already exist (case-insensitive comparison on the id), then it will be updated. All fields are mandatory (id, mime-type, namespace, document and scope).
*
* @code
txcap_stack_set(stack,
	TXCAP_STACK_SET_AUID("my-xcap-caps", "application/my-xcap-caps+xml", "urn:ietf:params:xml:ns:my-xcap-caps", "my-document", tsk_true),
	TXCAP_STACK_SET_AUID("my-resource-lists", "application/my-resource-lists+xml", "urn:ietf:params:xml:ns:my-resource-lists", "my-document", tsk_false),

	TXCAP_STACK_SET_NULL()); // mandatory
*
* @endcode
*
* The stack should be created as shown at section 16.1.
* Only AUIDs which don’t appear in the table above should be registered using this method
*
*

* <h2>16.3	Selector</h2>
*
* The selector is a helper function which could be used to construct XCAP URIs. XCAP URI is constructed as per RFC 4825 section 6. @a TXCAP_SELECTOR_NODE_SET*() macros are used to build a complete and well-formed URI (already percent encoded).
* All examples below assume that our AOR (used as XUI) is sip:bob@doubango.com, we are using the ‘rcs’list and the xcap-root URI is http://doubango.org:8080/services. All these parameters should be set when the stack is created. You will also notice that TXCAP_SELECTOR_NODE_SET_NULL() macro is used to ends the node selection parameters passed to txcap_selector_get_url(), it’s mandatory and should always be the last one.
*
* -	Select XDMS capabilities:
* @code
char* urlstring = txcap_selector_get_url(stack, "xcap-caps",
		TXCAP_SELECTOR_NODE_SET_NULL());
	TSK_DEBUG_INFO("%s\n", urlstring);
	TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/xcap-caps/global/index</i>
* -	Select 'resource-lists' document
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index</i>
*
* -	Select 'rcs' list
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22rcs\%22\%5D</i>
*
* -	Select the 2nd list
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_POS("list", 2),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B2\%5D</i>
*
* -	Select the 4th list using wildcard
* @code
urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_POS("*", 4),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/*\%5B4\%5D</i>
*
* -	Select bob's entry
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", "sip:bob@doubango.com"),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22rcs\%22\%5D/entry\%5B\@uri=\%22sip:bob@doubango.org\%22\%5D</i>
*
* -	Select bob’s display-name
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", "sip:bob@doubango.org"),
		TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22rcs\%22\%5D/entry\%5B\@uri=\%22sip:bob@doubango.org\%22\%5D/display-name</i>
*
* -	Select the display-name of the 1st entry
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_POS("entry", 1),
		TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22rcs\%22\%5D/entry\%5B1\%5D/display-name</i>
*
* -	Select bob from position 23
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_POS_ATTRIBUTE("entry", 23, "uri", "sip:bob@doubango.org"),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22rcs\%22\%5D/entry\%5B23\%5D\%5B\@uri=\%22sip:bob@doubango.org\%22\%5D</i>
*
* -	Namespaces test
* @code
char* urlstring = txcap_selector_get_url(stack, "resource-lists",
		TXCAP_SELECTOR_NODE_SET_NAME("foo"),
		TXCAP_SELECTOR_NODE_SET_NAME("a:bar"),
		TXCAP_SELECTOR_NODE_SET_NAME("b:baz"),
		TXCAP_SELECTOR_NODE_SET_NAMESPACE("a", "urn:namespace1-uri"),
		TXCAP_SELECTOR_NODE_SET_NAMESPACE("b", "urn:namespace2-uri"),
		TXCAP_SELECTOR_NODE_SET_NULL());
TSK_DEBUG_INFO("%s\n", urlstring);
TSK_FREE(urlstring);
* @endcode
*
* Console Output:
* <i>http://doubango.org:8080/services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/foo/a:bar/b:baz%3Fxmlns(a=\%22urn:namespace1-uri\%22)xmlns(b=\%22urn:namespace2-uri\%22)</i>
*
* <h2>16.4	XDMC Usage</h2>
*
* It is assumed that the address of the XDMS (or aggregation Proxy) is “doubango.org:8080/services” and thus the XCAP Root URI is “doubango.org:8080/services”. “sip:bob@doubango.org” will be used as the XUI.
* An XDMC can perform twelve actions:
* -	@ref txcap_action_create_element(): Creates new element by sending a HTTP/HTTPS PUT request. The default Content-Type will be “application/xcap-el+xml”, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_create_document(): Creates new document by sending a HTTP/HTTPS PUT request. The default Content-Type will be the one associated with the AUID of the document, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_create_attribute(): Creates new attribute by sending a HTTP/HTTPS PUT request. The default Content-Type will be “application/xcap-att+xml”, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_replace_element(): Replaces an element by sending a HTTP/HTTPS PUT request. The default Content-Type will be “application/xcap-el+xml”, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_replace_document(): Replaces a document by sending a HTTP/HTTPS PUT request. The default Content-Type will be the one associated with the AUID of the document, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_replace_attribute(): Replaces an attribute by sending a HTTP/HTTPS PUT request. The default Content-Type will be “application/xcap-att+xml”, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_fetch_element(): Retrieves an element from the XDMS by sending a HTTP/HTTPS GET request. The default Content-Type will be “application/xcap-el+xml”, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_fetch_document(): Retrieves a document from the XDMS by sending a HTTP/HTTPS GET request. The default Content-Type will be the one associated with the AUID of the document, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_fetch_attribute(): Retrieves an attribute from the XDMS by sending a HTTP/HTTPS GET request. The default Content-Type will be “application/xcap-att+xml”, unless you provide your own Content-Type by using TXCAP_ACTION_SET_HEADER().
* -	@ref txcap_action_delete_element(): Deletes an element from the XDMS by sending a HTTP/HTTPS DELETE request.
* -	@ref txcap_action_delete_document(): Deletes a document from the XDMS by sending a HTTP/HTTPS DELETE request.
* -	@ref txcap_action_delete_attribute(): Deletes an attribute from the XDMS by sending a HTTP/HTTPS DELETE request.
*
* To understand how the stack is created, please refer to section 16.1.
*
*
* <h3>16.4.1	Retrieving XDMS capabilities</h3>
* === The code below shows how an XDMC obtains the XDMS capabilities document.
* @code
int ret = txcap_action_fetch_document(stack,
		// selector
		TXCAP_ACTION_SET_SELECTOR("xcap-caps",
			TXCAP_SELECTOR_NODE_SET_NULL()),
		// ends parameters
		TXCAP_ACTION_SET_NULL()
		);
* @endcode
*
* The XDMC will send:
* @code
GET /services/xcap-caps/global/index HTTP/1.1
Host: doubango.org:8080
Connection: Keep-Alive
User-Agent: XDM-client/OMA1.1
X-3GPP-Intended-Identity: sip:bob@doubango.org
Content-Type: application/xcap-caps+xml
* @endcode
*
* <h3>16.4.2	Address Book</h3>
*
* ===	The code below shows how an XDMC obtains URI Lists (Address Book).
*
* @code
int ret = txcap_action_fetch_document(stack,
		// action-level options
		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
		//action-level headers
		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
		// selector
		TXCAP_ACTION_SET_SELECTOR("resource-lists",
			TXCAP_SELECTOR_NODE_SET_NULL()),
		// ends parameters
		TXCAP_ACTION_SET_NULL()
		);
* @endcode
*
* The XDMC will send:
* @code
GET /services/resource-lists/users/sip:bob@doubango.org/index HTTP/1.1
Host: doubango.org:8080
Connection: Keep-Alive
User-Agent: XDM-client/OMA1.1
X-3GPP-Intended-Identity: sip:bob@doubango.org
Pragma: No-Cache
Content-Type: application/resource-lists+xml
* @endcode
*
* ===	The code below shows how to add a new list to the address book
* @code
int ret = txcap_action_create_element(stack,
		// selector
		TXCAP_ACTION_SET_SELECTOR("resource-lists",
			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
			TXCAP_SELECTOR_NODE_SET_NULL()),
		// payload
		TXCAP_ACTION_SET_PAYLOAD(PAYLOAD, strlen(PAYLOAD)),
		// ends parameters
		TXCAP_ACTION_SET_NULL()
		);
* @endcode
*
* The XDMC will send:
* @code
PUT /services/resource-lists/users/sip:bob@doubano.org/index/~~/resource-lists/list\%5B\@name=\%22newlist\%22\%5D HTTP/1.1
Host: doubango.org:8080
Content-Length: 110
Connection: Keep-Alive
User-Agent: XDM-client/OMA1.1
X-3GPP-Intended-Identity: sip:bob@doubango.org
Content-Type: application/xcap-el+xml

<list name="newlist" xmlns="urn:ietf:params:xml:ns:resource-lists"><display-name>newlist</display-name></list>
* @endcode
*
* ===	The code below shows how to retrieve the previously added list
* @code
int ret = txcap_action_fetch_element(stack,
		// action-level selector
		TXCAP_ACTION_SET_SELECTOR("resource-lists",
			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
			TXCAP_SELECTOR_NODE_SET_NULL()),
		// ends parameters
		TXCAP_ACTION_SET_NULL()
		);
* @endcode
*
* The XDMC will send:
* @code
GET /services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22newlist\%22\%5D HTTP/1.1
Host: doubango.org:8080
Connection: Keep-Alive
User-Agent: XDM-client/OMA1.1
X-3GPP-Intended-Identity: sip:bob@doubango.org
Content-Type: application/xcap-el+xml
** @endcode
*
* ===	The code below shows how to add a new entry (“sip:alice@doubango.org”) to the previously added list
* @code
int ret = txcap_action_create_element(stack,
		// selector
		TXCAP_ACTION_SET_SELECTOR("resource-lists",
			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
			TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("entry", "uri", “sip:alice@doubango.org”),
			TXCAP_SELECTOR_NODE_SET_NULL()),
		// payload
		TXCAP_ACTION_SET_PAYLOAD(PAYLOAD, strlen(PAYLOAD)),
		// ends parameters
		TXCAP_ACTION_SET_NULL()
		);
* @endcode
*
* The XDMC will send:
* @code
PUT /services/resource-lists/users/sip:bob@doubango.org/index/~~/resource-lists/list\%5B\@name=\%22newlist\%22\%5D/entry\%5B\@uri=\%22sip:alice@doubango.org\%22\%5D HTTP/1.1
Host: doubango.org:8080
Content-Length: 125
Connection: Keep-Alive
User-Agent: XDM-client/OMA1.1
X-3GPP-Intended-Identity: sip:bob@doubango.org
Content-Type: application/xcap-el+xml

<entry uri="sip:alice@doubango.org" xmlns="urn:ietf:params:xml:ns:resource-lists"><display-name>alice</display-name></entry>
* @endcode
*
* <h3>16.4.3	Obtaining Presence Content Document </h3>
*
* === The code below shows how an XDMC obtains the Presence Content document (avatar).
*
* @code
int ret = txcap_action_fetch_document(stack,
		// action-level options
		TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
		//action-level headers
		TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
		// selector
		TXCAP_ACTION_SET_SELECTOR("org.openmobilealliance.pres-content",
			TXCAP_SELECTOR_NODE_SET_NULL()),
		// ends parameters
		TXCAP_ACTION_SET_NULL()
		);
* @endcode
*
* The XDMC will send:
*
* @code
GET /services/org.openmobilealliance.pres-content/users/sip:mamadou@micromethod.com/oma_status-icon/rcs_status_icon HTTP/1.1
Host: doubango.org:8080
Connection: Keep-Alive
User-Agent: XDM-client/OMA1.1
X-3GPP-Intended-Identity: sip:bob@doubango.org
Pragma: No-Cache
Content-Type: application/vnd.oma.pres-content+xml
* @endcode
*/

/** Internal function used to set options.
*/
int __txcap_stack_set(txcap_stack_t* self, va_list *app)
{
    txcap_stack_param_type_t curr;
    tsk_bool_t cred_updated = tsk_false;

    if(!self || !self->http_session) {
        return -1;
    }

    while((curr = va_arg(*app, txcap_stack_param_type_t)) != xcapp_null) {
        switch(curr) {
        case xcapp_option: {
            /* (txcap_stack_option_t)ID_ENUM, (const char*)VALUE_STR */
            txcap_stack_option_t ID_ENUM = va_arg(*app, txcap_stack_option_t);
            const char* VALUE_STR = va_arg(*app, const char*);
            switch(ID_ENUM) {
            case TXCAP_STACK_OPTION_ROOT: {
                tsk_strupdate(&self->xcap_root, VALUE_STR);
                break;
            }
            /* PASSWORD and XUI are not used as options in the HTTP/HTTPS stack */
            case TXCAP_STACK_OPTION_PASSWORD: {
                tsk_strupdate(&self->password, VALUE_STR);
                cred_updated = tsk_true;
                break;
            }
            case TXCAP_STACK_OPTION_XUI: {
                tsk_strupdate(&self->xui, VALUE_STR);
                cred_updated = tsk_true;
                break;
            }
            case TXCAP_STACK_OPTION_LOCAL_IP: {
                thttp_stack_set(self->http_stack, THTTP_STACK_SET_LOCAL_IP(VALUE_STR),
                                THTTP_STACK_SET_NULL());
                break;
            }
            case TXCAP_STACK_OPTION_LOCAL_PORT: {
                int port = atoi(VALUE_STR);
                thttp_stack_set(self->http_stack, THTTP_STACK_SET_LOCAL_PORT(port),
                                THTTP_STACK_SET_NULL());
                break;
            }
            default: {
                /* HTTP Options: MUST be changed to valid HTTP option enum */
                switch(ID_ENUM) {
                case TXCAP_STACK_OPTION_TIMEOUT:
                    tsk_options_add_option(&((thttp_session_t*)self->http_session)->options, THTTP_SESSION_OPTION_TIMEOUT, VALUE_STR);
                    break;
                case TXCAP_STACK_OPTION_TTL:
                    tsk_options_add_option(&((thttp_session_t*)self->http_session)->options, THTTP_SESSION_OPTION_TTL, VALUE_STR);
                    break;
                default:
                    TSK_DEBUG_WARN("%d is an invalid XCAP option", ID_ENUM);
                    break;
                }

                break;
            }
            }
            break;
        }

        case xcapp_header: {
            /* (const char*)NAME_STR, (const char*)VALUE_STR */
            const char* NAME_STR = va_arg(*app, const char*);
            const char* VALUE_STR = va_arg(*app, const char*);
            if(VALUE_STR == (const char*)-1) {
                tsk_params_remove_param(((thttp_session_t*)self->http_session)->headers, NAME_STR);
            }
            else {
                tsk_params_add_param(&((thttp_session_t*)self->http_session)->headers, NAME_STR, VALUE_STR);
            }
            break;
        }

        case xcapp_userdata: {
            /* (const void*)CTX_PTR */
            const void* CTX_PTR = va_arg(*app, const void*);
            ((thttp_session_t*)self->http_session)->userdata = CTX_PTR;
            break;
        }

        case xcapp_auid: {
            /* (const char*)ID_STR, (const char*)MIME_TYPE_STR, (const char*)NS_STR, (const char*)DOC_NAME_STR, (tsk_bool_t)IS_GLOBAL_BOOL */
            const char* ID_STR = va_arg(*app, const char*);
            const char* MIME_TYPE_STR = va_arg(*app, const char*);
            const char* NS_STR = va_arg(*app, const char*);
            const char* DOC_NAME_STR = va_arg(*app, const char*);
            tsk_bool_t IS_GLOBAL_BOOL = va_arg(*app, tsk_bool_t);

            if(txcap_auid_register(self->auids, ID_STR, MIME_TYPE_STR, NS_STR, DOC_NAME_STR, IS_GLOBAL_BOOL)) {
                // do nothing
            }

            break;
        }

        default: {
            TSK_DEBUG_ERROR("NOT SUPPORTED.");
            goto bail;
        }
        } /* switch */
    } /* while */

    if(cred_updated && self->http_session) {
        /* credentials */
        thttp_session_set(self->http_session,
                          THTTP_SESSION_SET_CRED(self->xui, self->password),
                          THTTP_SESSION_SET_NULL());
    }
    return 0;

bail:
    return -2;
}

/**@ingroup txcap_stack_group
* Creates new XCAP stack.
* @param callback Poiner to the callback function to call when new messages come to the transport layer.
* Can be set to Null if you don't want to be alerted.
* @param xui user's id as per RFC 4825 subclause 4. Also used to fill @b "X-3GPP-Intended-Identity" header.
* This paramter is mandatory and must not be null. If for any reason you'd like to update the user's id, then use @ref TXCAP_STACK_SET_XUI().
* @param password user's password used to authenticate to the XDMS.
* This parameter is not mandatory. If for any reason you'd like to update the password, then use @ref TXCAP_STACK_SET_PASSWORD().
* @param xcap_root xcap-root URI as per RFC 4825 subclause 6.1, used to build all request-uris.
* This parameter is not mandatory and must be a valid HTPP/HTTPS URL.
* @param ... User configuration. You must use @a TXCAP_STACK_SET_*() macros to set these options.
* The list of options must always end with @ref TXCAP_STACK_SET_NULL() even if these is no option to pass to the stack.
* @retval A Pointer to the newly created stack if succeed and @a Null otherwise.
* A stack is a well-defined object.
*
* @code
int test_stack_callback(const thttp_event_t *httpevent);

txcap_stack_handle_t* stack = txcap_stack_create(test_stack_callback, "sip:bob@example.com", "mysecret", "http://doubango.org:8080/services",

		// stack-level options
		TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_TIMEOUT, "6000"),

		// stack-level headers
		TXCAP_STACK_SET_HEADER("Connection", "Keep-Alive"),
		TXCAP_STACK_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
		TXCAP_STACK_SET_HEADER("X-3GPP-Intended-Identity", XUI),

		TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref txcap_stack_set
*/
txcap_stack_handle_t* txcap_stack_create(thttp_stack_callback_f callback, const char* xui, const char* password, const char* xcap_root, ...)
{
    txcap_stack_t* ret = tsk_null;

    if(!xui || !xcap_root) {
        TSK_DEBUG_ERROR("Both xui and xcap_root are mandatory and should be non-null");
        goto bail;
    }

    /* check url validity */
    if(!thttp_url_isvalid(xcap_root)) {
        TSK_DEBUG_ERROR("%s is not a valid HTTP/HTTPS url", xcap_root);
        goto bail;
    }

    if(!(ret = tsk_object_new(txcap_stack_def_t, callback, xui, password, xcap_root))) {
        TSK_DEBUG_FATAL("Failed to create the XCAP stack");
        goto bail;
    }
    else {
        /* set parameters */
        va_list ap;
        va_start(ap, xcap_root);
        __txcap_stack_set(ret, &ap);
        va_end(ap);
        /* credendials */
        tsk_strupdate(&ret->xui, xui);
        tsk_strupdate(&ret->password, password);
        if(ret->http_session) {
            thttp_session_set(ret->http_session,
                              THTTP_SESSION_SET_CRED(ret->xui, ret->password),
                              THTTP_SESSION_SET_NULL());
        }
    }

bail:
    return ret;
}

/**@ingroup txcap_stack_group
* Starts the stack.
* @param self A pointer to the stack to stark. The stack shall be created using @ref txcap_stack_create().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref txcap_stack_stop
*/
int txcap_stack_start(txcap_stack_handle_t* self)
{
    int ret = -1;
    txcap_stack_t* stack = self;

    if(!stack) {
        goto bail;
    }

    if(!(ret = thttp_stack_start(stack->http_stack))) {
        //if(!stack->http_session){
        //	stack->http_session = thttp_session_create(stack->http_stack ,
        //		THTTP_SESSION_SET_NULL());
        //}
    }

bail:
    return ret;
}

/**@ingroup txcap_stack_group
* Updates the stack configuration.
* @param self  The XCAP stack to update. The stack shall be created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_STACK_SET_*() macros. MUST ends with @ref TXCAP_STACK_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*
* @code
txcap_stack_handle_t* stack;
// ... create the stack

txcap_stack_set(stack,
		// add new AUIDs
		TXCAP_STACK_SET_AUID("xcap-caps2", "application/xcap-caps2+xml", "urn:ietf:params:xml:ns:xcap-caps2", "index2", tsk_true),
		TXCAP_STACK_SET_AUID("resource-lists2", "application/resource-lists+xml2", "urn:ietf:params:xml:ns:resource-lists2", "index2", tsk_false),
		// stack-level headers
		TXCAP_STACK_SET_HEADER("Connection", "Keep-Alive"),
		TXCAP_STACK_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),

		TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref txcap_stack_create
*/
int txcap_stack_set(txcap_stack_handle_t* self, ...)
{
    int ret = -1;
    va_list ap;

    if(!self) {
        goto bail;
    }

    va_start(ap, self);
    ret = __txcap_stack_set(self, &ap);
    va_end(ap);

bail:
    return ret;
}

/**@ingroup txcap_stack_group
* Stops the stack. The stack must already be started.
* @param self A pointer to the stack to stop. The stack shall be created using @ref txcap_stack_create().
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref txcap_stack_start
*/
int txcap_stack_stop(txcap_stack_handle_t* self)
{
    int ret = -1;
    txcap_stack_t* stack = self;

    if(!stack) {
        goto bail;
    }
    else {
        if(!(ret = thttp_stack_stop(stack->http_stack))) {
            //TSK_OBJECT_SAFE_FREE(stack->http_session);
        }
    }

bail:
    return ret;
}









//========================================================
//	XCAP stack object definition
//
static tsk_object_t* _txcap_stack_create(tsk_object_t * self, va_list * app)
{
    txcap_stack_t *stack = self;
    if(stack) {
        thttp_stack_callback_f callback;
        tsk_safeobj_init(stack);

        callback = va_arg(*app, thttp_stack_callback_f);
        stack->xui = tsk_strdup( va_arg(*app, const char*) );
        stack->password = tsk_strdup( va_arg(*app, const char*) );
        stack->xcap_root = tsk_strdup( va_arg(*app, const char*) );

        /* HTTP/HTTPS stack and session */
        stack->http_stack = thttp_stack_create(callback,
                                               THTTP_STACK_SET_NULL());
        stack->http_session = thttp_session_create(stack->http_stack ,
                              THTTP_SESSION_SET_NULL());

        /* Options */
        stack->options = tsk_list_create();

        /* AUIDs */
        txcap_auids_init(&stack->auids);
    }
    return self;
}

static tsk_object_t* txcap_stack_destroy(tsk_object_t* self)
{
    txcap_stack_t *stack = self;
    if(stack) {
        /* vars */
        TSK_FREE(stack->xui);
        TSK_FREE(stack->password);
        TSK_FREE(stack->xcap_root);

        /* HTTP/HTTPS resources */
        TSK_OBJECT_SAFE_FREE(stack->http_session);
        TSK_OBJECT_SAFE_FREE(stack->http_stack);

        /* Options */
        TSK_OBJECT_SAFE_FREE(stack->options);

        /* AUIDs */
        TSK_OBJECT_SAFE_FREE(stack->auids);

        tsk_safeobj_deinit(stack);
    }
    return self;
}

static const tsk_object_def_t txcap_stack_def_s = {
    sizeof(txcap_stack_t),
    _txcap_stack_create,
    txcap_stack_destroy,
    tsk_null,
};
const tsk_object_def_t *txcap_stack_def_t = &txcap_stack_def_s;
