/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
#ifndef _TEST_SIPURI_H
#define _TEST_SIPURI_H

const char* uris[] = {

    //== SIP:
    "sip:123.com",
    "sip:open-ims.test",
    "sip:pcscf.open-ims.test:4060;lr;transport=udp",
    "sip:2233392625@sip2sip.info",
    "sip:alice@iatlanta.com;p1=23",
    "sip:*666*@atlanta.com",
    "sip:#66#@atlanta.com", // should fail: # must be replaced with %23
    "sip:alice:secretword@atlanta.com",
    "sip:alice:secretword@atlanta.com:65535;transport=tcp",
    "sip:+1-212-555-1212:1234@gateway.com;user=phone",
    "sip:alice@192.0.2.4:5060",
    "sip:alice@[1111::aaa:bbb:ccc:ddd]:5060",
    "sip:atlanta.com",
    "sip:alice@[1111::aaa:bbb:ccc:ddd]",
    "sip:alice@[1111::aaa:bbb:ccc:ddd]:5060;user=phone",
    "sip:alice@1111::aaa:bbb:ccc:ddd", // should fail
    "sip:alice@[::127]",
    "sip:ss2.biloxi.example.com;lr",// FIXME
    "sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sip:alice@atlanta.com;maddr=239.255.255.1;ttl=15",
    "sip:alice@atlanta.com;comp=sigcomp",
    "sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sip:alice@atlanta.com?subject=project%20x&priority=urgent",

    //== SIPS:
    "sips:alice@atlanta.com",
    "sips:alice:secretword@atlanta.com;transport=tcp",
    "sips:+1-212-555-1212:1234@gateway.com;user=phone",
    "sips:alice@192.0.2.4",
    "sips:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sips:alice@atlanta.com;maddr=239.255.255.1;ttl=15",
    "sips:alice@atlanta.com;comp=sigcomp",
    "sips:atlanta.com;method=REGISTER?to=alice%40atlanta.com",
    "sips:alice@atlanta.com?subject=project%20x&priority=urgent",

    //== TEL:
    "tel:+1-201-555-0123",
    "tel:7042;phone-context=example.com;ff=ff",
    "tel:863-1234;phone-context=+1-914-555",
    "tel:#666#",
};
void test_uri_tostring(const tsip_uri_t *uri)
{
    char* ret = tsip_uri_tostring(uri, 1, 1);
    TSK_DEBUG_INFO("uri_to_string=%s", ret);
    TSK_FREE(ret);
}

void test_uri_parser()
{
    int i;
    tsk_list_item_t *item = 0;

    for(i=0; i<sizeof(uris)/sizeof(const char*); i++) {
        tsip_uri_t *uri = tsip_uri_parse(uris[i], tsk_strlen(uris[i]));

        printf("\n== Parsing {{ %s }} ==\n\n", uris[i]);

        if(uri) {
            printf("scheme: %s\n", uri->scheme);
            printf("user-name: %s\n", uri->user_name);
            printf("password: %s\n", uri->password);
            printf("host: %s\n", uri->host);
            printf("port: %d\n", uri->port);
            printf("host-type: %s\n", uri->host_type == host_ipv4 ? "IPv4" : (uri->host_type == host_ipv6 ? "IPv6" : (uri->host_type == host_hostname ? "HOSTNAME" : "UNKNOWN")) );

            printf("---PARAMS---\n");

            /* dump all parameters */
            tsk_list_foreach(item, uri->params) {
                tsk_param_t* param = item->data;
                printf("-->%s=%s\n", param->name, param->value);
            }

            printf("Is-secure: %s\n", TSIP_URI_IS_SECURE(uri) ? "YES" : "NO");

            test_uri_tostring(uri);
        }
        else {
            printf("INVALID SIP URI.\n");
        }

        printf("\n\n");
        getchar();

        TSK_OBJECT_SAFE_FREE(uri);
    }
}

struct test_uri_bundle {
    const char* uri1;
    const char* uri2;

    unsigned match:1;
};

// From RFC 3261 - 19.1.4 URI Comparison
struct test_uri_bundle test_uri_bundles[] = {
    /* Match */
    { "sip:%61lice@atlanta.com;transport=TCP", "sip:alice@AtLanTa.CoM;Transport=tcp", 1 },
    { "sip:carol@chicago.com", "sip:carol@chicago.com;newparam=5", 1 },
    { "sip:carol@chicago.com", "sip:carol@chicago.com;security=on", 1 },
    { "sip:carol@chicago.com;newparam=5", "sip:carol@chicago.com;security=on", 1 },
    { "sip:biloxi.com;transport=tcp;method=REGISTER?to=sip:bob%40biloxi.com", "sip:biloxi.com;method=REGISTER;transport=tcp?to=sip:bob%40biloxi.com", 1 },
    { "sip:alice@atlanta.com?subject=project%20x&priority=urgent", "sip:alice@atlanta.com?priority=urgent&subject=project%20x", 1 },

    /* Do not match */
    { "SIP:ALICE@AtLanTa.CoM;Transport=udp", "sip:alice@AtLanTa.CoM;Transport=UDP", 0 }, /* different usernames */
    { "sip:bob@biloxi.com", "sip:bob@biloxi.com:5060", 0 }, /* can resolve to different ports */
    { "sip:bob@biloxi.com", "sip:bob@biloxi.com;transport=udp", 0 }, /* can resolve to different transports */
    { "sip:bob@biloxi.com", "sip:bob@biloxi.com:6000;transport=tcp", 0 }, /* can resolve to different port and transports */
    { "sip:carol@chicago.com", "sip:carol@chicago.com?Subject=next%20meeting", 0 }, /* different header component */
    { "sip:bob@phone21.boxesbybob.com", "sip:bob@192.0.2.4", 0 }, /* even though that's what phone21.boxesbybob.com resolves to*/
};

void test_uri_cmp()
{
    size_t i;

    for(i=0; i< sizeof(test_uri_bundles)/sizeof(struct test_uri_bundle); i++) {
        tsip_uri_t *uri1 = tsip_uri_parse(test_uri_bundles[i].uri1, tsk_strlen(test_uri_bundles[i].uri1));
        tsip_uri_t *uri2 = tsip_uri_parse(test_uri_bundles[i].uri2, tsk_strlen(test_uri_bundles[i].uri2));

        if(tsk_object_cmp(uri1, uri2) && test_uri_bundles[i].match) {
            TSK_DEBUG_ERROR("URI Comparison failed.");
        }
        else {
            TSK_DEBUG_INFO("URI Comparison ok.");
        }

        TSK_OBJECT_SAFE_FREE(uri1);
        TSK_OBJECT_SAFE_FREE(uri2);
    }
}

void test_uri()
{
    test_uri_parser();
    test_uri_cmp();
}

#endif /* _TEST_SIPURI_H */
