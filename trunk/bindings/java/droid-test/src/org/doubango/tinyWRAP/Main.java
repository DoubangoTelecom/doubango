package org.doubango.tinyWRAP;

import android.app.Activity;
import android.os.Bundle;

public class Main extends Activity {
	final String REALM = "ericsson.com";
	final String USER = "mamadou";
	final String PROXY_CSCF_IP = "192.168.0.13";
	final int PROXY_CSCF_PORT = 5081;
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        boolean success;

        /* Create call back */
        callback = new MyCallback();
        /* Create and configure the IMS/LTE stack */
        stack = new SipStack(callback, String.format("sip:%s", REALM), String.format("%s@%s", USER, REALM), String.format("sip:%s@%s", USER, REALM));
        stack.addHeader("Privacy", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
        stack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
        stack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");            

        /* Sets Proxy-CSCF */
        success = stack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "udp", "ipv4");
        /* Sets Local IP */
        success = stack.setLocalIP("10.0.2.15");
        success = stack.setLocalPort(5060);
        /* Starts the stack */
        success = stack.Start();

        /* Send REGISTER */
        regSession = new RegistrationSession(stack);
        regSession.addCaps("+g.oma.sip-im");
        regSession.addCaps("+g.3gpp.smsip");
        regSession.addCaps("language", "\"en,fr\"");
        if((success = regSession.Register(35))){
        }
        
        
    }
    
    
    static RegistrationSession regSession;
    static MyCallback callback;
    static SipStack stack;
    
    
    
    static {
        try {
        	System.loadLibrary("tinySAK");
        	System.loadLibrary("tinyNET");
        	System.loadLibrary("tinyIPSec");
        	System.loadLibrary("tinyHTTP");
        	System.loadLibrary("tinySDP");
        	System.loadLibrary("tinyMEDIA");
        	System.loadLibrary("tinySIP");
            System.loadLibrary("tinyWRAP");
        } catch (UnsatisfiedLinkError e) {
          System.err.println("Native code library failed to load.\n" + e);
          System.exit(1);
        }
        catch(Exception e){
        	e.printStackTrace();
        }
      }
}


class MyCallback extends SipCallback
{
  public MyCallback()
  {
    super();
  }

  public int OnRegistrationChanged(RegistrationEvent e)
  {
	  short code = e.getCode();
      //tsip_event_type_t type = e.getType();

      //RegistrationSession session = (RegistrationSession) e.getSession();
      
      System.out.println("Code=" + code);
      System.out.println("Phrase=" + e.getPhrase());

      return 0;
  }
}