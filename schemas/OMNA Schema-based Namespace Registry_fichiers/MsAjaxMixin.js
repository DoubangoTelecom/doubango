/***************************************************************
USAGE:

1. Include this script before your client object class definition.

2. Call the MsAjaxMixin right after your function definition, 
so that it attaches the necessary methods to the prototype

RadControlsNamespace.MsAjaxMixin.Initialize(RadMenu, 'RadMenu');

3. Change your client object initialization:
 - rework your construction logic so that the constructor accepts only one element -- a DOM node.
 - pass any additional data to an Initalize() method after the object is constructed.
4. Call the initializeBase method in the constructor of your JavaScript object:

function RadMenu(element)
{
	...
	
	if (typeof(RadMenu.initializeBase) == "function")
	{
		RadMenu.initializeBase(this, [element]);
	}
}

5. Create a static factory method that will create a new object.  Let it accept a client ID only:

RadMenu.Create = function(clientID)
{
	return new RadMenu(document.getElementById(clientID));
}

6. Change your InitScript rendering:
 - it should not call new RadMenu() directly.  use the factory method instead: RadMenu.Create()

Look below for a sample RadMenu.Create() implementation.  That method will create your object and 
register it with the MS AJAX script runtime if it is present on the page.

***************************************************************/

//RadMenu.Create = function ()
//{
//	var clientID = arguments[0];
//	
//	if (typeof ($create) == "function")
//	{
//		//the global RadMenu function object
//		var type = this;
//		
//		//{name : 'value'} will trigger a menuInstance.set_name('value');
//		var properties = {};
//		
//		//{itemClick : OnItemClick} will trigger a menuInstance.add_itemClick(OnItemClick);
//		var events = {};
//		
//		//{"Treeview" : "RadTreeView1} will trigger a menuInstance.set_Treeview($find('RadTreeView1'));
//		var references = {};
//		
//		//the DOM element that this component will attach to.  crucial for partial updates and disposes
//		var domElement = $get(clientID);
//		return $create(type, properties, events, references, domElement);
//	}
//	else
//	{
//		var element = document.getElementById(clientID);
//		return new this(element);
//	}
//}

if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.MsAjaxMixin) == "undefined" ||
	typeof(window.RadControlsNamespace.MsAjaxMixin.Version) == null ||
	window.RadControlsNamespace.MsAjaxMixin.Version < 1
	)
{	
	RadControlsNamespace.MsAjaxMixin =
	{
		Version : 1,
		Initialize : function(type, typeName)
		{
			if (typeof(type.registerClass) != "function")
			{
				return;
			}	
			
			type.inheritsFrom(Sys.UI.Control);
			type.registerClass(typeName, Sys.UI.Control, Sys.IDisposable);

			type.prototype.initialize = function()
			{
				Sys.UI.Control.callBaseMethod(this, 'initialize');
			}

			type.prototype.dispose = function()
			{
				Sys.UI.Control.callBaseMethod(this, 'dispose');
				this.Dispose();
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
