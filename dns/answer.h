//
// Created by root on 2.5.18.
//

#pragma once

#include <stdint.h>

#include "../bbuffer.h"
#include "../ipaddress.h"

typedef struct
{
    char *domain_name;
    uint16_t type;
    uint16_t class;
    uint32_t time_to_live;
    uint16_t data_length;
    union
    {
        IPv4Address *ip_address;
    };
}
DnsAnswer;

DnsAnswer *dns_answer_parse(ByteBuffer *buffer, int message_offset);
void dns_answer_free(DnsAnswer *answer);
void dns_answer_print(char buffer[], const DnsAnswer *answer);