#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>      // Necessário para getaddrinfo e struct addrinfo
#include <arpa/inet.h>  // Necessário para inet_ntop
#include <stdint.h>
#include <sys/socket.h> // Para a definição de struct sockaddr

// Definição de funções externas 
void run_applet_by_name(int applet_id);
void get_terminal_width_height(int* width, int* height);
int32_t getIpByMac(int32_t arg);
int32_t sendACK(int32_t arg1, int32_t arg2);
int32_t sendNAK(int32_t arg1, int32_t arg2);
void log_message(int32_t level, int32_t type, int32_t arg1, int32_t arg2);
int32_t getServerConfig();
int32_t callArpping(int32_t arg1, int32_t arg2, int32_t* arg4);
int32_t addLease(int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
int32_t getBlankHostName();
int32_t getBlankChaddr();
int32_t if_nametoindex(const char* name);
void close(int32_t fd);
void msglogd(int32_t level, const char* msg);
void run_script(int32_t scriptId);
char* inet_ntoa(uint32_t addr);
void send_release(const char* addr);

// variáveis globais
static const char* bb_msg_full_version = "BusyBox v1.0.0";

struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    size_t ai_addrlen;
    char *ai_canonname;
    struct sockaddr *ai_addr;
    struct addrinfo *ai_next;
};


int32_t INET6_resolve(const char *hostname);
int32_t br_find_port(int32_t arg1, int32_t arg2);
int32_t check_ip(int32_t arg1, int32_t arg2, int32_t arg4);
int32_t check_and_ack(int32_t arg1, int32_t arg2, int32_t arg3);
int32_t checksum(int32_t arg1, int32_t arg2);
void busybox_main(int32_t arg1, int32_t arg2, int32_t arg3);


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    if (INET6_resolve(argv[1]) < 0) {
        return 1;
    }

    int32_t arg1 = argc;
    int32_t arg2 = (argc > 2) ? atoi(argv[2]) : 0;
    int32_t arg3 = (argc > 3) ? atoi(argv[3]) : 0;

    busybox_main(arg1, arg2, arg3);

    return 0;
}

// Função para resolver um hostname para IPv6
int32_t INET6_resolve(const char *hostname) {
    int32_t status;
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    for (p = result; p != NULL; p = p->ai_next) {
        char ipstr[INET6_ADDRSTRLEN];
        void *addr;

        if (p->ai_family == AF_INET6) {
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(s->sin6_addr);
        } else {
            continue; // Ignora se não for IPv6
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("IPv6 address: %s\n", ipstr);
    }

    freeaddrinfo(result);
    return 0;
}

// Função para encontrar a porta
int32_t br_find_port(int32_t arg1, int32_t arg2) {
    int32_t result = -1;
    int32_t var_10h = if_nametoindex((const char*)arg1);

    if (var_10h != 0) {
        if (arg2 != 0) {
            int32_t portInfo = *(int32_t*)(arg2 + 0x18);
            if (portInfo != 0) {
                result = portInfo;
            }
        }
    }

    return result;
}

// Função para verificar o IP
int32_t check_ip(int32_t arg1, int32_t arg2, int32_t arg4) {
    int32_t server_config = getServerConfig();
    int32_t result = callArpping(arg1 + 0x1C, arg2, &arg4);

    if (result != 0) {
        log_message(5, 2, arg2, result);
        return 1;
    } else {
        result = addLease(arg1, arg2, getBlankHostName(), getBlankChaddr());
        return 0;
    }
}

// Função para verificar e enviar ACK/NAK
int32_t check_and_ack(int32_t arg1, int32_t arg2, int32_t arg3) {
    int32_t server_config = *(int32_t *)(0x7A240);
    int32_t temp_result = getIpByMac(arg1);
    int32_t ack_status;

    if (temp_result == 0) {
        ack_status = sendACK(arg1, arg2);
    } else {
        int32_t reserved_ip = *(int32_t *)(0x47F920);
        ack_status = (reserved_ip - arg3);

        log_message(5, 2, arg2, reserved_ip - 0x5064);

        if (temp_result) {
            ack_status = sendACK(arg1, arg2);
        } else {
            ack_status = sendNAK(arg1, arg2);
        }
    }

    return ack_status;
}

// Função para calcular o checksum
int32_t checksum(int32_t arg1, int32_t arg2) {
    int32_t sum = 0;
    int32_t temp_value;

    if (arg2 < 2) {
        return 0;
    }

    while (arg2 >= 2) {
        temp_value = *((uint16_t *)arg1);
        sum += temp_value;
        arg1 += 2;
        arg2 -= 2;

        if (arg2 >= 2) {
            temp_value = *((uint16_t *)arg1);
            sum += temp_value;
            arg1 += 2;
            arg2 -= 2;
        }
    }

    if (arg2 > 0) {
        sum += *((uint8_t *)arg1);
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum & 0xFFFF;
}

// Função principal do BusyBox
void busybox_main(int32_t arg1, int32_t arg2, int32_t arg3) {
    int32_t s1 = (arg3 == 1) ? 0 : 1;
    int32_t s3 = arg1 - 1;

    if (s3 > 0) {
        int width, height;
        get_terminal_width_height(&width, &height);

        printf("Terminal Width: %d, Height: %d\n", width, height);
        printf("%s\n", bb_msg_full_version);

        if (s1 == 0) {
            run_applet_by_name(arg2);
        }
    } else {
        puts("No applet ID provided.");
        exit(1);
    }
}

// Função de exemplo que simula a execução de um applet
void run_applet_by_name(int applet_id) {
    printf("Running applet with ID: %d\n", applet_id);
}
