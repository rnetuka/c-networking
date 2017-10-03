//
// Created by root on 11.7.18.
//

#include "packet.h"

TcpPacket *tcp_packet_new(void)
{
    TcpPacket *packet = malloc(sizeof(TcpPacket));
    packet->source_port = 0;
    packet->destination_port = 0;
    packet->sequence_number = 0;
    packet->acknowledgment_number = 0;
    packet->data_offset = 0;
    packet->ns = 0;
    packet->cwr = 0;
    packet->ece = 0;
    packet->urg = 0;
    packet->ack = 0;
    packet->psh = 0;
    packet->rst = 0;
    packet->syn = 0;
    packet->fin = 0;
    packet->checksum = 0;
    packet->window_size = 0;
    packet->urgent_pointer = 0;
    packet->options = calloc(1, sizeof(TcpOption *));
    packet->options[0] = NULL;
    packet->data = byte_buffer_new();
}

TcpPacket *tcp_syn_packet_new(unsigned syn)
{
    TcpPacket *packet = tcp_packet_new();
    packet->syn = true;
    packet->sequence_number = syn;
    return packet;
}

TcpPacket *tcp_ack_packet_new(unsigned ack)
{

}

TcpPacket *tcp_fin_packet_new(void)
{

}

TcpPacket *tcp_packet_parse(const ByteBuffer *buffer)
{
    buffer = byte_buffer_clone(buffer);

    TcpPacket *packet = tcp_packet_new();
}

static void tcp_options_free(TcpPacket *packet)
{
    TcpOption **options = packet->options;
    while (*options)
    {
        TcpOption *option = *options;
        free(option);
        options++;
    }
    free(packet->options);
}

void tcp_packet_free(TcpPacket *packet)
{
    tcp_options_free(packet);

    if (packet->data)
        byte_buffer_free(packet->data);
}