//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include <stdint.h>

#include "../ipaddress.h"
#include "../mac.h"

typedef struct
{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_address_length;
    uint8_t protocol_address_length;
    uint16_t operation;
    MacAddress *sender_mac_address;
    IPv4Address *sender_ip_address;
    MacAddress *target_mac_address;
    IPv4Address *target_ip_address;
}
ArpPacket;

ArpPacket *arp_request_new(const IPv4Address *address);
ArpPacket *arp_packet_parse(const ByteBuffer *buffer);
void arp_packet_free(ArpPacket *packet);
void arp_packet_print(const ArpPacket *packet);