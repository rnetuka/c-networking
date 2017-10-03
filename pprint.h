//
// Created by root on 10.4.18.
//

#pragma once

#include <stdint.h>
#include <stdlib.h>

//void pprint_field(int length, const char *name, const char *format, const void *value);

#define pprint_field(pad_to, field_name, field_value) \
            _Generic((field_value), \
                char *: pprint_field_s, \
                char[]: pprint_field_s, \
                uint8_t: pprint_field_i, \
                uint16_t: pprint_field_i, \
                const char *: pprint_field_s, \
                default: pprint_field_s \
            )(pad_to, field_name, field_value)

void pprint(const char *string);
void pprint_field_i(const char *field_name, int field_value);
void pprint_field_s(const char *field_name, const char *field_value);
void pprint_field_v(const char *field_name, const char *format, ...);
void pprint_flush(void);