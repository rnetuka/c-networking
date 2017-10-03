//
// Created by rnetuka on 28.3.18.
//

#include <stdio.h>

#include <stdlib.h>
#include <netdb.h>

#include "../arp/resolver.h"
#include "../dns/resolver.h"
#include "connection.h"
#include "../bbuffer.h"
#include "../os.h"
#include "../ip.h"
#include "../ethernet.h"
#include "packet.h"

const int MAXIMUM_SEGMENT_SIZE = 2;
const int WINDOW_SCALE = 3;


static TcpPacket *wait_for_packet(TcpConnection *connection, bool (*filter)(const TcpPacket *))
{
    while (true)
    {
        EthernetFrame *frame = socket_read(connection->socket);
        switch (frame->ether_type)
        {
            case ETHER_TYPE_IPV4:
            {
                IPv4Datagram *datagram = ipv4_datagram_parse(frame->payload);
                if (ipv4_address_equals(datagram->destination_address, connection->my_ipv4_address))
                {
                    TcpPacket *packet = tcp_packet_parse(datagram->data);

                    if (connection->my_port == packet->destination_port && connection->destination_port == packet->source_port)
                    {
                        if (filter(packet))
                            return packet;
                    }
                }
                break;
            }
            case ETHER_TYPE_IPV6:
                break;
        }
    }
}

static bool syn_ack_filter(const TcpPacket *packet)
{
    return packet->syn && packet->ack;
}

static bool ack_filter(const TcpPacket *packet)
{
    return packet->ack;
}

static bool fin_filter(const TcpPacket *packet)
{
    return packet->fin;
}

static void byte_buffer_push_tcp(ByteBuffer *buffer, const TcpPacket *packet)
{

}

// Sends packet
// Frees packet object after sending
static void *tcp_connection_send_packet(TcpConnection *connection, TcpPacket *packet)
{
    if (connection->destination_ipv4_address) {
        MacAddress gateway_mac_address = resolve_ip_address(gateway()->ipv4_address);

        IPv4Datagram *datagram = ipv4_datagram_new();
        datagram->time_to_live = 128;
        datagram->identification = 0;
        datagram->transport_protocol = IP_TRANSP_PROTO_TCP;
        datagram->source_address = active_net_interface_ipv4_address();
        datagram->destination_address = connection->destination_ipv4_address;
        datagram->total_length = 20 + packet->length;
        datagram->header_checksum = ipv4_datagram_checksum(datagram);

        EthernetFrame *frame = ethernet_frame_new();
        frame->source = active_net_interface_mac_address();
        frame->destination = mac_address_clone(&gateway_mac_address);
        frame->ether_type = ETHER_TYPE_IPV4;

        ByteBuffer *buffer = byte_buffer_new();
        byte_buffer_push_eth(buffer, frame);
        byte_buffer_push_ipv4(buffer, datagram);
        byte_buffer_push_tcp(buffer, packet);

        socket_write_buf(connection->socket, buffer);

        if (packet->syn || packet->fin)
            connection->sequence++;
        else
            connection->sequence += packet->data->size;

        // cleanup
        ipv4_datagram_free(datagram);
        ethernet_frame_free(frame);
        byte_buffer_free(buffer);
    }
    tcp_packet_free(packet);
}

static void tcp_connection_establish(TcpConnection *connection)
{
    TcpPacket *syn = tcp_syn_packet_new(connection->sequence++);
    tcp_connection_send_packet(connection, syn);

    syn = wait_for_packet(connection, syn_ack_filter);
    connection->acknowledgment = syn->sequence_number + 1;
    //max_data_size = received_packet.option(Options::MAXIMUM_SEGMENT_SIZE);
    tcp_packet_free(syn);

    TcpPacket *ack = tcp_ack_packet_new(connection->acknowledgment);
    ack->window_size = 40000;
    tcp_connection_send_packet(connection, ack);
}

TcpConnection *tcp_ipv4_connection_open(const IPv4Address *address, int destination_port)
{
    TcpConnection *connection = malloc(sizeof(TcpConnection));
    connection->my_port = generate_port();
    connection->destination_port = destination_port;
    connection->sequence = rand();
    connection->acknowledgment = 0;
    connection->my_ipv4_address = active_net_interface_ipv4_address();
    connection->my_ipv6_address = active_net_interface_ipv6_address();
    connection->destination_ipv4_address = ipv4_address_clone(address);
    connection->destination_ipv6_address = NULL;
    connection->socket = socket_new();
    return connection;
}

TcpConnection *tcp_ipv6_connection_open(const IPv6Address *address, int destination_port)
{

}

ByteBuffer *tcp_connection_send(const TcpConnection *connection, const ByteBuffer *buffer)
{

}

ByteBuffer *tcp_connection_send_string(const TcpConnection *connection, const char *data)
{

    return NULL;
}

void tcp_connection_close(TcpConnection *connection)
{
    TcpPacket *fin = tcp_fin_packet_new();
    tcp_connection_send_packet(connection, fin);

    TcpPacket *ack = wait_for_packet(connection, ack_filter);
    if (ack->fin)
    {
        tcp_packet_free(ack);
    }
    else
    {
        tcp_packet_free(ack);
        fin = wait_for_packet(connection, fin_filter);
        tcp_packet_free(fin);
    }

    ack = tcp_ack_packet_new(connection->acknowledgment++);
    ack->window_size = 1000;
    tcp_connection_send_packet(connection, ack);
}

void tcp_connection_free(TcpConnection *connection)
{
    if (connection->destination_ipv4_address)
        ipv4_address_free(connection->destination_ipv4_address);

    if (connection->destination_ipv6_address)
        ipv6_address_free(connection->destination_ipv6_address);

    free(connection);
}