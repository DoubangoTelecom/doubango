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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using org.doubango.ipsecWRAP;
using System.Diagnostics;
using err = org.doubango.ipsecWRAP.tipsec_error_t;
using System.Runtime.InteropServices;
namespace ipsec
{
    class Program
    {
        static tipsec_ipproto_t __ipproto = tipsec_ipproto_t.tipsec_ipproto_udp;
        static bool __use_ipv6 = false;
        static tipsec_mode_t __mode = tipsec_mode_t.tipsec_mode_trans;
        static tipsec_ealg_t __ealg = tipsec_ealg_t.tipsec_ealg_des_ede3_cbc;
        static tipsec_alg_t __alg = tipsec_alg_t.tipsec_alg_hmac_md5_96;
        static tipsec_proto_t __proto = tipsec_proto_t.tipsec_proto_esp;

        static String __addr_local = "0.0.0.0";
        static String __addr_remote = "192.168.0.34";
        static ushort __port_local_out = 5062; // PORT_UC
        static ushort __port_local_in = 5064; // PORT_US
        static ushort __port_remote_out = 5066; // PORT_PC
        static ushort __port_remote_in = 5068; // PORT_PS
        static UInt32 __spi_remote_out = 3333; // SPI_PC
        static UInt32 __spi_remote_in = 4444; // SPI_PS
        static UInt64 __lifetime = 1800; /* always set it to the maximum value. (Not possible to update the value after REGISTER 200OK. ) */

        static String __key_ik = "1234567890123456";
        static String __key_ck = "1234567890121234"; 
 
        static void Main(string[] args)
        {
            /* Create the context */
            IPSecCtx ipsecCtx = new IPSecCtx(__ipproto, __use_ipv6, __mode, __ealg, __alg, __proto);

            /* Set local */
            Debug.Assert(ipsecCtx.setLocal(__addr_local, __addr_remote, __port_local_out, __port_local_in) == err.tipsec_error_success);

            /* Dump SPIs created by the OS after calling set_local() */
            Console.WriteLine("SPI-UC={0}, SPI-US={1}", ipsecCtx.getSpiUC(), ipsecCtx.getSpiUS());

            /* Set remote */
            Debug.Assert(ipsecCtx.setRemote(__spi_remote_out, __spi_remote_in, __port_remote_out, __port_remote_in, __lifetime) == err.tipsec_error_success);

            /* Set Integrity (IK) and Confidentiality (CK) keys */
            IntPtr keyIK = Marshal.StringToHGlobalAnsi(__key_ik);
            IntPtr keyCK = Marshal.StringToHGlobalAnsi(__key_ck);
            Debug.Assert(ipsecCtx.setKeys(keyIK, keyCK) == err.tipsec_error_success);
            Marshal.FreeHGlobal(keyIK);
            Marshal.FreeHGlobal(keyCK);

            /* Start (Setup) the SAs */
            Debug.Assert(ipsecCtx.start() == err.tipsec_error_success);

            Console.WriteLine("!!! IPSec SAs started (Press any key to stop) !!!");

            Console.ReadLine();

            ipsecCtx.Dispose(); // Not required. GC will collect it when refCount reach zero.

            Console.ReadLine();
        }
    }
}
