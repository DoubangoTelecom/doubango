if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

RadControlsNamespace.Ticker = function (listener)
{
	this.Listener = listener;
	this.IntervalPointer = null;
}

RadControlsNamespace.Ticker.prototype =
{
	Configure : function (config)
	{
		this.Duration = config.Duration;
		this.Interval = 16;
	},
	
	Start : function ()
	{
		clearInterval(this.IntervalPointer);
		this.TimeElapsed = 0;
		var instance = this;
		var closure = function ()
		{
			instance.Tick();
		}
		
		this.Tick();
		this.IntervalPointer = setInterval(closure, this.Interval);
	},
	
	Tick : function ()
	{
		this.TimeElapsed += this.Interval;
		this.Listener.OnTick(this.TimeElapsed);
		
		if (this.TimeElapsed >= this.Duration)
		{
			this.Stop();
		}
	},
	
	Stop : function ()
	{
		if (this.IntervalPointer)
		{
			this.Listener.OnTickEnd();
			clearInterval(this.IntervalPointer);
			this.IntervalPointer = null;
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