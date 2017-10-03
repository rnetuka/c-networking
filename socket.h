//
// Created by rnetuka on 28.3.18.
//

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "bbuffer.h"
#include "ethernet.h"

typedef struct
{
    const char *interface_name;
    int file_descriptor;
    bool connected;
}
Socket;

Socket *socket_new(void);
void socket_free(Socket *socket);
void socket_write(const Socket *socket, uint8_t *data, size_t length);
void socket_write_buf(const Socket *socket, const ByteBuffer *buffer);
EthernetFrame *socket_read(const Socket *socket);