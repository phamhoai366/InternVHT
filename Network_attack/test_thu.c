#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SIZE_ETHERNET 14
#define SIZE_TCP 20

struct sniff_ip {
    u_char ip_vhl;
    u_char ip_tos;
    u_short ip_len;
    u_short ip_id;
    u_short ip_off;
    u_char ip_ttl;
    u_char ip_p;
    u_short ip_sum;
    struct in_addr ip_src, ip_dst;
};

struct sniff_tcp {
    u_short th_sport;
    u_short th_dport;
    u_char th_offx2;
    u_char th_flags;
    u_short th_win;
    u_short th_sum;
    u_short th_urp;
};

void parse_sni(const u_char *packet, int ext_length) {
    int pos = 0;
    while (pos < ext_length) {
        int type = (packet[pos] << 8) + packet[pos + 1];
        int length = (packet[pos + 2] << 8) + packet[pos + 3];

        if (type == 0x00) { // server_name extension
            int server_name_length = (packet[pos + 5] << 8) + packet[pos + 6];
            printf("SNI: %.*s\n", server_name_length, packet + pos + 7);
            break;
        }

        pos += 4 + length;
    }
}

int main() {
    int s;
    struct sockaddr_in source, dest;
    unsigned char *buffer = (unsigned char *)malloc(65536);

    s = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    while(1) {
        socklen_t saddr_size = sizeof source;
        int data_size = recvfrom(s, buffer, 65536, 0, &source, &saddr_size);
        if(data_size <0 ) {
            printf("Recvfrom error , failed to get packets\n");
            return 1;
        }

        struct sniff_ip *iph = (struct sniff_ip *)(buffer + SIZE_ETHERNET);
        struct sniff_tcp *tcph=(struct sniff_tcp *)(buffer + SIZE_ETHERNET + iph->ip_vhl*4);

        // Check if the packet is a TLS Client Hello
        if (ntohs(tcph->th_dport) == 443 && buffer[SIZE_ETHERNET + iph->ip_vhl*4 + ((tcph->th_offx2 & 0xf0) >> 4)*4] == 0x16) {
            // Parse the Client Hello
            int handshake_length = (buffer[SIZE_ETHERNET + iph->ip_vhl*4 + ((tcph->th_offx2 & 0xf0) >> 4)*4 + 1] << 16) +
                                   (buffer[SIZE_ETHERNET + iph->ip_vhl*4 + ((tcph->th_offx2 & 0xf0) >> 4)*4 + 2] << 8) +
                                    buffer[SIZE_ETHERNET + iph->ip_vhl*4 + ((tcph->th_offx2 & 0xf0) >> 4)*4 + 3];
            int extensions_start = SIZE_ETHERNET + iph->ip_vhl*4 + ((tcph->th_offx2 & 0xf0) >> 4)*4 + 44;
            int extensions_length = handshake_length - 38;

            parse_sni(buffer + extensions_start, extensions_length);
        }
    }

    close(s);
    return 0;
}
