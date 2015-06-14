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

/* functions for server */
int mylisten(short);			// listen on port


/* functions for client */
int myconnect()

#endif
