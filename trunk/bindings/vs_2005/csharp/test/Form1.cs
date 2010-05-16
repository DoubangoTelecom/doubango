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
        const String REALM = "ims.inexbee.com";
        const String USER = "mamadou";
        const String PASSWORD = "mamadou";
        const String PROXY_CSCF_IP = "192.168.16.225";
        const uint PROXY_CSCF_PORT = 4060;

        RegistrationSession regSession = null;
        MyCallback callback = null;
        SipStack stack = null;

        public delegate void PrintDebug(String text);
        public PrintDebug mydel;

        public Form1()
        {
            InitializeComponent();

            bool success;

            mydel = new PrintDebug(PrintDebugMethod);

            /* Create call back */
            callback = new MyCallback(this);
            /* Create and configure the IMS/LTE stack */
            stack = new SipStack(callback, String.Format("sip:{0}", REALM), String.Format("{0}@{1}", USER, REALM), String.Format("sip:{0}@{1}", USER, REALM));
            stack.addHeader("Allow", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
            stack.addHeader("Privacy", "header; id");
            stack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
            stack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");

            /* set password */
            success = stack.setPassword(PASSWORD);
            /* Sets Proxy-CSCF */
            success = stack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "tcp", "ipv4");
            /* Starts the stack */
            success = stack.Start();
        }

        private void buttonRegister_Click(object sender, EventArgs e)
        {
            if (this.regSession == null)
            {
                this.regSession = new RegistrationSession(stack);
                this.regSession.addCaps("+g.oma.sip-im");
                this.regSession.addCaps("+g.3gpp.smsip");
                this.regSession.addCaps("language", "\"en,fr\"");
            }
            this.regSession.Register(35);
        }

        private void buttonUnRegister_Click(object sender, EventArgs e)
        {
            if (this.regSession != null)
            {
                this.regSession.UnRegister();
            }
        }

        void PrintDebugMethod(String text)
        {
            this.textBoxDebug.Text = text;
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

        public override int OnRegistrationChanged(RegistrationEvent e)
        {
            short code = e.getCode();
            tsip_register_event_type_t type = e.getType();
            String text;
            
            RegistrationSession session = e.getSession();

            switch (type)
            {
                case tsip_register_event_type_t.tsip_i_register:
                case tsip_register_event_type_t.tsip_ai_register:
                case tsip_register_event_type_t.tsip_o_register:
                case tsip_register_event_type_t.tsip_ao_register:
                case tsip_register_event_type_t.tsip_i_unregister:
                case tsip_register_event_type_t.tsip_ai_unregister:
                case tsip_register_event_type_t.tsip_o_unregister:
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
    }
}