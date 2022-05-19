#include "stdafx.h"

hdr_t PacketAnalyzing(const bit8_t* packet_data)
{
	hdr_t headers;
	headers.eth = (eth_t*)packet_data;
	headers.ip4h = NULL;
	headers.ip6h = NULL;
	headers.arph = NULL;
	headers.icmph = NULL;
	headers.tcph = NULL;
	headers.udph = NULL;

	switch (ntohs(headers.eth->eth_type))
	{
	case 0x0800:
		headers.ip4h = (ipv4h_t*)(packet_data + sizeof(eth_t));
		switch (headers.ip4h->ip4_protocol)
		{
		case 6:		// TCP
			headers.tcph = (tcph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			break;
		case 17:	// UDP
			headers.udph = (udph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			break;
		case 1:		// ICMP
			headers.icmph = (icmph_t*)(packet_data + sizeof(eth_t) + headers.ip4h->ip4_hdr_len * 4);
			break;
		}
		break;

	case 0x86DD:
		headers.ip6h = (ipv6h_t*)(packet_data + sizeof(eth_t));
		switch (headers.ip6h->ip6_next)
		{
		case 6:		// TCP
			headers.tcph = (tcph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			break;
		case 17:	// UDP
			headers.udph = (udph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			break;
		case 58:	// ICMPv6
			headers.icmph = (icmph_t*)(packet_data + sizeof(eth_t) + sizeof(ipv6h_t));
			break;
		}
		break;

	case 0x0806:
		headers.arph = (arph_t*)(packet_data + sizeof(eth_t));
		break;
	}

	return headers;
}

int PrintPacketData(hdr_t packet_headers, CString& strPrintString)
{
	strPrintString = "/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-/\r\n";

	if (packet_headers.eth)
	{
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

	}

	if (packet_headers.ip4h)
	{

	}

	if (packet_headers.ip6h)
	{

	}

	if (packet_headers.tcph)
	{

	}

	if (packet_headers.udph)
	{

	}

	if (packet_headers.icmph)
	{

	}

	strPrintString += "/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-/\r\n";
	return 0;
}