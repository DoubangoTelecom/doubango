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
#ifndef IPSEC_CTX_H
#define IPSEC_CTX_H

#include "tipsec.h"

class IPSecCtx
{
public:
    IPSecCtx(tipsec_ipproto_t ipproto,
             bool use_ipv6,
             tipsec_mode_t mode,
             tipsec_ealg_t ealg,
             tipsec_alg_t alg,
             tipsec_proto_t protocol);
    virtual ~IPSecCtx();
    tipsec_error_t start();
    tipsec_error_t setLocal(const char* addr_local, const char* addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us);
    tipsec_error_t setKeys(const tipsec_key_t* ik, const tipsec_key_t* ck);
    tipsec_error_t setRemote(tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime);
    tipsec_error_t stop();

    tipsec_spi_t getSpiUC();
    tipsec_spi_t getSpiUS();
    tipsec_spi_t getSpiPC();
    tipsec_spi_t getSpiPS();

private:
    static bool sInitialized;
    tipsec_ctx_t* m_pCtx;
};

#endif /* IPSEC_CTX_H */
