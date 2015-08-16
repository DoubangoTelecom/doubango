function RadMenuItemState()
{
}

RadMenuItemState = 
{
	Closed : 0,
	Open : 1,
	AboutToClose : 2, 
	AboutToOpen : 3	
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