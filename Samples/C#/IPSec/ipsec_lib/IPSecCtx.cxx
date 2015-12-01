/* Copyright (C) 2010-2014 Mamadou DIOP
* Copyright (C) 2011-2014 Doubango Telecom <http://www.doubango.org>
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
#include "IPSecCtx.h"
#include "tsk_debug.h"

#include <assert.h>

bool IPSecCtx::sInitialized = false;
extern "C" const tipsec_plugin_def_t *plugin_win_ipsec_vista_plugin_def_t;

IPSecCtx::IPSecCtx(tipsec_ipproto_t ipproto,
		bool use_ipv6,
		tipsec_mode_t mode,
		tipsec_ealg_t ealg,
		tipsec_alg_t alg,
		tipsec_proto_t protocol)
: m_pCtx(NULL)
{
	tipsec_ctx_t* pCtx = NULL;
	if (!IPSecCtx::sInitialized)
	{
		assert (tipsec_plugin_register_static(plugin_win_ipsec_vista_plugin_def_t) == 0);
		IPSecCtx::sInitialized = true;
	}
	assert (tipsec_ctx_create(ipproto, use_ipv6, mode, ealg, alg, protocol, &m_pCtx) == 0 && m_pCtx != NULL);
}

IPSecCtx::~IPSecCtx()
{
	TSK_OBJECT_SAFE_FREE(m_pCtx);
}

tipsec_error_t IPSecCtx::start()
{
	return tipsec_ctx_start(m_pCtx);
}

tipsec_error_t IPSecCtx::setLocal(const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
	return tipsec_ctx_set_local(m_pCtx, addr_local, addr_remote, port_uc, port_us);
}

tipsec_error_t IPSecCtx::setKeys(const tipsec_key_t* ik, const tipsec_key_t* ck)
{
	return tipsec_ctx_set_keys(m_pCtx, ik, ck);
}

tipsec_error_t IPSecCtx::setRemote(tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
	return tipsec_ctx_set_remote(m_pCtx, spi_pc, spi_ps, port_pc, port_ps, lifetime);
}

tipsec_error_t IPSecCtx::stop()
{
	return tipsec_ctx_stop(m_pCtx);
}

tipsec_spi_t IPSecCtx::getSpiUC()
{
	return m_pCtx->spi_uc;
}

tipsec_spi_t IPSecCtx::getSpiUS()
{
	return m_pCtx->spi_us;
}

tipsec_spi_t IPSecCtx::getSpiPC()
{
	return m_pCtx->spi_pc;
}

tipsec_spi_t IPSecCtx::getSpiPS()
{
	return m_pCtx->spi_ps;
}



