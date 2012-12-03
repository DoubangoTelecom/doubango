var numdivs=3

var IE5=NN4=NN6=false
if(document.all)IE5=true
else if(document.layers)NN4=true
else if(document.getElementById)NN6=true


function copyParentStylesToEditor(editor)
{
  var theIFrame = document.getElementById("RadEContentIframe" + editor.Id);
  var theMainEditorTable = document.getElementById("RadEWrapper" + editor.Id);

  var theParentEl = theMainEditorTable.parentNode.parentNode;
  var theContentArea = editor.GetContentArea();
  if (document.all)
  {
      for (var attr in theParentEl.currentStyle)
      {
          var strStyle = "" + attr;
          // Style Overflow don't be change otherwise Scroll dispear in Raeditor in Big document
          if ( ( strStyle  != "overflow" ) && ( strStyle  != "overflowX" ) && ( strStyle  != "overflowY" ) )
             {
                theContentArea.style[attr] = theParentEl.currentStyle[attr]; 
             }     
      }
  }
  else //Firefox
  {
 
    theContentArea.style.background = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("background");
    theContentArea.style.backgroundAttachment = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("background-attachment");
    theContentArea.style.backgroundColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("background-color");
    theContentArea.style.backgroundImage = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("background-image");
    theContentArea.style.backgroundPosition = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("background-position");
    theContentArea.style.backgroundRepeat = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("background-repeat");
    theContentArea.style.border = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border");
    theContentArea.style.borderBottom = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-bottom");
    theContentArea.style.borderBottomColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-bottom-color");
    theContentArea.style.borderBottomStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-bottom-style");
    theContentArea.style.borderBottomWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-bottom-width");
    theContentArea.style.borderColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-color");
    theContentArea.style.borderLeft = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-left");
    theContentArea.style.borderLeftColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-left-color");
    theContentArea.style.borderLeftStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-left-style");
    theContentArea.style.borderLeftWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-left-width");
    theContentArea.style.borderRight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-right");
    theContentArea.style.borderRightColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-right-color");
    theContentArea.style.borderRightStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-right-style");
    theContentArea.style.borderRightWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-right-width");
    theContentArea.style.borderStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-style");
    theContentArea.style.borderTop = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-top");
    theContentArea.style.borderTopColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-top-color");
    theContentArea.style.borderTopStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-top-style");
    theContentArea.style.borderTopWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-top-width");
    theContentArea.style.borderWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("border-width");
    theContentArea.style.captionSide = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("caption-side");
    theContentArea.style.clear = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("clear");
    theContentArea.style.clip = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("clip");
    theContentArea.style.color = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("color");
    theContentArea.style.cursor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("cursor");
    theContentArea.style.direction = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("direction");
    theContentArea.style.display = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("display");
    theContentArea.style.emptyCells = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("empty-cells");
    theContentArea.style.float = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("float");
    theContentArea.style.font = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font");
    theContentArea.style.fontFamily = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font-family");
    theContentArea.style.fontSize = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font-size");
    theContentArea.style.fontStretch = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font-stretch");
    theContentArea.style.fontStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font-style");
    theContentArea.style.fontVariant = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font-variant");
    theContentArea.style.fontWeight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("font-weight");
    theContentArea.style.height = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("height");
    theContentArea.style.left = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("left");
    theContentArea.style.letterSpacing = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("letter-spacing");
    theContentArea.style.lineHeight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("line-height");
    theContentArea.style.listStyle = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("list-style");
    theContentArea.style.listStyleImage = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("list-style-image");
    theContentArea.style.listStylePosition = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("list-style-position");
    theContentArea.style.listStyleType = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("list-style-type");
    theContentArea.style.margin = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("margin");
    theContentArea.style.marginBottom = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("margin-bottom");
    theContentArea.style.marginLeft = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("margin-left");
    theContentArea.style.marginRight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("margin-right");
    theContentArea.style.marginTop = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("margin-top");
    theContentArea.style.maxHeight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("max-height");
    theContentArea.style.maxWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("max-width");
    theContentArea.style.minHeight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("min-height");
    theContentArea.style.minWidth = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("min-width");
    // Style Overflow don't be change otherwise Scroll dispear in Raeditor in Big document
    //theContentArea.style.overflow = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("overflow");
    theContentArea.style.padding = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("padding");
    theContentArea.style.paddingBottom = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("padding-bottom");
    theContentArea.style.paddingLeft = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("padding-left");
    theContentArea.style.paddingRight = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("padding-right");
    theContentArea.style.paddingTop = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("padding-top");
    theContentArea.style.pageBreakAfter = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("page-break-after");
    theContentArea.style.pageBreakBefore = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("page-Break-Before");
    theContentArea.style.position = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("position");
    theContentArea.style.right = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("right");
    theContentArea.style.scrollbar3dLightColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-3d-light-color");
    theContentArea.style.scrollbarArrowColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-arrow-color");
    theContentArea.style.scrollbarBaseColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-base-color");
    theContentArea.style.scrollbarDarkshadowColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-darkshadow-color");
    theContentArea.style.scrollbarFaceColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-face-color");
    theContentArea.style.scrollbarHighlightColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-highlight-color");
    theContentArea.style.scrollbarShadowColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-shadow-color");
    theContentArea.style.scrollbarTrackColor = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("scrollbar-track-color");
    theContentArea.style.tableLayout = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("table-layout");
    theContentArea.style.textAlign = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("text-align");
    theContentArea.style.textDecoration = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("text-decoration");
    theContentArea.style.textIndent = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("text-indent");
    theContentArea.style.textTransform = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("text-transform");
    theContentArea.style.top = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("top");
    theContentArea.style.verticalAlign = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("vertical-align");
    theContentArea.style.visibility = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("visibility");
    theContentArea.style.width = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("width");
    theContentArea.style.wordSpacing = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("word-spacing");
    theContentArea.style.zIndex = document.defaultView.getComputedStyle(theParentEl, '').getPropertyValue("z-index");
    
  }
}


function copyParentStylesToEditor_Old(editor)
{
  if (NN6) { 
    return CopyStylesToEditorHelper(editor);
  }
  else {
      var theIFrame = document.getElementById("RadEContentIframe" + editor.Id);
      var theMainEditorTable = document.getElementById("RadEWrapper" + editor.Id);

      var theParentEl = theMainEditorTable.parentNode.parentNode;
      var theContentArea = editor.GetContentArea();
      for (var attr in theParentEl.currentStyle)
      {
         theContentArea.style[attr] = theParentEl.currentStyle[attr];
      }
  }
}
function CopyStylesToEditorHelper(element)
{
   if (element.currentStyle) // Handle IE
       return element.currentStyle;
   else // Handle Others
       return document.defaultView.getComputedStyle(element,null);
       return null;
}


function CopyStylesToEditor(editor)
{
   var theIFrame = document.getElementById("RadEContentIframe" + editor.Id);
   var theDocBody = editor.Document.body;
   var IFrameCompStyle = copyParentStylesToEditor(theIFrame);

  if (IFrameCompStyle != null)
  {
      theDocBody.style.fontFamily = IFrameCompStyle.fontFamily;
      theDocBody.style.fontSize = IFrameCompStyle.fontSize;
      theDocBody.style.fontWeight = IFrameCompStyle.fontWeight;
      theDocBody.style.lineHeight = IFrameCompStyle.lineHeight;
  }
}


function DisplayTab(tab, ens) {
	if (document.getElementById){
		document.cookie = tab + ' = ' + (document.getElementById(ens + 'tab'+tab).style.display==''?0:1);
		if (document.getElementById(ens + 'tab'+tab).style.display == ''){
			document.getElementById(ens + 'tab'+tab).style.display = 'none'; 
			document.getElementById('Expand'+tab).style.display = ''; 
			document.getElementById('Collapse'+tab).style.display = 'none'; 
			
		} else {
			document.getElementById(ens + 'tab'+tab).style.display = ''; 
			document.getElementById('Collapse'+tab).style.display = ''; 
			document.getElementById('Expand'+tab).style.display = 'none';
			
		}
	} else if (document.all) {
		document.cookie = tab + ' = ' + (document.all(ens + 'tab'+tab).style.display==''?0:1);
		if (document.all(ens + 'tab'+tab).style.display == ''){
			document.all(ens + 'tab'+tab).style.display = 'none'; 
			document.all('expand'+tab).style.display = ''; 
			document.all('collapse'+tab).style.display = 'none'; 
		} else {
			document.all(ens + 'tab'+tab).style.display = ''; 
			document.all('collapse'+tab).style.display = ''; 
			document.all('expand'+tab).style.display = 'none'; 
		}
	}
}

function loadTabState(tab, ens)
{	
	var cookieValue = readCookie(tab);
	
	if(cookieValue == null)
	{
		cookieValue = 1;
	}

	if (cookieValue == 0)
	{
		document.getElementById(ens + 'tab'+tab).style.display = 'none'; 
		document.getElementById('Expand'+tab).style.display = ''; 
		document.getElementById('Collapse'+tab).style.display = 'none'; 
	}
	else
	{
		document.getElementById(ens + 'tab'+tab).style.display = ''; 
		document.getElementById('Collapse'+tab).style.display = ''; 
		document.getElementById('Expand'+tab).style.display = 'none';
	}
}

function readCookie(name)
{
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++)
	{
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}

function DisplayAllTab(listOfTab,action){
	var arrOfTab = listOfTab.split(',');
	for (var i=0; i < arrOfTab.length; i++) {
		if (document.getElementById){
			if(action=='collapse'){
				document.getElementById('tab'+arrOfTab[i]).style.display = 'none'; 
				document.getElementById('expand'+arrOfTab[i]).style.display = ''; 
				document.getElementById('collapse'+arrOfTab[i]).style.display = 'none'; 
			} else {
				document.getElementById('tab'+arrOfTab[i]).style.display = ''; 
				document.getElementById('expand'+arrOfTab[i]).style.display = 'none'; 
				document.getElementById('collapse'+arrOfTab[i]).style.display = ''; 
			}
		} else if (document.all) {
			if(action=='collapse'){
				document.all('tab'+arrOfTab[i]).style.display = 'none'; 
				document.all('expand'+arrOfTab[i]).style.display = ''; 
				document.all('collapse'+arrOfTab[i]).style.display = 'none';
			} else {
				document.all('tab'+arrOfTab[i]).style.display = ''; 
				document.all('expand'+arrOfTab[i]).style.display = 'none'; 
				document.all('collapse'+arrOfTab[i]).style.display = '';
			}
		}
	}
}

function ResizeDiv(td_id,div_id){
	if (document.getElementById)
		document.getElementById(div_id).style.height = document.getElementById(td_id).offsetHeight;
	else if (document.all)
		document.all(div_id).style.height = document.all(td_id).offsetHeight;
	else
		alert('error resizing content');
}

var OpenWindow;

function PrintContent(rootfolder){
	//document.getElementById("table_frame").className = "ONLYSCREEN";
	//document.getElementById("div_print").innerHTML = document.getElementById("div_content").innerHTML;
	//document.getElementById("div_print").className = "PRINTER HIDDEN";
 	//window.print(); 	
	OpenWindow=window.open(rootfolder+"Common/blank.htm", "printscreen", "height=600, width=800, left=20, top=20, " + 
					"location=no, menubar=no, resizable=yes, " + 
					"scrollbars=yes, titlebar=no, toolbar=no", true);
	return;
}

function CloseWindowFromPopupScreen()
{
	OpenWindow.close();	
}

// redirection after 20 minutes

var time1;
 
function timer() { 
time1=window.setTimeout("redirect()",1200000); 
} 

function redirect() {
window.location = "\\"; 
} 

function detime() { 
window.clearTimeout(time1); 
timer(); 
} 

function sendMailCoded (addressStart, addressEnd)
{
	document.location.href="mailto:"+addressStart+'@'+addressEnd;
}


function modifyRadEditorIcon(theID)
{
var tableSplit=theID.split('#');
for (var i=0;i<tableSplit.length;i++)
{
    try
    {
    var placeholder = document.getElementById(tableSplit[i] + '_wrapper');
    var images = placeholder.getElementsByTagName('img');
    var image = images[0];
    if(image.src.indexOf('editor.gif')>0)
        image.style.position = 'absolute';
    }
    catch(e){}
}
//image.style.top = '100px';
//image.style.left = '100px';
}