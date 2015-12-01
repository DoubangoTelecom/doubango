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

#include "rt_Config.h"

class DDebugCallback;
class DDebugCallbackProxy;

namespace doubango_rt
{
	namespace BackEnd
	{
		public interface class rtIDDebugCallback
		{
			virtual int OnDebugInfo(Platform::String^ message);
			virtual int OnDebugWarn(Platform::String^ message);
			virtual int OnDebugError(Platform::String^ message);
			virtual int OnDebugFatal(Platform::String^ message);
		};

		public ref class rtDDebugCallback sealed
		{
		internal:
			rtDDebugCallback(rtIDDebugCallback^ pI);
		public:
			virtual ~rtDDebugCallback();
			friend class DDebugCallbackProxy;
		
		internal:
			const DDebugCallback* getWrappedCallback();

		private:
			rtIDDebugCallback^ m_pI;
			DDebugCallbackProxy* m_pCallback;
		};
	}
}