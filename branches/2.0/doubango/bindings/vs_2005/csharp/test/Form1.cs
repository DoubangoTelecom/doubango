using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace test
{
    public partial class Form1 : Form
    {
        /*const String REALM = "ims.inexbee.com";
        const String USER = "mamadou";
        const String PASSWORD = "mamadou";
        const String PROXY_CSCF_IP = "192.168.16.225";
        const uint PROXY_CSCF_PORT = 4060;*/

        const String REALM = "micromethod.com";
        const String USER = "mamadou";
        const String PASSWORD = "mamadou";
        const String PROXY_CSCF_IP = "192.168.16.104";
        const uint PROXY_CSCF_PORT = 5060;

        RegistrationSession regSession = null;
        PublicationSession pubSession = null;
        SubscriptionSession subSession = null;
        MyCallback callback = null;
        SipStack stack = null;

        public delegate void PrintDebug(String text);
        public PrintDebug mydel;

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

        public Form1()
        {
            InitializeComponent();

            bool success;

            mydel = new PrintDebug(PrintDebugMethod);

            /* Create call back */
            callback = new MyCallback(this);
            /* Create and configure the IMS/LTE stack */
            // stack = new SipStack(callback, String.Format("sip:{0}", REALM), String.Format("{0}@{1}", USER, REALM), String.Format("sip:{0}@{1}", USER, REALM));
            stack = new SipStack(callback, String.Format("sip:{0}", REALM), String.Format("{0}@{1}", USER, REALM), String.Format("sip:{0}@{1}", USER, REALM));

            if (!stack.isValid())
            {
                this.textBoxDebug.Text = "Invalid stack";
                return;
            }

            stack.addHeader("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
            stack.addHeader("Privacy", "header; id");
            stack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
            stack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");

            /* set password */
            success = stack.setPassword(PASSWORD);
            /* Sets Proxy-CSCF */
            success = stack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "udp", "ipv4");
            /* Starts the stack */
            success = stack.start();
        }

        private void buttonRegister_Click(object sender, EventArgs e)
        {
            if (this.regSession == null)
            {
                this.regSession = new RegistrationSession(stack);
                this.regSession.addCaps("+g.oma.sip-im");
                this.regSession.addCaps("+g.3gpp.smsip");
                this.regSession.addCaps("language", "\"en,fr\"");
                this.regSession.setExpires(30);
            }
            this.regSession.Register();
        }

        private void buttonUnRegister_Click(object sender, EventArgs e)
        {
            if (this.regSession != null)
            {
                this.regSession.UnRegister();
            }
        }

        private void buttonPublish_Click(object sender, EventArgs e)
        {
            if (this.pubSession == null)
            {
                this.pubSession = new PublicationSession(stack);
                this.pubSession.addHeader("Content-Type", "application/pidf+xml");
                this.pubSession.addHeader("Event", "presence");
                this.pubSession.setExpires(30);

                byte[] payload = Encoding.UTF8.GetBytes(PUBLISH_PAYLOAD);
                this.pubSession.Publish(payload, (uint)payload.Length);
            }
        }

        private void buttonUnPublish_Click(object sender, EventArgs e)
        {
            if (this.pubSession != null)
            {
                this.pubSession.UnPublish();
            }
        }

        void PrintDebugMethod(String text)
        {
            this.textBoxDebug.Text = text;
        }

        private void buttonSub_Click(object sender, EventArgs e)
        {
            if (this.subSession == null)
            {
                this.subSession = new SubscriptionSession(stack);
                this.subSession.addHeader("Accept", "application/reginfo+xml");
                this.subSession.addHeader("Event", "reg");
                this.subSession.setExpires(30);

                this.subSession.Subscribe();
            }
        }
    }





    public class MyCallback : SipCallback
    {
        Form1 form;

        public MyCallback(Form1 form)
            : base()
        {
            this.form = form;
        }

        public override int OnRegistrationEvent(RegistrationEvent e)
        {
            short code = e.getCode();
            tsip_register_event_type_t type = e.getType();
            String text;
            
            RegistrationSession session = e.getSession();

            switch (type)
            {
                case tsip_register_event_type_t.tsip_i_register:
                case tsip_register_event_type_t.tsip_ao_register:
                case tsip_register_event_type_t.tsip_i_unregister:
                case tsip_register_event_type_t.tsip_ao_unregister:
                    break;
            }

            text = String.Format("OnRegistrationChanged() ==> {0}:{1}", code, e.getPhrase());

            if (this.form.InvokeRequired)
            {
                this.form.Invoke(this.form.mydel, new object[] { text });
            }

            return 0;
        }

        public override int OnSubscriptionEvent(SubscriptionEvent e)
        {
            switch (e.getType())
            {
                case tsip_subscribe_event_type_t.tsip_i_notify:
                    String ev = e.getSipMessage().getSipHeaderValue("Event");
                    Console.WriteLine("Event=%s", ev);
                    break;
                default:
                    break;
            }
            return base.OnSubscriptionEvent(e);
        }

        public override int OnPublicationEvent(PublicationEvent e)
        {
            short code = e.getCode();
            tsip_publish_event_type_t type = e.getType();
            String text;

            PublicationSession session = e.getSession();

            switch (type){
                default:    
                    break;
            }

            text = String.Format("OnPublicationChanged() ==> {0}:{1}", code, e.getPhrase());

            if (this.form.InvokeRequired)
            {
                this.form.Invoke(this.form.mydel, new object[] { text });
            }

            return 0;
        }
    }



}