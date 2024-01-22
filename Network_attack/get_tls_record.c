// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/socket.h>
// #include <net/ethernet.h>
// #include <linux/if_packet.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/ioctl.h>
// #include <net/if.h>
// #include <sys/types.h>
// #include <linux/if_arp.h>
// #include <linux/if_ether.h>

// #include <netinet/ip.h>
// #include <net/ethernet.h>
// #include <linux/tcp.h>
// #include <netinet/ip_icmp.h>


// #define MAX_PACKET_SIZE 65536


// typedef struct __attribute__((packed)) {
// 	uint8_t content_type;  // 0x16 for TLS handshake
// 	uint16_t version;
// 	uint16_t length;
// } TLSRecord;

// typedef struct __attribute__((packed)) {
//     uint8_t handshake_type;  // 0x01 for Client Hello
//     uint16_t length;
//     uint16_t version;
//     uint8_t random[32];  // Random data
//     uint8_t session_id_length;
//     uint8_t session_id[32];  // Session ID (variable length)
//     uint8_t cipher_suites_length;
//     uint16_t cipher_suites[32];  // Supported cipher suites (variable length)
//     uint8_t compression_methods_length;
//     uint8_t compression_methods[32];  // Supported compression methods (variable length)
//     uint16_t extensions_length;
//     uint8_t extensions[32];  // Extensions (variable length)
// } ClientHello;

// typedef struct __attribute__((packed)) {
//     uint16_t type;  // 0x0000 for SNI   server name
//     uint16_t length;
//     uint16_t server_name_list_length;
//     uint8_t server_name_type;  // 0x00 for host_name
//     uint16_t server_name_length;
//     uint8_t server_name[256];  // Server name (variable length)
// } SNIExtension;



// void get_tls_header(unsigned char* buffer, int size) {
//     // Skip the Ip and TCP headers
//     struct iphdr *iph = (struct iphdr*)(buffer);
//     unsigned short iphdrlen = iph->ihl*4;
//     struct tcphdr *tcph=(struct tcphdr*)(buffer + iphdrlen);
//     unsigned short tcphdrlen = tcph->doff*4;

//     TLSRecord* tls_header = (TLSRecord*)(buffer + iphdrlen + tcphdrlen);
//     printf("TLS Header:\n");
//     printf("| - Content Type: 0x%02X\n", tls_header->content_type);
//     printf("| - Version: 0x%04X\n", ntohs(tls_header->version));
//     printf("| - Length: %d\n", ntohs(tls_header->length));
// }

// int main(int argc, char **argv) {

//     // Creates a raw socket, that accepts packets.
//     int r_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
//     if (r_sock == -1){
//         perror("Socket creation failed.\n");
//         exit(1);
//     }

//     //Create a buffer to accept incomming packets.
//     int packet_size;
//     unsigned char *buffer = (unsigned char *)malloc(65536);

//     // Loop & Accept packets:
//     while(1) {
//         packet_size = recvfrom(r_sock, buffer, 65536, 0 ,NULL, NULL);
//         if (packet_size == -1) {
//         printf("Failed to get packets\n");
//         return 1;
//         }
//         // Extract data from each packet.
//         int pointer = sizeof(struct ethhdr);
//         // extract_sni(buffer, pointer);
//         get_tls_header(buffer, packet_size);
        
//     }
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define MAX_PACKET_SIZE 65536

typedef struct __attribute__((packed)) {
    uint8_t content_type;  // 0x16 for TLS handshake
    uint16_t version;
    uint16_t length;
} TLSRecord;

void get_tls_record(unsigned char* buffer, int size) {
    // Skip the IP and TCP headers
    struct iphdr *iph = (struct iphdr *)buffer;
    unsigned short iphdrlen = iph->ihl*4;
    struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen);
    unsigned short tcphdrlen = tcph->doff*4;

    // Check if the packet is long enough to contain a TLS record
    if (size < iphdrlen + tcphdrlen + sizeof(TLSRecord)) {
        printf("Packet is too short for a TLS record\n");
        return;
    }

    // Get the TLS record
    TLSRecord *tls = (TLSRecord*)(buffer + iphdrlen + tcphdrlen);
    printf("| - Content type: 0x%02X\n", tls->content_type);
    printf("| - Version: 0x%04X\n", ntohs(tls->version));
    printf("| - Length: %d\n", ntohs(tls->length));
    if(tls->content_type == 0x16){
        printf("TLS handshake\n");
    }
}

void get_clienthello_packet(unsigned char *buffer, int size){
    // Skip the IP and TCP headers
    struct iphdr *iph = (struct iphdr *)buffer;
    unsigned short iphdrlen = iph->ihl*4;
    struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen);
    unsigned short tcphdrlen = tcph->doff*4;

    // Check if the packet is long enough to contain a TLS record
    if (size < iphdrlen + tcphdrlen + sizeof(TLSRecord)) {
        printf("Packet is too short for a TLS record\n");
        return;
    }

    // Get the TLS record
    TLSRecord *tls = (TLSRecord*)(buffer + iphdrlen + tcphdrlen);
    
    printf("| - Content type: 0x%02X\n", tls->content_type);
    printf("| - Version: 0x%04X\n", ntohs(tls->version));
    printf("| - Length: %d\n", ntohs(tls->length));
    if(tls->content_type == 0x16){
        printf("| --> TLS handshake\n");
    }
    // Get the Client Hello
    ClientHello *client_hello = (ClientHello*)(buffer + iphdrlen + tcphdrlen + sizeof(TLSRecord));
    printf("| - Handshake type: 0x%02X\n", client_hello->handshake_type);
    printf("| - Length: %d\n", ntohs(client_hello->length));
    printf("| - Version: 0x%04X\n", ntohs(client_hello->version));
    printf("| - Random: ");
    for (int i = 0; i < 32; i++) {
        printf("%02X", client_hello->random[i]);
    }
    printf("\n");
    printf("| - Session ID length: %d\n", client_hello->session_id_length);
    printf("| - Cipher suites length: %d\n", client_hello->cipher_suites_length);
    printf("| - Compression methods length: %d\n", client_hello->compression_methods_length);
    printf("| - Extensions length: %d\n", ntohs(client_hello->extensions_length));
    printf("| - Extensions: ");
    // for (int i = 0; i < ntohs(client_hello->extensions_length); i++) {
    //     printf("%02X", client_hello->extensions[i]);
    // }
    printf("\n");
}

int main() {
    int sockfd;
    char buffer[MAX_PACKET_SIZE];

    // Create a raw socket
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) {
        perror("Could not create socket");
        return 1;
    }

    while (1) {
        socklen_t saddr_size = sizeof(struct sockaddr);
        struct sockaddr saddr;

        // Receive a packet
        int data_size = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, &saddr, &saddr_size);
        if (data_size == -1) {
            perror("Could not receive packet");
            return 1;
        }

        // Process the packet
        get_tls_record(buffer, data_size);
    }

    return 0;
}
