/**
 * source file to implement some tcp functions.
 * @author LIAO I033040007
 * 2015.6.12
 */

#include "mytcp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

extern int glb_rtt_delay;
extern int glb_threshold;
extern int glb_mss;

inline PSegm _get_segm(char *pkg, char *header)
{
  memcpy(header, pkg, HED_LEN);
  return (PSegm)header;
}

inline void _mk_pkg(char *pkg, Segm s)
{
  memcpy(pkg, &s, sizeof(s));
}

/**
 * server listen on port
 * @param port listen port number
 * @return On success, return file descriptor
 *         On error, return -1
 */
int mylisten(short port)
{
  int fd;
  struct sockaddr_in srv;
  struct sockaddr_in cli;
  char pkg[PKG_LEN], header[HED_LEN];
  PSegm pseg;
  Segm seg;
  int cli_len = sizeof(cli);
  srand(time(NULL));
  
  fd=socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) return -1;
  
  srv.sin_family = AF_INET;
  srv.sin_port = htons(port);
  inet_aton("127.0.0.1", &srv.sin_addr); 
  
  if (bind(fd, (struct sockaddr *)&srv, sizeof(srv)) < 0) return -1;
    
  /* set variable */
  printf("==========\n");
  glb_rtt_delay = 200;
  glb_threshold = 65535;
  glb_mss 	= 512;
  printf("Set RTT delay = %d ms\n", glb_rtt_delay);
  printf("Set threshold = %d bytes\n", glb_threshold);
  printf("Set MSS = %d bytes\n", glb_mss);
  printf("Buffer size = %d bytes\n", BUF_SIZE);
  printf("Server's ip is %s\n", inet_ntoa(srv.sin_addr));
  printf("Server is listening on port %hu\n", port);
  
  
  printf("==========\n");
  printf("listening......\n");
  
  /* establish connection */
  
  // wait SYN
  recvfrom(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&cli, &cli_len);
  printf("Received a SYN packet from %s : %hu\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
  
  pseg = _get_segm(pkg, header);
  printf("\t Get a packet (seq = %hu , ack = %hu)\n", pseg->seq, pseg->ack_seq);
  
  if (pseg->syn){
    
    // send SYN/ACK
    seg.dest_port = pseg->src_port;
    seg.src_port = port;
    seg.syn = 1;
    seg.ack = 1;
    seg.ack_seq = pseg->seq + 1;
    seg.seq = (short)(rand() % 10000) + 1;
    
    _mk_pkg(pkg, seg);
    
    if (sendto(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&cli, sizeof(cli))<0) return -1;
    printf("Send a SYN/ACK packet to %s : %hu\n", inet_ntoa(cli.sin_addr), pseg->dest_port);
  }else{
    fprintf(stderr, "segment SYN bit not set! \n");
    return -1;
  }
  
  // wait ACK
  recvfrom(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&cli, &cli_len);
  printf("Received an ACK pakcet from %s : %hu\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
  
  pseg = _get_segm(pkg, header);
  printf("\t Get a packet (seq = %hu , ack = %hu)\n", pseg->seq, pseg->ack_seq);
  
  if (pseg->ack && pseg->ack_seq == seg.seq + 1){
    printf("Complete three way handshake\n");
    printf("==========\n");
    return fd;
  }
  else{
    fprintf(stderr, "not a wanted ACK! \n");
    return -1;
  }
}

/**
 * client connect to server function
 * @param ip server ip
 * @param port server port number
 * @return On success, return file descriptor
 *         On error, return -1
 */
int myconnect(char *srv_ip, short srv_port)
{
  int fd;
  struct sockaddr_in cli;
  struct sockaddr_in srv;
  char pkg[PKG_LEN], header[HED_LEN];
  int srv_len = sizeof(srv);
  PSegm pseg;
  Segm seg;
  srand(time(NULL));
  
  /* create server address structure */
  if (inet_pton(AF_INET, srv_ip, &srv.sin_addr) == 0) return -1;
  srv.sin_family = AF_INET;
  srv.sin_port = htons(srv_port);
  
  /* create client socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) return -1;
  
  memset(&cli, 0, sizeof(cli));
  cli.sin_family = AF_INET;
  cli.sin_addr.s_addr = htonl(INADDR_ANY); // get local host ip
  cli.sin_port = htons(CLI_PORT); // certain client port
  
  if (bind(fd, (struct sockaddr *)&cli, sizeof(cli)) < 0) return -1;
  
  /* establish connection */
  
  //send SYN
  memset(&seg, 0, sizeof(seg));
  seg.src_port = CLI_PORT;
  seg.dest_port = srv_port;
  seg.seq = (short)(rand() % 10000);
  seg.syn = 1;
  
  _mk_pkg(pkg, seg);
  
  if (sendto(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&srv, sizeof(srv))<0) return -1;
  printf("Send a SYN packet to %s : %hu\n", srv_ip, srv_port);
  
  // wait for ack
  recvfrom(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&srv, &srv_len);
  printf("Received a SYN/ACK packet from %s : %hd\n", inet_ntoa(srv.sin_addr), ntohs(srv.sin_port));
  
  pseg = _get_segm(pkg, header);
  printf("\t Get a packet (seq = %hu , ack = %hu)\n", pseg->seq, pseg->ack_seq);

  if (pseg->syn && pseg->ack && pseg->ack_seq == seg.seq + 1){
   
    // send ack
    seg.src_port = CLI_PORT;
    seg.dest_port = srv_port;
    seg.syn = 0;
    seg.ack = 1;
    seg.ack_seq = pseg->seq + 1;
    seg.seq = pseg->ack_seq;
    
    _mk_pkg(pkg, seg);
    
    if (sendto(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&srv, sizeof(srv))<0) return -1;
    
    printf("Send an ACK packet to %s : %hu\n", srv_ip, srv_port);
  }
  else{
    fprintf(stderr, "not a wanted SYN/ACK! \n");
    return -1;
  }
  printf("Complete three way handshake\n");
  printf("==========\n");
  return fd;
}
