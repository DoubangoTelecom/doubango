if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

RadControlsNamespace.ScrollButtonsPosition = {Left : 0, Middle : 1, Right : 2 };

RadControlsNamespace.Scroll = function (element, vertical, scrollOptionsObject) 
{
    this.Owner = scrollOptionsObject;
    this.Element = element;
    this.IsVertical = vertical;
    this.ScrollButtonsPosition = scrollOptionsObject.ScrollButtonsPosition;
    this.ScrollPosition = scrollOptionsObject.ScrollPosition;
    this.PerTabScrolling = scrollOptionsObject.PerTabScrolling;
    this.ScrollOnHover = false;
    this.WrapNeeded = false;
    this.LeaveGapsForArrows = true;
    
    this.LeftArrowClass = "leftArrow";
    this.LeftArrowClassDisabled = "leftArrowDisabled";
    this.RightArrowClass = "rightArrow";
    this.RightArrowClassDisabled = "rightArrowDisabled";
    this.Initialized = false;
};


RadControlsNamespace.Scroll.Create = function (element, vertical, scrollOptionsObject)
{
    return new RadControlsNamespace.Scroll(element, vertical, scrollOptionsObject);
}


RadControlsNamespace.Scroll.prototype.Initialize = function ()
{
	if (this.Initialized)
	{
		this.ApplyOverflow();
		this.CalculateMinMaxPosition();
		this.EvaluateArrowStatus();
		return false;
	}

	if (
			(this.Element.offsetWidth == 0 && !this.IsVertical) 
				|| 
			(this.Element.offsetHeight == 0 && this.IsVertical) 
	    )
	{
		return false;
	}
	
	this.Initialized = true;
	
	this.ScrollAmount = 2;
	this.Direction = 0;
	
	if (this.WrapNeeded)
	{
		var scrollWrap = this.CreateScrollWrap();
	}
    this.ApplyOverflow();
    this.Element.style.position = "relative";
	this.AttachArrows();	
	
    this.CalculateMinMaxPosition();
    
    if (this.PerTabScrolling)
    {
		this.CalculateInitialTab();
    }
    

    this.AttachScrollMethods();   
    
    this.EvaluateArrowStatus();
    
    this.AttachEventHandlers();
    
    this.ScrollTo(this.ScrollPosition);
    this.ApplyOverflow();
    return scrollWrap;
};

RadControlsNamespace.Scroll.prototype.ApplyOverflow = function ()
{
	if (RadControlsNamespace.Browser.IsIE)
	{
		this.Element.parentNode.style.overflow = "visible";
		if (this.IsVertical)
		{
			this.Element.parentNode.style.overflowX = "";
			this.Element.parentNode.style.overflowY = "hidden";
		}
		else
		{
			this.Element.parentNode.style.overflowX = "hidden";
			this.Element.parentNode.style.overflowY = "hidden";
		}
	}
	else
	{
		this.Element.parentNode.style.overflow = "hidden";
	}
	
	if (!this.ScrollNeeded())
	{
		this.Element.parentNode.style.overflow =  "visible";
		this.Element.parentNode.style.overflowX = "visible";
		this.Element.parentNode.style.overflowY = "visible";
	}

}

RadControlsNamespace.Scroll.prototype.ResizeHandler = function ()
{
	if (this.Disposed)
	{
	    return;
	}
	if (!this.Initialized)
	{
		this.Initialize();
	}
	
	if (!this.Initialized)
	{
		return;
	}

	if (!this.Element.offsetHeight || !this.Element.offsetWidth)
	{
		return;
	}

	
	this.CalculateMinMaxPosition();
	
	if (this.Element.offsetWidth < this.Element.parentNode.offsetWidth)
	{
		this.ScrollTo(0);
	}

	var stylePosition = parseInt(this.IsVertical ? this.Element.style.top : this.Element.style.left);

	if (isNaN(stylePosition))
	{
		stylePosition = 0;
	}
	
	var instance = this;
/*
	this.intervalPointer = setTimeout(function () 
	{
	    if (instance.Disposed)
	    {
	        return;
	    }
		instance.ApplyOverflow();
		instance.ScrollTo(stylePosition);
		instance.EvaluateArrowStatus();
	}, 100);*/
}

RadControlsNamespace.Scroll.prototype.AttachEventHandlers = function ()
{
    var element = this.Element;
    var instance = this;
    this.resizeClosure = function()
    {
		instance.ResizeHandler();
    }
    
    if (window.addEventListener)
    {
    	window.addEventListener("resize", this.resizeClosure, false);
    }
    else 
    {
		window.attachEvent("onresize", this.resizeClosure);
    }
}

RadControlsNamespace.Scroll.prototype.Dispose = function()
{
    this.Disposed = true;
    this.Element = null;
    clearTimeout(this.intervalPointer);
    if (window.removeEventListener)
    {
    	window.removeEventListener("resize", this.resizeClosure, false);
    }
    else 
    {
		window.detachEvent("onresize", this.resizeClosure);
    }
}

RadControlsNamespace.Scroll.prototype.AttachArrows = function ()
{
    // arrow creations
    var leftArrow = this.CreateArrow("&laquo;", 1, this.LeftArrowClass);
    
	var rightArrow = this.CreateArrow("&raquo;", -1, this.RightArrowClass);
	this.LeftArrow = leftArrow;
	this.RightArrow = rightArrow;	
	if (this.IsVertical)
	{
		leftArrow.style.left = "0px";   	
		rightArrow.style.left = "0px";	
		if (this.ScrollButtonsPosition == RadControlsNamespace.ScrollButtonsPosition.Middle)
		{
			leftArrow.style.top = "0px";    
			rightArrow.style.bottom = "0px";
		}
		else if (this.ScrollButtonsPosition == RadControlsNamespace.ScrollButtonsPosition.Left)
		{
			leftArrow.style.top = "0px";    
			rightArrow.style.top = leftArrow.offsetHeight + "px";
		}
		else
		{
			rightArrow.style.bottom = "0px";    
			leftArrow.style.bottom = leftArrow.offsetHeight + "px";
		}	
	}
	else
	{
		leftArrow.style.top = "0px";   	
		rightArrow.style.top = "0px";	
		if (this.ScrollButtonsPosition == RadControlsNamespace.ScrollButtonsPosition.Middle)
		{
			leftArrow.style.left = "-1px";    
			rightArrow.style.right = "-1px";
		}
		else if (this.ScrollButtonsPosition == RadControlsNamespace.ScrollButtonsPosition.Left)
		{
			leftArrow.style.left = "-1px";    
			rightArrow.style.left = (leftArrow.offsetWidth -1) + "px";
		}
		else
		{
			rightArrow.style.right = "-1px";
			leftArrow.style.right = (rightArrow.offsetWidth - 1) + "px";    
		}
	}
	

}

RadControlsNamespace.Scroll.prototype.CreateArrow = function(arrowText, scrollDirection, cssClass)
{
    var arrow = document.createElement('a');
    arrow.href = "#";
    arrow.className = cssClass;
    arrow.style.zIndex = "2000";
	arrow.appendChild(document.createTextNode("&nbsp;"));
    this.Element.parentNode.appendChild(arrow);
    
	var instance = this;
    arrow.ScrollDirection = scrollDirection;
	if (this.ScrollOnHover)
	{
		arrow.onmousedown = function ()
		{
    		if (this.disabled)
			{
				return false;
			}
			instance.ScrollAmount = 3;
			return true;
		}

		arrow.onmouseup = function ()
		{
			instance.ScrollAmount = 1;
		}
	    
		arrow.onmouseover = function ()
		{
    		if (this.disabled)
			{
				return false;
			}
			instance.ScrollAmount = 1;
			instance.Scroll(this.ScrollDirection);
			return true;
		}

		arrow.onmouseout = function ()
		{
			instance.scrollAmount = 0;
			instance.Stop();
			return false;
		}
	}
	else
	{
		arrow.onmousedown = function ()
		{
    		instance.Scroll(this.ScrollDirection);
		}
		
		arrow.onmouseup = function ()
		{
			instance.Stop();
		}
	}
	
	    
	arrow.onclick = function ()
	{
		return false;
	}
	    
	
	return arrow;
}

RadControlsNamespace.Scroll.prototype.SetHeight = function (value)
{
	if (parseInt(value) == 0)
	{
		return;
	}
	this.Element.parentNode.style.height = value;
	this.Initialize();
}

RadControlsNamespace.Scroll.prototype.SetWidth = function (value)
{
	if (parseInt(value) == 0)
	{
		return;
	}
	this.Element.parentNode.style.width = value;
	this.Initialize();
}

RadControlsNamespace.Scroll.prototype.CreateScrollWrap = function ()
{
    var scrollWrap = document.createElement('div');
    var originalContainer = this.Element.parentNode;
    scrollWrap.appendChild(this.Element); 
    scrollWrap.style.position = "relative";
    scrollWrap.align = "left"; // WTF??
    
    originalContainer.appendChild(scrollWrap);
    
    if (this.IsVertical)
    {
        scrollWrap.style.styleFloat = "left";
        scrollWrap.style.cssFloat = "left";
        this.Element.style.display = "none";
        scrollWrap.style.height = scrollWrap.parentNode.parentNode.offsetHeight + "px";
        this.Element.style.display = "block";
    }
    else
    {
		
		var realWidth = 0;
        for (var i = 0; i < this.Element.childNodes.length; i ++)
        {
			var node = this.Element.childNodes[i];
			if (!node.tagName) continue;
			
			realWidth += node.offsetWidth;
        }
        
        this.Element.style.width = (realWidth + 3) + "px";
   }
   
   return scrollWrap;
};

RadControlsNamespace.Scroll.prototype.CalculateMinMaxPosition = function ()
{
	if (!this.Initialized)
	{
		return;
	}

	if (this.IsVertical)
	{
		var scrollSize = this.Element.parentNode.offsetHeight - this.Element.offsetHeight;
		var leftSize = this.LeftArrow.offsetHeight;
		var rightSize = this.RightArrow.offsetHeight;
	}
	else
	{
		var scrollSize = this.Element.parentNode.offsetWidth - this.Element.offsetWidth;
		var leftSize = this.LeftArrow.offsetWidth;
		var rightSize = this.RightArrow.offsetWidth;	
	}
	
	if (!this.LeaveGapsForArrows)
	{
		leftSize = 0;
		rightSize = 0;
	}
		
	this.MaxScrollPosition = 0;
	this.MinScrollPosition = scrollSize - rightSize - leftSize;
	
	if (this.ScrollButtonsPosition ==
		RadControlsNamespace.ScrollButtonsPosition.Middle)
	{
		this.Offset = leftSize;
	}
	
	else if (this.ScrollButtonsPosition == 
	RadControlsNamespace.ScrollButtonsPosition.Left)
	{
		this.Offset = leftSize + rightSize;
	}
	else
	{
		this.Offset = 0;
	}	
	
}

RadControlsNamespace.Scroll.prototype.CalculateInitialTab = function ()
{
    // calculate the selected tab
    var lis = this.Element.getElementsByTagName('li');
    if (lis.length > 0)
    {
		var i = 0;
	
		while (this.ScrollPosition < - (this.IsVertical ? lis[i].offsetTop : lis[i].offsetLeft))
		{
			i ++;
		}
		this.CurrentTab = i;
	}
}

RadControlsNamespace.Scroll.prototype.AttachScrollMethods = function ()
{
    if (this.PerTabScrolling)
    {
        this.Scroll = RadControlsNamespace.Scroll.StartPerTabScroll;
        this.Stop = RadControlsNamespace.Scroll.StopPerTabScroll;    
    }
    
    else
    {
        this.Scroll = RadControlsNamespace.Scroll.StartSmoothScroll;
        this.Stop = RadControlsNamespace.Scroll.StopSmoothScroll;
    }

};

RadControlsNamespace.Scroll.prototype.EvaluateArrowStatus = function ()
{
	var rightEndReached = ! (this.ScrollPosition > this.MinScrollPosition);
	var leftEndReached = ! (this.ScrollPosition < this.MaxScrollPosition);
	this.RightArrow.disabled = rightEndReached;
	this.LeftArrow.disabled = leftEndReached;
	if (leftEndReached)
	{
		if (this.LeftArrow.className != this.LeftArrowClassDisabled)
		{
			this.LeftArrow.className = this.LeftArrowClassDisabled;
		}
	} 
	else
	{
		if (this.LeftArrow.className != this.LeftArrowClass)
		{
			this.LeftArrow.className = this.LeftArrowClass;
		}
	}
    
    if (rightEndReached)
	{
	
		if (this.RightArrow.className != this.RightArrowClassDisabled)
		{	
			this.RightArrow.className = this.RightArrowClassDisabled;
		}
	} 
	else
	{
		if (this.RightArrow.className != this.RightArrowClass)
		{	
			this.RightArrow.className = this.RightArrowClass;
		}
	}
}

RadControlsNamespace.Scroll.StartSmoothScroll = function (direction)
{
	this.Stop();
    this.Direction = direction;
    var instance = this;
    var scrollActivity = function () 
    {
        instance.ScrollBy(instance.Direction * instance.ScrollAmount); 
    } 
    
    scrollActivity();
    this.scrollInterval = setInterval(scrollActivity, 10);    
};


RadControlsNamespace.Scroll.prototype.ScrollTo = function (position)
{
    position = Math.max(position, this.MinScrollPosition);    
	position = Math.min(position, this.MaxScrollPosition);
	position += this.Offset;
    if (this.IsVertical)
    {
        this.Element.style.top = position + "px";
    }
    else
    {
        this.Element.style.left = position + "px";
    }
    
    this.Owner.ScrollPosition = this.ScrollPosition = position - this.Offset;
    this.EvaluateArrowStatus();
}

RadControlsNamespace.Scroll.prototype.ScrollBy = function (amount)
{
	var newScrollPosition = this.ScrollPosition;
    this.ScrollTo(newScrollPosition + amount);
}


/*Static methods, transformed in the AttachScrollMethods */

RadControlsNamespace.Scroll.StartPerTabScroll = function (direction)
{
	this.Stop();
    var lis = this.Element.getElementsByTagName('li');    
    var nextTab = this.CurrentTab - direction;
    
    if (nextTab < 0 || nextTab > lis.length)
    {
        return;
    }
    var dimDiv = direction == -1 ? this.CurrentTab : nextTab;
    this.CurrentTab = nextTab;
    

    
    if (this.IsVertical)
    {
        var newPosition = lis[dimDiv].offsetHeight;
    }
    else
    {
        var newPosition = lis[dimDiv].offsetWidth;
    }       
   
    this.ScrollBy(newPosition * direction);
    this.EvaluateArrowStatus();
};


RadControlsNamespace.Scroll.prototype.ScrollNeeded = function ()
{
	return true;
	
	if (this.IsVertical)
	{
		return this.Element.offsetHeight > this.Element.parentNode.offsetHeight;
	}
	
	return this.Element.offsetWidth > this.Element.parentNode.offsetWidth;
}

RadControlsNamespace.Scroll.StopSmoothScroll = function (direction)
{
	if (this.OnScrollStop)
	{
		this.OnScrollStop();
	}
    clearInterval(this.scrollInterval); 
};

RadControlsNamespace.Scroll.StopPerTabScroll = function (direction)
{
    if (this.OnScrollStop)
	{
		this.OnScrollStop();
	}
};

//BEGIN_ATLAS_NOTIFY
if (typeof(Sys) != "undefined")
{
	if (Sys.Application != null && Sys.Application.notifyScriptLoaded != null)
	{
		Sys.Application.notifyScriptLoaded();
	}
}
//END_ATLAS_NOTIFY