//
// Created by root on 10.7.18.
//

#include <stdio.h>

#include "../arp/resolver.h"
#include "../ethernet.h"
#include "../ip.h"
#include "../os.h"
#include "../pprint.h"
#include "../socket.h"
#include "../udp.h"
#include "cache.h"
#include "message.h"
#include "resolver.h"

// global DNS cache (if supplied by main.c)
// optional, but if present, stores DNS query results
extern DnsCache *dns_cache;

const int DNS_UDP_PORT = 53;

static DnsMessage *dns_query_new(const char *hostname)
{
    DnsMessage *message = dns_message_new();
    message->flags.message_type = DNS_MESSAGE_REQUEST;
    message->question_count = 1;
    message->questions = calloc(1, sizeof(DnsQuestion *));
    message->questions[0] = dns_question_new(hostname);
    return message;
}

static bool is_for_me(const IPv4Datagram *datagram)
{
    return ipv4_address_equals(active_net_interface_ipv4_address(), datagram->destination_address)
           && datagram->transport_protocol == IP_TRANSP_PROTO_UDP;
}

const HostAddress *get_host_address(const char *hostname)
{
    if (dns_cache)
    {
        const HostAddress *address = dns_cache_get_address(dns_cache, hostname);
        if (address)
            return address;
    }

    MacAddress gateway_mac_address = resolve_ip_address(gateway()->ipv4_address);
    int port = generate_port();

    DnsMessage *message = dns_query_new(hostname);

    UdpPacket *packet = udp_packet_new();
    packet->source_port = port;
    packet->destination_port = DNS_UDP_PORT;
    packet->length = 8 + dns_message_size(message);
    packet->checksum = 0;

    IPv4Datagram *datagram = ipv4_datagram_new();
    datagram->time_to_live = 128;
    datagram->identification = 0;
    datagram->transport_protocol = IP_TRANSP_PROTO_UDP;
    datagram->source_address = active_net_interface_ipv4_address();
    datagram->destination_address = dns_server_ipv4_address();
    datagram->total_length = 20 + packet->length;
    datagram->header_checksum = ipv4_datagram_checksum(datagram);

    EthernetFrame *frame = ethernet_frame_new();
    frame->source = active_net_interface_mac_address();
    frame->destination = mac_address_clone(&gateway_mac_address);
    frame->ether_type = ETHER_TYPE_IPV4;

    ByteBuffer *buffer = byte_buffer_new();
    byte_buffer_push_eth(buffer, frame);
    byte_buffer_push_ipv4(buffer, datagram);
    byte_buffer_push_udp(buffer, packet);
    byte_buffer_push_dns(buffer, message);

    Socket *socket = socket_new();
    socket_write_buf(socket, buffer);

    ethernet_frame_print(frame);
    ipv4_datagram_print(datagram);
    udp_packet_print(packet);
    dns_message_print(message);
    pprint_flush();
    printf("\n");

    byte_buffer_free(buffer);
    ethernet_frame_free(frame);
    ipv4_datagram_free(datagram);
    udp_packet_free(packet);
    dns_message_free(message);

    while (true)
    {
        frame = socket_read(socket);
        switch (frame->ether_type)
        {
            case ETHER_TYPE_IPV4:
            {
                datagram = ipv4_datagram_parse(frame->payload);

                if (is_for_me(datagram))
                {
                    packet = udp_packet_parse(datagram->data);

                    if (packet->source_port == DNS_UDP_PORT && packet->destination_port == port)
                    {
                        message = dns_message_parse(packet->data);

                        printf("Received\n");
                        ethernet_frame_print(frame);
                        ipv4_datagram_print(datagram);
                        udp_packet_print(packet);
                        dns_message_print(message);
                        pprint_flush();

                        ethernet_frame_free(frame);
                        ipv4_datagram_free(datagram);
                        udp_packet_free(packet);
                        dns_message_free(message);

                        //TODO return result
                        return NULL;
                    }
                    else
                    {
                        ethernet_frame_free(frame);
                        ipv4_datagram_free(datagram);
                        udp_packet_free(packet);
                    }
                }
                else
                {
                    ethernet_frame_free(frame);
                    ipv4_datagram_free(datagram);
                }
                break;
            }
            case ETHER_TYPE_IPV6:
                ethernet_frame_free(frame);
                break;
            default:
                ethernet_frame_free(frame);
        }
    }
}