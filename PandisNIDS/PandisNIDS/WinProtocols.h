#pragma once

typedef unsigned char			bit8_t;
typedef unsigned short			bit16_t;
typedef unsigned int			bit32_t;
typedef unsigned long long int	bit64_t;

/*
*	��Ŷ Ÿ�� ��ũ��
*/
#define ARP				1
#define IPV4_TCP		2
#define IPV4_UDP		3
#define IPV4_TCP_ICMP	4
#define IPV6_TCP		5
#define IPV6_UDP		6
#define IPV6_TCP_ICMP	7

/*
*  ��Ŷ ��� ���� (���� ����)
*/
#define ETH_HEADER_LEN 22
#define IPV6_HEADER_LEN 40
#define UDP_HEADER_LEN 8

/*
*	�̴��� ���
*/

#define ETH_ADDR_LEN	6
#define ETH_HDR_LEN		14

typedef struct ethernet_headers {
	bit8_t	eth_dst_mac[ETH_ADDR_LEN];	// ������ MAC
	bit8_t	eth_src_mac[ETH_ADDR_LEN];	// ����� MAC
	bit16_t eth_type;					// ��Ŷ Ÿ�� 
}eth_t;

/*
*	IP ���
*/

#define IP4_ADDR_LEN	4
#define IP6_ADDR_LEN	8

typedef struct ipv4_headers {
	bit8_t	ip4_hdr_len : 4;			// ��� ����
	bit8_t	ip4_ver : 4;				// ����
	bit8_t	ip4_tos;					// ���� Ÿ��
	bit16_t ip4_tot_len;				// ��ü ����
	bit16_t ip4_id;						// Ȯ�� ����
	bit16_t ip4_frag_off;				// �÷���, ���� ������
	bit8_t	ip4_ttl;					// ���� �ð�
	bit8_t	ip4_protocol;				// �������� Ÿ��
	bit16_t ip4_checksum;				// üũ��
	bit8_t	ip4_src_ip[IP4_ADDR_LEN];	// ����� ip
	bit8_t	ip4_dst_ip[IP4_ADDR_LEN];	// ������ ip
}ipv4h_t;

typedef struct ipv6_headers {
	bit32_t ip6_flow;					// 4 bits ����, 8 bits Ʈ���� Ŭ����, 20 bits �÷ο� ��
	bit16_t ip6_pay_len;				// ���̷ε� ����
	bit8_t	ip6_next;					// ���� ���
	bit8_t	ip6_hop_limit;				// ipv6 ȩ �Ѱ�(���� �ð�)
	bit16_t	ip6_src_ip[IP6_ADDR_LEN];	// ����� ipv6
	bit16_t	ip6_dst_ip[IP6_ADDR_LEN];	// ������ ipv6
}ipv6h_t;

/*
*	ARP ���
*/

#define ARP_MAC_LEN		6
#define ARP_ADDR_LEN	4

typedef struct arp_headers {
	bit16_t arp_hard_type;				// �ϵ���� Ÿ��
	bit16_t arp_protocol;				// ��������
	bit8_t	arp_addr_len;				// MAC �ּ� ����
	bit8_t	arp_protocol_len;			// �������� ����
	bit16_t arp_opcode;					// �����ڵ�
	bit8_t	arp_src_mac[ARP_MAC_LEN];	// ����� MAC
	bit8_t	arp_src_ip[ARP_ADDR_LEN];	// ����� IP
	bit8_t	arp_dst_mac[ARP_MAC_LEN];	// ������ MAC
	bit8_t	arp_dst_ip[ARP_ADDR_LEN];	// ������ IP
}arph_t;

/*
*	ICMP ���
*/

typedef struct icmp_headers {
	bit8_t	icmp_type;		// ICMP Ÿ��
	bit8_t	icmp_code;		// �ڵ�
	bit16_t icmp_checksum;	// üũ��
}icmph_t;

/*
*	TCP, UDP ���
*/

typedef struct tcp_headers {
	bit16_t tcp_src_port;	// ����� ��Ʈ
	bit16_t tcp_dst_port;	// ������ ��Ʈ
	bit32_t tcp_seq;		// ����
	bit32_t tcp_seq_ack;	// ����
	bit8_t	tcp_rsvd : 4;	// ����
	bit8_t	tcp_hlen : 4;	// ��� ����
	bit8_t	tcp_flags;		// �÷���
#define TCP_FIN		0X01
#define TCP_SYN		0x02
#define TCP_RST		0x04
#define TCP_PUSH	0x08
#define TCP_ACK		0x10
#define TCP_URG		0x20
#define TCP_ECE		0x40
#define TCP_CWR		0x80
	bit16_t tcp_window;		// ������ ������
	bit16_t tcp_checksum;	// üũ��
	bit16_t tcp_urgptr;		// ���������
}tcph_t;

typedef struct udp_headers {
	bit16_t udp_src_port;	// ����� ��Ʈ
	bit16_t udp_dst_port;	// ������ ��Ʈ
	bit16_t udp_len;		// ����
	bit16_t udp_checksum;	// UDP üũ��
}udph_t;

/*
*   �ڵ� ��Ŷ �ؼ�
*/

typedef class p_packettype{
public:
	p_packettype();
	UINT eth;
	UINT ip4;
	UINT ip6;
	UINT arp;
	UINT icmp;
	UINT tcp;
	UINT udp;
}Pt_t;

typedef class p_headers {
public:
	p_headers();
	const eth_t*	eth;
	const ipv4h_t*	ip4h;
	const ipv6h_t*	ip6h;
	const arph_t*	arph;
	const icmph_t*	icmph;
	const tcph_t*	tcph;
	const udph_t*	udph;
	UINT			type;
}hdr_t;

hdr_t __out PacketAnalyzing(const bit8_t* __in packet_data, Pt_t& __out packet_count);
int PrintPacketData(hdr_t __in packet_headers, CString& __out strPrintString);
int __out FindStringPacketData(const struct pcap_pkthdr* __in header, const bit8_t* __in packet_data, std::vector<CString> __in find_strings);