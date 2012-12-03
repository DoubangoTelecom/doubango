if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.Overlay) == "undefined" ||
	typeof(window.RadControlsNamespace.Overlay.Version) == null ||
	window.RadControlsNamespace.Overlay.Version < 1.1
	)
{	
	window.RadControlsNamespace.Overlay = function (element)
	{
		
		if (!this.SupportsOverlay())
		{
			return;
		}

		this.Element = element;

		this.Shim = document.createElement("IFRAME");
		this.Shim.src="javascript:'';";
		this.Element.parentNode.insertBefore(this.Shim, this.Element);

		if (element.style.zIndex > 0)
		{
			this.Shim.style.zIndex = element.style.zIndex - 1;
		}
		this.Shim.style.position = "absolute";	
		this.Shim.style.border = "0px";	
		this.Shim.frameBorder = 0;
		this.Shim.style.filter = "progid:DXImageTransform.Microsoft.Alpha(style=0,opacity=0)";
		this.Shim.disabled = "disabled";
	}

	window.RadControlsNamespace.Overlay.Version = 1.1;// update in the header IF also

	RadControlsNamespace.Overlay.prototype.SupportsOverlay = function()
	{
		return (RadControlsNamespace.Browser.IsIE);
	}

	RadControlsNamespace.Overlay.prototype.Update = function ()
	{
		if (!this.SupportsOverlay())
		{
			return;
		}
		
		this.Shim.style.top = this.ToUnit(this.Element.style.top);
		this.Shim.style.left = this.ToUnit(this.Element.style.left);
		this.Shim.style.width = this.Element.offsetWidth + "px";
		this.Shim.style.height = this.Element.offsetHeight + "px";	
		
	//	this.Shim.style.border = "0px solid red";
	}

	RadControlsNamespace.Overlay.prototype.ToUnit = function (value)
	{
		if (!value) return "0px";
		return parseInt(value) + "px";
	}

	RadControlsNamespace.Overlay.prototype.Dispose = function ()
	{
		if (!this.SupportsOverlay())
		{
			return;
		}
				
		if (this.Shim.parentNode)
		{
			this.Shim.parentNode.removeChild(this.Shim);
		}
		
		this.Element = null;
		this.Shim = null;
		
	}
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