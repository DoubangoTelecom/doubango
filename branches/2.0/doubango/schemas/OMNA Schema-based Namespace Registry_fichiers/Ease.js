if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.Ease) == "undefined" ||
	typeof(window.RadControlsNamespace.Ease.Version) == null ||
	window.RadControlsNamespace.Ease.Version < 1.3
	)
{

	RadControlsNamespace.Ease = function (element, configObject, offsetX, offsetY, listener, createOverlay)
	{
		
		this.Element = element;
		if (createOverlay)
		{
			this.Overlay = new RadControlsNamespace.Overlay(element);	
		}
		

		
		this.OffsetX = offsetX;
		this.OffsetY = offsetY;
		
		this.Invert = false;
		var parent = this.Element.parentNode;

		
		this.ExpandConfig = this.MergeConfig(configObject.ExpandAnimation);
		this.CollapseConfig = this.MergeConfig(configObject.CollapseAnimation);
		
		this.Ticker = new RadControlsNamespace.Ticker(this);
		this.Listener = listener;
		this.SlideParent = false;
			
	}

	RadControlsNamespace.Ease.Version = 1.3;// update in the header IF also
	RadControlsNamespace.Ease.Coef = 0;

	RadControlsNamespace.Ease.prototype = 
	{
		SetSide : function(side)
		{
			this.InitialSide = side.charAt(0).toUpperCase() + side.substr(1, side.length - 1);
			this.Invert = false;
			if (side == 'right')
			{
				side = 'left';
				this.Invert = true;
			}
			if (side == 'bottom')
			{
				side = 'top';
				this.Invert = true;
			}
			this.Side = side;
			this.Horizontal = side == "left";
		},
		
		MergeConfig : function (config)
		{
			if (!config.Type)
			{
				config.Type = "OutQuint";
			}
			
			if (!config.Duration)
			{
				config.Duration = 200;
			}
			
			return config;
		},
		
		
		GetSide : function ()
		{
			return this.InitialSide;
		},
		
		ShowElements : function ()
		{
			if (!this.Element.parentNode) return;
			if (!this.Element.parentNode.style) return;
			
			this.Element.parentNode.style.display = "block";
			this.Element.style.display = "block";
			this.Element.parentNode.style.overflow = "hidden";	
		},
		
		Dispose : function ()
		{
			this.Ticker.Stop();
			this.Element = null;
			if (this.Overlay)
			{
				this.Overlay.Dispose();
			}
		},
		
		ResetState : function (resetPosition)
		{
			this.ShowElements();

			if (resetPosition)
			{
				var initialPosition = (this.Horizontal ? this.Element.offsetWidth : this.Element.offsetHeight);
				if (!this.Invert)
				{
					initialPosition = -initialPosition;
				}
				this.SetPosition(initialPosition); 
			}

			this.InitialPosition = this.GetPosition();
		},


		UpdateContainerSize : function ()
		{
			if (!this.Element.parentNode)
			{
			    //r.a.d.callback
			    return;
			}
			
			if (!this.Element.offsetWidth || !this.Element.offsetHeight )
			{
			    return;
			}
			
			if (this.Invert)
			{
				if (this.Side == "left")
				{
					this.Element.parentNode.style.height = this.Element.offsetHeight + "px";
				}
				else if (this.Side == "top")
				{
					this.Element.parentNode.style.width = this.Element.offsetWidth + "px";
				}
				
				return;
			}
			var top = 0;
			var left = 0;
			

			if (this.Element.style.top != "")
			{
				top = Math.max(parseInt(this.Element.style.top), 0);
			}
			
			if (this.Element.style.left != "")
			{
				left = Math.max(parseInt(this.Element.style.left), 0);
			}
			
			
			if (this.SlideParent) // Panelbar
			{
				top = parseInt(this.Element.style.top);
				if (isNaN(top)) top = 0;
			}					
			
			if ( typeof(RadMenuItem) != "undefined" && this.Listener instanceof RadMenuItem)
			{
    			if (this.Element.parentNode.style.height != this.Element.offsetHeight + top + "px")
			    {
				    this.Element.parentNode.style.height = Math.max(this.Element.offsetHeight + top, 0) + "px";	
			    }
    			
			    if (this.Element.parentNode.style.width != (this.Element.offsetWidth + left)+ "px")
			    {
				    this.Element.parentNode.style.width = Math.max(this.Element.offsetWidth + left, 0)+ "px";
			    }
			}
			else
			{		
			    if (this.Element.parentNode.offsetHeight != this.Element.offsetHeight + top)
			    {
				    this.Element.parentNode.style.height = Math.max(this.Element.offsetHeight + top, 0) + "px";	
			    }
			
			    if (this.Element.parentNode.offsetWidth != (this.Element.offsetWidth + left))
			    {
    			    this.Element.parentNode.style.width = Math.max(this.Element.offsetWidth + left, 0)+ "px";
			    }
            }
		},
		
		
		GetSize : function ()
		{
			return this.Horizontal ? this.Element.offsetWidth : this.Element.offsetHeight;
		},

		GetPosition : function ()
		{
			if (!this.Element.style[this.Side])
			{
				return 0;
			}
			return parseInt(this.Element.style[this.Side]);
		},
		
		SetPosition : function(value)
		{
			this.Element.style[this.Side] = value + "px";
		},
		
		Out : function ()
		{
			this.ResetState();
			this.Direction = -1;
			if (this.Invert)
			{
				this.Delta = this.GetSize() - this.GetPosition();
			}
			else
			{
				this.Delta = this.GetPosition() - this.GetSize();
			}
			this.Start(this.CollapseConfig);
		}, 
		
		In : function ()
		{
			this.ResetState(true);
			this.Direction = 1;
			this.Delta = - this.GetPosition();
			this.Start(this.ExpandConfig);
		}, 

		Start : function (configuration)
		{
			if (configuration.Type == "None")
			{
				this.UpdateContainerSize();
				this.Ticker.Stop();
				
				this.ChangePosition(this.InitialPosition + this.Delta);
				if (this.Overlay)
				{
					this.Overlay.Update();
				}
				
				this.UpdateContainerSize();
				
				this.OnTickEnd();
				
				return;
			}

			this.Tween = configuration.Type;		
			this.Ticker.Configure(configuration);	
			this.Ticker.Start();
			this.UpdateContainerSize();
		},

		ChangePosition : function (newValue)
		{
		    if (isNaN(newValue)) return;
		    
			var dimensionValue, dimensionName, dimensionOffset;
			if (this.Invert)
			{
				if (this.Horizontal)
				{
					dimensionValue = this.Element.offsetWidth;
					dimensionName = "width";
					dimensionOffset = this.OffsetX;
				}
				else
				{
					dimensionValue = this.Element.offsetHeight;
					dimensionName = "height";
					dimensionOffset = this.OffsetY;
				}
				this.SetPosition(0);
				var updatedValue = Math.max(1, dimensionValue - newValue) + "px"; 
				this.Element.parentNode.style[dimensionName] = updatedValue;
				this.Element.parentNode.style[this.Side] = ((dimensionValue - newValue + dimensionOffset) * -1) + "px";
			}
			else
			{		
			    this.Element.style[this.Side] = newValue + "px";
			}

			if (this.Listener && this.Listener.OnEase)
				this.Listener.OnEase(newValue);
		},

		OnTick : function (time)
		{
			var newValue = Math.round(Penner[this.Tween](time, this.InitialPosition, this.Delta, this.Ticker.Duration));
			if (newValue == this.InitialPosition + this.Delta)
			{
			    this.Ticker.Stop();
			    return;
			}
			this.ChangePosition(newValue);
			this.UpdateContainerSize();
			if (this.Overlay)
			{
				this.Overlay.Update();
			}
		},
		

		OnTickEnd : function ()
		{
		    try
		    {
		        if (this.Direction == 0)
			    {
				    return;
			    }
	    		

			    this.ChangePosition(this.InitialPosition + this.Delta);
				
				if (this.Overlay)
				{
					this.Overlay.Update();
				}
				
			    if (this.Direction > 0)
			    {
				    this.Element.parentNode.style.overflow = "visible"; 
				    if (this.Listener && this.Listener.OnExpandComplete)
				    {
					    this.Listener.OnExpandComplete();
				    }
			    }
			    else
			    {
				    this.Element.parentNode.style.display = "none";
				    if (this.Listener)
				    {
					    this.Listener.OnCollapseComplete();
				    }
			    }				
				
			    this.Direction = 0;
			}
			catch(e)
			{
			    //Atlas updates
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