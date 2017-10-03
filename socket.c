//
// Created by rnetuka on 28.3.18.
//

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <stdio.h>

#include "os.h"
#include "socket.h"

Socket *socket_new(void)
{
    Socket *s = malloc(sizeof(Socket));
    s->file_descriptor = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    s->connected = false;
    s->interface_name = active_net_interface()->name;

    if (s->file_descriptor < 0)
    {
        free(s);
        return NULL;
    }

    return s;
}

void socket_free(Socket *socket)
{
    free(socket);
}

void socket_write(const Socket *socket, uint8_t *data, size_t length)
{
    struct sockaddr_in dst;
    struct in_addr inaddr = { .s_addr = 3627733411 };
    dst.sin_addr = inaddr;
    dst.sin_family = AF_INET;

    struct ifreq if_idx;
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, socket->interface_name, IFNAMSIZ-1);

    if (ioctl(socket->file_descriptor, SIOCGIFINDEX, &if_idx) < 0)
        return;

    struct ifreq ifopts;
    ioctl(socket->file_descriptor, SIOCGIFFLAGS, &ifopts);
    ifopts.ifr_flags |= IFF_PROMISC;
    ioctl(socket->file_descriptor, SIOCSIFFLAGS, &ifopts);

    struct sockaddr_ll socket_address;
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    socket_address.sll_halen = ETH_ALEN;

    if (sendto(socket->file_descriptor,
               data,
               length,
               0,
               (struct sockaddr *) &socket_address,
               sizeof(struct sockaddr_ll)) < 0)
    {
        close(socket->file_descriptor);
        return;
    }
}

void socket_write_buf(const Socket *socket, const ByteBuffer *buffer)
{
    printf("Sending:\n");
    byte_buffer_print(buffer);
    printf("\n\n");
    socket_write(socket, buffer->data, buffer->size);
}

EthernetFrame *socket_read(const Socket *socket)
{
    size_t buffer_size = 2000;
    uint8_t buffer[buffer_size];

    struct sockaddr_ll socket_address;
    socket_address.sll_halen = ETH_ALEN;
    socklen_t addr_len = sizeof(socket_address);

    struct sockaddr_in sender_address;
    int sender_address_length;

    long int bytes_read = recvfrom(
            socket->file_descriptor,
            buffer,
            buffer_size,
            0,
            (struct sockaddr *) &sender_address,
            &sender_address_length
    );

    if (bytes_read == 0)
        fprintf(stderr, "Connection closed by the server");

    if (bytes_read < 0)
        fprintf(stderr, strerror(errno));

    return ethernet_frame_parse(buffer, bytes_read);
}