#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

void get_interface_by_ip(const char* ip_address) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;  

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((strcmp(ifa->ifa_name,"lo") !=0 ) && (ifa->ifa_addr->sa_family==AF_INET)) {
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            if(strcmp(host, ip_address) == 0) {
                printf("Interface : <%s>\n",ifa->ifa_name );
            }
        }
    }

    freeifaddrs(ifaddr);
}

int main() {
    get_interface_by_ip("192.168.1.1");
    return 0;
}







import netifaces

def get_interface_by_ip(ip_address):
    for interface in netifaces.interfaces():
        for link in netifaces.ifaddresses(interface).get(netifaces.AF_INET, ()):
            if link['addr'] == ip_address:
                return interface
    return None

  interface = get_interface_by_ip('192.168.1.1')
if interface is not None:
    print(f'The interface for IP 192.168.1.1 is {interface}')
else:
    print('No interface found for IP 192.168.1.1')

pip install netifaces
