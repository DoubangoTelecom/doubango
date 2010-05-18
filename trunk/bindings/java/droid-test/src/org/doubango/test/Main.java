package org.doubango.test;

import org.doubango.tinyWRAP.RegistrationEvent;
import org.doubango.tinyWRAP.RegistrationSession;
import org.doubango.tinyWRAP.SipCallback;
import org.doubango.tinyWRAP.SipStack;

import android.app.Activity;
import android.net.DhcpInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;

public class Main extends Activity {
	
	final String REALM = "ericsson.com";
	final String USER = "mamadou";
	final String PROXY_CSCF_IP = "192.168.0.13";
	//final String PASSWORD = null;
	final int PROXY_CSCF_PORT = 5081;
    
	private String getDns1()
	{
		WifiManager wifiManager = (WifiManager) getSystemService(WIFI_SERVICE);
		String[] dhcpInfos = wifiManager.getDhcpInfo().toString().split(" ");
		int i = 0;
		
		while (i++ < dhcpInfos.length) {
		  if (dhcpInfos[i-1].equals("dns1")) {
			  return dhcpInfos[i];
		  }
		}
		return null;
	}
	
	private String getLocalIP(){
		WifiManager wifiManager = (WifiManager) getSystemService(WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();
		int ipAddress = wifiInfo.getIpAddress(); /*what about IPv6?*/
		
		if(ipAddress != 0){
			return String.format("%d.%d.%d.%d",
					(ipAddress>>0)&0xFF,
					(ipAddress>>8)&0xFF,
					(ipAddress>>16)&0xFF,
					(ipAddress>>24)&0xFF
					);
		}
		else{
			return null;
		}
	}
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        @SuppressWarnings("unused")
		boolean success;

        /* Create call back */
        callback = new MyCallback();
        /* Create and configure the IMS/LTE stack */
        stack = new SipStack(callback, String.format("sip:%s", REALM), String.format("%s@%s", USER, REALM), String.format("sip:%s@%s", USER, REALM));
        stack.addHeader("Privacy", "INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER");
        stack.addHeader("P-Access-Network-Info", "ADSL;utran-cell-id-3gpp=00000000");
        stack.addHeader("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0");            

        /* Sets Proxy-CSCF */
        success = stack.setProxyCSCF(PROXY_CSCF_IP, PROXY_CSCF_PORT, "tcp", "ipv4");
        /* Sets Local IP if we are on the emulator */
        /*if(android.provider.Settings.Secure.ANDROID_ID == null)*/{
        	String localIP, dns1;
        	if((localIP = this.getLocalIP()) == null){
        		localIP = "10.0.2.15";
        	}
        	dns1 = this.getDns1();
        	success = stack.setLocalIP(localIP);
        }
        /* Set Password */
        //success = stack.setPassword(PASSWORD);
        /* Starts the stack */
        success = stack.start();

        /* Send REGISTER */
        regSession = new RegistrationSession(stack);
        regSession.addCaps("+g.oma.sip-im");
        regSession.addCaps("+g.3gpp.smsip");
        regSession.addCaps("language", "\"en,fr\"");
        regSession.setExpires(30);
        if((success = regSession.Register())){
        }
        
        
    }
    
    
    static RegistrationSession regSession;
    static MyCallback callback;
    static SipStack stack;
    
    
    
    static {
        try {
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