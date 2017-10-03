//
// Created by rnetuka on 4.4.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ethernet.h"
#include "pprint.h"

EthernetFrame *ethernet_frame_new(void)
{
    EthernetFrame *frame = malloc(sizeof(EthernetFrame));
    frame->destination = NULL;
    frame->source = NULL;
    frame->ether_type = 0;
    frame->payload = NULL;
    return frame;
}

EthernetFrame *ethernet_frame_parse(const uint8_t bytes[static ETH_HEADER_LENGTH], int length)
{
    if (length <= ETH_HEADER_LENGTH)
        return NULL;

    ByteBuffer *buffer = byte_buffer_from_bytes(bytes, length);

    EthernetFrame *frame = ethernet_frame_new();
    frame->destination = mac_address_from_bytes(byte_buffer_pop(buffer, MAC_ADDR_SIZE));
    frame->source = mac_address_from_bytes(byte_buffer_pop(buffer, MAC_ADDR_SIZE));
    frame->ether_type = byte_buffer_pop_int16(buffer);
    frame->payload = byte_buffer_sub(buffer, ETH_HEADER_LENGTH);

    byte_buffer_free(buffer);
    return frame;
}

void ethernet_frame_free(EthernetFrame *frame)
{
    if (frame->destination)
        mac_address_free(frame->destination);

    if (frame->source)
        mac_address_free(frame->source);

    if (frame->payload)
        byte_buffer_free(frame->payload);

    free(frame);
}

void ethernet_frame_print(const EthernetFrame *frame)
{
    pprint("Ethernet Header\n");
    pprint_field_s("Destination MAC", frame->destination->string);
    pprint_field_s("Source MAC", frame->source->string);

    switch (frame->ether_type)
    {
        case ETHER_TYPE_ARP:
            pprint_field_s("Ether Type", "ARP");
            break;
        case ETHER_TYPE_IPV4:
            pprint_field_s("Ether Type", "IPv4");
            break;
        case ETHER_TYPE_IPV6:
            pprint_field_s("Ether Type", "IPv6");
            break;
        default:
            pprint_field_i("Ether Type", frame->ether_type);
    }
    pprint("\n");
}

void byte_buffer_push_eth(ByteBuffer *buffer, const EthernetFrame *frame)
{
    byte_buffer_push(buffer, frame->destination->bytes, MAC_ADDR_SIZE);
    byte_buffer_push(buffer, frame->source->bytes, MAC_ADDR_SIZE);
    byte_buffer_push_int16(buffer, frame->ether_type);
}