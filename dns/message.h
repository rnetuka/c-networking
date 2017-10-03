//
// Created by root on 2.5.18.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "answer.h"
#include "question.h"

#define DNS_MESSAGE_REQUEST 0
#define DNS_MESSAGE_RESPONSE 1

typedef struct
{
    uint16_t transaction_id;    // 2 bytes
    struct
    {
        bool message_type;
        short opcode;
        bool authoritative_answer;
        bool truncated;
        bool recursion_desired;
        bool recursion_available;
        short rcode;
    } flags;                    // 2 bytes
    uint16_t question_count;    // 2 bytes, no. of items in Question Section
    uint16_t answer_count;      // 2 bytes, no. of items in Answer Section
    uint16_t authority_count;   // 2 bytes, no. of items in Authority Section
    uint16_t additional_count;  // 2 bytes, no. of items in Additional Section
    DnsQuestion **questions;
    DnsAnswer **answers;
}
DnsMessage;

DnsMessage *dns_message_new(void);
DnsMessage *dns_message_parse(const ByteBuffer *buffer);
void dns_message_free(DnsMessage *message);
void dns_message_print(const DnsMessage *message);
int dns_message_size(const DnsMessage *message);

void byte_buffer_push_dns(ByteBuffer *buffer, const DnsMessage *message);