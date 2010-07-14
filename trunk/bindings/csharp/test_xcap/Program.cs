using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace test_xcap
{
    class Program
    {
        const String REALM = "micromethod.com";
        const String USER = "mamadou";
        const String PASSWORD = "mamadou";
        const String HOST = "192.168.0.10";
        const int PORT = 8080;
        
        static void Main(string[] args)
        {
            xcapStack = new MyXcapStack(new MyXcapCallback(), String.Format("sip:{0}@{1}", USER, REALM), PASSWORD, String.Format("http://{0}:{1}/services", HOST, PORT));
            xcapSelector = new XcapSelector(xcapStack);

            if (!xcapStack.start())
            {
                Console.WriteLine("Failed to start the XCAP stack");
            }

            xcapStack.addHeader("Connection", "Keep-Alive");
		    xcapStack.addHeader("User-Agent", "XDM-client/OMA1.1");
            xcapStack.addHeader("X-3GPP-Intended-Identity", String.Format("sip:{0}@{1}", USER, REALM));

            //xcapSelector.setAUID("resource-lists").
            //    setAttribute("list", "name", "rcs").
            //    setAttribute("entry", "uri", String.Format("sip:{0}@{1}", USER, REALM)).
            //    setName("display-name");

            xcapSelector.setAUID("xcap-caps");
            xcapStack.getDocument(xcapSelector.getString());
            xcapSelector.reset();
            Console.ReadLine();

            xcapSelector.setAUID("resource-lists");
            xcapStack.getDocument(xcapSelector.getString());
            xcapSelector.reset();
            Console.ReadLine();

            xcapStack.stop();

            Console.ReadLine();
        }

        static XcapSelector xcapSelector;
        static MyXcapStack xcapStack;


        class MyXcapStack : XcapStack
        {
            public MyXcapStack(MyXcapCallback callback, string xui, string password, string xcap_root)
                : base(callback, xui, password, xcap_root)
            {
            }
        }

        class MyXcapCallback : XcapCallback
        {
            public MyXcapCallback()
                :base()
            {
            }

            public override int onEvent(XcapEvent e)
            {
                XcapMessage message = e.getXcapMessage();
                String content_type;

                if(message == null){
                    Console.WriteLine("Invalid Xcap message");
                    return -1;
                }

                Console.WriteLine("code={0} and Phrase={1}", message.getCode(), message.getPhrase());
                
                if((content_type = message.getXcapHeaderValue("Content-Type")) != null){
                    Console.WriteLine("Content-Type={0}", content_type);
                    uint clen = message.getXcapContentLength();
                    if (clen > 0)
                    {
                        byte[] content = new byte[clen];
                        uint read = message.getXcapContent(content, (uint)content.Length);
                        Console.WriteLine("Content-Value ==> {0}", Encoding.UTF8.GetString(content));
                    }
                }

                

                return base.onEvent(e);
            }
        }
    }
}
