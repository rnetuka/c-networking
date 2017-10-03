//
// Created by rnetuka on 28.3.18.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../bbuffer.h"
#include "../ip.h"
#include "../socket.h"

typedef struct
{
    unsigned sequence;
    unsigned acknowledgment;
    int my_port;
    int destination_port;
    IPv4Address *my_ipv4_address;
    IPv6Address *my_ipv6_address;
    IPv4Address *destination_ipv4_address;
    IPv6Address *destination_ipv6_address;
    Socket *socket;
}
TcpConnection;

TcpConnection* tcp_ipv4_connection_open(const IPv4Address *address, int destination_port);
TcpConnection* tcp_ipv6_connection_open(const IPv6Address *address, int destination_port);
void tcp_connection_close(TcpConnection *connection);
void tcp_connection_free(TcpConnection *connection);

ByteBuffer *tcp_connection_send(const TcpConnection *connection, const ByteBuffer *buffer);
ByteBuffer *tcp_connection_send_string(const TcpConnection *connection, const char *data);