#pragma once

typedef unsigned char			bit8_t;
typedef unsigned short			bit16_t;
typedef unsigned int			bit32_t;
typedef unsigned long long int	bit64_t;

/*
*	이더넷 헤더
*/

#define ETH_ADDR_LEN	6
#define ETH_HDR_LEN		14

typedef struct ethernet_headers {
	bit8_t	eth_dst_mac[ETH_ADDR_LEN];	// 도착지 MAC
	bit8_t	eth_src_mac[ETH_ADDR_LEN];	// 출발지 MAC
	bit16_t eth_type;					// 패킷 타입 
}eth_t;

/*
*	IP 헤더
*/

#define IP4_ADDR_LEN	4
#define IP6_ADDR_LEN	8

typedef struct ipv4_headers {
	bit8_t	ip4_hdr_len : 4;			// 헤더 길이
	bit8_t	ip4_ver : 4;				// 버전
	bit8_t	ip4_tos;					// 서비스 타입
	bit16_t ip4_tot_len;				// 전체 길이
	bit16_t ip4_id;						// 확인 숫자
	bit16_t ip4_frag_off;				// 플래그, 분할 오프셋
	bit8_t	ip4_ttl;					// 생존 시간
	bit8_t	ip4_protocol;				// 프로토콜 타입
	bit16_t ip4_checksum;				// 체크섬
	bit8_t	ip4_src_ip[IP4_ADDR_LEN];	// 출발지 ip
	bit8_t	ip4_dst_ip[IP4_ADDR_LEN];	// 도착지 ip
}ipv4h_t;

typedef struct ipv6_headers {
	bit32_t ip6_flow;					// 4 bits 버전, 8 bits 트래픽 클래스, 20 bits 플로우 라벨
	bit16_t ip6_pay_len;				// 페이로드 길이
	bit8_t	ip6_next;					// 다음 헤더
	bit8_t	ip6_hop_limit;				// ipv6 홉 한계(생존 시간)
	bit16_t	ip6_src_ip[IP6_ADDR_LEN];	// 출발지 ipv6
	bit16_t	ip6_dst_ip[IP6_ADDR_LEN];	// 도착지 ipv6
}ipv6h_t;

/*
*	ARP 헤더
*/

#define ARP_MAC_LEN		6
#define ARP_ADDR_LEN	4

typedef struct arp_headers {
	bit16_t arp_hard_type;				// 하드웨어 타입
	bit16_t arp_protocol;				// 프로토콜
	bit8_t	arp_addr_len;				// MAC 주소 길이
	bit8_t	arp_protocol_len;			// 프로토콜 길이
	bit16_t arp_opcode;					// 명령코드
	bit8_t	arp_src_mac[ARP_MAC_LEN];	// 출발지 MAC
	bit8_t	arp_src_ip[ARP_ADDR_LEN];	// 출발지 IP
	bit8_t	arp_dst_mac[ARP_MAC_LEN];	// 도착지 MAC
	bit8_t	arp_dst_ip[ARP_ADDR_LEN];	// 도착지 IP
}arph_t;

/*
*	ICMP 헤더
*/

typedef struct icmp_headers {
	bit8_t	icmp_type;		// ICMP 타입
	bit8_t	icmp_code;		// 코드
	bit16_t icmp_checksum;	// 체크섬
}icmph_t;

/*
*	TCP, UDP 헤더
*/

typedef struct tcp_headers {
	bit16_t tcp_src_port;	// 출발지 포트
	bit16_t tcp_dst_port;	// 도착지 포트
	bit32_t tcp_seq;		// 순서
	bit32_t tcp_seq_ack;	// 승인
	bit8_t	tcp_rsvd : 4;	// 예약
	bit8_t	tcp_hlen : 4;	// 헤더 길이
	bit8_t	tcp_flags;		// 플래그
#define TCP_FIN		0X01
#define TCP_SYN		0x02
#define TCP_RST		0x04
#define TCP_PUSH	0x08
#define TCP_ACK		0x10
#define TCP_URG		0x20
#define TCP_ECE		0x40
#define TCP_CWR		0x80
	bit16_t tcp_window;		// 윈도우 사이즈
	bit16_t tcp_checksum;	// 체크섬
	bit16_t tcp_urgptr;		// 긴급포인터
}tcph_t;

typedef struct udp_headers {
	bit16_t udp_src_port;	// 출발지 포트
	bit16_t udp_dst_port;	// 도착지 포트
	bit16_t udp_len;		// 길이
	bit16_t udp_checksum;	// UDP 체크섬
}udph_t;

/*
*   자동 패킷 해석
*/

typedef struct p_headers {
	const eth_t*	eth;
	const ipv4h_t*	ip4h;
	const ipv6h_t*	ip6h;
	const arph_t*	arph;
	const icmph_t*	icmph;
	const tcph_t*	tcph;
	const udph_t*	udph;
}hdr_t;

hdr_t PacketAnalyzing(const bit8_t* packet_data);
int PrintPacketData(hdr_t packet_headers, CString& strPrintString);