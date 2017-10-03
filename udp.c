//
// Created by rnetuka on 4.4.18.
//

#include <stdlib.h>
#include <string.h>

#include "pprint.h"
#include "udp.h"

UdpPacket *udp_packet_new(void)
{
    UdpPacket *packet = malloc(sizeof(UdpPacket));
    packet->source_port = 0;
    packet->destination_port = 0;
    packet->length = 0;
    packet->checksum = 0;
    packet->data = NULL;
    return packet;
}

UdpPacket *udp_packet_parse(const ByteBuffer *buffer_)
{
    ByteBuffer *buffer = byte_buffer_clone(buffer_);

    UdpPacket *packet = malloc(sizeof(UdpPacket));
    packet->source_port = byte_buffer_pop_int16(buffer);
    packet->destination_port = byte_buffer_pop_int16(buffer);
    packet->length = byte_buffer_pop_int16(buffer);
    packet->checksum = byte_buffer_pop_int16(buffer);
    packet->data = buffer;
    return packet;
}

void udp_packet_free(UdpPacket *packet)
{
    if (packet->data)
        free(packet->data);

    free(packet);
}

void udp_packet_print(const UdpPacket *packet)
{
    pprint("UDP Header\n");
    pprint_field_i("Source Port", packet->source_port);
    pprint_field_i("Destination Port", packet->destination_port);
    pprint_field_i("UDP Length", packet->length);
    pprint_field_i("UDP Checksum", packet->checksum);
    pprint("\n");
}

void byte_buffer_push_udp(ByteBuffer *buffer, const UdpPacket *packet)
{
    byte_buffer_push_int16(buffer, packet->source_port);
    byte_buffer_push_int16(buffer, packet->destination_port);
    byte_buffer_push_int16(buffer, packet->length);
    byte_buffer_push_int16(buffer, packet->checksum);
}