if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.Browser) == "undefined" ||
	typeof(window.RadControlsNamespace.Browser.Version) == null ||
	window.RadControlsNamespace.Browser.Version < 1
	)
{
	
	window.RadControlsNamespace.Browser = {
		Version : 1
	};
	
	window.RadControlsNamespace.Browser.ParseBrowserInfo = function ()
	{

		this.IsMacIE = (navigator.appName == "Microsoft Internet Explorer") &&
				((navigator.userAgent.toLowerCase().indexOf("mac") != -1) ||
				(navigator.appVersion.toLowerCase().indexOf("mac") != -1));

		this.IsSafari = (navigator.userAgent.toLowerCase().indexOf("safari") != -1);

		this.IsSafari3 = (this.IsSafari && navigator.userAgent.toLowerCase().indexOf("ersion/3.") != -1);

		this.IsMozilla = window.netscape && !window.opera;

		this.IsFirefox = window.netscape && !window.opera;

		this.IsNetscape = /Netscape/.test(navigator.userAgent);

		this.IsOpera = window.opera;

		this.IsOpera9 = window.opera && (parseInt(window.opera.version()) > 8);

		this.IsIE = !this.IsMacIE && !this.IsMozilla && !this.IsOpera && !this.IsSafari;

		this.IsIE7 = /MSIE 7/.test(navigator.appVersion);

		this.StandardsMode = this.IsSafari || this.IsOpera9 || this.IsMozilla || document.compatMode == "CSS1Compat";

		this.IsMac = /Mac/.test(navigator.userAgent);
	}

	RadControlsNamespace.Browser.ParseBrowserInfo();
}

//BEGIN_ATLAS_NOTIFY
if (typeof(Sys) != "undefined")
{
	if (Sys.Application != null && Sys.Application.notifyScriptLoaded != null)
	{
		Sys.Application.notifyScriptLoaded();
	}
}
//END_ATLAS_NOTIFY