#ifndef MYTCP_H_
#define MYTCP_H_

typedef int BOOL;
#define TRUE  1
#define FALSE 0

typedef struct{
	short src_port;		// 16-bit source port
	short dest_port;	// 16-bit destination port
	int   seq;		// 32-bit sequence number
	int   ack_seq;		// 32-bit acknowledgement number
	short head_len:4,	//  4-bit header length
	      not_use:6,	//  6-bit reserved
	      urg:1,		//  1-bit urgent flag
	      ack:1,		//  1-bit acknowledgement flag
	      psh:1,		//  1-bit push flag
	      rst:1,		//  1-bit reset flag
	      syn:1,		//  1-bit syn flag
	      fin:1;		//  1-bit finish flag
	short rcv_win;		// 16-bit receive window
	short checksum;		// 16-bit checksum
	short urg_ptr;		// 16-bit urgent pointer
	char *data;		// application data
}Segm, *PSegm;

// #define HEAD_LEN(x) (((x).head_len & 0xf0) >> 4) // get segment header length
// #define URG(x) ((x).ctrl_bits & 32)	// get segment control bit - URG
// #define ACK(x) ((x).ctrl_bits & 16)	// get segment control bit - ACK
// #define PSH(x) ((x).ctrl_bits &  8)	// get segment control bit - PSH
// #define RST(x) ((x).ctrl_bits &  4)	// get segment control bit - RST
// #define SYN(x) ((x).ctrl_bits &  2)	// get segment control bit - SYN
// #define FIN(x) ((x).ctrl_bits &  1)	// get segment control bit - FIN
// void header_len(PSegm, short);		// set segment header length
// void set_ctrl_bits(PSegm, short);	// set segment control bits

/* functions for server */
int mylisten(short);			// listen on port


#endif
