if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.Box) == "undefined" ||
	typeof(window.RadControlsNamespace.Box.Version) == null ||
	window.RadControlsNamespace.Box.Version < 2
	)
{

	window.RadControlsNamespace.Box = 
	{
		Version : 2, /// Change the version when make changes. Change the value in the IF also
		
		GetOuterWidth : function (element)
		{
			return element.offsetWidth;
		},
		
		GetOuterHeight : function (element)
		{
			return element.offsetHeight;	
		},	
		
		SetOuterHeight : function (element, height)
		{
			if (height <= 0 || height == "") 
			{
				element.style.height = "";
			} 
			else
			{
				element.style.height = height + "px";
				var diff = element.offsetHeight - height;
				var newHeight = height - diff;
				if (newHeight > 0) {
					element.style.height = newHeight + "px";
				} else {
					element.style.height = "";
				}
			}
		},
		
		SetOuterWidth : function (element, width)
		{
	
			if (width <= 0 || width == "") 
			{
				element.style.width = "";
			}
			else
			{
				element.style.width = width + "px";
				var diff = element.offsetWidth - width;
				var newWidth = width - diff;
				if (newWidth > 0) {
					element.style.width = newWidth + "px";
				} else {
					element.style.width = "";
				}
				return newWidth;
			}
		},
		
	
		GetPropertyValue : function(element, styleProperty)
		{
	        var computedStyle = this.GetStyle(element);
	        return this.GetStyleValues(computedStyle, styleProperty);	
		},
		
		
		GetStyle : function (element)
		{
			if (document.defaultView && document.defaultView.getComputedStyle)
			{
				return document.defaultView.getComputedStyle(element, null);
			}
			else if (element.currentStyle)
			{
				return element.currentStyle;
			}
			else
			{
				return element.style;
			}
		}
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