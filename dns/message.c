//
// Created by root on 2.5.18.
//

#include "message.h"
#include "../pprint.h"

DnsMessage *dns_message_new(void)
{
    DnsMessage *message = malloc(sizeof(DnsMessage));
    message->transaction_id = 0;
    message->flags.message_type = 0;
    message->flags.opcode = 0;
    message->flags.authoritative_answer = false;
    message->flags.truncated = false;
    message->flags.recursion_desired = true;
    message->flags.recursion_available = false;
    message->flags.rcode = 0;
    message->question_count = 0;
    message->answer_count = 0;
    message->authority_count = 0;
    message->additional_count = 0;
    message->questions = NULL;
    message->answers = NULL;
    return message;
}

static void dns_message_fill_flags(DnsMessage *message, uint16_t flags_value)
{
    message->flags.message_type = flags_value & 32768;
    message->flags.opcode = flags_value & 30720;
    message->flags.authoritative_answer = flags_value & 1024;
    message->flags.truncated = flags_value & 512;
    message->flags.recursion_desired = flags_value & 256;
    message->flags.recursion_available = flags_value & 128;
    message->flags.rcode = flags_value & 15;
}

DnsMessage *dns_message_parse(const ByteBuffer *buffer_)
{
    ByteBuffer *buffer = byte_buffer_clone(buffer_);
    int message_offset = buffer->i;

    DnsMessage *message = dns_message_new();
    message->transaction_id = byte_buffer_pop_int16(buffer);
    dns_message_fill_flags(message, byte_buffer_pop_int16(buffer));
    message->question_count = byte_buffer_pop_int16(buffer);
    message->answer_count = byte_buffer_pop_int16(buffer);
    message->authority_count = byte_buffer_pop_int16(buffer);
    message->additional_count = byte_buffer_pop_int16(buffer);
    message->questions = calloc(message->question_count, sizeof(DnsQuestion *));

    for (int i = 0; i < message->question_count; i++)
        message->questions[i] = dns_question_parse(buffer);

    message->answers = calloc(message->answer_count, sizeof(DnsAnswer *));

    for (int i = 0; i < message->answer_count; i++)
        message->answers[i] = dns_answer_parse(buffer, message_offset);

    byte_buffer_free(buffer);
    return message;
}

void dns_message_free(DnsMessage *message)
{
    for (int i = 0; i < message->question_count; i++)
        dns_question_free(message->questions[i]);

    for (int i = 0; i < message->answer_count; i++)
        dns_answer_free(message->answers[i]);

    free(message->questions);
    free(message->answers);
    free(message);
}

void dns_message_print(const DnsMessage *message)
{
    pprint("DNS Data\n");
    pprint_field_i("ID", message->transaction_id);
    pprint_field_s("Message Type", message->flags.message_type ? "Response" : "Request");
    pprint_field_i("OPCode", message->flags.opcode);
    pprint_field_s("Authoritative Answer", message->flags.authoritative_answer ? "true" : "false");
    pprint_field_s("Truncated", message->flags.truncated ? "true" : "false");
    pprint_field_s("Recursion Desired", message->flags.recursion_desired ? "true" : "false");
    pprint_field_s("Recursion Available", message->flags.recursion_available ? "true" : "false");
    pprint_field_i("RCode", message->flags.rcode);
    pprint_field_i("Question Count", message->question_count);
    pprint_field_i("Answer Count", message->answer_count);
    pprint_field_i("NSCount", message->authority_count);
    pprint_field_i("ARCount", message->additional_count);

    for (int i = 0; i < message->question_count; i++)
    {
        char buffer[200] = {};
        dns_question_print(buffer, message->questions[i]);
        pprint_field_s("Question", buffer);
    }
    for (int i = 0; i < message->answer_count; i++)
    {
        char buffer[200] = {};
        dns_answer_print(buffer, message->answers[i]);
        pprint_field_s("Answer", buffer);
    }
    pprint("\n");
}

static uint16_t dns_message_flags_value(const DnsMessage *message)
{
    uint16_t value = 0;
    value += message->flags.message_type;
    value <<= 4;
    value += message->flags.opcode;
    value <<= 1;
    value += message->flags.authoritative_answer;
    value <<= 1;
    value += message->flags.truncated;
    value <<= 1;
    value += message->flags.recursion_desired;
    value <<= 1;
    value += message->flags.recursion_available;
    value <<= 2;
    value <<= 1;
    value <<= 4;
    value += message->flags.rcode;
    return value;
}

void byte_buffer_push_dns(ByteBuffer *buffer, const DnsMessage *message)
{
    byte_buffer_push_int16(buffer, message->transaction_id);
    byte_buffer_push_int16(buffer, dns_message_flags_value(&message->flags));
    byte_buffer_push_int16(buffer, message->question_count);
    byte_buffer_push_int16(buffer, message->answer_count);
    byte_buffer_push_int16(buffer, message->authority_count);
    byte_buffer_push_int16(buffer, message->additional_count);

    for (int i = 0; i < message->question_count; i++)
        byte_buffer_push_dns_question(buffer, message->questions[i]);
}

int dns_message_size(const DnsMessage *message)
{
    int size = 12;

    for (int i = 0; i < message->question_count; i++)
        size += dns_question_size(message->questions[i]);

    return size;
}