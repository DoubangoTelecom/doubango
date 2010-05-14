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

        static void Main(string[] args)
        {
            Boolean success;

            /* Create call back */
            callback = new MyCallback();
            /* Create and configure the IMS/LTE stack */
            stack = new SipStack(callback, String.Format("sip:{0}", REALM), String.Format("{0}@{1}", USER, REALM), String.Format("sip:{0}@{1}", USER, REALM));
            stack.addHeader("Privacy", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
            stack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
            stack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");            

            /* Sets Proxy-CSCF */
            success = stack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "tcp", "ipv4");
            /* Starts the stack */
            success = stack.Start();

            /* Send REGISTER */
            regSession = new RegistrationSession(stack);
            regSession.addCaps("+g.oma.sip-im");
            regSession.addCaps("+g.3gpp.smsip");
            regSession.addCaps("language", "\"en,fr\"");
            regSession.Register(35);

            Console.Read();
        }

        static RegistrationSession regSession;
        static MyCallback callback;
        static SipStack stack;
    }


    public class MyCallback : SipCallback
    {
        public MyCallback()
            : base()
        {
        }

        public override int OnRegistrationChanged(RegistrationEvent e)
        {
            short code = e.getCode();
            tsip_event_type_t type = e.getType();

            RegistrationSession session = (e.getSession2() as RegistrationSession);
            
            Console.WriteLine("OnRegistrationChanged() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }
    }
}
