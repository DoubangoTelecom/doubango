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
#include "config.h"

#if HAVE_SIGCOMP

#include "tport_internal.h"

#include "sofia-sip/sigcomp.h"

#define VSC_SIZEOF_CONTEXT 1212 // FIXME
char const sigcomp_name[] = "sigcomp";

/* functions declaration */
int vsc_init_comp(tp_stack_t *, tport_t *, tport_compressor_t *, char const *comp_name, tagi_t const *tags);
void vsc_deinit_comp(tp_stack_t *, tport_t *, tport_compressor_t *);
char const *vsc_comp_name(tport_compressor_t const *master_sc, char const *compression, tagi_t const *tags);

static
tport_comp_vtable_t const comp_vtable =
  {
    sizeof comp_vtable,
    sigcomp_name,
    VSC_SIZEOF_CONTEXT,
    vsc_init_comp,
    vsc_deinit_comp,
    vsc_comp_name,
	0
  };

/** Initialize sigcomp resources and plug vtable
 *
 * @retval 0 when successful
 * @retval ERRNO upon an error
 */
int sigcomp_init()
{
	return tport_plug_in_comp(&comp_vtable);
}

/** Denitialize sigcomp resources and unplug vtable
 *
 * @retval 0 when successful
 * @retval -1 upon an error
 */
int sigcomp_deinit()
{
	return 0;
}

/** Destroy an sigcomp compartment
 *
 * @retval 0 when successful
 * @retval -1 upon an error
 */
int sigcomp_compartment_unref(struct sigcomp_compartment* compartment)
{
	return 0;
}

/** Initialize compressor
 *
 * @retval 0 when successful
 * @retval -1 upon an error
 */
int vsc_init_comp(tp_stack_t * tp_stack, tport_t * tport, tport_compressor_t * compressor, char const *comp_name, tagi_t const *tags)
{
	return 0;
}

/** Deinitialize compressor
 */
void vsc_deinit_comp(tp_stack_t *tp_stack, tport_t * tport, tport_compressor_t * compressor)
{
}

/** Get compressor name
 *
 * @retval compressor name
 */
char const *vsc_comp_name(tport_compressor_t const *master_sc, char const *compression, tagi_t const *tags)
{
	return sigcomp_name;
}

#endif /* HAVE_SIGCOMP */