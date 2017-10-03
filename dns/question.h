//
// Created by root on 2.5.18.
//

#pragma once

#include <stdint.h>

#include "../bbuffer.h"

typedef struct
{
    char *domain_name;
    uint8_t *encoded_name;
    uint16_t type;
    uint16_t class;
}
DnsQuestion;

DnsQuestion *dns_question_new(const char *domain_name);
DnsQuestion *dns_question_parse(ByteBuffer *buffer);
void dns_question_free(DnsQuestion *question);
void dns_question_print(char buffer[], const DnsQuestion *question);
int dns_question_size(const DnsQuestion *question);

void byte_buffer_push_dns_question(ByteBuffer *buffer, const DnsQuestion *question);