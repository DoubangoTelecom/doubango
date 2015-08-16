if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.Screen) == "undefined" ||
	typeof(window.RadControlsNamespace.Screen.Version) == null ||
	window.RadControlsNamespace.Screen.Version < 1.1
	)
{	

	window.RadControlsNamespace.Screen = 
	{
	
		Version : 1.1,	
	
	    GetViewPortSize : function() 
	    {
			var width = 0;
			var height = 0;
			
			var canvas = document.body;

			if (RadControlsNamespace.Browser.StandardsMode && !RadControlsNamespace.Browser.IsSafari)
			{
				canvas = document.documentElement;
			}			
			
			if (RadControlsNamespace.Browser.IsMozilla && document.compatMode != "CSS1Compat")
			{
			    canvas = document.body;
			}
			
			if (window.innerWidth) 
			{
				width = window.innerWidth;
				height = window.innerHeight;
			} 
			else
			{
				width = canvas.clientWidth;
				height = canvas.clientHeight;
			}
			
			width += canvas.scrollLeft;
			height += canvas.scrollTop;
			
			return { width : width - 6, height : height - 6 };
	    },
		
		GetElementPosition : function (el)
		{
			var parent = null;
			var pos = {x: 0, y: 0};
			var box;
	
			if (el.getBoundingClientRect) 
			{ 
				// IE
				box = el.getBoundingClientRect();
				var scrollTop = document.documentElement.scrollTop || document.body.scrollTop;
				var scrollLeft = document.documentElement.scrollLeft || document.body.scrollLeft;
	
				pos.x = box.left + scrollLeft - 2;
				pos.y = box.top + scrollTop - 2;
			    
				return pos;
			}
			else if (document.getBoxObjectFor) 
			{ 
				// gecko
				try
				{
				    box = document.getBoxObjectFor(el);
				    pos.x = box.x - 2;
				    pos.y = box.y - 2;
				}catch(e){}
			}
			else 
			{ 
				// safari/opera
				pos.x = el.offsetLeft;
				pos.y = el.offsetTop;
				parent = el.offsetParent;
				if (parent != el)
				{
					while (parent) 
					{
						pos.x += parent.offsetLeft;
						pos.y += parent.offsetTop;
						parent = parent.offsetParent;
					}
				}
			}
	
	
			if (window.opera)
			{
				parent = el.offsetParent;
				
				while (parent && parent.tagName.toLowerCase() != 'body' && parent.tagName.toLowerCase() != 'html') 
				{
					pos.x -= parent.scrollLeft;
					pos.y -= parent.scrollTop;
					parent = parent.offsetParent;
				}
			}
			else
			{
				parent = el.parentNode; 
				while (parent && parent.tagName.toLowerCase() != 'body' && parent.tagName.toLowerCase() != 'html') 
				{
					pos.x -= parent.scrollLeft;
					pos.y -= parent.scrollTop;
	
					parent = parent.parentNode;
				}
			}
	
			return pos;
		},
		
		ElementOverflowsTop : function (element)
		{
			return this.GetElementPosition(element).y < 0;
		}, 
	
		ElementOverflowsLeft : function (element)
		{
			return this.GetElementPosition(element).x < 0;
		},
		
		ElementOverflowsBottom : function (screenSize, element)
		{
			var bottomEdge = this.GetElementPosition(element).y + RadControlsNamespace.Box.GetOuterHeight(element);
			return bottomEdge > screenSize.height;	
		},	
		
		ElementOverflowsRight : function (screenSize, element)
		{
			var rightEdge = this.GetElementPosition(element).x + RadControlsNamespace.Box.GetOuterWidth(element);
			return rightEdge > screenSize.width;
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