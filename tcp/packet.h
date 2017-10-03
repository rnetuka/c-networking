//
// Created by root on 11.7.18.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../bbuffer.h"

typedef struct
{
    int id;
    int value;
}
TcpOption;

typedef struct
{
    uint16_t source_port;
    uint16_t destination_port;
    uint32_t sequence_number;
    uint32_t acknowledgment_number;
    uint8_t data_offset;
    bool ns;
    bool cwr;
    bool ece;
    bool urg;
    bool ack;
    bool psh;
    bool rst;
    bool syn;
    bool fin;
    uint16_t window_size;
    uint16_t checksum;
    uint16_t urgent_pointer;
    TcpOption **options;
    ByteBuffer *data;
}
TcpPacket;

TcpPacket *tcp_packet_new(void);
TcpPacket *tcp_syn_packet_new(unsigned syn);
TcpPacket *tcp_ack_packet_new(unsigned ack);
TcpPacket *tcp_fin_packet_new(void);
TcpPacket *tcp_packet_parse(const ByteBuffer *buffer);
void tcp_packet_free(TcpPacket *packet);