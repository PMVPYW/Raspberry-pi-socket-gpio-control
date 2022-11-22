#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "memory.h"
#include "debug.h"
#include "common.h"
#include "client_opt.h"


struct sockaddr_in get_server_addr(const char *ip, unsigned short port);
void do_request(int sockfd, const struct sockaddr_in *server_addr) ;


int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;


    if (DEFAULT_PORT < 1 || DEFAULT_PORT > USHRT_MAX) {
        fprintf(stderr, "Invalid port range. Acepted values: 1 - %hu", USHRT_MAX);
        exit(EXIT_INVALID_ARGS);
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        ERROR(ERR_SYSCALL_FAILURE, "sock() failed.");
    }
    /*
     * Inserir codigo do cliente
     */

    struct sockaddr_in server_addr = get_server_addr(DEFAULT_IP, DEFAULT_PORT);
    do_request(sockfd, &server_addr);

    close(sockfd);
    


    return 0;
}


void do_request(int sockfd, const struct sockaddr_in *server_addr) {
    ssize_t read_bytes;
    int number;
    int control;

    do
    {
        printf("on/off [0/1]\n");
        control = scanf("%d", &number);
    } while (control != 1 || number > 1 || number < 0);
    
    number = htons(number);

	// UDP IPv4: "sendto" para o cliente
	if (sendto(sockfd, &number, sizeof(int), 0, (const struct sockaddr *) server_addr, sizeof(struct sockaddr_in)) == -1)
		ERROR(ERR_SYSCALL_FAILURE, "sendto() failed.");
    socklen_t server_addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in from_addr;
	if ((read_bytes = recvfrom(sockfd, &number, sizeof(int), 0, (struct sockaddr *) &from_addr, &server_addr_len)) == -1)
		ERROR(ERR_SYSCALL_FAILURE, "recvfrom() failed.");
        printf("number: %d\n", number);
    if (!number) {
        printf("return\n");
        return;
    }
    
    do
    {
        printf("Pin Number: ");
        control = scanf("%d", &number);
    } while (control != 1);
    number = htons(number);
    if (sendto(sockfd, &number, sizeof(int), 0, (const struct sockaddr *) server_addr, sizeof(struct sockaddr_in)) == -1)
		ERROR(ERR_SYSCALL_FAILURE, "sendto() failed.");
}


struct sockaddr_in get_server_addr(const char *ip, unsigned short port)  {
    //socklen_t server_addr_lenght = sizeof(struct sockaddr_in);	
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;	
	switch (inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr)) {
		case 0:
			fprintf(stderr, "[%s@%d] ERROR - Cannot convert IP address (IPv4): Invalid Network Address\n",
				__FILE__, __LINE__);
			exit(ERR_SYSCALL_FAILURE);
		case -1:
			ERROR(22, "Cannot convert IP address (IPv4)");
	}
	server_addr.sin_port = htons(port);
    return server_addr;
}
