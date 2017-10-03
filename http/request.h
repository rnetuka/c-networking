//
// Created by rnetuka on 28.3.18.
//

#pragma once

#include "../ip.h"
#include "../tcp/connection.h"
#include "../url.h"
#include "response.h"

typedef struct
{
    char *method;
    char *path;
    HttpHeader **headers;
    char *body;
}
HttpRequest;

HttpRequest *http_get_request_new(const char *path);
HttpRequest *http_put_request_new(const char *path, const char *body);
HttpRequest *http_post_request_new(const char *path, const char *body);
HttpRequest *http_delete_request_new(const char *path);
HttpRequest *http_head_request_new(const char *path);
HttpRequest *http_options_request_new(const char *path);
void http_request_free(HttpRequest *request);
void http_request_set_header(HttpRequest *request, const char *name, const char *value);
const char *http_request_get_header(const HttpRequest *request, const char *name);
unsigned http_request_string_length(const HttpRequest *request);



HttpResponse *http_request_send(const char *domain, const HttpRequest *request);
HttpResponse *http_ipv4_request_send(const IPv4Address *address, const HttpRequest *request);
HttpResponse *http_ipv6_request_send(const IPv6Address *address, const HttpRequest *request);
HttpResponse *http_get(const char *url);
HttpResponse *http_post(const char *url, const char *data);