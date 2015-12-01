/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*/
#pragma once

#include "rt_Enums.h"

class ActionConfig;

namespace doubango_rt
{
	namespace BackEnd
	{
		public ref class rtActionConfig sealed
		{			
		public:
			rtActionConfig();
			virtual ~rtActionConfig();
	
			bool addHeader(Platform::String^ name, Platform::String^ value);
#if COM_VISIBLE
			bool addPayload(Platform::String^ payload);
#else
			bool addPayload(Platform::IntPtr payload, unsigned len);
#endif
			bool setActiveMedia(rt_twrap_media_type_t type);
	
			rtActionConfig^ setResponseLine(short code, Platform::String^ phrase);
			rtActionConfig^ setMediaString(rt_twrap_media_type_t type, Platform::String^ key, Platform::String^ value);
			rtActionConfig^ setMediaInt(rt_twrap_media_type_t type, Platform::String^ key, int value);

		internal:
			ActionConfig* getWrappedActionConfig(){ return m_pActionConfig; }

		private:
			ActionConfig* m_pActionConfig;
		};
	}
}