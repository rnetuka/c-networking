//
// Created by root on 2.5.18.
//

#include <stdio.h>
#include <string.h>

#include "answer.h"
#include "encoding.h"
#include "util.h"
#include "../bbuffer.h"

DnsAnswer *dns_answer_parse(ByteBuffer *buffer, int message_offset)
{
    DnsAnswer *answer = malloc(sizeof(DnsAnswer));

    if (byte_buffer_peek_int8(buffer) >> 6 == 3)                // first two bits are 1s
    {
        int offset_pointer = byte_buffer_pop_int16(buffer);
        int offset = offset_pointer & 16383;                    // 0b_00111111_11111111

        char *domain_name = (char *) buffer->data + message_offset + offset;
        answer->domain_name = dns_domain_name_decode(domain_name);
    }
    else
    {
        size_t domain_name_length = 0;
        for (int i = 0; i < buffer->size; i++)
        {
            domain_name_length++;
            if (buffer->data[buffer->i + i] == 0)
                break;
        }
        answer->domain_name = malloc(domain_name_length);
        uint8_t *bytes = byte_buffer_pop(buffer, domain_name_length);
        strcpy(answer->domain_name, bytes);
    }

    answer->type = byte_buffer_pop_int16(buffer);
    answer->class = byte_buffer_pop_int16(buffer);
    answer->time_to_live = byte_buffer_pop_int32(buffer);
    answer->data_length = byte_buffer_pop_int16(buffer);

    switch (answer->type)
    {
        case A:
            answer->ip_address = ipv4_address_from_value(byte_buffer_pop_int32(buffer));
            break;
        case AAAA:
            break;
    }
    return answer;
}

void dns_answer_free(DnsAnswer *answer)
{
    free(answer->domain_name);
    switch (answer->type)
    {
        case A:
            if (answer->ip_address)
                ipv4_address_free(answer->ip_address);
            break;
        case AAAA:
            break;
    }
}

void dns_answer_print(char buffer[], const DnsAnswer *answer)
{
    sprintf(buffer, "[%s, %s] %s : IP Address: %s",
            dns_question_type_str(answer->type),
            dns_question_class_str(answer->class),
            answer->domain_name,
            answer->ip_address->string);
}