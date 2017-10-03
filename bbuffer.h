//
// Created by rnetuka on 6.4.18.
//

#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    uint8_t *data;
    size_t size;
    size_t capacity;
    unsigned int i;
}
ByteBuffer;

ByteBuffer *byte_buffer_new(void);
ByteBuffer *byte_buffer_clone(const ByteBuffer *buffer);
ByteBuffer *byte_buffer_from_bytes(const uint8_t bytes[], size_t length);
ByteBuffer *byte_buffer_sub(const ByteBuffer *buffer, int from);
void byte_buffer_free(ByteBuffer *buffer);
void byte_buffer_push_int8(ByteBuffer *buffer, uint8_t byte);
void byte_buffer_push_int16(ByteBuffer *buffer, uint16_t bytes);
void byte_buffer_push_int32(ByteBuffer *buffer, uint32_t bytes);
void byte_buffer_push(ByteBuffer *buffer, const uint8_t bytes[], unsigned int length);
void byte_buffer_push_zeros(ByteBuffer *buffer, int n);
void byte_buffer_print(const ByteBuffer *buffer);

uint8_t *byte_buffer_pop(ByteBuffer *buffer, int n);
uint8_t byte_buffer_pop_int8(ByteBuffer *buffer);
uint16_t byte_buffer_pop_int16(ByteBuffer *buffer);
uint32_t byte_buffer_pop_int32(ByteBuffer *buffer);

uint8_t byte_buffer_peek_int8(const ByteBuffer *buffer);