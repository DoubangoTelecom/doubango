if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.DomEventMixin) == "undefined" ||
	typeof(window.RadControlsNamespace.DomEventMixin.Version) == null ||
	window.RadControlsNamespace.DomEventMixin.Version < 3
	)
{	
	
	RadControlsNamespace.DomEventMixin = 
	{
		Version : 3, // Change the version when make changes. Change the value in the IF also
		
		Initialize : function(obj)
		{
			obj.CreateEventHandler = this.CreateEventHandler;
			obj.AttachDomEvent = this.AttachDomEvent;
			obj.DetachDomEvent = this.DetachDomEvent;
			obj.DisposeDomEventHandlers = this.DisposeDomEventHandlers;
			obj._domEventHandlingEnabled = true;
			obj.EnableDomEventHandling = this.EnableDomEventHandling;
			obj.DisableDomEventHandling = this.DisableDomEventHandling;
			
			obj.RemoveHandlerRegister = this.RemoveHandlerRegister;
			obj.GetHandlerRegister	  = this.GetHandlerRegister;
			obj.AddHandlerRegister    = this.AddHandlerRegister;
			obj.handlerRegisters = [];
		},
		
		EnableDomEventHandling : function ()
		{
			this._domEventHandlingEnabled = true;
		},
		
		DisableDomEventHandling : function ()
		{
			this._domEventHandlingEnabled = false;
		},
	
		CreateEventHandler : function (methodName, fireIfDisabled)
		{
			var instance = this;
			return function (e)
			{
				if (!instance._domEventHandlingEnabled && !fireIfDisabled)
				{
					return;
				}
				
				return instance[methodName](e || window.event);
			}
		},
		
		AttachDomEvent : function(element, eventName, eventHandlerName, fireIfDisabled)
		{
			var eventHandler = this.CreateEventHandler(eventHandlerName, fireIfDisabled);

			// if such entry exist already - detach it first
			var oldRegister = this.GetHandlerRegister(element, eventName, eventHandlerName);
			if (oldRegister != null)
			{
				this.DetachDomEvent(oldRegister.Element, oldRegister.EventName, eventHandlerName);
			}
			
			// register the new entry
			var eventRegister = { 
				"Element" : element, 
				"EventName" : eventName, 
				"HandlerName" : eventHandlerName, 
				"Handler" : eventHandler 
			};
			this.AddHandlerRegister(eventRegister);
			
			// now do the "real" job
			if (element.addEventListener)
			{
				element.addEventListener(eventName, eventHandler, false);
			}
			else if (element.attachEvent)
			{
				element.attachEvent("on" + eventName, eventHandler);
			}
		},
		
		
		DetachDomEvent : function(element, eventName, eventHandler)
		{
			var eventRegister = null;
			var eventHandlerName = "";

			if (typeof eventHandler == "string") 
			{
				eventHandlerName = eventHandler;
				eventRegister    = this.GetHandlerRegister(element, eventName, eventHandlerName);
				if(eventRegister == null)
					return;
				eventHandler     = eventRegister.Handler;
			}
			
			if (!element)
			{
			    return;
			}
			
			if (element.removeEventListener)
			{
				element.removeEventListener(eventName, eventHandler, false);
			}
			else if (element.detachEvent)
			{
				element.detachEvent("on" + eventName, eventHandler);
			}
			
			if (eventRegister != null && eventHandlerName != "")
			{
				this.RemoveHandlerRegister(eventRegister);
				eventRegister = null;
			}
		},
		
		DisposeDomEventHandlers : function()
		{
			for (var i=0; i < this.handlerRegisters.length; i ++)
			{
				var eventRegister = this.handlerRegisters[i];
				if (eventRegister != null)
				{
					this.DetachDomEvent(
						eventRegister.Element, 
						eventRegister.EventName, 
						eventRegister.Handler);
				}
			}
			
			this.handlerRegisters = [];
		},

		RemoveHandlerRegister : function(eventRegister)
		{
			try {
				var regIndex = eventRegister.index;
				for (var i in eventRegister)
				{
					eventRegister[i] = null;
				}
				this.handlerRegisters[regIndex] = null;
			} catch (e) {}
		},

		GetHandlerRegister : function(element, eventName, handlerName)
		{
			for (var i=0; i < this.handlerRegisters.length; i ++)
			{
				var eventRegister = this.handlerRegisters[i];
				if (eventRegister != null &&
					eventRegister.Element	  == element && 
					eventRegister.EventName	  == eventName &&
					eventRegister.HandlerName == handlerName 
				)
				{
					return this.handlerRegisters[i];
				}
			}
			
			return null;
		},
		
		AddHandlerRegister : function(props)
		{
			props.index = this.handlerRegisters.length;
			this.handlerRegisters[this.handlerRegisters.length] = props;
		}
	}
	
	RadControlsNamespace.DomEvent = {};
	
	RadControlsNamespace.DomEvent.PreventDefault = function (e)
	{
		if (!e) return true;
		
		if (e.preventDefault)
		{
			e.preventDefault();
		}
	
		e.returnValue = false;
		return false;
	}
	
	RadControlsNamespace.DomEvent.StopPropagation = function (e)
	{
		if (!e) return;
		
		if (e.stopPropagation)
		{
			e.stopPropagation();
		}
		else
		{
			e.cancelBubble = true;
		}
	}
	
	RadControlsNamespace.DomEvent.GetTarget = function (e)
	{
		if (!e) return null;
		
		return e.target || e.srcElement;
	}
	
	
	RadControlsNamespace.DomEvent.GetRelatedTarget = function (e)
	{
		if (!e) return null;
		
		return e.relatedTarget || (e.type == "mouseout" ? e.toElement : e.fromElement);
	}
	
	RadControlsNamespace.DomEvent.GetKeyCode = function (e)
	{
		if (!e) return 0;
		
		return e.which || e.keyCode;
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

