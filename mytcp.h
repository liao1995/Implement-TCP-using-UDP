#ifndef MYTCP_H_
#define MYTCP_H_

#include <arpa/inet.h>

#define SRV_PORT 10220
#define CLI_PORT 10219
#define HED_LEN  32
#define PKG_LEN  544
#define BUF_SIZE 10240

typedef int boolean;

enum role{unknown, client, server};

#define TRUE  1
#define FALSE 0

/* tcp segment structure */
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
	int   padding;
	char *data;		// application data
}Segm, *PSegm;


/* global variable */
int glb_rtt_delay;		// RTT delay. Unit: millisecond
int glb_threshold;		// Threshold. Unit: byte
int glb_mss;			// MSS. Unit byte

enum role myrole;		// My Role: Client or Server
struct sockaddr_in glb_srv;	// server socket address
struct sockaddr_in glb_cli;	// client socket address
char pkg[PKG_LEN];		// package
char header[HED_LEN];		// package header
Segm seg;			// segment structure variable
PSegm pseg;			// pointer to a segment

int myclose(int);		// active terminate a tcp connection
int mysend(int, char *,int);	// send data function
int myreceive(int);		// receive data function
boolean isfin(int);

/* functions for server */
int mylisten(short);		// listen on port

/* functions for client */
int myconnect(char *, short);	// connect to server


#endif
