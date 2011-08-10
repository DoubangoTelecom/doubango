
var Telligent_Common=new Object();Telligent_Common._state=new Array();Telligent_Common.ConvertHsvColorToRgbColor=function(hsv)
{var h=hsv[0];var s=hsv[1];var v=hsv[2];var r;var g;var b;if(s==0)
return new Array(v,v,v);var htemp;if(h==360)
htemp=0;else
htemp=h;htemp=htemp/60;var i=Math.floor(htemp);var f=htemp-i;var p=v*(1-s);var q=v*(1-(s*f));var t=v*(1-(s*(1-f)));if(i==0){r=v;g=t;b=p;}
if(i==1){r=q;g=v;b=p;}
if(i==2){r=p;g=v;b=t;}
if(i==3){r=p;g=q;b=v;}
if(i==4){r=t;g=p;b=v;}
if(i==5){r=v;g=p;b=q;}
r=Math.round(r);g=Math.round(g);b=Math.round(b);return new Array(r,g,b);}
Telligent_Common.ConvertRgbColorToHsvColor=function(rgb)
{var r=rgb[0];var g=rgb[1];var b=rgb[2];var h;var s;var v=Math.max(Math.max(r,g),b);var min=Math.min(Math.min(r,g),b);var delta=v-min;if(v==0)
s=0
else
s=delta/v;if(s==0)
h=0;else
{if(r==v)
h=60*(g-b)/delta;else if(g==v)
h=120+60*(b-r)/delta;else if(b==v)
h=240+60*(r-g)/delta;}
if(h<0)
h+=360;return new Array(h,s,v);}
Telligent_Common.ConvertDecimalToHexadecimal=function(d,digits)
{var chars="0123456789abcdef";var h='';var mod;while(d>0)
{mod=d%16;h=chars.substr(mod,1)+h;d-=mod;d/=16;}
if(digits)
{while(h.length<digits)
{h="0"+h;}}
return h;}
Telligent_Common.ConvertHtmlColorToRgbColor=function(html)
{html=html.replace(/[^0-9a-f]/ig,'');if(html.length==3)
return new Array(parseInt(html.substr(0,1)+html.substr(0,1),16),parseInt(html.substr(1,1)+html.substr(1,1),16),parseInt(html.substr(2,1)+html.substr(2,1),16));else if(html.length==6)
return new Array(parseInt(html.substr(0,2),16),parseInt(html.substr(2,2),16),parseInt(html.substr(4,2),16));else
return new Array(255,255,255);}
Telligent_Common.ConvertRgbColorToHtmlColor=function(rgbColor)
{return"#"+Telligent_Common.ConvertDecimalToHexadecimal(rgbColor[0],2)+Telligent_Common.ConvertDecimalToHexadecimal(rgbColor[1],2)+Telligent_Common.ConvertDecimalToHexadecimal(rgbColor[2],2);}
Telligent_Common.GetContrastingHtmlColorForRgbColor=function(rgb)
{var illuminance=(rgb[0]*2)+(rgb[1]*5)+(rgb[2]);if(illuminance>1024)
return"#000000";else
return"#ffffff";}
Telligent_Common.GetElementInfo=function(element)
{var curleft=0;var curtop=0;var obj=element;var leftComplete=false;var topComplete=false;var isIE=Telligent_Common.IsIE();while(obj)
{if(!leftComplete)
curleft+=obj.offsetLeft;if(!topComplete)
curtop+=obj.offsetTop;if(isIE&&obj.offsetParent&&obj.offsetParent.style.position=='relative')
{if(!obj.offsetParent.style.width)
{leftComplete=true;if((obj.offsetParent.style.top||obj.offsetParent.style.bottom))
{curtop+=obj.offsetHeight;topComplete=true;}}}
obj=obj.offsetParent;}
var elementInfo=new Object();elementInfo.Left=curleft;elementInfo.Top=curtop;elementInfo.Width=element.offsetWidth;elementInfo.Height=element.offsetHeight;return elementInfo;}
Telligent_Common.GetWindowInfo=function()
{var scrollX=0,scrollY=0,width=0,height=0;if(typeof(window.pageXOffset)=='number')
{scrollX=window.pageXOffset;scrollY=window.pageYOffset;}
else if(document.body&&(document.body.scrollLeft||document.body.scrollTop))
{scrollX=document.body.scrollLeft;scrollY=document.body.scrollTop;}
else if(document.documentElement&&(document.documentElement.scrollLeft||document.documentElement.scrollTop))
{scrollX=document.documentElement.scrollLeft;scrollY=document.documentElement.scrollTop;}
if(typeof(window.innerWidth)=='number')
{width=window.innerWidth;height=window.innerHeight;}
else if(document.documentElement&&(document.documentElement.clientWidth||document.documentElement.clientHeight))
{width=document.documentElement.clientWidth;height=document.documentElement.clientHeight;}
else if(document.body&&(document.body.clientWidth||document.body.clientHeight))
{width=document.body.clientWidth;height=document.body.clientHeight;}
if(document.documentElement&&(document.documentElement.scrollHeight||document.documentElement.offsetHeight))
{if(document.documentElement.scrollHeight>document.documentElement.offsetHeight)
{contentWidth=document.documentElement.scrollWidth;contentHeight=document.documentElement.scrollHeight;}
else
{contentWidth=document.documentElement.offsetWidth;contentHeight=document.documentElement.offsetHeight;}}
else if(document.body&&(document.body.scrollHeight||document.body.offsetHeight))
{if(document.body.scrollHeight>document.body.offsetHeight)
{contentWidth=document.body.scrollWidth;contentHeight=document.body.scrollHeight;}
else
{contentWidth=document.body.offsetWidth;contentHeight=document.body.offsetHeight;}}
else
{contentWidth=width;contentHeight=height;}
if(height>contentHeight)
height=contentHeight;if(width>contentWidth)
width=contentWidth;var rect=new Object();rect.ScrollX=scrollX;rect.ScrollY=scrollY;rect.Width=width;rect.Height=height;rect.ContentWidth=contentWidth;rect.ContentHeight=contentHeight;return rect;}
Telligent_Common.GetCurrentStyleValue=function(element,styleRule,jsStyleRule,defaultValue)
{var value='';try
{if(document.defaultView&&document.defaultView.getComputedStyle)
value=document.defaultView.getComputedStyle(element,"").getPropertyValue(styleRule);else if(element.currentStyle)
value=element.currentStyle[jsStyleRule];}catch(e){}
if((value=='inherit'||value=='transparent')&&element.parentNode!=null)
return Telligent_Common.GetCurrentStyleValue(element.parentNode,styleRule,jsStyleRule,defaultValue);else if(value!=''&&value!=undefined&&value!='rgba(0, 0, 0, 0)')
return value;else
return defaultValue;}
Telligent_Common.GetStyleOffset=function(element)
{var result=new Object();result.Height=Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'border-top-width','borderTopWidth','0')),0)+
Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'border-bottom-width','borderBottomWidth','0')),0)+
Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'padding-top','paddingTop','0')),0)+
Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'padding-bottom','paddingBottom','0')),0);result.Width=Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'border-left-width','borderLeftWidth','0')),0)+
Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'border-right-width','borderRightWidth','0')),0)+
Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'padding-left','paddingLeft','0')),0)+
Telligent_Common.IsNanDefault(parseInt(Telligent_Common.GetCurrentStyleValue(element,'padding-right','paddingRight','0')),0);return result;}
Telligent_Common.IsNanDefault=function(value,defaultValue)
{if(isNaN(value))
return defaultValue;else
return value;}
Telligent_Common.EscapeForRegExp=function(value)
{return value.replace(/([\\\(\^\$\*\+\?\{\}\.\)\|\-])/g,'\\$1');}
Telligent_Common.GetSelectedHtmlInElement=function(element,includeAllContentIfNoSelection,includeAllContentIfInvalidSelection,invalidSelectionMessage)
{var selectionIsValid=true;var content=null;if(window.getSelection)
{var selection=window.getSelection();if(selection&&selection.rangeCount>0&&selection.toString().length>0)
{selectionIsValid=false;var selectedRange=selection.getRangeAt(0);var availableRange=document.createRange();availableRange.selectNode(element);if(availableRange.compareBoundaryPoints(Range.START_TO_START,selectedRange)<=0&&availableRange.compareBoundaryPoints(Range.END_TO_END,selectedRange)>=0)
{var temp=document.createElement('div');temp.appendChild(selectedRange.cloneContents());content=temp.innerHTML;}
else if(invalidSelectionMessage)
alert(invalidSelectionMessage);}}
else if(document.selection)
{var range=document.selection.createRange();if(range&&range.text)
{selectionIsValid=false;var parent=range.parentElement();if(parent!=null&&Telligent_Common.ElementContainsElement(element,parent))
content=range.htmlText;else if(invalidSelectionMessage)
alert(invalidSelectionMessage);}}
if(content==null&&((selectionIsValid&&includeAllContentIfNoSelection)||includeAllContentIfInvalidSelection))
content=element.innerHTML;return content;}
Telligent_Common.ElementContainsElement=function(parent,child)
{if(!parent||!child)
return false;if(parent==child)
return true;if(parent&&parent.childNodes)
{for(var i=0;i<parent.childNodes.length;i++)
{if(parent.childNodes[i]==child||Telligent_Common.ElementContainsElement(parent.childNodes[i],child))
return true;}}
return false;}
Telligent_Common.GetCurrentCursorIndex=function(inputElement)
{var index=0;if(inputElement.selectionStart||inputElement.selectionStart=='0')
index=inputElement.selectionStart;else if(document.selection)
{var originalValue=inputElement.value;var range=document.selection.createRange();var escapeChar=String.fromCharCode(1);range.text=escapeChar;index=inputElement.value.indexOf(escapeChar);inputElement.value=originalValue;}
return index;}
Telligent_Common._delayedSetCurrentCursorSelection=function(stateId)
{var state=Telligent_Common.GetStateByStateId(stateId,'Telligent_Common_SetCurrentCursorSelection');if(state)
{Telligent_Common.SetCurrentCursorSelection(state.InputElement,state.StartIndex,state.EndIndex,false,state.PersistedValue);Telligent_Common.ClearStateByStateId(stateId,'Telligent_Common_SetCurrentCursorSelection');}}
Telligent_Common.IsSafari=function()
{return navigator.userAgent.indexOf('Safari')!=-1;}
Telligent_Common.IsOpera=function()
{return window.opera!=null;}
Telligent_Common.IsIE=function()
{return document.getElementsByTagName('body')[0].currentStyle!=null;}
Telligent_Common.CreateSafeFunction=function(instance,func)
{return function(){return func.apply(instance,arguments);}}
Telligent_Common.DisposeContent=function(parentElement)
{if(parentElement&&parentElement.childNodes&&parentElement.childNodes.length>0)
{for(var i=0;i<parentElement.childNodes.length;i++)
{if(parentElement.childNodes[i].dispose)
{try
{parentElement.childNodes[i].dispose();}
catch(e){}}
Telligent_Common.DisposeContent(parentElement.childNodes[i]);}}}
Telligent_Common.SetCurrentCursorSelection=function(inputElement,startIndex,endIndex,functionKeyWasHandled,persistedValue)
{if(Telligent_Common.IsSafari()||Telligent_Common.IsOpera())
{if(functionKeyWasHandled)
{var state=Telligent_Common.GetStateByKey(inputElement,'Telligent_Common_SetCurrentCursorSelection');if(state&&state.CursorTimeout)
window.clearTimeout(state.CursorTimeout);state=new Object();state.InputElement=inputElement;state.StartIndex=startIndex;state.EndIndex=endIndex;state.PersistedValue=persistedValue;var stateId=Telligent_Common.SaveStateByKey(inputElement,'Telligent_Common_SetCurrentCursorSelection',state);state.CursorTimeout=window.setTimeout(new Function('Telligent_Common._delayedSetCurrentCursorSelection('+stateId+');'),9);Telligent_Common.SaveStateByKey(inputElement,'Telligent_Common_SetCurrentCursorSelection',state);return;}
else
{inputElement.focus();if(persistedValue)
inputElement.value=persistedValue;}}
try
{if(document.selection)
{var range=inputElement.createTextRange();range.move('character',startIndex);range.moveEnd('character',endIndex-startIndex);range.select();}
else if(inputElement.setSelectionRange)
{inputElement.setSelectionRange(startIndex,endIndex);inputElement.focus();}
else if(inputElement.selectionStart||inputElement.selectionStart=='0')
{inputElement.selectionStart=startIndex;inputElement.selectionEnd=endIndex;}}catch(e){}}
Telligent_Common.GetStateByStateId=function(stateId,processId)
{if(Telligent_Common._state.length>stateId&&stateId>=0&&Telligent_Common._state[stateId]&&Telligent_Common._state[stateId]._stateProcessId==processId)
return Telligent_Common._state[stateId]._state;else
return null;}
Telligent_Common.GetStateByKey=function(key,processId)
{for(var i=0;i<Telligent_Common._state.length;i++)
{if(Telligent_Common._state[i]&&Telligent_Common._state[i]._stateKey==key&&Telligent_Common._state[i]._stateProcessId==processId)
return Telligent_Common._state[i]._state;}
return null;}
Telligent_Common.SaveStateByStateId=function(stateId,processId,state)
{if(Telligent_Common._state.length>stateId&&stateId>=0&&Telligent_Common._state[stateId]&&Telligent_Common._state[stateId]._stateProcessId==processId)
Telligent_Common._state[stateId]._state=state;}
Telligent_Common.SaveStateByKey=function(key,processId,state)
{var emptyIndex=-1;for(var i=0;i<Telligent_Common._state.length;i++)
{if(!Telligent_Common._state[i])
emptyIndex=i;else if(Telligent_Common._state[i]._stateKey==key&&Telligent_Common._state[i]._stateProcessId==processId)
{Telligent_Common._state[i]._state=state;return i;}}
var stateBucket=new Object();stateBucket._stateKey=key;stateBucket._stateProcessId=processId;stateBucket._state=state;if(emptyIndex!=-1)
{Telligent_Common._state[emptyIndex]=stateBucket;return emptyIndex;}
else
{Telligent_Common._state[Telligent_Common._state.length]=stateBucket;return Telligent_Common._state.length-1;}}
Telligent_Common.ClearStateByStateId=function(stateId,processId)
{if(Telligent_Common._state.length>stateId&&stateId>=0&&Telligent_Common._state[stateId]&&Telligent_Common._state[stateId]._stateProcessId==processId)
Telligent_Common._state[stateId]=null;}
Telligent_Common._hiddenSelects=new Array();Telligent_Common._elementsHidingSelects=new Array();Telligent_Common._addHiddenSelect=function(elementHidingSelect,selectElement)
{var hiddenSelect=null;var emptyIndex=-1;for(var i=0;i<Telligent_Common._hiddenSelects.length&&hiddenSelect==null;i++)
{if(Telligent_Common._hiddenSelects[i])
{if(Telligent_Common._hiddenSelects[i].SelectElement==selectElement)
hiddenSelect=Telligent_Common._hiddenSelects[i];}
else
emptyIndex=i;}
if(hiddenSelect==null)
{hiddenSelect=new Object();hiddenSelect.SelectElement=selectElement;hiddenSelect.ElementsHidingSelectsIndeces=new Array();hiddenSelect.OriginalVisibility=selectElement.style.visibility;hiddenSelect.SelectElement.style.visibility='hidden';if(emptyIndex==-1)
{Telligent_Common._hiddenSelects[Telligent_Common._hiddenSelects.length]=hiddenSelect;hiddenSelect.Index=Telligent_Common._hiddenSelects.length-1;}
else
{Telligent_Common._hiddenSelects[emptyIndex]=hiddenSelect;hiddenSelect.Index=emptyIndex;}}
var ignore=false;for(var i=0;i<hiddenSelect.ElementsHidingSelectsIndeces.length&&!ignore;i++)
{if(hiddenSelect.ElementsHidingSelectsIndeces[i]==elementHidingSelect.Index)
ignore=true;}
if(!ignore)
{elementHidingSelect.HiddenSelectsIndeces[elementHidingSelect.HiddenSelectsIndeces.length]=hiddenSelect.Index;hiddenSelect.ElementsHidingSelectsIndeces[hiddenSelect.ElementsHidingSelectsIndeces.length]=elementHidingSelect.Index;}}
Telligent_Common._addElementHidingSelect=function(element)
{var elementHidingSelect=null;var emptyIndex=-1;for(var i=0;i<Telligent_Common._elementsHidingSelects.length&&elementHidingSelect==null;i++)
{if(Telligent_Common._elementsHidingSelects[i])
{if(Telligent_Common._elementsHidingSelects[i].Element==element)
elementHidingSelect=Telligent_Common._elementsHidingSelects[i];}
else
emptyIndex=i;}
if(elementHidingSelect==null)
{elementHidingSelect=new Object();elementHidingSelect.Element=element;elementHidingSelect.HiddenSelectsIndeces=new Array();if(emptyIndex==-1)
{Telligent_Common._elementsHidingSelects[Telligent_Common._elementsHidingSelects.length]=elementHidingSelect;elementHidingSelect.Index=Telligent_Common._elementsHidingSelects.length-1;}
else
{Telligent_Common._elementsHidingSelects[emptyIndex]=elementHidingSelect;elementHidingSelect.Index=emptyIndex;}}
return elementHidingSelect;}
Telligent_Common.HideSelectBoxes=function(element,hideAll)
{if(element.getClientRects)
{var selectBoxes=document.getElementsByTagName('select');var elementRect=element.getClientRects()[0];var elementHidingSelect=null;for(var i=0;i<selectBoxes.length;i++)
{if(selectBoxes[i].getClientRects)
{var rects=selectBoxes[i].getClientRects();for(var j=0;j<rects.length;j++)
{if(hideAll||(rects[j].top<elementRect.bottom&&elementRect.top<rects[j].bottom&&rects[j].left<elementRect.right&&elementRect.left<rects[j].right))
{var inElement=false;var selectParent=selectBoxes[i].offsetParent;while(selectParent!=null)
{if(selectParent==element)
{inElement=true;break;}
selectParent=selectParent.offsetParent;}
if(!inElement)
{if(elementHidingSelect==null)
elementHidingSelect=Telligent_Common._addElementHidingSelect(element);Telligent_Common._addHiddenSelect(elementHidingSelect,selectBoxes[i]);selectBoxes[i].style.visibility='hidden';}
break;}}}}}}
Telligent_Common.ShowSelectBoxes=function(element)
{var elementHidingSelect=null;for(var i=0;i<Telligent_Common._elementsHidingSelects.length&&elementHidingSelect==null;i++)
{if(Telligent_Common._elementsHidingSelects[i])
{if(Telligent_Common._elementsHidingSelects[i].Element==element)
elementHidingSelect=Telligent_Common._elementsHidingSelects[i];}}
if(elementHidingSelect==null)
return;var hiddenSelect;for(var i=0;i<elementHidingSelect.HiddenSelectsIndeces.length;i++)
{hiddenSelect=Telligent_Common._hiddenSelects[elementHidingSelect.HiddenSelectsIndeces[i]];if(hiddenSelect)
{if(hiddenSelect.ElementsHidingSelectsIndeces.length==1)
{if(hiddenSelect.SelectElement.style.visibility=='hidden')
hiddenSelect.SelectElement.style.visibility=hiddenSelect.OriginalVisibility;Telligent_Common._hiddenSelects[hiddenSelect.Index]=null;}
else
{var elementsHidingSelects=new Array();for(var j=0;j<hiddenSelect.ElementsHidingSelectsIndeces.length;j++)
{if(hiddenSelect.ElementsHidingSelectsIndeces[j]!=elementHidingSelect.Index)
elementsHidingSelects[elementsHidingSelects.length]=hiddenSelect.ElementsHidingSelectsIndeces[i];}
hiddenSelect.ElementsHidingSelectsIndeces=elementsHidingSelects;}}}
Telligent_Common._elementsHidingSelects[elementHidingSelect.Index]=null;}