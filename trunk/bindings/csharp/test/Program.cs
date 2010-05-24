using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace test
{
    class Program
    {
        const String REALM = "ericsson.com";
        const String USER = "mamadou";
        const String PROXY_CSCF_IP = "192.168.0.13";
        const uint PROXY_CSCF_PORT = 5081;
        const String PASSWORD = "";

        /*
        const String REALM = "sip2sip.info";
        const String USER = "2233392625";
        const String PASSWORD = "d3sb7j4fb8";
        const String PROXY_CSCF_IP = "192.168.0.13";
        const uint PROXY_CSCF_PORT = 5081;
        */

        static void Main(string[] args)
        {
            Boolean success;

            /* Create callbacks */
            sipCallback = new MySipCallback();
            sipDebugCallback = new MySipDebugCallback();

            /* Create and configure the IMS/LTE stack */
            sipStack = new SipStack(sipCallback, String.Format("sip:{0}", REALM), String.Format("{0}@{1}", USER, REALM), String.Format("sip:{0}@{1}", USER, REALM));
            sipStack.setDebugCallback(sipDebugCallback);
            sipStack.addHeader("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
            sipStack.addHeader("Privacy", "header; id");
            sipStack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
            sipStack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");
            
            /* Sets Proxy-CSCF */
            success = sipStack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "tcp", "ipv4");
            /* Starts the stack */
            success = sipStack.start();

            /* Set Password */
            //stack.setPassword(PASSWORD);

            /* Send REGISTER */
            regSession = new RegistrationSession(sipStack);
            regSession.addCaps("+g.oma.sip-im");
            regSession.addCaps("+g.3gpp.smsip");
            regSession.addCaps("language", "\"en,fr\"");
            regSession.setExpires(35);
            regSession.Register();

            /* Send SUBSCRIBE(reg) */
            /*subSession = new SubscriptionSession(stack);
            subSession.addHeader("Event", "reg");
            subSession.addHeader("Accept", "application/reginfo+xml");
            subSession.addHeader("Allow-Events", "refer, presence, presence.winfo, xcap-diff, conference");
            subSession.setExpires(35);

            //String s = "H\x00\x15eg\x09\x20mamadou";
            //subSession.setPayload(s, (uint)s.Length);
            subSession.Subscribe();*/

            Console.Read();

            sipStack.stop();
        }

        static RegistrationSession regSession;
        static SubscriptionSession subSession;
        static MySipCallback sipCallback;
        static SipStack sipStack;
        static MySipDebugCallback sipDebugCallback;
    }

    public class MySipDebugCallback : SipDebugCallback
    {
        public override int OnDebugInfo(string message)
        {
            Console.WriteLine(".NET____" + message);
            return 0;
        }

        public override int OnDebugWarn(string message)
        {
            Console.WriteLine(".NET____" + message);
            return 0;
        }

        public override int OnDebugError(string message)
        {
            Console.WriteLine(".NET____" + message);
            return 0;
        }

        public override int OnDebugFatal(string message)
        {
            Console.WriteLine(".NET____" + message);
            return 0;
        }
    }

    public class MySipCallback : SipCallback
    {
        public MySipCallback()
            : base()
        {
        }

        private static bool isSipCode(short code)
        {
            return (code <=699 && code >=100);
        }

        private static bool is2xxCode(short code)
        {
            return (code <= 299 && code >= 200);
        }

        private static bool is1xxCode(short code)
        {
            return (code <= 199 && code >= 100);
        }

        public override int OnRegistrationChanged(RegistrationEvent e)
        {
            short code = e.getCode();
            tsip_register_event_type_t type = e.getType();
            RegistrationSession session = e.getSession();

            switch (type)
            {
                case tsip_register_event_type_t.tsip_ao_register:
                case tsip_register_event_type_t.tsip_ao_unregister:
                    break;
            }
            
            Console.WriteLine("OnRegistrationChanged() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }

        public override int OnSubscriptionChanged(SubscriptionEvent e)
        {
            short code = e.getCode();
            tsip_subscribe_event_type_t type = e.getType();
            SubscriptionSession session = e.getSession();

            switch (type)
            {
                case tsip_subscribe_event_type_t.tsip_ao_subscribe:
                case tsip_subscribe_event_type_t.tsip_ao_unsubscribe:
                case tsip_subscribe_event_type_t.tsip_i_notify:
                    break;
            }

            Console.WriteLine("OnSubscriptioChanged() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }

    }
}
