if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.EventMixin) == "undefined" ||
	typeof(window.RadControlsNamespace.EventMixin.Version) == null ||
	window.RadControlsNamespace.EventMixin.Version < 2
	)
{	
	
	RadControlsNamespace.EventMixin = 
	{
		Version : 2, // Change the version when make changes. Change the value in the IF also

		Initialize : function (obj)
		{
	
			obj._listeners = {};
			obj._eventsEnabled = true;
			obj.AttachEvent = this.AttachEvent;
		    
			obj.DetachEvent = this.DetachEvent;
			obj.RaiseEvent = this.RaiseEvent;
		    
			obj.EnableEvents = this.EnableEvents;
			obj.DisableEvents = this.DisableEvents;
			
			obj.DisposeEventHandlers = this.DisposeEventHandlers;
	
		},
	
		DisableEvents : function ()
		{
			this._eventsEnabled = false;
		},
	
		EnableEvents : function ()
		{
			this._eventsEnabled = true;
		},
	
		AttachEvent : function (eventName, handler)
		{
			if (!this._listeners[eventName]) 
			{
				this._listeners[eventName] = [];
			}
	
			this._listeners[eventName][this._listeners[eventName].length] = (RadControlsNamespace.EventMixin.ResolveFunction(handler));
		},
	
	
		DetachEvent : function (eventName, handler)
		{
			var listeners = this._listeners[eventName];
			if (!listeners) 
			{
				return false;
			}
		    
			var funcHandler = RadControlsNamespace.EventMixin.ResolveFunction(handler);
		    
			for (var i = 0; i < listeners.length; i ++)
			{
				if (funcHandler == listeners[i])
				{
					listeners.splice(i, 1);
					return true;
				}
			}
	
			return false;
		},
		
		DisposeEventHandlers : function()
		{
            for (var eventName in this._listeners)
            {
                var listeners = null;
                if (this._listeners.hasOwnProperty(eventName))
                {
                    listeners = this._listeners[eventName];
                    for (var i = 0; i < listeners.length; i++)
			        {
				        listeners[i] = null;
			        }
			        
			        listeners = null;
                }
            }
		},
	
		ResolveFunction : function (func)
		{
			if (typeof(func) == "function")
			{
				return func;
			}
			else if (typeof(window[func]) == "function")
			{
				return window[func];
			}
			else
			{
				return new Function("var Sender = arguments[0]; var Arguments = arguments[1];" + func);
			}
		},
	
	
		RaiseEvent : function (eventName, eventArgs)
		{
			if (!this._eventsEnabled)
			{
				return true;
			}
			var outcome = true;
		    
			if (this[eventName])
			{
		        
				var eventResult = RadControlsNamespace.EventMixin.ResolveFunction(this[eventName])(this, eventArgs);
				if (typeof(eventResult) == "undefined")
				{
					eventResult = true;
				}
				outcome = outcome && eventResult;        
			}
		    
			if (!this._listeners[eventName]) return outcome;
		    
			for (var i = 0; i < this._listeners[eventName].length; i ++)
			{
				var handler = this._listeners[eventName][i];
				var eventResult = handler(this, eventArgs);
				if (typeof(eventResult) == "undefined")
				{
					eventResult = true;
				}
				outcome = outcome && eventResult;
			}
		    
			return outcome;
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
