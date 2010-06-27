using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace test
{
    class Program
    {
        const String REALM = "micromethod.com";
        const String USER = "mamadou";
        const String PROXY_CSCF_IP = "192.168.0.17";
        const uint PROXY_CSCF_PORT = 5060;
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

            String s = "mamadou";
            byte[] b = Encoding.UTF8.GetBytes(s);

            /* Create callbacks */
            sipCallback = new MySipCallback();
            //sipDebugCallback = new MySipDebugCallback();

            /* Create consumers */
            audioConsumer = new MyProxyAudioConsumer();
            videoConsumer = new MyProxyVideoConsumer(tmedia_chroma_t.tmedia_rgb565le);
            /* Create producers */
            audioProducer = new MyProxyAudioProducer();
            videoProducer = new MyProxyVideoProducer(tmedia_chroma_t.tmedia_rgb24);
            

            /* Create and configure the IMS/LTE stack */
            sipStack = new SipStack(sipCallback, String.Format("sip:{0}", REALM), /*String.Format("{0}@{1}", USER, REALM)*/USER, String.Format("sip:{0}@{1}", USER, REALM));
            sipStack.setDebugCallback(sipDebugCallback);
            sipStack.addHeader("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
            sipStack.addHeader("Privacy", "header; id");
            sipStack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
            sipStack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");

            /* Do it after stack creation */
            ProxyAudioConsumer.registerPlugin();
            ProxyAudioProducer.registerPlugin();
            ProxyVideoProducer.registerPlugin();
            ProxyVideoConsumer.registerPlugin();

            /* Sets Proxy-CSCF */
            success = sipStack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "udp", "ipv4");
            /* Starts the stack */
            success = sipStack.start();

            /* Set Password */
            sipStack.setPassword(PASSWORD);

            /* Early IMS */
            sipStack.setEarlyIMS(true);

            //sipStack.setAoR("127.0.0.1", 1234);

            audioConsumer.setActivate(true);
            audioProducer.setActivate(true);
            videoProducer.setActivate(true);
            videoConsumer.setActivate(true);

            /* Send REGISTER */
            regSession = new RegistrationSession(sipStack);
            regSession.addCaps("+g.oma.sip-im");
            regSession.addCaps("+g.3gpp.smsip");
            regSession.addCaps("language", "\"en,fr\"");
            regSession.setExpires(35);
            regSession.Register();

            Console.ReadLine();
            


            callSession = new CallSession(sipStack);
            callSession.CallAudioVideo(String.Format("sip:bob@{0}", REALM));

            tcb = new TimerCallback(OnTimer);
            timer = new Timer(tcb, new AutoResetEvent(false), 0, 20);

            Console.ReadLine();
            callSession.Hold();
            Console.ReadLine();
            callSession.Resume();
            Console.ReadLine();
            callSession.Hangup();


            ////Thread.Sleep(2000);

            ///* Send SUBSCRIBE(reg) */
            //subSession = new SubscriptionSession(sipStack);
            //subSession.addHeader("Event", "reg");
            //subSession.addHeader("Accept", "application/reginfo+xml");
            //subSession.addHeader("Allow-Events", "refer, presence, presence.winfo, xcap-diff, conference");
            //subSession.setExpires(35);
            ////subSession.Subscribe();

            ///* Send MESSAGE */
            //MessagingSession msg = new MessagingSession(sipStack);
            //byte [] content = Encoding.ASCII.GetBytes("Hello World");
            //msg.setToUri(String.Format("sip:{0}@{1}", "alice", REALM));
            //msg.addHeader("NS", "imdn <urn:ietf:params:imdn>");
            //msg.addHeader("imdn.Message-ID", "34jk324j");
            //msg.addHeader("DateTime", "2006-04-04T12:16:49-05:00");
            //msg.addHeader("imdn.Disposition-Notification", "positive-delivery, negative-delivery");
            //msg.addHeader("Content-Type", "text/plain");
            ////msg.Send(content, (uint)content.Length);

            ///* Send OPTIONS */
            //OptionsSession opt = new OptionsSession(sipStack);
            //opt.setToUri(String.Format("sip:{0}@{1}", "hacking_the_aor", REALM));
            //opt.Send();

            Console.ReadLine();

            sipStack.stop();
        }


        public static void OnTimer(Object stateInfo)
        {
            byte[] bytesAudio = new byte[320];
            uint ret = audioConsumer.pull(bytesAudio, (uint)bytesAudio.Length);
            //Console.WriteLine("pull="+ret);

            int ret2 = audioProducer.push(bytesAudio, (uint)bytesAudio.Length);
            //Console.WriteLine("push=" + ret);

            byte[] bytesVideo = new byte[176*144*3];
            int ret3 = videoProducer.push(bytesVideo, (uint)bytesVideo.Length);
        }

        static Timer timer;
        static TimerCallback tcb;
        static CallSession callSession;
        static RegistrationSession regSession;
        static SubscriptionSession subSession;
        static MySipCallback sipCallback;
        static SipStack sipStack;
        static MySipDebugCallback sipDebugCallback;
        static MyProxyAudioConsumer audioConsumer;
        static MyProxyAudioProducer audioProducer;
        static MyProxyVideoProducer videoProducer;
        static MyProxyVideoConsumer videoConsumer;
    }


    public class MySipDebugCallback : DDebugCallback
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

    public class MyProxyAudioConsumer : ProxyAudioConsumer
    {
        public override int prepare(int ptime, int rate, int channels)
        {
            return base.prepare(ptime, rate, channels);
        }

        public override int start()
        {
            return base.start();
        }

        public override int pause()
        {
            return base.pause();
        }

        public override int stop()
        {
            return base.stop();
        }
    }

    public class MyProxyAudioProducer : ProxyAudioProducer
    {
        public override int prepare(int ptime, int rate, int channels)
        {
            return base.prepare(ptime, rate, channels);
        }

        public override int start()
        {
            return base.start();
        }

        public override int pause()
        {
            return base.pause();
        }

        public override int stop()
        {
            return base.stop();
        }
    }

    public class MyProxyVideoProducer : ProxyVideoProducer
    {
        public MyProxyVideoProducer(tmedia_chroma_t chroma)
            : base(chroma)
        {
        }

        public override int prepare(int width, int height, int fps)
        {
            return base.prepare(width, height, fps);
        }

        public override int start()
        {
            return base.start();
        }

        public override int pause()
        {
            return base.pause();
        }

        public override int stop()
        {
            return base.stop();
        }
    }

    public class MyProxyVideoConsumer : ProxyVideoConsumer
    {
        public MyProxyVideoConsumer(tmedia_chroma_t chroma)
            : base(chroma)
        {
        }

        public override int prepare(int width, int height, int fps)
        {
            return base.prepare(width, height, fps);
        }

        public override int start()
        {
            return base.start();
        }

        public override int consume(ProxyVideoFrame frame)
        {
            uint size = frame.getSize(); // for test
            byte[] bytes = new byte[1200];
            uint ret = frame.getContent(bytes, (uint)bytes.Length);
            return base.consume(frame);
        }

        public override int pause()
        {
            return base.pause();
        }

        public override int stop()
        {
            return base.stop();
        }
    }

    public class MySipCallback : SipCallback
    {
        public MySipCallback()
            : base()
        {
        }

        public override int OnRegistrationEvent(RegistrationEvent e)
        {
            short code = e.getCode();
            tsip_register_event_type_t type = e.getType();
            RegistrationSession session = e.getSession();
            SipMessage message = e.getSipMessage();

            if (message != null)
            {
                Console.WriteLine("call-id={0}", message.getSipHeaderValue("call-id"));
                //byte[] bytes = message.getContent();
            }

            switch (type)
            {
                case tsip_register_event_type_t.tsip_ao_register:
                case tsip_register_event_type_t.tsip_ao_unregister:
                    break;
            }
            
            Console.WriteLine("OnRegistrationChanged() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }

        public override int OnCallEvent(CallEvent e)
        {
            tsip_invite_event_type_t type = e.getType();
            CallSession session = e.getSession();
            SipMessage message = e.getSipMessage();

            switch(type){
                case tsip_invite_event_type_t.tsip_i_newcall:
                    if (session != null)
                    {
                        Console.WriteLine("ERRRRRRRRRRRORRRR");
                        return 0;
                    }
                    else if ((session = e.takeSessionOwnership()) != null)
                    {
                        session.Accept();
                    }
                    break;
                case tsip_invite_event_type_t.tsip_i_request:
                    break;
                case tsip_invite_event_type_t.tsip_ao_request:
                    break;
                case tsip_invite_event_type_t.tsip_o_ect_ok:
                    break;
                case tsip_invite_event_type_t.tsip_o_ect_nok:
                    break;
                case tsip_invite_event_type_t.tsip_i_ect:
                    break;
                case tsip_invite_event_type_t.tsip_m_local_hold_ok:
                    Console.WriteLine("Local Hold OK");
                    break;
                case tsip_invite_event_type_t.tsip_m_local_hold_nok:
                    Console.WriteLine("Local Hold NOK");
                    break;
                case tsip_invite_event_type_t.tsip_m_local_resume_ok:
                    Console.WriteLine("Local Resume OK");
                    break;
                case tsip_invite_event_type_t.tsip_m_local_resume_nok:
                    Console.WriteLine("Local Resume NOK");
                    break;
                case tsip_invite_event_type_t.tsip_m_remote_hold:
                    Console.WriteLine("Remote Hold");
                    break;
                case tsip_invite_event_type_t.tsip_m_remote_resume:
                    Console.WriteLine("Remote Resume");
                    break;
            }
              
            

            



            return 0;
        }

        public override int OnOptionsEvent(OptionsEvent e)
        {
            short code = e.getCode();
            tsip_options_event_type_t type = e.getType();
            OptionsSession session = e.getSession();
            SipMessage message = e.getSipMessage();

            if (message != null)
            {
                Console.WriteLine("call-id={0}", message.getSipHeaderValue("call-id"));
                //byte[] bytes = message.getContent();
            }

            switch (type)
            {
                case tsip_options_event_type_t.tsip_ao_options:
                    String rport = message.getSipHeaderParamValue("v", "rport");
                    String received_ip = message.getSipHeaderParamValue("v", "received");
                    if (rport == null)
                    {   /* Ericsson SDS */
                        rport = message.getSipHeaderParamValue("v", "received_port_ext");
                    }
                    Console.WriteLine("Via(rport, received)=({0}, {1})", rport, received_ip);
                    break;
                case tsip_options_event_type_t.tsip_i_options:
                    break;
            }

            Console.WriteLine("OnRegistrationChanged() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }

        public override int OnMessagingEvent(MessagingEvent e)
        {
            short code = e.getCode();
            tsip_message_event_type_t type = e.getType();
            MessagingSession session = e.getSession();
            SipMessage message = e.getSipMessage();

            if (session == null && message != null)
            { /* "Server-side-session" e.g. Initial MESSAGE/INVITE sent by the remote party */
                session = e.takeSessionOwnership();
                Console.WriteLine("From:{0} == To:{1}", message.getSipHeaderValue("f"), message.getSipHeaderValue("t"));
            }

            if (message != null)
            {
                Console.WriteLine("call-id={0}", message.getSipHeaderValue("call-id"));
                //byte[] bytes = message.getContent();
            }

            switch (type)
            {
                case tsip_message_event_type_t.tsip_i_message:
                    byte[] content = message.getSipContent();
                    if (content != null)
                    {
                        Console.WriteLine("Message Content ==> {0}", Encoding.UTF8.GetString(content));
                        session.Accept();
                    }
                    else
                    {
                        session.Reject();
                    }
                    break;
                case tsip_message_event_type_t.tsip_ao_message:
                    break;
            }

            Console.WriteLine("OnMessagingEvent() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }

        public override int OnSubscriptionEvent(SubscriptionEvent e)
        {
            short code = e.getCode();
            tsip_subscribe_event_type_t type = e.getType();
            SubscriptionSession session = e.getSession();
            SipMessage message = e.getSipMessage();

            switch (type)
            {
                case tsip_subscribe_event_type_t.tsip_ao_subscribe:
                case tsip_subscribe_event_type_t.tsip_ao_unsubscribe:
                    break;

                case tsip_subscribe_event_type_t.tsip_i_notify:
                    byte[] content = message.getSipContent();
                    if (content != null)
                        Console.WriteLine("Notify Content ==> {0}", Encoding.UTF8.GetString(content));
                    break;
            }

            Console.WriteLine("OnSubscriptioChanged() ==> {0}:{1}", code, e.getPhrase());

            return 0;
        }



       const String PUBLISH_PAYLOAD = "<?xml version=\"1.0\" encoding=\"utf-8\"?>" +
"<presence xmlns:cp=\"urn:ietf:params:xml:ns:pidf:cipid\" xmlns:caps=\"urn:ietf:params:xml:ns:pidf:caps\" xmlns:rpid=\"urn:ietf:params:xml:ns:pidf:rpid\" xmlns:pdm=\"urn:ietf:params:xml:ns:pidf:data-model\" xmlns:p=\"urn:ietf:params:xml:ns:pidf-diff\" xmlns:op=\"urn:oma:xml:prs:pidf:oma-pres\" entity=\"sip:bob@ims.inexbee.com\" xmlns=\"urn:ietf:params:xml:ns:pidf\">" +
  "<pdm:person id=\"RPVRYNJH\">" +
    "<op:overriding-willingness>" +
      "<op:basic>open</op:basic>" +
    "</op:overriding-willingness>" +
    "<rpid:activities>" +
      "<rpid:busy />" +
    "</rpid:activities>" +
    "<rpid:mood>" +
      "<rpid:guilty />" +
    "</rpid:mood>" +
    "<cp:homepage>http://doubango.org</cp:homepage>" +
    "<pdm:note>Come share with me RCS Experience</pdm:note>" +
  "</pdm:person>" +
  "<pdm:device id=\"d0001\">" +
    "<status>" +
      "<basic>open</basic>" +
    "</status>" +
    "<caps:devcaps>" +
      "<caps:mobility>" +
        "<caps:supported>" +
          "<caps:fixed />" +
        "</caps:supported>" +
      "</caps:mobility>" +
    "</caps:devcaps>" +
    "<op:network-availability>" +
      "<op:network id=\"IMS\">" +
        "<op:active />" +
      "</op:network>" +
    "</op:network-availability>" +
    "<pdm:deviceID>urn:uuid:3ca50bcb-7a67-44f1-afd0-994a55f930f4</pdm:deviceID>" +
  "</pdm:device>" +
"</presence>";
    }
}
