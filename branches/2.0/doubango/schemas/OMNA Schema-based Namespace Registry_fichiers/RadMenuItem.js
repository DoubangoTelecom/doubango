function RadMenuItem(domElement)
{
	if (domElement)
	{
		this.SetDomElement(domElement);
	}
	else
	{
		this.ClientSide = true;
	}
	
	this.IsSeparator = false;
	this.AnimationContainer = null;
	this.OpenedItem = null;
	this.FocusedItem = null;
	this.Items =  [];
	this.Attributes = {};
	this.Index = -1;
	this.Level = -1;
	this.Parent = null;
	this.Menu = null;
	this.GroupSettings = {};
	this.TimeoutPointer = null;
	this.Templated = false;
	this.NavigateAfterClick = true;
	
	// css classes, load from the server
	this.FocusedCssClass = "focused";
	this.ClickedCssClass = "clicked";
	this.ExpandedCssClass = "expanded";
	this.DisabledCssClass = "disabled";
	this.CssClass = "";
	
	// state flags
	this.State = RadMenuItemState.Closed;
	this.Focused = false;
	this.Clicked = false;
	this.Enabled = true;
	
	this.Initialized = false;
}

RadMenuItem.prototype.SetDomElement = function(domElement)
{
	this.DomElement = domElement;
	
	this.LinkElement = RadMenu.GetFirstChildByTagName(this.DomElement, "a");
	
	if (this.LinkElement == null)
	{
		
		this.ID = this.DomElement.id;
		this.TextElement = RadMenu.GetFirstChildByTagName(this.DomElement, "span");
		this.NavigateUrl = "";	
	}
	else
	{
		this.ID = this.LinkElement.id;
		this.TextElement = RadMenu.GetFirstChildByTagName(this.LinkElement, "span");
		this.NavigateUrl = this.LinkElement.href;
	}
}
RadMenuItem.prototype.PostInitializeItems = function ()
{
	for (var i = 0; i < this.Items.length; i ++)
	{
		this.Items[i].PostInitialize();
	}
}

RadMenuItem.prototype.SetText = function (text)
{
	this.PostInitialize();
	this.TextElement.innerHTML = text;
	this.Text = text;
	this.RecordState();
}

RadMenuItem.prototype.SetNavigateUrl = function (url)
{
	this.PostInitialize();
	this.LinkElement.setAttribute("href", url);
	this.NavigateUrl = url;
	this.RecordState();
}

RadMenuItem.prototype.SetValue = function (value)
{
	this.PostInitialize();
	this.Value = value;
	this.RecordState();
}

RadMenuItem.prototype.InitializeDomElements = function()
{
	if (!this.Templated)
	{
		this.AnimationContainer = RadMenu.GetFirstChildByTagName(this.DomElement, "div");
		this.ImageElement = RadMenu.GetFirstChildByTagName(this.LinkElement || this.DomElement, "img");
		if (this.ImageElement)
		{
			this.ImageUrl = this.ImageElement.src;
		}
	}
	else
	{
		this.TextElement = RadMenu.GetFirstChildByTagName(this.DomElement, "div");
		this.AnimationContainer = this.TextElement.nextSibling;
	}
	
	if (this.AnimationContainer)
	{
		var ul = this.AnimationContainer.getElementsByTagName("ul")[0];
		
		this.ChildItemList = ul;
		
		if (this.ChildItemList.parentNode != this.AnimationContainer)
		{
			this.ScrollWrap = this.ChildItemList.parentNode;	
		}
	}
}

RadMenuItem.prototype.InitializeAnimation = function()
{
	this.DetermineExpandDirection();
	
	if (!this.AnimationContainer)
	{
		return;
	}
	
	this.Ease = new RadControlsNamespace.Ease(
		this.ScrollWrap || this.ChildItemList, 
		this.Menu, 
		this.GroupSettings.OffsetX,
		this.GroupSettings.OffsetY,
		this,
		true
	); 

	if (this.Ease.Overlay.Shim && this.LinkElement)
	{
	    this.Ease.Overlay.Shim.id = this.LinkElement.id + "IframeOverlay";
	}

	var expandDirection = this.GroupSettings.ExpandDirection;
	var easeProperty = RadMenuNamespace.ExpandDirectionPropertyName[expandDirection];
	
	this.Ease.SetSide(easeProperty);
	this.TextElement.className = "text expand" + this.Ease.GetSide();
	this.AnimationContainer.style.zIndex = this.GlobalIndex + 10;
	this.ChildItemList.style.zIndex = this.GlobalIndex + 10;
	if (this.ScrollWrap)
	{
		this.CreateScroll();
	}
}

RadMenuItem.prototype.Initialize = function ()
{
	RadControlsNamespace.DomEventMixin.Initialize(this);

	this.LoadConfiguration();

	this.InitializeDomElements();

	if (this.TextElement && this.TextElement.firstChild)
	{
	
		this.Text = this.TextElement.firstChild.nodeValue;
	}
	
	this.OriginalZIndex = Math.max(this.DomElement.style.zIndex, this.Menu.OriginalZIndex);
	
};

RadMenuItem.prototype.PostInitialize = function ()
{
	if (this.Initialized)
	{
		return;
	}
	this.InitializeAnimation();
	this.AttachEventHandlers();
	this.RenderAccessKey();
	RadMenu.CreateState(this);
	this.UpdateCssClass();
	this.Initialized = true;
}

RadMenuItem.prototype.RenderAccessKey = function ()
{
	if (this.IsSeparator || this.Templated)
	{
		return;
	}
	
	var accessKey = this.LinkElement.accessKey.toLowerCase();
	
	// If accessKey is not set
	if (!accessKey)
	{
		return;
	}
	var text = this.TextElement.firstChild.nodeValue;
	var indexOfAccessKey = text.toLowerCase().indexOf(accessKey);
	
	// If accesKey is not found
	if (indexOfAccessKey == -1)
	{
		return;
	}
	
	this.TextElement.innerHTML = 
		text.substr(0, indexOfAccessKey) + 
		"<u>" + 
		text.substr(indexOfAccessKey,  1) + 
		"</u>" + 
		text.substr(indexOfAccessKey + 1, text.length);
}

RadMenuItem.prototype.CreateScroll = function ()
{
	this.ScrollWrap.style.zIndex = this.GlobalIndex + 10;
	this.BuildScrollObject(false);

}

RadMenuItem.prototype.BuildScrollObject = function (wrapNeeded)
{
	var isVertical = RadMenuNamespace.ItemFlow.Vertical == this.GroupSettings.Flow;
	var options = {PerTabScrolling : false, ScrollButtonsPosition : 1, ScrollPosition : 0};
	this.Scroll = new RadControlsNamespace.Scroll(this.ChildItemList, isVertical, options);
	this.Scroll.ScrollOnHover = true;
	this.Scroll.LeaveGapsForArrows = false;
	this.Scroll.WrapNeeded = wrapNeeded;
	if (this.GroupSettings.Flow == RadMenuNamespace.ItemFlow.Vertical)	
	{
		this.Scroll.LeftArrowClass = "topArrow";
		this.Scroll.LeftArrowClassDisabled = "topArrowDisabled";
		this.Scroll.RightArrowClass = "bottomArrow";
		this.Scroll.RightArrowClassDisabled = "bottomArrowDisabled";		
	}
}

RadMenuItem.prototype.CreateRuntimeScroll = function (height)
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
	this.ChildItemList.className = this.ChildItemList.className.replace("group", "");
	this.Scroll.SetHeight(height);
}

RadMenuItem.prototype.Dispose = function ()
{
	if (!this.Initialized) return;
	this.DisposeDomEventHandlers();
	if (this.Ease)
	{
		this.Ease.Dispose();
	}
	this.DomElement = null;
	this.LinkElement = null;	
	this.AnimationContainer = null;	
}

RadMenuItem.prototype.Focus = function ()
{
	if (!this.CanFocus())
	{
		return;
	}
	
	this.PostInitializeItems();
	
	if (this.Parent.OpenedItem && this.Parent.OpenedItem != this)
	{
		this.Parent.OpenedItem.Close();
	}
	
	if (this.Parent.State != RadMenuItemState.Open && this.Parent.Open)
	{
		this.Parent.Open();
	}

	this.Parent.FocusedItem = this;
	
	if (!this.Focused && this.LinkElement)
	{
		this.LinkElement.focus();
	}
	
	this.UpdateCssClass();
	
	this.RaiseEvent("OnClientItemFocus");
}

RadMenuItem.prototype.Hide = function()
{
    if (this.LinkElement)
    {
        this.LinkElement.style.display = "none";
    }
    else
    {
        this.TextElement.style.display = "none";
    }
    
    if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Vertical)    
    {
        var siblings = this.Parent.Items;
        for (var i = 0; i < siblings.length; i++)
        {
            if (siblings[i] != this)
            {
                if (siblings[i].LinkElement)
                {
                    siblings[i].LinkElement.style.width = "auto";
                }
            }
        }
        if (RadControlsNamespace.Browser.IsSafari)
        {
            this.Parent.ChildItemList.style.width = "auto";
        }
      
        this.Menu.WidthFixed = false;
        
        if (this.Parent.DomElement.offsetWidth > 0)
        {
            this.Menu.FixItemWidth(this.Parent);
        }
    }
}

RadMenuItem.prototype.IsVisible = function()
{
    if (!this.LinkElement)
    {
        return this.TextElement.style.display != "none";
    }
    
    return this.LinkElement.style.display != "none";
}

RadMenuItem.prototype.Show = function()
{
    if (this.LinkElement)
    {
        this.LinkElement.style.display = "";
    }
    else
    {
        this.TextElement.style.display = "";
    }
    
    if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Vertical)
    {
        this.Menu.WidthFixed = false;
        
        if (this.Parent.DomElement.offsetWidth > 0)
        {
            this.Menu.FixItemWidth(this.Parent);
        }
    }
}


RadMenuItem.prototype.Blur = function ()
{
	if (this.IsSeparator)
	{
		return;
	}
	
	if (this.Focused)
	{
		this.LinkElement.blur();
	}
	
	this.Parent.FocusedItem = null;
	this.UpdateCssClass();
	this.RaiseEvent("OnClientItemBlur");
}
	

RadMenuItem.prototype.GetEaseSide = function ()
{
	var expandDirection = this.GroupSettings.ExpandDirection;
	return RadMenuNamespace.ExpandDirectionPropertyName[expandDirection];
}

RadMenuItem.prototype.RaiseEvent = function(eventName)
{
	return this.Menu.RaiseEvent(eventName, {Item:this});
}

RadMenuItem.prototype.UpdateCssClass = function ()
{
	if (this.IsSeparator || this.Templated)
	{
		return;
	}
	
	var cssClass = "link " + this.CssClass;
	
	if (this.Focused)
	{
		cssClass = cssClass + " " + this.FocusedCssClass;
	}

	if (this.State == RadMenuItemState.Open)
	{
		cssClass = cssClass + " " + this.ExpandedCssClass;
	}

	if (this.Clicked)
	{
		cssClass = cssClass + " " + this.ClickedCssClass;
	}

    if (!this.Enabled)
    {
        cssClass = cssClass + " " + this.DisabledCssClass;
    }

	this.LinkElement.className = cssClass;
	this.UpdateImageUrl();
}

RadMenuItem.prototype.UpdateImageUrl = function ()
{
	if (!this.ImageElement) return;

	var newUrl = this.ImageUrl;

	if (this.Hovered && this.ImageOverUrl)
	{
		newUrl = this.ImageOverUrl;
	}

	if (this.State == RadMenuItemState.Open && this.ExpandedImageUrl)
	{
		newUrl = this.ExpandedImageUrl;
	}
	
	if (!this.Enabled && this.DisabledImageUrl)
	{
		newUrl = this.DisabledImageUrl;
	}
	
	if (this.Clicked && this.ImageClickedUrl)
	{
	    newUrl = this.ImageClickedUrl;
	}
	
	    
    newUrl = newUrl.replace(/&amp;/ig, "&");
	
	if (newUrl != this.ImageElement.src)
	{
		this.ImageElement.src = newUrl;
	}
}


RadMenuItem.prototype.Enable = function ()
{
	if (this.IsSeparator || this.Templated)
	{
		return;
	}
	
	this.LinkElement.disabled = "";	
	
	if (this.ImageElement)
	{
	    this.ImageElement.disabled = "";
	}
	
	this.Enabled = true;
	this.EnableDomEventHandling();
	this.UpdateCssClass();
}

RadMenuItem.prototype.Disable = function ()
{
	if (this.IsSeparator || this.Templated)
	{
		return;
	}
	
	this.LinkElement.disabled = "disabled";	
	
	if (this.ImageElement)
	{
	    this.ImageElement.disabled = "disabled";
	}

	this.Enabled = false;
	this.DisableDomEventHandling();
	this.UpdateCssClass();
}
	


RadMenuItem.prototype.OnCollapseComplete = function()
{
	this.RaiseEvent("OnClientItemClose");
}

RadMenuItem.prototype.HideChildren = function ()
{
	for (var i = 0; i < this.Items.length; i++)
	{
		if (this.Items[i].AnimationContainer)
		{
			this.Items[i].AnimationContainer.style.display = "none";
		}
	}
}

RadMenuItem.prototype.CalculateScrollWrapSize = function ()
{
	if (!this.ScrollWrap)
	{
		return;
	}
	if (!this.ScrollWrap.style.height)
	{
		this.ScrollWrap.style.height = this.ChildItemList.offsetHeight + "px";
	}	

	var isVertical = RadMenuNamespace.ItemFlow.Vertical == this.Flow;
	if (isVertical)
	{
		this.ScrollWrap.style.width = this.ChildItemList.offsetWidth + "px";
	}			
};

RadMenuItem.prototype.OnEase = function(newValue)
{
	var isVertical = RadMenuNamespace.ItemFlow.Vertical == this.Flow;
	
	if (!isVertical) return;

	if (this.ChildrenDetached && this.Scroll)
	{
		//Hack for IE6 - arrows disappear after scrolling
		this.Scroll.RightArrow.style.cssText = this.Scroll.RightArrow.style.cssText;
		this.Scroll.LeftArrow.style.cssText = this.Scroll.LeftArrow.style.cssText;
	}
}

/**
 * Event handlers
 */

RadMenuItem.prototype.AttachEventHandlers = function ()	
{
	this.AttachDomEvent(this.DomElement, "mouseover", "MouseOverHandler");
	this.AttachDomEvent(this.DomElement, "mouseout", "MouseOutHandler");
	
	if (this.IsSeparator || this.Templated)
	{
		return;
	}
	
	this.AttachDomEvent(this.LinkElement, "click", "ClickHandler", true);

	this.AttachDomEvent(this.LinkElement, "mouseout", "HRefMouseOutHandler");
	this.AttachDomEvent(this.LinkElement, "mouseover", "HRefMouseOverHandler");
	
	this.AttachDomEvent(this.LinkElement, "mousedown", "MouseDownHandler");
	this.AttachDomEvent(this.LinkElement, "mouseup", "MouseUpHandler");
	
	this.AttachDomEvent(this.LinkElement, "blur", "BlurHandler");
	this.AttachDomEvent(this.LinkElement, "focus", "FocusHandler");
	
	this.AttachDomEvent(this.LinkElement, "keydown", "KeyDownHandler");
};

RadMenuItem.prototype.MouseDownHandler = function (e)
{
	this.Clicked = true;
	this.UpdateCssClass();
}

RadMenuItem.prototype.MouseUpHandler = function (e)
{
	this.Clicked = false;
	this.UpdateCssClass();
}

RadMenuItem.prototype.HRefMouseOutHandler = function (e)
{
	var to = RadControlsNamespace.DomEvent.GetRelatedTarget(e);
	
	if (this.Menu.IsChildOf(this.LinkElement, to) || to == this.LinkElement)
	{
		return;
	}
	this.Hovered = false;
	
	this.UpdateImageUrl();	
	
	this.RaiseEvent("OnClientMouseOut");
}

RadMenuItem.prototype.HRefMouseOverHandler = function (e)
{
	var from = RadControlsNamespace.DomEvent.GetRelatedTarget(e);
	if (this.Menu.IsChildOf(this.LinkElement, from) || this.LinkElement == from)
	{
		return;
	}
	
	this.Hovered = true;
	
	this.UpdateImageUrl();
	
	this.RaiseEvent("OnClientMouseOver");
}

RadMenuItem.prototype.KeyDownHandler = function (e)
{
	var arrows = {left : 37, up : 38, right : 39, down : 40, esc : 27 };
	
	var keyCode = RadControlsNamespace.DomEvent.GetKeyCode(e);
	
	if (keyCode == arrows.right)
	{
		if (this.Menu.RightToLeft)
		{
			this.HandleLeftArrow();
		}
		else
		{
			this.HandleRightArrow();
		}
	}
	else if (keyCode == arrows.left)
	{
		if (this.Menu.RightToLeft)
		{
			this.HandleRightArrow();
		}
		else
		{
			this.HandleLeftArrow();
		}
	}
	else if (keyCode == arrows.up)
	{
		this.HandleUpArrow();	
	}
	else if (keyCode == arrows.down)
	{
		this.HandleDownArrow();
	}
	
	else if (keyCode == arrows.esc)
	{
		if (this.Parent == this.Menu)
		{
			this.Blur();
		}
		else
		{
			this.Parent.Close();
			this.Parent.Focus();
		}
	}
	else
	{
		return;
	}
	
	RadControlsNamespace.DomEvent.PreventDefault(e);

}


RadMenuItem.prototype.FocusHandler = function (e)
{
	this.Focused = true;
	this.Focus();
}

RadMenuItem.prototype.BlurHandler = function (e)
{
	this.Focused = false;
	//Korchev: Causes problem when second level item is clicked - the item cannot be open after that
	//this.Menu.Clicked = false;
	
	this.Blur();
}

RadMenuItem.prototype.NavigatesToURL = function ()
{
	if (location.href + "#" == this.NavigateUrl || location.href == this.NavigateUrl)
	{
	    return false;
	}
	return (new RegExp("//")).test(this.LinkElement.href);
}

RadMenuItem.prototype.Validate = function ()
{
	if (!this.Menu.CausesValidation || this.NavigatesToURL())
	{
		return true;
	}
	
	if (typeof (Page_ClientValidate) != 'function')
	{
		return true;
	}
	
	return Page_ClientValidate(this.Menu.ValidationGroup);
}


RadMenuItem.prototype.ClickHandler = function (e)
{
	if (!this.Enabled)
	{
		return RadControlsNamespace.DomEvent.PreventDefault(e);
	}
	if (!this.RaiseEvent("OnClientItemClicking"))
	{
		return RadControlsNamespace.DomEvent.PreventDefault(e);
	}	
	if (!this.Validate())
	{
		return RadControlsNamespace.DomEvent.PreventDefault(e);
	}
    
	var returnValue = true;
	
	if (!this.Menu.ClickToOpen)
	{
		returnValue = true;
	}
	else if (this.Level > 1) // only first level items toggle this
	{
		returnValue = true;
	}
	else 
	{
		if (!this.Menu.Clicked)
		{
			this.Open();
		}
		else
		{
			this.Close();
		}
		
		this.Menu.Clicked = !this.Menu.Clicked;
	}
	
	this.RaiseEvent("OnClientItemClicked");

	if (!this.NavigateAfterClick || !returnValue)
	{
		RadControlsNamespace.DomEvent.PreventDefault(e);
	}
}

RadMenuItem.prototype.PreventClose = function()
{
	if (this.State == RadMenuItemState.AboutToClose)
	{
		this.ClearTimeout();
		this.State = RadMenuItemState.Open;
		this.Parent.OpenedItem = this;
	}
	if (this.Parent.PreventClose)
	{
		this.Parent.PreventClose();
	}
}

RadMenuItem.prototype.Open = function ()
{	
	this.PostInitializeItems();

	this.Menu.AboutToCollapse = false;
	
	if (this.Parent != this.Menu && this.Parent.State != RadMenuItemState.Open)
	{
		this.Parent.Open();
	}
	
	if (!this.AnimationContainer) return;
	
	this.Parent.OpenedItem = this;
	
	this.State = RadMenuItemState.Open;
		
	var documentSize = RadControlsNamespace.Screen.GetViewPortSize();
	
	this.ChildItemList.style.display = "block";
	
	this.Ease.ShowElements();

	if (this.GroupSettings.Flow == RadMenuNamespace.ItemFlow.Vertical)
	{
		this.Menu.FixItemWidth(this);
	}
	else
	{
		this.Menu.FixListWidth(this);
	}
	
	if (this.Menu.EnableAutoScroll && this.ChildItemList.offsetHeight > documentSize.height)
	{
		if (!this.ScrollWrap || this.ScrollWrap.offsetHeight > documentSize.height)
		{
			this.CreateRuntimeScroll(documentSize.height + "px");	
			this.Ease.ShowElements();
			this.Ease.UpdateContainerSize();
		}
	}
	
	this.Ease.SetSide(this.GetEaseSide());
	this.Ease.UpdateContainerSize();	
	
	if (this.Scroll)
	{
		this.CalculateScrollWrapSize();		
		this.Scroll.Initialize();
	}
	

	this.PositionChildContainer(documentSize);
	
	
	this.Ease.In();

	this.UpdateCssClass();
	
	this.DomElement.style.zIndex = this.OriginalZIndex + 1000;
	
	if (!RadControlsNamespace.Browser.IsNetscape)
	{
		this.Menu.DomElement.style.zIndex = this.Menu.OriginalZIndex + 1000;
	}
	
	this.RaiseEvent("OnClientItemOpen");
};	

RadMenuItem.prototype.MouseOverHandler = function (e)
{
	this.PreventClose();
	
	if (this.Menu.ClickToOpen && !this.Menu.Clicked)
	{
		return;
	}
	
	if (this.State == RadMenuItemState.Open || this.State == RadMenuItemState.AboutToOpen)
	{
		return;
	}
	
    if (this.Parent.OpenedItem != this && this.Parent.OpenedItem)
    {
		var openedItem = this.Parent.OpenedItem;
		if (openedItem.TimeoutPointer)
		{
			openedItem.ClearTimeout();
		}
		openedItem.State = RadMenuItemState.AboutToClose;
		openedItem.SetTimeout(function() {
			openedItem.Close();
			openedItem.TimeoutPointer = null;
		}, this.Menu.ExpandDelay);
    }
	
	if (this.Items.length < 1)
		return;
    
	this.Menu.LastOpenedItem = this;
	this.State = RadMenuItemState.AboutToOpen;

	var instance = this;
	this.SetTimeout(function () {
		instance.Open();
		instance.TimeoutPointer = null;
	}, this.Menu.ExpandDelay);
}

RadMenuItem.prototype.Close = function ()
{
	if (this.IsSeparator)
	{
		return;
	}
	
    if (this.State == RadMenuItemState.Closed)
	{
		return;
    }
    
    if (this.OpenedItem)
    {
        this.OpenedItem.Close();
    }
    
	this.Parent.OpenedItem = null;
	
	if (!this.AnimationContainer) return;
	
	this.State = RadMenuItemState.Closed;
		
	if (this.Level == 1)
	{
		this.Menu.AboutToCollapse = true;
	}
	
	this.Ease.Out();
	
	this.UpdateCssClass();
	
	this.DomElement.style.zIndex = this.OriginalZIndex;
	if (!RadControlsNamespace.Browser.IsNetscape)
	{
		if (this.Level == 1)
		{
		    this.Menu.DomElement.style.zIndex = this.Menu.OriginalZIndex;
		}
	}
	this.HideChildren ();
}

RadMenuItem.prototype.MouseOutHandler = function (e)
{
	var to = RadControlsNamespace.DomEvent.GetRelatedTarget(e);
	
	if ((!to) || this.Menu.IsChildOf(this.DomElement, to) || to == this.DomElement)
	{
		return;
	}
    
    if (this.ChildrenDetached)
    {
		if (this.Menu.IsChildOf(this.Parent.AnimationContainer, to))
		{
			return;
		}
    }
    
    try
    {
        //Fix FireFox "_moz_editor_bogus_node" problem.
        var bogusNode = to.parentNode;
    }catch (e)
    {
        return;
    }

	
	if (this.State == RadMenuItemState.Closed || this.State == RadMenuItemState.AboutToClose)
	{
		return;
	}
	
	if (this.State == RadMenuItemState.AboutToOpen)
	{
		this.ClearTimeout();
		this.State = RadMenuItemState.Closed;
		this.Parent.OpenedItem = null;
		return;
	}
	
	if (this.Menu.ClickToOpen)
	{
		return;
	}
	
	this.State = RadMenuItemState.AboutToClose;
	var instance = this;
	this.SetTimeout(function ()	{
			instance.Close();
			instance.TimeoutPointer = null;
		}, this.Menu.CollapseDelay);
}

RadMenuItem.prototype.SetTimeout = function (closure, delay)
{
	this.TimeoutPointer = setTimeout(closure, delay);
}

RadMenuItem.prototype.ClearTimeout = function()
{
	if (this.TimeoutPointer)
	{
		clearTimeout(this.TimeoutPointer);
		this.TimeoutPointer = null;
	}
}

RadMenuItem.prototype.PositionChildContainer = function (documentSize)
{
	var top = 0;
	var left = 0;
	var expandDirection = this.GroupSettings.ExpandDirection;
	var easeProperty = RadMenuNamespace.ExpandDirectionPropertyName[expandDirection];
	
	var ownerBox = this.DomElement;
	
	var itemHeight = RadControlsNamespace.Box.GetOuterHeight(ownerBox);
	var itemWidth = RadControlsNamespace.Box.GetOuterWidth(ownerBox);
	
	var itemBox = this.AnimationContainer;
	
	//itemBox.style.border = "1px solid red";
	var childItemsHeight = RadControlsNamespace.Box.GetOuterHeight(itemBox);
	var childItemsWidth = RadControlsNamespace.Box.GetOuterWidth(itemBox);


	
		
	if (expandDirection == RadMenuNamespace.ExpandDirection.Down)
	{
		top = itemHeight;
	}
	else if (expandDirection == RadMenuNamespace.ExpandDirection.Right)
	{
		left = itemWidth;
	}
	
	this.SetContainerPosition(left, top);

	var childItemsPosition = RadControlsNamespace.Screen.GetElementPosition(itemBox);


	if (this.Menu.RightToLeft)
	{
		left = itemWidth - childItemsWidth;
	}


	if (!this.Menu.EnableScreenBoundaryDetection)
	{	
		this.Ease.SetSide(easeProperty);
		this.TextElement.className = "text expand" + this.Ease.GetSide();
		return;
	}

	// Screen boundary detection	
	
	
		
	if (
		RadControlsNamespace.Screen.ElementOverflowsRight(documentSize, itemBox) && 
		expandDirection == RadMenuNamespace.ExpandDirection.Right
	)	
	{
		expandDirection = RadMenuNamespace.ExpandDirection.Left;
		easeProperty = "right";
		left = 0;
	}
	
	else if (childItemsPosition.x - childItemsWidth < 0 && expandDirection == RadMenuNamespace.ExpandDirection.Left)	
	{
		expandDirection = RadMenuNamespace.ExpandDirection.Right;
		easeProperty = "left";
		left = itemWidth;
	}
	
	else if (childItemsPosition.y - childItemsHeight < 0 && expandDirection == RadMenuNamespace.ExpandDirection.Up)	
	{
		expandDirection = RadMenuNamespace.ExpandDirection.Down;
		easeProperty = "top";
		top = itemHeight;
	}
	
	else if (
		RadControlsNamespace.Screen.ElementOverflowsBottom(documentSize, itemBox) && 
		expandDirection == RadMenuNamespace.ExpandDirection.Down
		)	
	{
		//Check if there is enough space to invert the expand direction
		var itemBox = RadControlsNamespace.Screen.GetElementPosition(this.DomElement);
		if (itemBox.y > this.AnimationContainer.offsetHeight)
		{
			expandDirection = RadMenuNamespace.ExpandDirection.Up;
			easeProperty = "bottom";
			top = itemHeight;
		}
	}
	
	// Side offset
	
	if (    RadControlsNamespace.Screen.ElementOverflowsRight(documentSize, itemBox) &&
			(expandDirection == RadMenuNamespace.ExpandDirection.Down ||
				expandDirection == RadMenuNamespace.ExpandDirection.Up)	
			)
	{
		if (!this.Menu.RightToLeft)
		{
		    left = documentSize.width - (childItemsPosition.x + childItemsWidth);
		}
	}
	else if (RadControlsNamespace.Screen.ElementOverflowsBottom(documentSize, itemBox))
	{
		if (expandDirection == RadMenuNamespace.ExpandDirection.Left ||
			expandDirection == RadMenuNamespace.ExpandDirection.Right)	
		{
			top = documentSize.height - (childItemsPosition.y + childItemsHeight);
			//alert(top);
		}
	}
	
	this.SetContainerPosition(left, top);
	
	
	this.Ease.SetSide(easeProperty);
	this.TextElement.className = "text expand" + this.Ease.GetSide();

};

RadMenuItem.prototype.SetContainerPosition = function(left, top)
{
 	var y = top + this.GroupSettings.OffsetY;
 	if (this.Parent.ScrollWrap)
 	{
 	    if (this.Parent == this.Menu && this.Menu.IsContext)
 	    {
 			this.Parent.AnimationContainer = this.Menu.DomElement;
 	    }
 	    
 	    if (this.Parent.AnimationContainer)
 	    {
 	        this.ChildrenDetached = true;
 	        this.Parent.AnimationContainer.appendChild(this.AnimationContainer);
 	    }
     	
 	    y += this.DomElement.offsetTop;
 	    
 	    var childListTop = parseInt(this.Parent.ChildItemList.style.top);
 	    
 	    if (isNaN(childListTop))
 	    {
 	        childListTop = 0;
 	    }
 	    
 	    if (this.GroupSettings.OffsetY == 0)
 	    {
 			// Compensate for the scroll offset only if there is no group offset Y set.
 			y += childListTop;
 	    }
    }
 	
 	this.AnimationContainer.style.top = y + "px";
	this.AnimationContainer.style.left = (left + this.GroupSettings.OffsetX) + "px";
}

RadMenuItem.prototype.SetAttribute = function (name, value)
{
	this.PostInitialize();
	this.Attributes[name] = value;
	this.RecordState();
}

RadMenuItem.prototype.SetImageUrl = function (src)
{
	this.PostInitialize();
	this.ImageUrl = src;
	this.ImageElement.src = src;
	this.RecordState();
}

RadMenuItem.prototype.SetImageOverUrl = function (src)
{
	this.PostInitialize();
	this.ImageOverUrl = src;
	this.RecordState();
}

RadMenuItem.prototype.GetAttribute = function (name)
{
	return this.Attributes[name];
}

RadMenuItem.prototype.DetermineExpandDirection = function ()
{
	if (this.GroupSettings.ExpandDirection != RadMenuNamespace.ExpandDirection.Auto)
	{
		return;
	}
	
	if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Vertical)
	{
		if (this.Menu.RightToLeft)
		{
			this.GroupSettings.ExpandDirection = RadMenuNamespace.ExpandDirection.Left;
		}
		else
		{
			this.GroupSettings.ExpandDirection = RadMenuNamespace.ExpandDirection.Right;
		}
	}
	else
	{
		this.GroupSettings.ExpandDirection = RadMenuNamespace.ExpandDirection.Down;
	}
}

RadMenuItem.prototype.LoadConfiguration = function ()
{
	if (this.Menu.ItemData[this.ID])
	{
		for (var property in this.Menu.ItemData[this.ID])
		{
			this[property] = this.Menu.ItemData[this.ID][property];
		}
	}
	
	var defaultSettings = this.Menu.DefaultGroupSettings;
	
    // default settings for nested config objects
    if (typeof this.GroupSettings.Flow == 'undefined')
    {
		this.GroupSettings.Flow = defaultSettings.Flow;
    }
    
    this.Flow = this.GroupSettings.Flow;
    
    
    
    if (typeof this.GroupSettings.ExpandDirection == 'undefined')
    {
		this.GroupSettings.ExpandDirection = defaultSettings.ExpandDirection;
    }
    
    if (typeof this.GroupSettings.OffsetX == 'undefined')
    {
		this.GroupSettings.OffsetX = defaultSettings.OffsetX;
    }		 
    
	if (typeof this.GroupSettings.OffsetY == 'undefined')
    {
		this.GroupSettings.OffsetY = defaultSettings.OffsetY;
    }	 
    
    if (!this.Enabled)   
    {
		this.Disable();
    }
};

/**
 * Keyboard handlers
 */

RadMenuItem.prototype.HandleRightArrow = function ()
{
	if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Horizontal)
	{
		this.FocusNextItem();
	}
	else
	{
		if (this.Items.length && this.GroupSettings.ExpandDirection == RadMenuNamespace.ExpandDirection.Right)
		{
			this.FocusFirstChild();
		}
		else if (this.Parent.GroupSettings && 
		this.Parent.GroupSettings.ExpandDirection == RadMenuNamespace.ExpandDirection.Left)
		{
			this.Parent.Focus();
		}
		else
		{
			if (this.Menu.OpenedItem)
			{
				this.Menu.OpenedItem.GetNextItem().Focus();
			}
		}
	}
}

RadMenuItem.prototype.HandleLeftArrow = function ()
{
	if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Horizontal)
	{
		this.FocusPreviousItem();
	}
	else
	{
		if (this.Items.length && this.GroupSettings.ExpandDirection == RadMenuNamespace.ExpandDirection.Left)
		{
			this.FocusFirstChild();
		}
		else if (this.Parent.GroupSettings && 
		this.Parent.GroupSettings.ExpandDirection == RadMenuNamespace.ExpandDirection.Right)
		{
			this.Parent.Focus();
		}
		else
		{
			if (this.Menu.OpenedItem)
			{
				this.Menu.OpenedItem.GetPreviousItem().Focus();
			}
		}
	}	
}

RadMenuItem.prototype.HandleUpArrow = function ()
{		
	if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Vertical)
	{
		this.FocusPreviousItem();
	}
	else
	{
		this.FocusLastChild();
	}	
}


RadMenuItem.prototype.HandleDownArrow = function ()
{		
	if (this.Parent.Flow == RadMenuNamespace.ItemFlow.Vertical)
	{
		this.FocusNextItem();
	}
	else
	{
		this.FocusFirstChild();
	}
}


RadMenuItem.prototype.GetNextItem = function ()
{
	if (this.Index == this.Parent.Items.length - 1)
	{
		return this.Parent.Items[0];
	}
	
	return this.Parent.Items[this.Index + 1];
}

RadMenuItem.prototype.GetPreviousItem = function ()
{
	if (this.Index == 0)
	{
		return this.Parent.Items[this.Parent.Items.length - 1];
	}
	
	return this.Parent.Items[this.Index - 1];
}

RadMenuItem.prototype.CanFocus = function ()
{
	return (!this.IsSeparator) && this.Enabled;
}

RadMenuItem.prototype.FocusFirstChild = function ()
{
	if (!this.Items.length)
	{
		return;
	}
	
	var item = this.Items[0];
	
	while (!item.CanFocus())
	{
		
		item = item.GetNextItem();
		if (item == this.Items[0]) 
		{
			return; // no items to focus
		}
	}
		
	item.Focus();

}



RadMenuItem.prototype.FocusLastChild = function ()
{
	if (!this.Items.length)
	{
		return;
	}
	
	var item = this.Items[this.Items.length - 1];
	
	while (!item.CanFocus())
	{
		item = item.GetPreviousItem();
		if (this.Items.length - 1) 
		{
			return; // no items to focus
		}		
	}	
	item.Focus();
}



RadMenuItem.prototype.FocusNextItem = function ()
{
    var item = this.GetNextItem();
	
	while (!item.CanFocus())
	{
		item = item.GetNextItem();
	}	
	item.Focus();
}

RadMenuItem.prototype.FocusPreviousItem = function ()
{
    var item = this.GetPreviousItem();
	
	while (!item.CanFocus())
	{
		item = item.GetPreviousItem();
	}	
	item.Focus();
}

RadMenuItem.prototype.RecordState = function (forceSerialize)
{
	if (this.ClientSide && !forceSerialize)
	{
		return;
	}
	var serialized = RadControlsNamespace.JSON.stringify(this, this.InitialState, RadMenu.JSONIncludeDeep);
	if (serialized == "{}")
	{
		this.Menu.ItemState[this.ID] = "";
	}
	else
	{
		this.Menu.ItemState[this.ID] = "\"" + this.ID + "\":" + serialized;
	}
	
	this.Menu.RecordState();
}

RadMenuItem.prototype.AddItem = function()
{
    this.Menu.AddItem.apply(this, arguments);
    this.Menu.FixItemWidth(this);
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