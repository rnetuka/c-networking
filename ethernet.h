//
// Created by rnetuka on 4.4.18.
//

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "bbuffer.h"
#include "mac.h"

#define ETHER_TYPE_IPV4 0x0800      // 2048 decimal
#define ETHER_TYPE_IPV6 0x86DD      // 34525 decimal
#define ETHER_TYPE_ARP  0x0806      // 2054 decimal

#define ETH_HEADER_LENGTH (\
            MAC_ADDR_SIZE + \
            MAC_ADDR_SIZE + \
            sizeof(((EthernetFrame *) 0)->ether_type))

typedef struct
{
    MacAddress *destination;        // 6 bytes
    MacAddress *source;             // 6 bytes
    uint16_t ether_type;            // 2 bytes
    ByteBuffer *payload;            // x bytes
}
EthernetFrame;

EthernetFrame *ethernet_frame_new(void);
EthernetFrame *ethernet_frame_parse(const uint8_t bytes[static ETH_HEADER_LENGTH], int length);
void ethernet_frame_free(EthernetFrame *frame);
void ethernet_frame_print(const EthernetFrame *frame);

void byte_buffer_push_eth(ByteBuffer *buffer, const EthernetFrame *frame);