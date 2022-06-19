#include "pch.h"

hdr_t PacketAnalyzing(const bit8_t* packet_data, Pt_t &packet_count)
{
	hdr_t headers;
	headers.eth = (eth_t*)packet_data;

	switch (ntohs(headers.eth->eth_type))
	{
	case 0x0800:
		headers.ip4h = (ipv4h_t*)(packet_data + sizeof(eth_t));
		packet_count.ip4++;
		switch (headers.ip4h->ip4_protocol)
		{
		case 6:		// TCP
			headers.tcph = (tcph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			headers.type = IPV4_TCP;
			packet_count.tcp++;
			break;
		case 17:	// UDP
			headers.udph = (udph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			headers.type = IPV4_UDP;
			packet_count.udp++;
			break;
		case 1:		// ICMP
			headers.icmph = (icmph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			headers.type = IPV4_TCP_ICMP;
			packet_count.icmp++;
			break;
		}
		break;

	case 0x86DD:
		headers.ip6h = (ipv6h_t*)(packet_data + sizeof(eth_t));
		packet_count.ip6++;
		switch (headers.ip6h->ip6_next)
		{
		case 6:		// TCP
			headers.tcph = (tcph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			headers.type = IPV6_TCP;
			packet_count.tcp++;
			break;
		case 17:	// UDP
			headers.udph = (udph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			headers.type = IPV6_UDP;
			packet_count.udp++;
			break;
		case 58:	// ICMPv6
			headers.icmph = (icmph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			headers.type = IPV6_TCP_ICMP;
			packet_count.icmp++;
			break;
		}
		break;

	case 0x0806:
		headers.arph = (arph_t*)(packet_data + sizeof(eth_t));
		packet_count.arp++;
		headers.type = ARP;
		break;
	}

	return headers;
}

int FindStringPacketData(const struct pcap_pkthdr* header, const bit8_t* packet_data, std::vector<CString> find_strings)
{
	CString packet_string;
	for (UINT i = 0; i < header->len; ++i)
		if (packet_data[i] != '\0')
			packet_string.AppendFormat(_T("%c"), packet_data[i]);

	for (CString str : find_strings)
	{
		if (str == _T(""))	continue;
		if (packet_string.Find(str) != -1)
			return 1;
	}
	return 0;
}

int PrintPacketData(hdr_t packet_headers, CString& strPrintString)
{
	strPrintString = "/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-/\r\n";

	if (packet_headers.eth)
	{
		strPrintString.AppendFormat(_T("[Ethernet Layer]\r\n"));
		strPrintString.AppendFormat(_T("Source MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.eth->eth_src_mac[0],
			packet_headers.eth->eth_src_mac[1],
			packet_headers.eth->eth_src_mac[2],
			packet_headers.eth->eth_src_mac[3],
			packet_headers.eth->eth_src_mac[4],
			packet_headers.eth->eth_src_mac[5]);

		strPrintString.AppendFormat(_T("Destination MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.eth->eth_dst_mac[0],
			packet_headers.eth->eth_dst_mac[1],
			packet_headers.eth->eth_dst_mac[2],
			packet_headers.eth->eth_dst_mac[3],
			packet_headers.eth->eth_dst_mac[4],
			packet_headers.eth->eth_dst_mac[5]);

		strPrintString.AppendFormat(_T("Ethernet Type:		0x%04X\r\n"), ntohs(packet_headers.eth->eth_type));
	}

	if (packet_headers.arph)
	{
		strPrintString.AppendFormat(_T("[ARP protocol]\r\n"));
		strPrintString.AppendFormat(_T("Source IP Address:  %0d.%0d.%0d.%0d\r\n"),
			packet_headers.arph->arp_src_ip[0],
			packet_headers.arph->arp_src_ip[1],
			packet_headers.arph->arp_src_ip[2],
			packet_headers.arph->arp_src_ip[3]);
		strPrintString.AppendFormat(_T("->   Source MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.arph->arp_src_mac[0],
			packet_headers.arph->arp_src_mac[1],
			packet_headers.arph->arp_src_mac[2],
			packet_headers.arph->arp_src_mac[3],
			packet_headers.arph->arp_src_mac[4],
			packet_headers.arph->arp_src_mac[5]);

		strPrintString.AppendFormat(_T("Destination IP Address:  %0d.%0d.%0d.%0d\r\n"),
			packet_headers.arph->arp_dst_ip[0],
			packet_headers.arph->arp_dst_ip[1],
			packet_headers.arph->arp_dst_ip[2],
			packet_headers.arph->arp_dst_ip[3]);
		strPrintString.AppendFormat(_T("->   Destination MAC Address:	%02X:%02X:%02X:%02X:%02X:%02X\r\n"),
			packet_headers.arph->arp_dst_mac[0],
			packet_headers.arph->arp_dst_mac[1],
			packet_headers.arph->arp_dst_mac[2],
			packet_headers.arph->arp_dst_mac[3],
			packet_headers.arph->arp_dst_mac[4],
			packet_headers.arph->arp_dst_mac[5]);
	}

	if (packet_headers.ip4h)
	{
		strPrintString.AppendFormat(_T("[Ipv4 Protocol]\r\n"));
		strPrintString.AppendFormat(_T("Source IP Address:   %0d.%0d.%0d.%0d\r\n"),
			packet_headers.ip4h->ip4_src_ip[0],
			packet_headers.ip4h->ip4_src_ip[1],
			packet_headers.ip4h->ip4_src_ip[2],
			packet_headers.ip4h->ip4_src_ip[3]);

		strPrintString.AppendFormat(_T("Destination IP Address:   %0d.%0d.%0d.%0d\r\n"),
			packet_headers.ip4h->ip4_dst_ip[0],
			packet_headers.ip4h->ip4_dst_ip[1],
			packet_headers.ip4h->ip4_dst_ip[2],
			packet_headers.ip4h->ip4_dst_ip[3]);
	}

	if (packet_headers.ip6h)
	{
		strPrintString.AppendFormat(_T("[Ipv6 Protocol]\r\n"));
		strPrintString.AppendFormat(_T("Source IP Address:   %4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x\r\n"),
			ntohs(packet_headers.ip6h->ip6_src_ip[0]),
			ntohs(packet_headers.ip6h->ip6_src_ip[1]),
			ntohs(packet_headers.ip6h->ip6_src_ip[2]),
			ntohs(packet_headers.ip6h->ip6_src_ip[3]),
			ntohs(packet_headers.ip6h->ip6_src_ip[4]),
			ntohs(packet_headers.ip6h->ip6_src_ip[5]),
			ntohs(packet_headers.ip6h->ip6_src_ip[6]),
			ntohs(packet_headers.ip6h->ip6_src_ip[7]));

		strPrintString.AppendFormat(_T("Destination IP Address:   %4x:%4x:%4x:%4x:%4x:%4x:%4x:%4x\r\n"),
			ntohs(packet_headers.ip6h->ip6_dst_ip[0]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[1]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[2]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[3]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[4]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[5]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[6]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[7]));
	}

	if (packet_headers.tcph)
	{
		strPrintString.AppendFormat(_T("[tcp Protocol]\r\n"));
		strPrintString.AppendFormat(_T("Source Port:   %5d \r\n"), ntohs(packet_headers.tcph->tcp_src_port));

		strPrintString.AppendFormat(_T("Destination Port:   %5d \r\n"), ntohs(packet_headers.tcph->tcp_dst_port));
	}

	if (packet_headers.udph)
	{
		strPrintString.AppendFormat(_T("[udp Protocol]\r\n"));
		strPrintString.AppendFormat(_T("Source Port:   %5d \r\n"), ntohs(packet_headers.udph->udp_src_port));
		strPrintString.AppendFormat(_T("Destination Port:   %5d \r\n"), ntohs(packet_headers.udph->udp_dst_port));
	}

	if (packet_headers.icmph)
	{
		strPrintString.AppendFormat(_T("[icmp Protocol]\r\n"));
		strPrintString.AppendFormat(_T("icmp type:   %d \r\n"), packet_headers.icmph->icmp_type);
		strPrintString.AppendFormat(_T("icmp code:   %d \r\n"), packet_headers.icmph->icmp_code);
	}

	strPrintString += "/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-/\r\n\r\n";
	return 0;
}

int ProtocolAnalyzing(UINT type, CString& strProtocol)
{
	switch (type)
	{
	case ARP:
		strProtocol = "ARP";
		break;

	case IPV4_TCP:
	case IPV6_TCP:
		strProtocol = "TCP";
		break;

	case IPV4_UDP:
	case IPV6_UDP:
		strProtocol = "UDP";
		break;

	case IPV4_TCP_ICMP:
		strProtocol = "ICMP";
		break;

	case IPV6_TCP_ICMP:
		strProtocol = "ICMPv6";
		break;
	}

	return 0;
}

int AddressAnalyzing(hdr_t __in packet_headers, CString& __out strSource, CString& __out strDestination)
{
	switch (packet_headers.type)
	{
	case ARP:
		strSource.Format(_T("%02X:%02X:%02X:%02X:%02X:%02X"),
			packet_headers.eth->eth_src_mac[0],
			packet_headers.eth->eth_src_mac[1],
			packet_headers.eth->eth_src_mac[2],
			packet_headers.eth->eth_src_mac[3],
			packet_headers.eth->eth_src_mac[4],
			packet_headers.eth->eth_src_mac[5]);

		strDestination.Format(_T("%02X:%02X:%02X:%02X:%02X:%02X"),
			packet_headers.eth->eth_dst_mac[0],
			packet_headers.eth->eth_dst_mac[1],
			packet_headers.eth->eth_dst_mac[2],
			packet_headers.eth->eth_dst_mac[3],
			packet_headers.eth->eth_dst_mac[4],
			packet_headers.eth->eth_dst_mac[5]);
		break;

	case IPV4_TCP:
	case IPV4_UDP:
	case IPV4_TCP_ICMP:
		strSource.Format(_T("%0d.%0d.%0d.%0d"),
			packet_headers.ip4h->ip4_src_ip[0],
			packet_headers.ip4h->ip4_src_ip[1],
			packet_headers.ip4h->ip4_src_ip[2],
			packet_headers.ip4h->ip4_src_ip[3]);

		strDestination.Format(_T("%0d.%0d.%0d.%0d"),
			packet_headers.ip4h->ip4_dst_ip[0],
			packet_headers.ip4h->ip4_dst_ip[1],
			packet_headers.ip4h->ip4_dst_ip[2],
			packet_headers.ip4h->ip4_dst_ip[3]);
		break;

	case IPV6_TCP:
	case IPV6_UDP:
	case IPV6_TCP_ICMP:
		strSource.Format(_T("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"),
			ntohs(packet_headers.ip6h->ip6_src_ip[0]),
			ntohs(packet_headers.ip6h->ip6_src_ip[1]),
			ntohs(packet_headers.ip6h->ip6_src_ip[2]),
			ntohs(packet_headers.ip6h->ip6_src_ip[3]),
			ntohs(packet_headers.ip6h->ip6_src_ip[4]),
			ntohs(packet_headers.ip6h->ip6_src_ip[5]),
			ntohs(packet_headers.ip6h->ip6_src_ip[6]),
			ntohs(packet_headers.ip6h->ip6_src_ip[7]));

		strDestination.Format(_T("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"),
			ntohs(packet_headers.ip6h->ip6_dst_ip[0]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[1]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[2]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[3]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[4]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[5]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[6]),
			ntohs(packet_headers.ip6h->ip6_dst_ip[7]));
		break;
	}
	return 0;
}

int InfoAnalzing(hdr_t __in packet_headers, UINT __in packetlen, CString& __out strInfomation)
{
	CString strFlags;

	switch (packet_headers.type)
	{
	case ARP:
		if (ntohs(packet_headers.arph->arp_opcode) == 1 || ntohs(packet_headers.arph->arp_opcode) == 3)
			strInfomation.Format(_T("%0d.%0d.%0d.%0d를 가진 장치? %d.%d.%d.%d으로 수신바람."),
				packet_headers.arph->arp_dst_ip[0],
				packet_headers.arph->arp_dst_ip[1],
				packet_headers.arph->arp_dst_ip[2],
				packet_headers.arph->arp_dst_ip[3],
				packet_headers.arph->arp_src_ip[0],
				packet_headers.arph->arp_src_ip[1],
				packet_headers.arph->arp_src_ip[2],
				packet_headers.arph->arp_src_ip[3]);
		else
			strInfomation.Format(_T("%02X:%02X:%02X:%02X:%02X:%02X가 %0d.%0d.%0d.%0d을 가지고 있음."),
				packet_headers.arph->arp_src_mac[0],
				packet_headers.arph->arp_src_mac[1],
				packet_headers.arph->arp_src_mac[2],
				packet_headers.arph->arp_src_mac[3],
				packet_headers.arph->arp_src_mac[4],
				packet_headers.arph->arp_src_mac[5],
				packet_headers.arph->arp_src_ip[0],
				packet_headers.arph->arp_src_ip[1],
				packet_headers.arph->arp_src_ip[2],
				packet_headers.arph->arp_src_ip[3]);
		break;
	case IPV4_TCP:
		strInfomation.Format(_T("%u -> %u [%s] 시퀀스=%u 승인=%u 윈도우=%u 길이=%u"),
			ntohs(packet_headers.tcph->tcp_src_port),
			ntohs(packet_headers.tcph->tcp_dst_port),
			TCPFlagCheck(packet_headers.tcph->tcp_flags),
			ntohl(packet_headers.tcph->tcp_seq),
			ntohl(packet_headers.tcph->tcp_seq_ack),
			ntohs(packet_headers.tcph->tcp_window),
			packetlen - 74);
	case IPV6_TCP:
		strInfomation.Format(_T("%u -> %u [%s] 시퀀스=%u 승인=%u 윈도우=%u 길이=%u"),
			ntohs(packet_headers.tcph->tcp_src_port),
			ntohs(packet_headers.tcph->tcp_dst_port),
			TCPFlagCheck(packet_headers.tcph->tcp_flags),
			ntohl(packet_headers.tcph->tcp_seq),
			ntohl(packet_headers.tcph->tcp_seq_ack),
			ntohs(packet_headers.tcph->tcp_window),
			packetlen - 54);
		break;
	case IPV4_UDP:
	case IPV6_UDP:
		strInfomation.Format(_T("%u -> %u 길이=%u"),
			ntohs(packet_headers.udph->udp_src_port),
			ntohs(packet_headers.udph->udp_dst_port),
			ntohs(packet_headers.udph->udp_len)-8);
		break;
	case IPV4_TCP_ICMP:
	case IPV6_TCP_ICMP:
		if (packet_headers.icmph->icmp_type == 0)
			strInfomation.Format(_T("에코 응답"));
		else if(packet_headers.icmph->icmp_type == 8)
			strInfomation.Format(_T("에코 요청"));
		else if(packet_headers.icmph->icmp_type == 3)
			strInfomation.Format(_T("도달할 수 없는 목적지"));
		break;
	}
	return 0;
}

CString TCPFlagCheck(bit8_t flags)
{
	CString strFlags;

	if (flags & TCP_FIN)
		strFlags += " FIN ";

	if (flags & TCP_SYN)
		strFlags += " SYN ";

	if (flags & TCP_RST)
		strFlags += " RST ";

	if (flags & TCP_PUSH)
		strFlags += " PSH ";

	if (flags & TCP_ACK)
		strFlags += " ACK ";

	if (flags & TCP_URG)
		strFlags += " URG ";

	if (flags & TCP_ECE)
		strFlags += " ECE ";

	if (flags & TCP_CWR)
		strFlags += " CWR ";

	return strFlags;
}

p_packettype::p_packettype()
	: eth(0), ip4(0), ip6(0), arp(0), icmp(0), tcp(0), udp(0)
{}

p_headers::p_headers()
	: eth(nullptr), ip4h(nullptr), ip6h(nullptr), arph(nullptr), icmph(nullptr), tcph(nullptr), udph(nullptr), type(NULL)
{}