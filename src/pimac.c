#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
        int sock = socket(PF_INET, SOCK_DGRAM, 0);
        struct ifreq req;
        int i = 0;

        if (argc < 2) {
                fprintf(stderr, "Usage: %s <interfacename>\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        if (sock < 0) {
                perror("socket");
                exit(EXIT_FAILURE);
        }

        memset(&req, 0, sizeof(req));
        strncpy(req.ifr_name, argv[1], IF_NAMESIZE - 1);

        if (ioctl(sock, SIOCGIFHWADDR, &req) < 0) {
                perror("ioctl");
                exit(EXIT_FAILURE);
        }

        // flip the 2nd-least-significant bit of the most-significant byte to switch it from
	// universal to local per:
	// https://serverfault.com/questions/40712/what-range-of-mac-addresses-can-i-safely-use-for-my-virtual-machines
        for(i=0;i<6;i++) {
                printf("%.2X", (unsigned char) (req.ifr_hwaddr.sa_data[i] | ((i == 0) ? 0x2 : 0x0))) ;
                if (i < 5)
                        printf(" ");
        }

        close(sock);

        exit(EXIT_SUCCESS);
        return 0;
}
