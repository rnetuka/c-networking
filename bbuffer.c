//
// Created by rnetuka on 6.4.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bbuffer.h"

const size_t INIT_CAPACITY = 50;
const size_t INCR_CAPACITY = 100;

ByteBuffer *byte_buffer_new(void)
{
    ByteBuffer *buffer = malloc(sizeof(ByteBuffer));
    buffer->data = malloc(INIT_CAPACITY);
    buffer->size = 0;
    buffer->capacity = INIT_CAPACITY;
    buffer->i = 0;
    return buffer;
}

ByteBuffer *byte_buffer_clone(const ByteBuffer *buffer)
{
    ByteBuffer *clone = malloc(sizeof(ByteBuffer));
    clone->data = malloc(buffer->capacity);
    clone->size = buffer->size;
    clone->capacity = buffer->capacity;
    clone->i = buffer->i;
    memcpy(clone->data, buffer->data, buffer->size);
    return clone;
}

ByteBuffer *byte_buffer_from_bytes(const uint8_t bytes[], size_t length)
{
    ByteBuffer *buffer = byte_buffer_new();
    byte_buffer_push(buffer, bytes, length);
    return buffer;
}

ByteBuffer *byte_buffer_sub(const ByteBuffer *buffer, int from)
{
    ByteBuffer *result = byte_buffer_new();
    byte_buffer_push(result, buffer->data + from, buffer->size - from);
    return result;
}

void byte_buffer_free(ByteBuffer *buffer)
{
    free(buffer->data);
    free(buffer);
}

void byte_buffer_push_int8(ByteBuffer *buffer, uint8_t byte)
{
    if (buffer->size == buffer->capacity)
    {
        buffer->data = realloc(buffer->data, buffer->capacity + INCR_CAPACITY);
        buffer->capacity += INCR_CAPACITY;
    }
    buffer->data[buffer->size] = byte;
    buffer->size++;
}

void byte_buffer_push_int16(ByteBuffer *buffer, uint16_t bytes)
{
    byte_buffer_push_int8(buffer, (uint8_t) (bytes >> 8));
    byte_buffer_push_int8(buffer, (uint8_t) bytes);
}

void byte_buffer_push_int32(ByteBuffer *buffer, uint32_t bytes)
{
    byte_buffer_push_int8(buffer, (uint8_t) (bytes >> 24));
    byte_buffer_push_int8(buffer, (uint8_t) (bytes >> 16));
    byte_buffer_push_int8(buffer, (uint8_t) (bytes >> 8));
    byte_buffer_push_int8(buffer, (uint8_t) bytes);
}

void byte_buffer_push(ByteBuffer *buffer, const uint8_t bytes[], unsigned int length)
{
    for (int i = 0; i < length; i++)
        byte_buffer_push_int8(buffer, bytes[i]);
}

void byte_buffer_push_zeros(ByteBuffer *buffer, int n)
{
    for (int i = 0; i < n; i++)
        byte_buffer_push_int8(buffer, 0);
}

uint8_t *byte_buffer_pop(ByteBuffer *buffer, int n)
{
    unsigned int t = buffer->i;
    buffer->i += n;
    return buffer->data + t;
}

uint8_t byte_buffer_pop_int8(ByteBuffer *buffer)
{
    uint8_t result = buffer->data[buffer->i];
    buffer->i++;
    return result;
}

uint16_t byte_buffer_pop_int16(ByteBuffer *buffer)
{
    uint16_t result = 0;
    result += byte_buffer_pop_int8(buffer);
    result <<= 8;
    result += byte_buffer_pop_int8(buffer);
    return result;
}

uint32_t byte_buffer_pop_int32(ByteBuffer *buffer)
{
    uint32_t result = 0;
    result += byte_buffer_pop_int8(buffer);
    result <<= 8;
    result += byte_buffer_pop_int8(buffer);
    result <<= 8;
    result += byte_buffer_pop_int8(buffer);
    result <<= 8;
    result += byte_buffer_pop_int8(buffer);
    return result;
}

uint8_t byte_buffer_peek_int8(const ByteBuffer *buffer)
{
    uint8_t result = buffer->data[buffer->i];
    return result;
}

void byte_buffer_print(const ByteBuffer *buffer)
{
    for (int i = 0; i < buffer->size; i++)
    {
        printf("%3i ", buffer->data[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
        else if ((i + 1) % 8 == 0)
            printf("  ");
    }
}