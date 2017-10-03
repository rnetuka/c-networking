//
// Created by root on 10.4.18.
//

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "pprint.h"

static char glb_buffer[4096] = {};
static int longest_field = 23;

void pprint(const char *string)
{
    strcat(glb_buffer, string);
}

void pprint_field_i(const char *field_name, int field_value)
{
    int field_name_length = strlen(field_name);

    //if (field_name_length > longest_field)
    //    longest_field = field_name_length;

    int padding_spaces = longest_field + 1 - field_name_length;

    char spaces[padding_spaces + 1];
    memset(spaces, ' ', (size_t) padding_spaces);
    spaces[padding_spaces] = '\0';

    char buffer[100] = {};
    strcpy(buffer, "    |-%s");
    strcat(buffer, spaces);
    strcat(buffer, ": ");
    strcat(buffer, "%i");
    strcat(buffer, "\n");

    char str[1000];
    sprintf(str, buffer, field_name, field_value);
    strcat(glb_buffer, str);
}

void pprint_field_s(const char *field_name, const char *field_value)
{
    long field_name_length = strlen(field_name);

    //if (field_name_length > longest_field)
    //    longest_field = field_name_length;

    long padding_spaces = longest_field + 1 - field_name_length;

    char spaces[padding_spaces + 1];
    memset(spaces, ' ', (size_t) padding_spaces);
    spaces[padding_spaces] = '\0';

    char buffer[100] = {};
    strcpy(buffer, "    |-%s");
    strcat(buffer, spaces);
    strcat(buffer, ": ");
    strcat(buffer, "%s");
    strcat(buffer, "\n");

    char str[1000];
    sprintf(str, buffer, field_name, field_value);
    strcat(glb_buffer, str);
}

void pprint_field_v(const char *field_name, const char *format, ...)
{
    long field_name_length = strlen(field_name);

    //if (field_name_length > longest_field)
    //    longest_field = field_name_length;

    long padding_spaces = longest_field + 1 - field_name_length;

    char spaces[padding_spaces + 1];
    memset(spaces, ' ', (size_t) padding_spaces);
    spaces[padding_spaces] = '\0';

    char buffer[100] = {};
    strcpy(buffer, "    |-%s");
    strcat(buffer, spaces);
    strcat(buffer, ": ");

    char buffer2[100] = {};
    strcpy(buffer2, format);
    strcat(buffer2, "\n");

    va_list args;
    va_start(args, format);

    //printf(buffer, field_name);
    //vprintf(buffer2, args);

    char str[100];
    sprintf(str, buffer, field_name);
    strcat(glb_buffer, str);

    char str2[100];
    vsprintf(str2, buffer2, args);
    strcat(glb_buffer, str2);

    va_end(args);
}

void pprint_flush(void)
{
    printf("%s", glb_buffer);

    memset(glb_buffer, 0, 4096);
    //longest_field = 0;
}