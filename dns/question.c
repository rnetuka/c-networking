//
// Created by root on 2.5.18.
//

#include <stdio.h>
#include <string.h>

#include "encoding.h"
#include "question.h"
#include "util.h"

#define DNS_QUESTION_TYPE_SIZE 2
#define DNS_QUESTION_CLASS_SIZE 2

#define encoded_name_length(domain_name) (1 + (int) strlen(domain_name) + 1)

DnsQuestion *dns_question_new(const char *domain_name)
{
    DnsQuestion *question = malloc(sizeof(DnsQuestion));
    question->domain_name = malloc(strlen(domain_name + 1));
    strcpy(question->domain_name, domain_name);
    question->encoded_name = dns_domain_name_encode(domain_name);
    question->type = A;
    question->class = IN;
    return question;
}

DnsQuestion *dns_question_parse(ByteBuffer *buffer)
{
    DnsQuestion *question = malloc(sizeof(DnsQuestion));
    int domain_name_length = 0;
    for (int i = 0; i < buffer->size; i++)
    {
        domain_name_length++;
        if (buffer->data[buffer->i + i] == 0)
            break;
    }
    uint8_t *bytes = byte_buffer_pop(buffer, domain_name_length);
    question->domain_name = dns_domain_name_decode(bytes);
    question->encoded_name = calloc(encoded_name_length(question->domain_name), sizeof(char));
    memcpy(question->encoded_name, bytes, encoded_name_length(question->domain_name));

    question->type = byte_buffer_pop_int16(buffer);
    question->class = byte_buffer_pop_int16(buffer);
    return question;
}

void dns_question_free(DnsQuestion *question)
{
    free(question->domain_name);
    free(question->encoded_name);
    free(question);
}

void dns_question_print(char buffer[], const DnsQuestion *question)
{
    sprintf(buffer, "[%s, %s] %s",
            dns_question_type_str(question->type),
            dns_question_class_str(question->class),
            question->domain_name
    );
}

int dns_question_size(const DnsQuestion *question)
{
    return encoded_name_length(question->domain_name) +
           DNS_QUESTION_TYPE_SIZE +
           DNS_QUESTION_CLASS_SIZE;
}

void byte_buffer_push_dns_question(ByteBuffer *buffer, const DnsQuestion *question)
{
    byte_buffer_push(buffer, question->encoded_name, encoded_name_length(question->domain_name));
    byte_buffer_push_int16(buffer, question->type);
    byte_buffer_push_int16(buffer, question->class);
}