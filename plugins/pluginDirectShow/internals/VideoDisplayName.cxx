/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#include "internals/VideoDisplayName.h"

VideoDisplayName::VideoDisplayName(std::string name_, std::string descr) : name(name_), description(descr)
{
}

std::string VideoDisplayName::getName() const
{
	return this->name;
}

std::string VideoDisplayName::getDescription() const
{
	return this->description;
}

int VideoDisplayName::operator==(const VideoDisplayName &dev) const
{
	return this->name == dev.name;
}
