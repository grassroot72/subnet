/*
 * calculate subnet mask stuff from a given ip address and CIDR bits
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8_t;
typedef unsigned int u32_t;

int main(int ac, char* av[])
{
  /*
   *              ip[0]    ip[1]    ip[2]    ip[3]
   *
   * binary ex.:  11111111 11111111 11111111 11000000
   *
   * decimal ex.: 255      255      255      192
   *
   */
  u32_t ip32;         /* for bitwise operations */
  u32_t mask32;       /* for bitwise operations */
  u32_t subnet32;     /* for bitwise operations */
  u32_t broadcast32;  /* for bitwise operations */
  u32_t e_ip32;       /* for bitwise operations */
  u8_t ip[4];         /* ip address */
  u8_t mask[4];       /* subnet mask */
  u8_t subnet[4];     /* subnet (ip) */
  u8_t broadcast[4];  /* broadcast (ip) */
  u8_t s_ip[4];       /* ip range start */
  u8_t e_ip[4];       /* ip range end */

  /*
   *  cidr and subnet mask
   *
   *  xxx.xxx.xxx.xxx/xx
   *                  ^
   *                  cidr
   */
  int cidr;
  int num_subnet;     /* number of subnets */
  int num_ip_subnet;  /* number of ip addresses per subnet */
  int subnet_id;
  int host_id;

  char* token;        /* for strtok() */
  int i;

  if (ac < 3) {
    printf("usage: subnet <ip> <cidr>\n");
    return 0;
  }

  /* avoid some exceptions */
  cidr = atoi(av[2]);
  if (cidr > 32) {
    printf("cidr should not be greater than 32!\n");
    return 0;
  }

  if (cidr < 8) {
    printf("cidr should not be less than 8!\n");
    return 0;
  }

  /* get the token */
  token = strtok(av[1], ".");
  i = 0;
  /* walk through other tokens */
  while (token != NULL) {
    ip[i] = (u8_t) atoi(token);
    i++;
    token = strtok(NULL, ".");
  }

  /* retrieve ip address */
  ip32  = ip[0] << 24;
  ip32 |= ip[1] << 16;
  ip32 |= ip[2] << 8;
  ip32 |= ip[3];

  mask32 = 0xffffffff << (32 - cidr);
  mask[0] = (mask32 & 0xff000000) >> 24;
  mask[1] = (mask32 & 0x00ff0000) >> 16;
  mask[2] = (mask32 & 0x0000ff00) >> 8;
  mask[3] = (mask32 & 0x000000ff);

  subnet32 = ip32 & mask32;
  subnet[0] = (subnet32 & 0xff000000) >> 24;
  subnet[1] = (subnet32 & 0x00ff0000) >> 16;
  subnet[2] = (subnet32 & 0x0000ff00) >> 8;
  subnet[3] = (subnet32 & 0x000000ff);

  broadcast32 = subnet32 | ~mask32;
  broadcast[0] = (broadcast32 & 0xff000000) >> 24;
  broadcast[1] = (broadcast32 & 0x00ff0000) >> 16;
  broadcast[2] = (broadcast32 & 0x0000ff00) >> 8;
  broadcast[3] = (broadcast32 & 0x000000ff);

  memcpy(s_ip, subnet, sizeof(u8_t)*4);
  s_ip[3] += 1;

  e_ip32 = broadcast32 - 1;
  e_ip[0] = (e_ip32 & 0xff000000) >> 24;
  e_ip[1] = (e_ip32 & 0x00ff0000) >> 16;
  e_ip[2] = (e_ip32 & 0x0000ff00) >> 8;
  e_ip[3] = (e_ip32 & 0x000000ff);

  num_ip_subnet = ~mask32 + 1;
  host_id = ip32 ^ subnet32;

  if (host_id == 0) {
    printf("do not use subnet ip!\n");
    return 0;
  }

  if (ip32 == broadcast32) {
    printf("do not use broadcast ip!\n");
    return 0;
  }

  /* initialize */
  num_subnet = 0;
  subnet_id = 0;

  if (cidr >= 24) {
    /*
     *  255.255.255.xxx
     */
    num_subnet = ((0x000000ff ^ ~mask32) >> (32 - cidr)) + 1;
    subnet_id = ip[3] / (256/num_subnet);
  }

  if (cidr < 24 && cidr >= 16) {
    /*
     *  255.255.xxx.xxx
     */
    num_subnet = ((0x0000ffff ^ ~mask32) >> (32 - cidr)) + 1;
    subnet_id = ip[2] / (256/num_subnet);
  }

  if (cidr < 16 && cidr >= 8) {
    /*
     *  255.xxx.xxx.xxx
     */
    num_subnet = ((0x00ffffff ^ ~mask32) >> (32 - cidr)) + 1;
    subnet_id = ip[1] / (256/num_subnet);
  }

  /*
   * show the results
   */
  printf("ip address: %d.%d.%d.%d\n",
         ip[0], ip[1], ip[2], ip[3]);
  printf("------------------------------\n");

  printf("number of subnets: %d\n", num_subnet);
  printf("subnet mask: %d.%d.%d.%d\n",
         mask[0], mask[1], mask[2], mask[3]);
  printf("subnet: %d.%d.%d.%d\n",
         subnet[0], subnet[1], subnet[2], subnet[3]);

  printf("subnet id: %d\n", subnet_id + 1);
  printf("host id: %d\n", host_id);
  printf("valid ip addresses per subnet: %d\n", num_ip_subnet - 2);
  printf("valid ip range: [%d.%d.%d.%d - %d.%d.%d.%d]\n",
         s_ip[0], s_ip[1], s_ip[2], s_ip[3],
         e_ip[0], e_ip[1], e_ip[2], e_ip[3]);
  printf("broadcast ip: %d.%d.%d.%d\n",
         broadcast[0], broadcast[1], broadcast[2], broadcast[3]);

  return 0;
}




