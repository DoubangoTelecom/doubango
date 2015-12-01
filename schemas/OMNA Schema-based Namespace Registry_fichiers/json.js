/*
Copyright (c) 2005 JSON.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The Software shall be used for Good, not Evil.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
    The global object JSON contains two methods.

    JSON.stringify(value) takes a JavaScript value and produces a JSON text.
    The value must not be cyclical.

    JSON.parse(text) takes a JSON text and produces a JavaScript value. It will
    throw a 'JSONError' exception if there is an error.
*/
if (typeof window.RadControlsNamespace == "undefined")
{
	window.RadControlsNamespace = {};
}

if (
	typeof(window.RadControlsNamespace.JSON) == "undefined" ||
	typeof(window.RadControlsNamespace.JSON.Version) == null ||
	window.RadControlsNamespace.JSON.Version < 1
	)
{	
	
	window.RadControlsNamespace.JSON = {
		
		Version : 1, // Change the version when make changes. Change the value in the IF also
	
	    copyright: '(c)2005 JSON.org',
	    license: 'http://www.crockford.com/JSON/license.html',
	/*
	    Stringify a JavaScript value, producing a JSON text.
	*/
	    stringify: function (v, def) {
	        var a = [];
	        var deep = arguments[2] || {};
	/*
	    Emit a string.
	*/
	        function e(s) {
	            a[a.length] = s;
	        }
	
	/*
	    Convert a value.
	*/
	        function g(x) 
	        {
	            var c, i, l, v;
	
	            switch (typeof x) {
	            case 'object':
	                if (x) {
	                    if (x instanceof Array) {
	                        e('[');
	                        l = a.length;
	                        for (i = 0; i < x.length; i += 1) {
	                            v = x[i];
	                            if (typeof v != 'undefined' &&
	                                    typeof v != 'function') {
	                                if (l < a.length) {
	                                    e(',');
	                                }
	                                g(v);
	                            }
	                        }
	                        e(']');
	                        return '';
	                    } else if (typeof x.valueOf == 'function') 
	                    {
	                        e('{');
	                        l = a.length;
	                        for (i in x) 
	                        {
	                        
	                            v = x[i];
	                            
	                            // Modified to skip default props
	                            if (def && v == def[i])
								{
									continue;
								}
								
								
	                            var type = typeof v;
	                            if (
									type == 'undefined' ||
									type == 'function' 
	                            )
	                            {
									continue;
	                            }
	                            
	                            if (type == "object" && !deep[i])
	                            {
									continue;
	                            }
	                            
	                            
	                            if (l < a.length) 
	                            {
	                                    e(',');
	                            }
	                            g(i);
	                            e(':');
	                            g(v);
	                        }
	                        return e('}');
	                    }
	                }
	                e('null');
	                return '';
	            case 'number':
	                e(isFinite(x) ? +x : 'null');
	                return '';
	            case 'string':
	                l = x.length;
	                e('"');
	                for (i = 0; i < l; i += 1) {
	                    c = x.charAt(i);
	                    if (c >= ' ') {
	                        if (c == '\\' || c == '"') {
	                            e('\\');
	                        }
	                        e(c);
	                    } else {
	                        switch (c) {
	                        case '\b':
	                            e('\\b');
	                            break;
	                        case '\f':
	                            e('\\f');
	                            break;
	                        case '\n':
	                            e('\\n');
	                            break;
	                        case '\r':
	                            e('\\r');
	                            break;
	                        case '\t':
	                            e('\\t');
	                            break;
	                        default:
	                            c = c.charCodeAt();
	                            e('\\u00' + Math.floor(c / 16).toString(16) +
	                                (c % 16).toString(16));
	                        }
	                    }
	                }
	                e('"');
	                return '';
	            case 'boolean':
	                e(String(x));
	                return '';
	            default:
	                e('null');
	                return '';
	            }
	        }
	       
	       g(v, 0);
	        return a.join('');
	    },
	    
	    // Addition
	    stringifyHashTable : function (hash, idMember, initial)
	    {
	        var a = [];
	        if (!initial) initial = [];
	        for (var i = 0; i < hash.length; i ++)
	        {
	            var ser = this.stringify(hash[i], initial[i]);
	            if (ser == '{}') continue;
	            a[a.length] = "\"" + hash[i][idMember] + "\":" + ser;
	        }
	        return "{" + a.join(',') + "}";
	    },
	/*
	    Parse a JSON text, producing a JavaScript value.
	*/
	    parse: function (text) {
	        return (/^([ \t\r\n,:{}\[\]]|"(\\["\\\/bfnrtu]|[^\x00-\x1f"\\]+)*"|-?\d+(\.\d*)?([eE][+-]?\d+)?|true|false|null)+$/.test(text)) &&
	            eval('(' + text + ')');
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