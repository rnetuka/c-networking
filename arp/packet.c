//
// Created by rnetuka on 4.4.18.
//

#include <stdlib.h>
#include <stdio.h>

#include "packet.h"
#include "../os.h"
#include "../ethernet.h"
#include "../pprint.h"
#include "../socket.h"

#define HW_TYPE_ETHERNET 1
#define PROTO_TYPE_IPV4 0x0800

enum Operation
{
    REQUEST = 1,
    REPLY = 2
};

ArpPacket *arp_request_new(const IPv4Address *address)
{
    ArpPacket *packet = malloc(sizeof(ArpPacket));
    packet->hardware_type = HW_TYPE_ETHERNET;
    packet->protocol_type = PROTO_TYPE_IPV4;
    packet->hardware_address_length = MAC_ADDR_SIZE;
    packet->protocol_address_length = IPv4_ADDR_SIZE;
    packet->operation = REQUEST;
    packet->sender_mac_address = active_net_interface_mac_address();
    packet->sender_ip_address = active_net_interface_ipv4_address();
    packet->target_mac_address = NULL;
    packet->target_ip_address = ipv4_address_clone(address);
    return packet;
}

ArpPacket *arp_packet_parse(const ByteBuffer *buffer_)
{
    ByteBuffer *buffer = byte_buffer_clone(buffer_);

    ArpPacket *packet = malloc(sizeof(ArpPacket));
    packet->hardware_type = byte_buffer_pop_int16(buffer);
    packet->protocol_type = byte_buffer_pop_int16(buffer);
    packet->hardware_address_length = byte_buffer_pop_int8(buffer);
    packet->protocol_address_length = byte_buffer_pop_int8(buffer);
    packet->operation = byte_buffer_pop_int16(buffer);
    packet->sender_mac_address = mac_address_from_bytes(byte_buffer_pop(buffer, MAC_ADDR_SIZE));
    packet->sender_ip_address = ipv4_address_from_bytes(byte_buffer_pop(buffer, IPv4_ADDR_SIZE));
    packet->target_mac_address = mac_address_from_bytes(byte_buffer_pop(buffer, MAC_ADDR_SIZE));
    packet->target_ip_address = ipv4_address_from_bytes(byte_buffer_pop(buffer, IPv4_ADDR_SIZE));

    byte_buffer_free(buffer);
    return packet;
}

void arp_packet_free(ArpPacket *packet)
{
    if (packet->sender_mac_address)
        mac_address_free(packet->sender_mac_address);

    if (packet->target_mac_address)
        mac_address_free(packet->target_mac_address);

    if (packet->sender_ip_address)
        ipv4_address_free(packet->sender_ip_address);

    if (packet->target_ip_address)
        ipv4_address_free(packet->target_ip_address);

    free(packet);
}

void arp_packet_print(const ArpPacket *packet)
{
    pprint("ARP\n");

    if (packet->hardware_type == HW_TYPE_ETHERNET)
        pprint_field_s("Hardware Type", "Ethernet");
    else
        pprint_field_i("Hardware Type", packet->hardware_type);

    if (packet->protocol_type == PROTO_TYPE_IPV4)
        pprint_field_s("Protocol Type", "IPv4");
    else
        pprint_field_i("Protocol Type", packet->protocol_type);

    pprint_field_i("Hardware Address Length", packet->hardware_address_length);
    pprint_field_i("Protocol Address Length", packet->protocol_address_length);

    switch (packet->operation)
    {
        case REQUEST:
            pprint_field_s("Operation", "Request");
            break;
        case REPLY:
            pprint_field_s("Operation", "Reply");
            break;
        default:
            pprint_field_i("Operation", packet->operation);
    }

    if (packet->sender_mac_address)
        pprint_field_s("Sender MAC", packet->sender_mac_address->string);
    else
        pprint_field_s("Sender MAC", "-");

    if (packet->protocol_type == PROTO_TYPE_IPV4)
        pprint_field_s("Sender IP", packet->sender_ip_address->string);
    else
        pprint_field_s("Sender IP", "-");

    if (packet->target_mac_address)
        pprint_field_s("Target MAC", packet->target_mac_address->string);
    else
        pprint_field_s("Target MAC", "-");

    if (packet->protocol_type == PROTO_TYPE_IPV4)
        pprint_field_s("Target IP", packet->target_ip_address->string);
    else
        pprint_field_s("Target IP", "-");
}