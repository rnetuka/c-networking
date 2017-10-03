//
// Created by root on 10.7.18.
//

#include <stdio.h>

#include "../ethernet.h"
#include "../os.h"
#include "../socket.h"
#include "../pprint.h"
#include "cache.h"
#include "resolver.h"

extern ArpCache *arp_cache;

static void byte_buffer_push_arp(ByteBuffer *buffer, const ArpPacket *packet)
{
    byte_buffer_push_int16(buffer, packet->hardware_type);
    byte_buffer_push_int16(buffer, packet->protocol_type);
    byte_buffer_push_int8(buffer, packet->hardware_address_length);
    byte_buffer_push_int8(buffer, packet->protocol_address_length);
    byte_buffer_push_int16(buffer, packet->operation);
    byte_buffer_push(buffer, packet->sender_mac_address->bytes, MAC_ADDR_SIZE);
    byte_buffer_push(buffer, packet->sender_ip_address->bytes, 4);

    if (packet->target_mac_address)
        byte_buffer_push(buffer, packet->target_mac_address->bytes, MAC_ADDR_SIZE);
    else
        byte_buffer_push_zeros(buffer, MAC_ADDR_SIZE);

    byte_buffer_push(buffer, packet->target_ip_address->bytes, 4);
}

MacAddress resolve_ip_address(const IPv4Address *address)
{
    if (arp_cache)
    {
        const MacAddress *mac = arp_cache_get_mac(arp_cache, address);
        if (mac)
            return *mac;
    }

    EthernetFrame *frame = ethernet_frame_new();
    frame->source = active_net_interface_mac_address();
    frame->destination = mac_address_parse("ff:ff:ff:ff:ff:ff");
    frame->ether_type = ETHER_TYPE_ARP;

    ArpPacket *packet = arp_request_new(address);

    ByteBuffer *buffer = byte_buffer_new();
    byte_buffer_push_eth(buffer, frame);
    byte_buffer_push_arp(buffer, packet);

    Socket *socket = socket_new();
    socket_write_buf(socket, buffer);

    ethernet_frame_print(frame);
    arp_packet_print(packet);
    pprint_flush();
    printf("\n");

    ethernet_frame_free(frame);
    arp_packet_free(packet);

    byte_buffer_free(buffer);

    while (true)
    {
        frame = socket_read(socket);
        if (frame->ether_type == ETHER_TYPE_ARP)
        {
            packet = arp_packet_parse(frame->payload);

            printf("Received:\n");
            ethernet_frame_print(frame);
            arp_packet_print(packet);
            pprint_flush();
            printf("\n");

            MacAddress result = *packet->sender_mac_address;
            ethernet_frame_free(frame);
            arp_packet_free(packet);

            if (arp_cache)
                arp_cache_insert(arp_cache, address, &result);

            return result;
        }
        else
            ethernet_frame_free(frame);
    }
}