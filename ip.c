//
// Created by rnetuka on 4.4.18.
//

#include <stdlib.h>
#include <string.h>

#include "pprint.h"
#include "ip.h"
#include "ipaddress.h"

IPv4Datagram *ipv4_datagram_new(void)
{
    IPv4Datagram *datagram = malloc(sizeof(IPv4Datagram));
    datagram->version = 4;
    datagram->ihl = 5;
    datagram->dscp = 0;
    datagram->ecn = 0;
    datagram->total_length = 20;
    datagram->identification = 0;
    datagram->dont_fragment = true;
    datagram->more_fragments = false;
    datagram->fragment_offset = 0;
    datagram->time_to_live = 0;
    datagram->transport_protocol = 0;
    datagram->header_checksum = 0;
    datagram->source_address = NULL;
    datagram->destination_address = NULL;
    datagram->options = NULL;
    datagram->data = NULL;
    return datagram;
}

IPv4Datagram *ipv4_datagram_parse(const ByteBuffer *buffer_)
{
    ByteBuffer *buffer = byte_buffer_clone(buffer_);
    IPv4Datagram *datagram = malloc(sizeof(IPv4Datagram));
    datagram->version = byte_buffer_peek_int8(buffer) >> 4;
    datagram->ihl = byte_buffer_pop_int8(buffer) & 0xF;
    datagram->dscp = byte_buffer_peek_int8(buffer) >> 2;
    datagram->ecn = byte_buffer_pop_int8(buffer) & 3;
    datagram->total_length = byte_buffer_pop_int16(buffer);
    datagram->identification = byte_buffer_pop_int16(buffer);
    datagram->dont_fragment = (byte_buffer_peek_int8(buffer) >> 6) & 1;
    datagram->more_fragments = (byte_buffer_peek_int8(buffer) >> 5) & 1;
    datagram->fragment_offset = byte_buffer_pop_int16(buffer) & 0x1FFF;
    datagram->time_to_live = byte_buffer_pop_int8(buffer);
    datagram->transport_protocol = byte_buffer_pop_int8(buffer);
    datagram->header_checksum = byte_buffer_pop_int16(buffer);

    uint8_t src_bytes[4] = {
            byte_buffer_pop_int8(buffer),
            byte_buffer_pop_int8(buffer),
            byte_buffer_pop_int8(buffer),
            byte_buffer_pop_int8(buffer)
    };
    uint8_t dest_bytes[4] = {
            byte_buffer_pop_int8(buffer),
            byte_buffer_pop_int8(buffer),
            byte_buffer_pop_int8(buffer),
            byte_buffer_pop_int8(buffer)
    };

    datagram->source_address = ipv4_address_new(
            src_bytes[0],
            src_bytes[1],
            src_bytes[2],
            src_bytes[3]);
    datagram->destination_address = ipv4_address_new(
            dest_bytes[0],
            dest_bytes[1],
            dest_bytes[2],
            dest_bytes[3]);
    datagram->data = buffer;
    return datagram;
}

void ipv4_datagram_free(IPv4Datagram *datagram)
{
    if (datagram->source_address)
        ipv4_address_free(datagram->source_address);

    if (datagram->destination_address)
        ipv4_address_free(datagram->destination_address);

    //if (datagram->options)
    //    free(datagram->options);

    if (datagram->data)
        byte_buffer_free(datagram->data);

    free(datagram);
}

static int ipv4_datagram_flags(const IPv4Datagram *datagram)
{
    int flags = 0;
    flags += datagram->dont_fragment;
    flags <<= 1;
    flags += datagram->more_fragments;
    return flags;
}

int ipv4_datagram_checksum(const IPv4Datagram *datagram)
{
    unsigned int sum =
            ((datagram->version << 12) + (datagram->ihl << 8) + (datagram->dscp << 2) + datagram->ecn) +
            datagram->total_length +
            datagram->identification +
            ((ipv4_datagram_flags(datagram) << 13) + datagram->fragment_offset) +
            ((datagram->time_to_live << 8) + datagram->transport_protocol) +
            (ipv4_address_value(datagram->source_address) >> 16) +
            (ipv4_address_value(datagram->source_address) & 65535) +
            (ipv4_address_value(datagram->destination_address) >> 16) +
            (ipv4_address_value(datagram->destination_address) & 65535);
    sum = (sum >> 16) + (sum & 65535);
    return ~sum & 65535;
}

void byte_buffer_push_ipv4(ByteBuffer *buffer, const IPv4Datagram *datagram)
{
    byte_buffer_push_int8(buffer, (datagram->version << 4) + datagram->ihl);
    byte_buffer_push_int8(buffer, (datagram->dscp << 2) + datagram->ecn);
    byte_buffer_push_int16(buffer, datagram->total_length);
    byte_buffer_push_int16(buffer, datagram->identification);
    byte_buffer_push_int16(buffer, (ipv4_datagram_flags(datagram) << 13) + datagram->fragment_offset);
    byte_buffer_push_int8(buffer, datagram->time_to_live);
    byte_buffer_push_int8(buffer, datagram->transport_protocol);
    byte_buffer_push_int16(buffer, datagram->header_checksum);
    byte_buffer_push(buffer, datagram->source_address->bytes, IPv4_ADDR_SIZE);
    byte_buffer_push(buffer, datagram->destination_address->bytes, IPv4_ADDR_SIZE);
    //TODO push options
}

void ipv4_datagram_print(const IPv4Datagram *datagram)
{
    pprint("IP Header\n");
    pprint_field_i("Version", datagram->version);
    pprint_field_i("Internet Header Length", datagram->ihl);
    //TODO print DSCP
    //TODO print ECN
    pprint_field_v("Total Length", "%i Bytes", datagram->total_length);
    pprint_field_i("Identification", datagram->identification);
    //TODO print flags
    //TODO print fragment offset
    pprint_field_i("Time To Live", datagram->time_to_live);

    switch (datagram->transport_protocol)
    {
        case IP_TRANSP_PROTO_TCP:
            pprint_field_s("Transport Protocol", "TCP");
            break;
        case IP_TRANSP_PROTO_UDP:
            pprint_field_s("Transport Protocol", "UDP");
            break;
        default:
            pprint_field_i("Transport Protocol", datagram->transport_protocol);
    }

    pprint_field_i("Header Checksum", datagram->header_checksum);

    if (datagram->source_address)
        pprint_field_s("Source Address", datagram->source_address->string);
    else
        pprint_field_s("Source Address", "-");

    if (datagram->destination_address)
        pprint_field_s("Destination Address", datagram->destination_address->string);
    else
        pprint_field_s("Destination Address", "-");
    //TODO print options
    pprint("\n");
}