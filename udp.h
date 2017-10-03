//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include <stdint.h>

#include "bbuffer.h"

typedef struct
{
    uint16_t source_port;
    uint16_t destination_port;
    uint16_t length;
    uint16_t checksum;
    ByteBuffer *data;
}
UdpPacket;

UdpPacket *udp_packet_new(void);
UdpPacket *udp_packet_parse(const ByteBuffer *buffer);
void udp_packet_free(UdpPacket *packet);
void udp_packet_print(const UdpPacket *packet);

void byte_buffer_push_udp(ByteBuffer *buffer, const UdpPacket *packet);