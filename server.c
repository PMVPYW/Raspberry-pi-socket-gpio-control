#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
#include "lights.h"
#include "misc.h"

#include "debug.h"
#include "common.h"
#include "server_opt.h"

int bind_udp_server(unsigned short port);
struct sockaddr_in get_addr_for_all_interfaces(unsigned short port);
void handle_request(int sockfd, int *number, int *light_masks);
void apply_state(int number);

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    wiringPiSetupGpio();
    for (int i = 0; i < LIGHTS_NUMBER; i++)
        pinMode(i+1, OUTPUT);
    int light_masks[LIGHTS_NUMBER];
    createMasks(light_masks);
    int number = 0;
    if (DEFAULT_PORT < 1 || DEFAULT_PORT > USHRT_MAX) {
        fprintf(stderr, "Invalid port range. Acepted values: 1 - %hu", USHRT_MAX);
        exit(EXIT_INVALID_ARGS);
    }
    /* Criar e registar socket UDP IPv4 */
    /* Registo local - bind */
    int sock_fd = bind_udp_server(DEFAULT_PORT);
    if (sock_fd == -1) {
        exit(ERR_SYSCALL_FAILURE);
    }
    printf("Listening on port %d\n", DEFAULT_PORT);

    while (1) {
        handle_request(sock_fd, &number, light_masks);
    }
    return 0;
}

void handle_request(int sockfd, int *number, int *light_masks) {
    // UDP IPv4: variáveis auxiliares para sendto() / recvfrom()
	socklen_t client_addr_lenght = sizeof(struct sockaddr_in);
	struct sockaddr_in client_addr;
	ssize_t read_bytes, sent_bytes;
    int operation, pin;
    int response;
    checkLights();

	// UDP IPv4: "recvfrom" do cliente (bloqueante)
	printf("lendo novo estado... \n"); fflush(stdout);
	if ((read_bytes = recvfrom(sockfd, &operation, sizeof(int), 0, (struct sockaddr *) &client_addr, &client_addr_lenght)) == -1)
		ERROR(ERR_SYSCALL_FAILURE, "recvfrom() failed.");

    operation = ntohs(operation);

    response = (operation < 2 && operation > -1);
	// UDP IPv4: "sendto" para o cliente
	printf("enviando confirmação... \n"); fflush(stdout);
	if ((sent_bytes = sendto(sockfd, &response, sizeof(response), 0, (struct sockaddr *) &client_addr, client_addr_lenght)) == -1)
		ERROR(ERR_SYSCALL_FAILURE, "sendto() failed.");
    if (!response) {
        printf("return\n");
        return;
    }

    if ((read_bytes = recvfrom(sockfd, &pin, sizeof(int), 0, (struct sockaddr *) &client_addr, &client_addr_lenght)) == -1)
		ERROR(ERR_SYSCALL_FAILURE, "recvfrom() failed.");

    pin = ntohs(pin);

    switch (operation)
    {
    case 1: //ligar
        turnOn(number, pin, light_masks);
        break;
    case 0:
        turnOff(number, pin, light_masks);
        break;
    
    default:
        break;
    }
    printf("pin: %d\n", pin);
    update(*number, light_masks);
    
}



int bind_udp_server(unsigned short port) {
    //criar socket
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        WARNING("Socket() failed."); 
        return -1;
    }
    //
    struct sockaddr_in addr = get_addr_for_all_interfaces(port);
    //bind
    if (bind(sock_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1)
    {
        WARNING("bind failed");
        close(sock_fd);
        return -1;
    }
    return sock_fd;
    
    }

struct sockaddr_in get_addr_for_all_interfaces(unsigned short port) {
    struct  sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    return addr;    
}
