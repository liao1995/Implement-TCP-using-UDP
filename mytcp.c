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


Segm _get_segm(char *pkg)
{
  Segm s;
  char head[HED_LEN];
  memcpy(head, pkg, HED_LEN);
  s = (Segm)head;
  return s;
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
  char pkg[PKG_LEN];
  Segm seg;
  int ini_seq;
  int cli_len = sizeof(cli);

  fd=socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) return -1;
  
  srv.sin_family = AF_INET;
  srv.sin_port = htons(port);
  srv.sin_addr.s_addr = htonl(INADDR_ANY); // any of server addresses
  
  if (bind(fd, (struct sockaddr *)&srv, sizeof(srv)) < 0) return -1;
  
  printf("listening......\n");
  /* establish connection */
  // wait syn
  recvfrom(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&cli, &cli_len);
  memcpy(&seg, pkg, HED_LEN);
  if (seg.syn == 1){
    printf("Received a SYN packet from %s : %hu\n", inet_ntoa(cli.sin_addr), seg.src_port);
    printf(" \tGet a packet (seq = %hu , ack = %hu)\n", seg.seq, seg.ack_seq);
    seg.dest_port = seg.src_port;
    seg.src_port = port;
    seg.ack = 1;
    seg.ack_seq = seg.seq + 1;
    srandom(time(NULL));
    ini_seq = (short)(random() % 10000);
    seg.seq = ini_seq;
    memcpy(pkg, &seg, sizeof(seg));
    // send ack/syn
    if (sendto(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&cli, sizeof(cli))<0) return -1;
    printf("Send a SYN/ACK packet to %s : %hu\n", inet_ntoa(cli.sin_addr), seg.dest_port);
  }else
    return -1;
  
  // wait ack
  recvfrom(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&cli, &cli_len);
  memcpy(&seg, pkg, HED_LEN);
  if (seg.ack == 1 && seg.ack_seq == ini_seq + 1){
    printf("Received an ACK pakcet from %s : %hu\n", inet_ntoa(cli.sin_addr), seg.src_port);
    printf("Complete three way handshake\n");
    return fd;
  }
  else 
    return -1;
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
  char pkg[PKG_LEN];
  int srv_len = sizeof(srv);
  int ini_seq;
  Segm seg;
  
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
  memset(&seg, 0, sizeof(seg));
  seg.src_port = CLI_PORT;
  seg.dest_port = srv_port;
  srandom(time(NULL));
  ini_seq = (short)(random() % 10000);
  seg.seq = ini_seq;
  seg.syn = 1;
  memcpy(pkg, &seg, sizeof(seg));
  // send syn
  if (sendto(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&srv, sizeof(srv))<0) return -1;
  printf("Send a SYN packet to %s : %hu\n", srv_ip, srv_port);
  // wait for ack
  recvfrom(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&srv, &srv_len);
  memcpy(&seg, pkg, HED_LEN);
  if (seg.syn == 1 && seg.ack == 1 && seg.ack_seq == ini_seq + 1){
    printf("Received a SYN/ACK packet from %s : %hd\n", inet_ntoa(srv.sin_addr), seg.src_port);
    printf("\t Get a packet (seq = %hu , ack = %hu)\n", seg.seq, seg.ack_seq);
    // send ack
    seg.src_port = CLI_PORT;
    seg.dest_port = srv_port;
    seg.syn = 0;
    seg.ack_seq = seg.seq + 1;
    memcpy(pkg, &seg, sizeof(seg));
    if (sendto(fd, pkg, PKG_LEN, 0, (struct sockaddr*)&srv, sizeof(srv))<0) return -1;
    printf("Send an ACK packet to %s : %hu\n", srv_ip, srv_port);
    printf("Complete three way handshake\n");
    return fd;
  }
  else return -1;
}
