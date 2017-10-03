//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "bbuffer.h"
#include "ipaddress.h"

#define IP_TRANSP_PROTO_TCP 6
#define IP_TRANSP_PROTO_UDP 17

typedef struct
{
    uint8_t version;                    // 4 bits
    uint8_t ihl;                        // 4 bits
    uint8_t dscp;                       // 6 bits
    uint8_t ecn;                        // 2 bits
    uint16_t total_length;              // 16 bits
    uint16_t identification;            // 16 bits
    bool dont_fragment;                 // 1 bit (+ 1 bit reserved 0)
    bool more_fragments;                // 1 bit
    uint16_t fragment_offset;           // 14 bits
    uint8_t time_to_live;               // 8 bits
    uint8_t transport_protocol;         // 8 bits
    uint16_t header_checksum;           // 16 bits
    IPv4Address *source_address;        // 32 bits
    IPv4Address *destination_address;   // 32 bits
    uint8_t *options;
    ByteBuffer *data;
}
IPv4Datagram;

IPv4Datagram *ipv4_datagram_new(void);
IPv4Datagram *ipv4_datagram_parse(const ByteBuffer *buffer);
void ipv4_datagram_free(IPv4Datagram *datagram);
void ipv4_datagram_print(const IPv4Datagram *datagram);
int ipv4_datagram_checksum(const IPv4Datagram *datagram);

void byte_buffer_push_ipv4(ByteBuffer *buffer, const IPv4Datagram *datagram);

typedef struct
{

}
IPv6Datagram;
