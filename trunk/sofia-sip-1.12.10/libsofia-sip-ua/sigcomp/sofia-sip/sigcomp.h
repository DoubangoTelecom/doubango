/**
* @file
* @author  xxxyyyzzz <imsframework(at)gmail.com>
* @version 1.0
*
* @section LICENSE
*
*	
* This file is part of Open Source Doubango IMS Client Framework project.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
* @section DESCRIPTION
*
*
*/
#ifndef __SIGCOMP_H__
#define __SIGCOMP_H__

/* This is libsigcomp version */
#define sigcomp_package_version "1.0.80"

struct sigcomp_compartment
{
	int test;
};

int sigcomp_init();
int sigcomp_deinit();

int sigcomp_compartment_unref(struct sigcomp_compartment* compartment);

#endif /*__SIGCOMP_H__*/