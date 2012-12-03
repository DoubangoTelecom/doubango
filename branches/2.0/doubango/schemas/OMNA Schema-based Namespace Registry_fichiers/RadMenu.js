if (typeof window.RadMenuNamespace == "undefined")
{
	window.RadMenuNamespace = {};
}

if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

RadControlsNamespace.AppendStyleSheet = function(callback, clientID, pathToCssFile)
{
	if (!pathToCssFile) 
	{ 
		return; 
	}

	var isGecko = window.netscape && !window.opera;
	
	if (!callback && isGecko)
	{
		//immediate css loading for Gecko
		document.write("<" + "link" + " rel='stylesheet' type='text/css' href='" + pathToCssFile + "' />");
	}
	else
	{
		var linkObject = document.createElement("link");
		linkObject.rel = "stylesheet";
		linkObject.type = "text/css";
		linkObject.href = pathToCssFile;
		document.getElementsByTagName("head")[0].appendChild(linkObject);
	}
};

RadMenuNamespace.ItemFlow = {Vertical : 0, Horizontal : 1};
RadMenuNamespace.ExpandDirection = {Auto : 0, Up : 1, Down : 2, Left : 3, Right : 4};
	
RadMenuNamespace.ExpandDirectionPropertyName = {"1" : 'bottom', "2" : 'top', "3" : 'right', "4" : 'left'};

function RadMenu (element)
{
	this.DomElement = element;
	
	this.ChildItemList = this.DomElement.getElementsByTagName("ul")[0];
	this.StateField = document.getElementById(element.id + "_Hidden");
	this.Items = [];
	this.AllItems = [];
	this.OpenedItem = null;
	this.LastExpandedItem = null;
	this.ExpandAnimation = {};
	this.CollapseAnimation = {};
	this.CollapseDelay = 500;
	this.ExpandDelay = 100;
	this.ID = element.id;
	this.Skin = "Default";
	this.RightToLeft = false;
	this.EnableScreenBoundaryDetection = true;
	this.InUpdate = false;
	this.Initialized = false;
	
	this.State = {};
	this.ItemState = {};
	
	this.CausesValidation = true;
	this.Flow = RadMenuNamespace.ItemFlow.Horizontal;
	this.ClickToOpen = false;
	this.Enabled = true;
	this.EnableAutoScroll = false;
	this.Clicked = false; // click to open flag
	this.OriginalZIndex = this.DomElement.style.zIndex;
	this.StateFieldDetached = false;
	
	
	this.Attributes = {};
	RadControlsNamespace.EventMixin.Initialize(this);
	RadControlsNamespace.DomEventMixin.Initialize(this);
	
/*	if (typeof(RadMenu.initializeBase) == "function")
	{
		RadMenu.initializeBase(this, [element]);
	}*/
}

/*RadControlsNamespace.MsAjaxMixin.Initialize(RadMenu, 'RadMenu');*/

RadMenu.Create = function ()
{
	var clientID = arguments[0];
	/*
	if (typeof ($create) == "function")
	{
		
		//the global RadMenu function object
		var type = this;
		
		//{name : 'value'} will trigger a menuInstance.set_name('value');
		var properties = {};
		
		//{itemClick : OnItemClick} will trigger a menuInstance.add_itemClick(OnItemClick);
		var events = {};
		
		//{"Treeview" : "RadTreeView1} will trigger a menuInstance.set_Treeview($find('RadTreeView1'));
		var references = {};
		
		//the DOM element that this component will attach to.  crucial for partial updates and disposes
		var domElement = $get(clientID);
		
		return $create(type, properties, events, references, domElement);
	}
	else
	{*/
	var oldMenu = window[clientID];
	
	if (oldMenu != null && oldMenu.Dispose)
	{
		oldMenu.Dispose();
	}
		var element = document.getElementById(clientID);
		return new this(element);
	/*}*/
}

RadMenu.JSONIncludeDeep = { "Attributes" : true };

RadMenu.CreateState = function (instance)
{
	instance.InitialState = {};
	for (var i in instance)
	{
		var type = typeof instance[i];
		if (type == "number" || type == "string" || type == "boolean")
		instance.InitialState[i] = instance[i];
	}
}

RadMenu.GetFirstChildByTagName = function(parentNode, tagName)
{
	var child = parentNode.getElementsByTagName(tagName)[0];
	
	if (child && child.parentNode == parentNode)
	{
		return child;
	}
	return null;
}

RadMenu.prototype.RenderInProgress = function()
{
   return this.DomElement.offsetWidth == 0 && RadControlsNamespace.Browser.IsIE;
}

RadMenu.prototype.Detach = function(e)
{
	if (!(RadControlsNamespace.Browser.IsIE) || document.readyState == "complete")
	{
		if (!this.StateFieldDetached)
		{
			var parentNode = this.DomElement.parentNode;
			this.StateField.parentNode.removeChild(this.StateField);
			parentNode.insertBefore(this.StateField, this.DomElement);
			this.StateFieldDetached = true;
		}
	
		//To solve the IE7 issue (disappearing html elements)
		this.DomElement.parentNode.removeChild(this.DomElement);
		document.forms[0].insertBefore(this.DomElement, document.forms[0].firstChild);
		this.DomElement.style.position = "absolute";
		this.Detached = true;
	}
}

RadMenu.prototype.ReAttach = function(e)
{
	if (!(RadControlsNamespace.Browser.IsIE) || document.readyState == "complete")
	{
		this.DomElement.parentNode.removeChild(this.DomElement);
		this.StateField.parentNode.insertBefore(this.DomElement, this.StateField);
		this.Detached = false;
	}
}

RadMenu.prototype.Show = function(e)
{
	if (!this.IsContext)
	{
	    return;
	}
	
	if (!this.RaiseEvent("OnClientContextShowing"))
	{
		return RadControlsNamespace.DomEvent.PreventDefault(e);
	}	

	for (var i in RadMenuNamespace.ContextMenus)
	{
		RadMenuNamespace.ContextMenus[i].Hide();
	}
	var x = this.MouseEventX(e);
	var y = this.MouseEventY(e);
	
	
	if (this.RightToLeft)
	{
	    this.ShowAt(x, y);
	    return this.ShowAt(x - this.ChildItemList.offsetWidth, y);
	}
	
	return this.ShowAt(x, y);
}

RadMenu.prototype.CreateRuntimeScroll = function (height)
{
	if (this.Scroll)
	{
		this.Scroll.SetHeight(height);
		return;
	}
	
	this.BuildScrollObject(true);
	this.Scroll.Initialize()
	
	this.ScrollWrap = this.ChildItemList.parentNode;
	this.Ease.Element = this.ScrollWrap;
	this.Ease.Overlay.Element = this.ScrollWrap;
	this.ScrollWrap.className = "scrollWrap " + this.ChildItemList.className;
	this.ChildItemList.className = "active vertical";
		
	this.Scroll.SetHeight(height);
}

RadMenu.prototype.ShowAt = function(x, y)
{
	if (!this.IsContext)
	{
	    return;
	}

	if (!this.Detached)
	{
		this.Detach();
	}
	this.ShownAsContext = true;
	
	this.Ease.ShowElements();
	
	var documentSize = RadControlsNamespace.Screen.GetViewPortSize();
	
	if (this.EnableAutoScroll && y + this.ChildItemList.offsetHeight > documentSize.height)
	{
		this.Ease.ShowElements();
		this.Ease.UpdateContainerSize();
		this.CreateRuntimeScroll(documentSize.height - y + "px");	
	}
	
	if (this.ScrollWrap)
	{
		this.ScrollWrap.style.width = this.ChildItemList.offsetWidth + "px";
		this.Scroll.Initialize();
	}
	
	this.Ease.UpdateContainerSize();
	
	if (!this.WidthFixed)
	{
		this.WidthFixed = true;
		this.FixItemWidth(this);
	}
	this.Position(x, y);
	this.Ease.In();

	this.RaiseEvent('OnClientContextShown', null);
	
	return false;

}

RadMenu.prototype.Position = function (x, y)
{
	var portSize = RadControlsNamespace.Screen.GetViewPortSize();
	
	x = Math.min(x, portSize.width - this.DomElement.offsetWidth);
	y = Math.min(y, portSize.height - this.DomElement.offsetHeight);
	
	if (isNaN(x)) x = 0;
	if (isNaN(y)) y = 0;
	this.DomElement.style.left = x + "px";
	this.DomElement.style.top = y + "px";
}

RadMenu.prototype.MouseEventX =  function(e)
{
	if (e.pageX)
	{
		return e.pageX;
	}
	else if (e.clientX)
	{
		if (RadControlsNamespace.Browser.StandardsMode)
		{
			return (e.clientX + document.documentElement.scrollLeft);
		}
		return (e.clientX + document.body.scrollLeft);
	}
};

RadMenu.prototype.MouseEventY = function(e)
{
	if (e.pageY)
	{
		return e.pageY;
	}
	else if (e.clientY)
	{
		if (RadControlsNamespace.Browser.StandardsMode)
		{
			return (e.clientY + document.documentElement.scrollTop);
		}
		return (e.clientY + document.body.scrollTop);
	}
};

RadMenu.prototype.EventSource = function(e)
{
	return RadControlsNamespace.DomEvent.GetTarget(e);
}

RadMenu.prototype.Hide = function()
{
	if (this.ShownAsContext)
	{
		if (this.Detached)
		{
			this.ReAttach();
		}

        this.Ease.Out();
		this.ShownAsContext = false;

        this.RaiseEvent('OnClientContextHidden', null);
	}
}

RadMenu.prototype.Initialize = function (configObject, itemData)
{
	this.LoadConfiguration(configObject);
	this.ItemData = itemData;	
	
	
    this.DetermineDirection();
    
    this.ApplyRTL();
		
	if (this.IsContext)
	{
		this.InitContextMenu();
	}
		
	this.CreateControlHierarchy(this, 0);
	
    if (!this.Enabled)
    {
		this.Disable();
    }
	
	if (this.Flow == RadMenuNamespace.ItemFlow.Vertical)
	{
		this.FixRootItemWidth();
	}
	
	this.AttachEventHandlers();

	this.Initialized = true;
	
	RadMenu.CreateState(this);

	this.RaiseEvent('OnClientLoad', null);
};


RadMenu.prototype.AttachEventHandlers = function ()
{
	var instance = this;
	
	this.DomElement.RadShow = function ()
	{
		if (instance.Flow == RadMenuNamespace.ItemFlow.Vertical)
		{
			instance.FixRootItemWidth();
		}
	}
    
    this.AttachDomEvent(window, "unload", "Dispose");
    
    if (!this.ClickToOpen && RadControlsNamespace.Browser.IsIE)
    {
        this.AttachDomEvent(document, "mouseout", "MouseOutHandler");
    }
    if (this.ClickToOpen)
    {
        this.AttachDomEvent(document, "click", "CloseOpenedItems");
    }
    
    //To refresh the menu position - required for IE because the menu is relatively positioned.
    this.AttachDomEvent(window, "resize", "RefreshPosition");
}

RadMenu.prototype.RefreshPosition = function(e)
{
	//Causing infinite window resizing when placed in iframe
    if (!this.IsContext)
    {
        this.DomElement.style.cssText = this.DomElement.style.cssText;
    }
}

RadMenu.prototype.MouseOutHandler = function(e)
{
    var sourceElement = RadControlsNamespace.DomEvent.GetTarget(e);
    var destinationElement = RadControlsNamespace.DomEvent.GetRelatedTarget(e);
    if (!destinationElement && !this.IsChildOf(this.DomElement, sourceElement))
    {
       //The mouse is out of the window or the current frame - close the menu
       var instance = this;
       setTimeout(function(){ instance.Close(); }, this.CollapseDelay);
    }
}

RadMenu.prototype.CloseOpenedItems = function(e)
{
    var sourceElement = this.EventSource(e);
    
    if (!this.IsChildOf(this.DomElement, sourceElement))
    {
        this.Close();
        
        this.Clicked = false;
    }
}

RadMenu.prototype.DetermineDirection = function ()
{
    var el = this.DomElement;
    while (el.tagName.toLowerCase() != 'html')
    {
        if (el.dir)
        {
            this.RightToLeft = (el.dir.toLowerCase() == "rtl");
            return;
        }
        el = el.parentNode;
    }
    
    this.RightToLeft = false;
}

RadMenu.prototype.ApplyRTL = function ()
{
	if (!this.RightToLeft) return;
    
    if (this.RenderInProgress())
    {
        //When the menu is inside a TABLE call it when the window loads
        this.AttachDomEvent(window, "load", "ApplyRTL");
        return;
    }
    
    this.FixItemWidthInRTL();
    
    if (RadControlsNamespace.Browser.IsIE)
    {
		this.DomElement.dir = "ltr";
    }
	if (!this.IsContext)
	{
		this.DomElement.className += " rtl RadMenu_" + this.Skin + "_rtl";
	}
	else
	{
		this.DomElement.className += " rtlcontext RadMenu_" + this.Skin + "_rtl";
	}
}


RadMenu.prototype.BuildScrollObject = function (wrapNeeded)
{
	var isVertical = RadMenuNamespace.ItemFlow.Vertical == this.Flow;
	var options = {PerTabScrolling : false, ScrollButtonsPosition : 1, ScrollPosition : 0};
	this.Scroll = new RadControlsNamespace.Scroll(this.ChildItemList, isVertical, options);
	this.Scroll.ScrollOnHover = true;
	this.Scroll.LeaveGapsForArrows = false;
	this.Scroll.WrapNeeded = wrapNeeded;
	if (isVertical)	
	{
		this.Scroll.LeftArrowClass = "topArrow";
		this.Scroll.LeftArrowClassDisabled = "topArrowDisabled";
		this.Scroll.RightArrowClass = "bottomArrow";
		this.Scroll.RightArrowClassDisabled = "bottomArrowDisabled";		
	}
}

RadMenu.prototype.InitContextMenu = function ()
{
	if (this.ChildItemList.parentNode != this.DomElement)
	{
		this.ScrollWrap = this.ChildItemList.parentNode;
		
		this.BuildScrollObject(false);
	}
	
	
	this.Ease = new RadControlsNamespace.Ease(
			this.ScrollWrap || this.ChildItemList, 
			this, 
			0,
			0,
			null,
			true
			
		);
	
	if (this.Ease.Overlay.Shim)
	{
	    this.Ease.Overlay.Shim.id = this.DomElement.id + "IframeOverlay";
	}
	
	this.Flow = RadMenuNamespace.ItemFlow.Vertical;
	this.Ease.SetSide("top");
	if (RadControlsNamespace.Browser.IsOpera)
	{
	    this.AttachDomEvent(document, "mousedown", "OnContextMenu");
	}
	else
	{
	    this.AttachDomEvent(document, "contextmenu", "OnContextMenu");
	}
	this.AttachDomEvent(document, "click", "OnDocumentClick");
	
	if (!RadMenuNamespace.ContextMenus)
	{
		RadMenuNamespace.ContextMenus = {};
	}
	
	if (!RadMenuNamespace.ContextElements)
	{
		RadMenuNamespace.ContextElements = {};
	}	
	
	if (this.ContextMenuElementID)
	{
		RadMenuNamespace.ContextElements[this.ContextMenuElementID] = true;
	}
	
	RadMenuNamespace.ContextMenus[this.ID] = this;
	
}

RadMenu.prototype.OnContextMenu = function(e)
{
	if (RadControlsNamespace.Browser.IsOpera)
	{
	    if (e.button != 2)
	    {
	        return;
	    }
	}
	
	this.ContextElement = document.getElementById(this.ContextMenuElementID);
	
	if (this.ContextMenuElementID && !this.ContextElement)
	{
	
		return;
	}
	
	var sourceElement = this.EventSource(e);
	if (this.ContextElement)
	{
		if (sourceElement == this.ContextElement || this.IsChildOf(this.ContextElement, sourceElement))
		{
			this.Show(e);
			RadControlsNamespace.DomEvent.PreventDefault(e);
			RadControlsNamespace.DomEvent.StopPropagation(e);
		}
	}
	else if (!RadMenuNamespace.ContextElements[sourceElement.id])
	{
		this.Show(e);
		RadControlsNamespace.DomEvent.PreventDefault(e);
		RadControlsNamespace.DomEvent.StopPropagation(e);
	}
}

RadMenu.prototype.IsChildOf = function(parent, child)
{
	if (child == parent)
	{
		return false;
	}
	
	while (child && (child != document.body))
	{
		if (child == parent)
		{
			return true;
		}
		try 
		{
			child = child.parentNode;
		}catch (e)
		{
			return false;
		}
	}

	return false;
};

RadMenu.prototype.OnDocumentClick = function(e)
{
	var sourceElement = this.EventSource(e);

	if (this.IsChildOf(this.DomElement, sourceElement))
	{
		if (this.ClickToOpen)
		{
			return;
		}
		
		if (!this.IsChildOfMenuItem(sourceElement))
		{
			return;
		}
	}
		
	this.Hide();
}

RadMenu.prototype.IsChildOfMenuItem = function (element)
{
	var tagName = element.tagName.toLowerCase();
	var className = element.className;
	
	if (tagName == "span" && className.indexOf("text") > -1)
	{
		return true;
	}
	
	if (tagName == "a" && className.indexOf("link") > -1)
	{
		return true;
	}
	
	if (tagName == "img" && className == "leftImage")
	{
		return true;
	}
	
	return false;
}

RadMenu.prototype.Enable = function ()
{
	this.Enabled = true;	
	this.DomElement.disabled = "";	
	for (var i = 0; i < this.AllItems.length; i ++)
    {
		this.AllItems[i].Enable();    
    }  
}

RadMenu.prototype.Disable = function ()
{	
	this.Enabled = false;
	this.DomElement.disabled = "disabled";	
	for (var i = 0; i < this.AllItems.length; i ++)
    {
		this.AllItems[i].Disable();    
    }
}

RadMenu.prototype.Focus = function ()
{
	this.DomElement.focus();
}

RadMenu.prototype.Dispose = function ()
{
	if (this.Disposed)
	{
		return;
	}
	
	this.Disposed = true;
	
	for (var i = 0; i < this.AllItems.length; i ++)
	{
		this.AllItems[i].Dispose();
	}	
	
	//TODO: Find the parent FORM
	if (this.Detached && this.DomElement)
	{
	    if(this.DomElement.parentNode)
	    {
	        this.DomElement.parentNode.removeChild(this.DomElement);
	    }
	}	
	
	if (this.DomElement)
	{
		this.DomElement.RadShow = null;
	}
	
	this.DomElement = null;
	this.ChildItemList = null;
	this.StateField = null;
	this.DisposeDomEventHandlers();
	if (this.IsContext && RadMenuNamespace.ContextMenus)
	{
		RadMenuNamespace.ContextMenus[this.ID] = null;
	}
}

RadMenu.prototype.CreateMenuItem = function (parent, domElement)
{
	var item = new RadMenuItem(domElement);
	this.AddItemToParent(parent, item);
	return item;
};

RadMenu.prototype.AddItemToParent = function(parent, item)
{
	item.Index = parent.Items.length;
	parent.Items[parent.Items.length] = item;
	
	item.GlobalIndex = this.AllItems.length;
	this.AllItems[this.AllItems.length] = item;

	item.Parent = parent;		
	item.Menu = this;
}

RadMenu.prototype.CreateControlHierarchy = function (parent, level)
{
	parent.Level = level;
	var element = parent.ChildItemList;
	if (!element) 
	{
		return;
	}
	
	for (var i = 0; i < element.childNodes.length; i ++)
	{
		var domNode = element.childNodes[i];
		if (domNode.nodeType == 3) continue;
		var item = this.CreateMenuItem(parent, domNode);
		item.Initialize();
		if (level == 0)
		{
			item.PostInitialize();
		}
		this.CreateControlHierarchy(item, level + 1);
	}
};

RadMenu.prototype.FixItemWidthInRTL = function ()
{
	var maxWidth = 0;
	var maxItemWidth = 0;
	var ul = this.ChildItemList;
	
	for (var i = 0; i < ul.childNodes.length; i ++)
	{
		var li = ul.childNodes[i];
		
		if (li.nodeType == 3) 
			continue;
		
		var element = RadMenu.GetFirstChildByTagName(li, "a");
		
		// Skip separators
		if (!element)
		{
			continue;
		}
		if (this.RightToLeft)
		{
		    var image = element.getElementsByTagName("img")[0];
		    if (image)
		    {
		        image.style.styleFloat = "left";
		    }
		}
		
		maxWidth = Math.max(RadControlsNamespace.Box.GetOuterWidth(element), maxWidth);
		
		if (this.RightToLeft)
		{
		    var image = element.getElementsByTagName("img")[0];
		    if (image)
		    {
		        image.style.styleFloat = "right";
		    }
		}
	}
	
	for (var i = 0; i < ul.childNodes.length; i ++)
	{
		var li = ul.childNodes[i];
		
		if (li.nodeType == 3) continue;
		
		if (RadControlsNamespace.Browser.IsOpera)
		{
			li.style.cssFloat = "none";
		}
		
		var a = RadMenu.GetFirstChildByTagName(li, "a");
		//Skip separators
		if (!a) continue;
		
		var imageOnly = !RadMenu.GetFirstChildByTagName(a, "span").firstChild;
		
		if (!RadControlsNamespace.Browser.IsIE || !imageOnly)
		{
			RadControlsNamespace.Box.SetOuterWidth(a, RadControlsNamespace.Box.GetOuterWidth(a));
		}
	}
	
	if (RadControlsNamespace.Browser.IsSafari)
	{
		ul.style.width = RadMenu.GetFirstChildByTagName(ul, "li").offsetWidth + "px";
	}
};


RadMenu.prototype.FixItemWidth = function (item)
{
	
	var maxWidth = 0;
	var ul = item.ChildItemList;
	var widestItem = null;
	
	for (var i = 0; i < ul.childNodes.length; i ++)
	{
		var li = ul.childNodes[i];
		
		if (li.nodeType == 3) 
			continue;
		
		var element = RadMenu.GetFirstChildByTagName(li, "a");
		
		
		if (!element)
		{
			element = RadMenu.GetFirstChildByTagName(li, "div");
			if (!element)
            {
		        //Separator
		        continue;
	        }
		}
		if (this.RightToLeft)
		{
		    var image = element.getElementsByTagName("img")[0];
		    if (image)
		    {
		        image.style.styleFloat = "left";
		        image.style.cssFloat = "left";
		    }
		}
		var width = RadControlsNamespace.Box.GetOuterWidth(element);
		
		if (isNaN(width))
		{   
		    continue;
		}
		
		if (width > maxWidth)
		{
		    maxWidth = width;
		    widestItem = li;
		}
		
		if (this.RightToLeft)
		{
		    var image = element.getElementsByTagName("img")[0];
		    if (image)
		    {
		        image.style.styleFloat = "right";
		        image.style.cssFloat = "right";
		    }
		}
	}

	var calculatedMaxWidth = 0;
		
	for (var i = 0; i < ul.childNodes.length; i ++)
	{
		var li = ul.childNodes[i];
		
		if (li.nodeType == 3) continue;
		
		if (RadControlsNamespace.Browser.IsOpera)
		{
			li.style.cssFloat = "none";
		}
		
		var a = RadMenu.GetFirstChildByTagName(li, "a");
		
		if (a)
		{
			var imageOnly = !RadMenu.GetFirstChildByTagName(a, "span").firstChild;
			
			if (!RadControlsNamespace.Browser.IsIE || !imageOnly)
			{
			    if (a.style.display != "none")
			    {
			        if (calculatedMaxWidth > 0)
			        {
						a.style.width = calculatedMaxWidth + "px";	
			        }
			        else
			        {
						calculatedMaxWidth = RadControlsNamespace.Box.SetOuterWidth(a, maxWidth);
					}
			    }
			}
		}
		else
		{
			li.style.width = maxWidth + "px";
		}
	}
	
	if (RadControlsNamespace.Browser.IsSafari)
	{
		if (widestItem)
		{
		    ul.style.width = widestItem.offsetWidth + "px";
		}
	}
};

RadMenu.prototype.FixRootItemWidth = function()
{
	var instance = this;
	var fixItemWidthClosure = function()
	{
		instance.FixItemWidth(instance);
	}
	
	if (this.RenderInProgress() || RadControlsNamespace.Browser.IsOpera || RadControlsNamespace.Browser.IsSafari)
	{
		if (window.addEventListener)
		{
			window.addEventListener('load', fixItemWidthClosure, false);
		}
		else
		{
			window.attachEvent('onload', fixItemWidthClosure);
		}
	}
	else
	{
		fixItemWidthClosure();
	}
}

RadMenu.prototype.FixListWidth = function (item)
{
    if (this.RightToLeft)
    {
       this.FixItemWidth(item);
    }
    
	var totalWidth = 0;
	var ul = item.ChildItemList;
	for (var i = 0; i < ul.childNodes.length; i ++)
	{
		var node = ul.childNodes[i];
		if (node.nodeType == 3) continue;
		totalWidth += node.offsetWidth;
		// Additional
		node.style.clear = "none";
	}
	ul.style.width = totalWidth + "px";
};


RadMenu.prototype.LoadConfiguration = function (configObject)
{
    for (var property in configObject)
    {
        this[property] = configObject[property];
    } 
    
    if (!this.DefaultGroupSettings)
    {
		this.DefaultGroupSettings = {};
    }
    
    if (typeof this.DefaultGroupSettings.Flow == 'undefined')
    {
		this.DefaultGroupSettings.Flow = RadMenuNamespace.ItemFlow.Vertical;
    }
    
    if (typeof this.DefaultGroupSettings.ExpandDirection == 'undefined')
    {
		this.DefaultGroupSettings.ExpandDirection = RadMenuNamespace.ExpandDirection.Auto;
    }
    
    if (typeof this.DefaultGroupSettings.OffsetX == 'undefined')
    {
		this.DefaultGroupSettings.OffsetX = 0;
    }		 
    
	if (typeof this.DefaultGroupSettings.OffsetY == 'undefined')
    {
		this.DefaultGroupSettings.OffsetY = 0;
    }	
    
};

RadMenu.prototype.Close = function (configObject)
{
	if (this.OpenedItem)
	{
		this.OpenedItem.Close();
	}
};

RadMenu.prototype.FindItemByText = function(text)
{
   for (var i = 0; i < this.AllItems.length; i++)
   {
		if (this.AllItems[i].Text == text)
		{
			return this.AllItems[i];
		}
   }
   
   return null;
};

RadMenu.prototype.FindItemById = function(id)
{	
	for (var i = 0; i < this.AllItems.length; i++)
	{
		if (this.AllItems[i].ID == id)
		{
			return this.AllItems[i];
		}
	}
	return null;
};

RadMenu.prototype.FindItemByValue = function(value)
{	
	for (var i = 0; i < this.AllItems.length; i++)
	{
		if (this.AllItems[i].Value == value)		
		{
			return this.AllItems[i];
		}		
	}
	return null;
};

RadMenu.prototype.FindItemByUrl = function(url)
{	
	for (var i = 0; i < this.AllItems.length; i++)
	{
		if (this.AllItems[i].NavigateUrl == url)
		{
			return this.AllItems[i];
		}		
	}
	return null;
};

RadMenu.prototype.SetContextElementID = function (id)
{
	if (!RadMenuNamespace.ContextElements)
	{
		RadMenuNamespace.ContextElements = {};
	}

	if (this.ContextMenuElementID)
	{
		RadMenuNamespace.ContextElements[this.ContextMenuElementID] = false;
	}
	
	this.ContextMenuElementID = id;
	RadMenuNamespace.ContextElements[this.ContextMenuElementID] = false;
}

RadMenu.prototype.RecordState = function ()
{
	if (this.InUpdate || !this.Initialized)
	{
		return;
	}
	
	
	var state = RadControlsNamespace.JSON.stringify(this, this.InitialState, RadMenu.JSONIncludeDeep);
	var itemState = []
	for (var i in this.ItemState)
	{
		if (this.ItemState[i] == "") continue;
		if (typeof this.ItemState[i] == "function") continue;
		itemState[itemState.length] = this.ItemState[i];
	}
	this.StateField.value = "{\"State\":" + state + ",\"ItemState\":{" + itemState.join(",") + "}}";
}

RadMenu.prototype.PersistClientSideItems = function ()
{
	for (var i = 0; i < this.AllItems.length; i++)
	{
		var item = this.AllItems[i];
		
		if (item.ClientSide)
		{
			item.RecordState(true);
		}
	}
}

RadMenu.prototype.SetAttribute = function (name, value)
{
	this.Attributes[name] = value;
	this.RecordState();
}

RadMenu.prototype.GetAttribute = function (name)
{
	return this.Attributes[name];
}

RadMenu.CreateChildItemContainer = function(parentItem)
{
	var animationContainer = document.createElement("div");
	animationContainer.className = "slide";
	parentItem.DomElement.appendChild(animationContainer);
	
	var groupContainer = document.createElement("ul");
	
	if (parentItem.Flow == RadMenuNamespace.ItemFlow.Horizontal)
	{
		groupContainer.className = "horizontal group level" + parentItem.Level;
	}else
	{
		groupContainer.className = "vertical group level" + parentItem.Level;
	}
	animationContainer.appendChild(groupContainer);
}

RadMenu.prototype.AddItem = function(item)
{
    var domElement = document.createElement("li");
    domElement.className = "item last";
    
    var linkElement = document.createElement("a");
    
    linkElement.className = "link";
    
    var textElement = document.createElement("span");
    textElement.className = "text";
    
    linkElement.appendChild(textElement);
    		    
    domElement.appendChild(linkElement);
    
    item.SetDomElement(domElement);
    
    var menu = this.Menu || this;
	
	if (menu != this && this.Items.length == 0)
	{
		RadMenu.CreateChildItemContainer(this);
		this.InitializeDomElements();
		this.InitializeAnimation();
	}

    this.ChildItemList.appendChild(domElement);
        
    menu.AddItemToParent(this, item);
	item.Level = this.Level + 1;    
    var text = item.Text;
    item.Text = "";
    item.ID = this.ID + "_m" + (this.Items.length - 1);
    item.Initialize();
    item.SetText(text);
    
    
    if (this.Menu)
    {
        item.ParentClientID = this.ID;
    }
    
    linkElement.href = "#";
    
    if (this.Items.length > 1)
    {
		var previousDomElement = this.Items[this.Items.length - 2].DomElement;
		previousDomElement.className = previousDomElement.className.replace(" last", "");
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
