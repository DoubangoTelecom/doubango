/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
*
*/

/**@file tnet_proto.h
 * @brief List of all assigned protocols as defined by the IANA (http://www.iana.org/assignments/protocol-numbers/).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_PROTO_H
#define TNET_PROTO_H

#include "tinynet_config.h"

TNET_BEGIN_DECLS

/**
 * @enum	tnet_proto_e
 *
 * @brief	List of all assigned protocols as defined by the IANA 
			(http://www.iana.org/assignments/protocol-numbers/).
**/
typedef enum tnet_proto_e
{
	TNET_PROTO_HOPOPT = 0,					/**< HOPOPT           IPv6 Hop-by-Hop Option                   [RFC1883] */
	TNET_PROTO_ICMP = 1,					/**< ICMP             Internet Control Message                 [RFC792]  */
	TNET_PROTO_IGMP = 2,					/**< IGMP             Internet Group Management                [RFC1112] */
	TNET_PROTO_GGP = 3,						/**< GGP              Gateway-to-Gateway                       [RFC823] */
	TNET_PROTO_IP = 4,						/**< IP               IP in IP (encapsulation)                 [RFC2003] */
	TNET_PROTO_ST = 5,						/**< ST               Stream                                   [RFC1190][RFC1819] */
	TNET_PROTO_TCP = 6,						/**< TCP              Transmission Control                     [RFC793] */
	TNET_PROTO_CBT = 7,						/**< CBT              CBT                                      [Ballardie] */
	TNET_PROTO_EGP = 8,						/**< EGP              Exterior Gateway Protocol                [RFC888][DLM1] */
	TNET_PROTO_IGP = 9,						/**< IGP              any private interior gateway             [IANA](used by Cisco for their IGRP)   */
	TNET_PROTO_BBN_RCC_MON = 10,			/**< BBN-RCC-MON      BBN RCC Monitoring                       [SGC] */
	TNET_PROTO_NVP_II = 11,					/**< NVP-II           Network Voice Protocol                   [RFC741][SC3] */
	TNET_PROTO_PUP = 12,					/**< PUP              PUP                                      [PUP][XEROX] */
	TNET_PROTO_ARGUS = 13,					/**< ARGUS            ARGUS                                    [RWS4] */
	TNET_PROTO_EMCON = 14,					/**< EMCON            EMCON                                    [BN7] */
	TNET_PROTO_XNET = 15,					/**< XNET             Cross Net Debugger                       [IEN158][JFH2] */
	TNET_PROTO_CHAOS = 16,					/**< CHAOS            Chaos                                    [NC3] */
	TNET_PROTO_UDP = 17,					/**< UDP              User Datagram                            [RFC768][JBP] */
	TNET_PROTO_MUX = 18,					/**< MUX              Multiplexing                             [IEN90][JBP] */
	TNET_PROTO_DCN_MEAS = 19,				/**< DCN-MEAS         DCN Measurement Subsystems               [DLM1] */
	TNET_PROTO_HMP = 20,					/**< HMP              Host Monitoring                          [RFC869][RH6] */
	TNET_PROTO_PRM = 21,					/**< PRM              Packet Radio Measurement                 [ZSU] */
	TNET_PROTO_XNS_IDP = 22,				/**< XNS-IDP          XEROX NS IDP                             [ETHERNET][XEROX] */
	TNET_PROTO_TRUNK_1 = 23,				/**< TRUNK-1          Trunk-1                                  [BWB6] */
	TNET_PROTO_TRUNK_2 = 24,				/**< TRUNK-2          Trunk-2                                  [BWB6] */
	TNET_PROTO_LEAF_1 = 25,					/**< LEAF-1           Leaf-1                                   [BWB6] */
	TNET_PROTO_LEAF_2 = 26,					/**< LEAF-2           Leaf-2                                   [BWB6] */
	TNET_PROTO_RDP = 27,					/**< RDP              Reliable Data Protocol                   [RFC908][RH6] */
	TNET_PROTO_IRTP = 28,					/**< IRTP             Internet Reliable Transaction            [RFC938][TXM] */
	TNET_PROTO_ISO_TP4 = 29,				/**< ISO-TP4          ISO Transport Protocol Class 4           [RFC905][RC77] */
	TNET_PROTO_NETBLT = 30,					/**< NETBLT           Bulk Data Transfer Protocol              [RFC969][DDC1] */
	TNET_PROTO_MFE_NSP = 31,				/**< MFE-NSP          MFE Network Services Protocol            [MFENET][BCH2] */
	TNET_PROTO_MERIT_INP = 32,				/**< MERIT-INP        MERIT Internodal Protocol                [HWB] */
	TNET_PROTO_DCCP = 33,					/**< DCCP             Datagram Congestion Control Protocol     [RFC4340] */
	TNET_PROTO_3PC = 34,					/**< 3PC              Third Party Connect Protocol             [SAF3] */
	TNET_PROTO_IDPR = 35,					/**< IDPR             Inter-Domain Policy Routing Protocol     [MXS1]  */
	TNET_PROTO_XTP = 36,					/**< XTP              XTP                                      [GXC] */
	TNET_PROTO_DDP = 37,					/**< DDP              Datagram Delivery Protocol               [WXC] */
	TNET_PROTO_IDPR_CMTP = 38,				/**< IDPR-CMTP        IDPR Control Message Transport Proto     [MXS1] */
	TNET_PROTO_TP_PP = 39,					/**< TP++             TP++ Transport Protocol                  [DXF] */
	TNET_PROTO_IL = 40,						/**< IL               IL Transport Protocol                    [Presotto] */
	TNET_PROTO_IPv6 = 41,					/**< IPv6             Ipv6                                     [Deering]  */   
	TNET_PROTO_SDRP = 42,					/**< SDRP             Source Demand Routing Protocol           [DXE1] */
	TNET_PROTO_IPv6_Route = 43,				/**< IPv6-Route       Routing Header for IPv6                  [Deering] */
	TNET_PROTO_IPv6_Frag = 44,				/**< IPv6-Frag        Fragment Header for IPv6                 [Deering] */
	TNET_PROTO_IDRP = 45,					/**< IDRP             Inter-Domain Routing Protocol            [Hares] */
	TNET_PROTO_RSVP = 46,					/**< RSVP             Reservation Protocol                     [Braden] */
	TNET_PROTO_GRE = 47,					/**< GRE              General Routing Encapsulation            [Li] */
	TNET_PROTO_DSR = 48,					/**< DSR              Dynamic Source Routing Protocol          [RFC4728] */
	TNET_PROTO_BNA = 49,					/**< BNA              BNA                                      [Salamon] */
	TNET_PROTO_ESP = 50,					/**< ESP              Encap Security Payload                   [RFC4303] */
	TNET_PROTO_AH = 51,						/**< AH               Authentication Header                    [RFC4302] */
	TNET_PROTO_I_NLSP = 52,					/**< I-NLSP           Integrated Net Layer Security  TUBA      [GLENN] */
	TNET_PROTO_SWIPE = 53,					/**< SWIPE            IP with Encryption                       [JI6] */
	TNET_PROTO_NARP = 54,					/**< NARP             NBMA Address Resolution Protocol         [RFC1735] */
	TNET_PROTO_MOBILE = 55,					/**< MOBILE           IP Mobility                              [Perkins] */
	TNET_PROTO_TLSP = 56,					/**< TLSP             Transport Layer Security Protocol        [Oberg] using Kryptonet key management */
	TNET_PROTO_SKIP = 57,					/**< SKIP             SKIP                                     [Markson] */
	TNET_PROTO_IPv6_ICMP = 58,				/**< IPv6-ICMP        ICMP for IPv6                            [RFC1883] */
	TNET_PROTO_IPv6_NoNxt = 59,				/**< IPv6-NoNxt       No Next Header for IPv6                  [RFC1883] */
	TNET_PROTO_IPv6_Opts = 60,				/**< IPv6-Opts        Destination Options for IPv6             [RFC1883] */
	TNET_PROTO_ANY_INTERNAL = 61,           /**< any host internal protocol               [IANA] */
	TNET_PROTO_CFTP = 62,					/**< CFTP             CFTP                                     [CFTP][HCF2] */
	TNET_PROTO_ANY_LOCAL = 63,              /**< any local network                        [IANA] */
	TNET_PROTO_SAT_EXPAK = 64,				/**< SAT-EXPAK        SATNET and Backroom EXPAK                [SHB] */
	TNET_PROTO_KRYPTOLAN = 65,				/**< KRYPTOLAN        Kryptolan                                [PXL1] */
	TNET_PROTO_RVD = 66,					/**< RVD              MIT Remote Virtual Disk Protocol         [MBG] */
	TNET_PROTO_IPPC = 67,					/**< IPPC             Internet Pluribus Packet Core            [SHB] */
	TNET_PROTO_ANY_DISTRIBUTED = 68,        /**< any distributed file system              [IANA] */
	TNET_PROTO_SAT_MON = 69,				/**< SAT-MON          SATNET Monitoring                        [SHB] */
	TNET_PROTO_VISA = 70,					/**< VISA             VISA Protocol                            [GXT1] */
	TNET_PROTO_IPCV = 71,					/**< IPCV             Internet Packet Core Utility             [SHB] */
	TNET_PROTO_CPNX = 72,					/**< CPNX             Computer Protocol Network Executive      [DXM2] */
	TNET_PROTO_CPHB = 73,					/**< CPHB             Computer Protocol Heart Beat             [DXM2] */
	TNET_PROTO_WSN = 74,					/**< WSN              Wang Span Network                        [VXD] */
	TNET_PROTO_PVP = 75,					/**< PVP              Packet Video Protocol                    [SC3] */
	TNET_PROTO_BR_SAT_MON = 76,				/**< BR-SAT-MON       Backroom SATNET Monitoring               [SHB] */
	TNET_PROTO_SUN_ND = 77,					/**< SUN-ND           SUN ND PROTOCOL-Temporary                [WM3] */
	TNET_PROTO_WB_MON = 78,					/**< WB-MON           WIDEBAND Monitoring                      [SHB] */
	TNET_PROTO_WB_EXPAK = 79,				/**< WB-EXPAK         WIDEBAND EXPAK                           [SHB] */
	TNET_PROTO_ISO_IP = 80,					/**< ISO-IP           ISO Internet Protocol                    [MTR] */
	TNET_PROTO_VMTP = 81,					/**< VMTP             VMTP                                     [DRC3] */
	TNET_PROTO_SECURE_VMTP = 82,			/**< SECURE-VMTP      SECURE-VMTP                              [DRC3] */
	TNET_PROTO_VINES = 83,					/**< VINES            VINES                                    [BXH] */
	TNET_PROTO_TTP = 84,					/**< TTP              TTP                                      [JXS] */
	TNET_PROTO_NSFNET_IGP = 85,				/**< NSFNET-IGP       NSFNET-IGP                               [HWB] */
	TNET_PROTO_DGP = 86,					/**< DGP              Dissimilar Gateway Protocol              [DGP][ML109] */
	TNET_PROTO_TCF = 87,					/**< TCF              TCF                                      [GAL5] */
	TNET_PROTO_EIGRP = 88,					/**< EIGRP            EIGRP                                    [CISCO][GXS] */
	TNET_PROTO_OSPFIGP = 89,				/**< OSPFIGP          OSPFIGP                                  [RFC1583][JTM4] */
	TNET_PROTO_Sprite_RPC = 90,				/**< Sprite-RPC       Sprite RPC Protocol                      [SPRITE][BXW]  */
	TNET_PROTO_LARP = 91,					/**< LARP             Locus Address Resolution Protocol        [BXH] */
	TNET_PROTO_MTP = 92,					/**< MTP              Multicast Transport Protocol             [SXA] */
	TNET_PROTO_AX25 = 93,					/**< AX.25            AX.25 Frames                             [BK29]     */     
	TNET_PROTO_IPIP = 94,					/**< IPIP             IP-within-IP Encapsulation Protocol      [JI6] */
	TNET_PROTO_MICP = 95,					/**< MICP             Mobile Internetworking Control Pro.      [JI6] */
	TNET_PROTO_SCC_SP = 96,					/**< SCC-SP           Semaphore Communications Sec. Pro.       [HXH]    */  
	TNET_PROTO_ETHERIP = 97,				/**< ETHERIP          Ethernet-within-IP Encapsulation         [RFC3378] */
	TNET_PROTO_ENCAP = 98,					/**< ENCAP            Encapsulation Header                     [RFC1241,RXB3] */
	TNET_PROTO_ANY_PRIV_ENC = 99,           /**< any private encryption scheme            [IANA] */
	TNET_PROTO_GMTP = 100,					/**< GMTP             GMTP                                     [RXB5] */
	TNET_PROTO_IFMP = 101,					/**< IFMP             Ipsilon Flow Management Protocol         [Hinden] */
	TNET_PROTO_PNNI = 102,					/**< PNNI             PNNI over IP                             [Callon] */
	TNET_PROTO_PIM = 103,					/**< PIM              Protocol Independent Multicast           [Farinacci] */
	TNET_PROTO_ARIS = 104,					/**< ARIS             ARIS                                     [Feldman] */
	TNET_PROTO_SCPS = 105,					/**< SCPS             SCPS                                     [Durst] */
	TNET_PROTO_QNX = 106,					/**< QNX              QNX                                      [Hunter] */
	TNET_PROTO_AN = 107,					/**< A/N              Active Networks                          [Braden] */
	TNET_PROTO_IPComp = 108,				/**< IPComp           IP Payload Compression Protocol          [RFC2393] */
	TNET_PROTO_SNP = 109,					/**< SNP              Sitara Networks Protocol                 [Sridhar] */
	TNET_PROTO_Compaq_Peer = 110,			/**< Compaq-Peer      Compaq Peer Protocol                     [Volpe] */
	TNET_PROTO_IPX_in_IP = 11,				/**< IPX-in-IP        IPX in IP                                [Lee] */
	TNET_PROTO_VRRP = 112,					/**< VRRP             Virtual Router Redundancy Protocol       [RFC3768][RFC-ietf-vrrp-unified-spec-05.txt] */
	TNET_PROTO_PGM = 113,					/**< PGM              PGM Reliable Transport Protocol          [Speakman] */
	TNET_PROTO_ANY_ZERO_HOP = 114,          /**< any 0-hop protocol                       [IANA] */
	TNET_PROTO_L2TP = 115,					/**< L2TP             Layer Two Tunneling Protocol             [Aboba] */
	TNET_PROTO_DDX = 116,					/**< DDX              D-II Data Exchange (DDX)                 [Worley]  */
	TNET_PROTO_IATP = 117,					/**< IATP             Interactive Agent Transfer Protocol      [Murphy] */
	TNET_PROTO_STP = 118,					/**< STP              Schedule Transfer Protocol               [JMP] */
	TNET_PROTO_SRP = 119,					/**< SRP              SpectraLink Radio Protocol               [Hamilton] */
	TNET_PROTO_UTI = 120,					/**< UTI              UTI                                      [Lothberg] */
	TNET_PROTO_SMP = 121,					/**< SMP              Simple Message Protocol                  [Ekblad] */
	TNET_PROTO_SM = 122,					/**< SM               SM                                       [Crowcroft] */
	TNET_PROTO_PTP = 123,					/**< PTP              Performance Transparency Protocol        [Welzl] */
	TNET_PROTO_ISIS = 124,					/**< ISIS over IPv4                                            [Przygienda] */
	TNET_PROTO_FIRE = 125,					/**< FIRE                                                      [Partridge] */
	TNET_PROTO_CRTP = 126,					/**< CRTP             Combat Radio Transport Protocol          [Sautter] */
	TNET_PROTO_CRUDP = 127,					/**< CRUDP            Combat Radio User Datagram               [Sautter] */
	TNET_PROTO_SSCOPMCE = 128,				/**< SSCOPMCE                                                  [Waber] */
	TNET_PROTO_IPLT = 129,					/**< IPLT                                                      [Hollbach] */
	TNET_PROTO_SPS = 130,					/**< SPS              Secure Packet Shield                     [McIntosh]  */
	TNET_PROTO_PIPE = 131,					/**< PIPE             Private IP Encapsulation within IP       [Petri] */
	TNET_PROTO_SCTP = 132,					/**< SCTP             Stream Control Transmission Protocol     [Stewart] */
	TNET_PROTO_FC = 133,					/**< FC               Fibre Channel                            [Rajagopal] */
	TNET_PROTO_RSVP_E2E_IGNORE = 134,		/**< RSVP-E2E-IGNORE                                           [RFC3175] */
	TNET_PROTO_Mobility_Header = 135,		/**< Mobility Header                                           [RFC3775] */
	TNET_PROTO_UDPLite = 136,				/**< UDPLite                                                   [RFC3828] */
	TNET_PROTO_MPLS_in_IP = 137,			/**< MPLS-in-IP                                                [RFC4023] */
	TNET_PROTO_manet = 138,					/**< manet            MANET Protocols                          [RFC5498] */
	TNET_PROTO_HIP = 139,					/**< HIP              Host Identity Protocol                   [RFC5201] */
	TNET_PROTO_Shim6 = 140,					/**< Shim6            Shim6 Protocol                           [RFC5533] */
	//TNET_PROTO_ = 141-252                   Unassigned                               [IANA]
	TNET_PROTO_EXP1 = 253,                  /**< Use for experimentation and testing      [RFC3692] */
	TNET_PROTO_EXP2 = 254,                  /**< Use for experimentation and testing      [RFC3692] */
	TNET_PROTO_Reserved = 255,				/**< Reserved                                                  [IANA] */

}
tnet_proto_t;

TNET_END_DECLS

#endif /* TNET_PROTO_H */
