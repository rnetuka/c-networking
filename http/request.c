//
// Created by rnetuka on 28.3.18.
//

#include <stdlib.h>
#include <string.h>

#include "../dns/resolver.h"
#include "request.h"

#define NEWLINE "\r\n"
#define SPACE " "
#define SEPARATOR ": "

static HttpRequest *http_request_new(const char *method, const char *path, const char *body)
{
    HttpRequest *request = malloc(sizeof(HttpRequest));
    request->method = malloc(strlen(method) + 1);
    request->path = malloc(strlen(path) + 1);
    request->headers = calloc(1, sizeof(HttpHeader *));
    request->headers[0] = NULL;

    strcpy(request->method, method);
    strcpy(request->path, path);

    if (body)
    {
        request->body = malloc(strlen(body) + 1);
        strcpy(request->body, body);
    }

    return request;
}

HttpRequest *http_get_request_new(const char *path)
{
    return http_request_new("GET", path, NULL);
}

HttpRequest *http_put_request_new(const char *path, const char *body)
{
    return http_request_new("PUT", path, body);
}

HttpRequest *http_post_request_new(const char *path, const char *body)
{
    return http_request_new("POST", path, body);
}

HttpRequest *http_delete_request_new(const char *path)
{
    return http_request_new("DELETE", path, NULL);
}

HttpRequest *http_head_request_new(const char *path)
{
    return http_request_new("HEAD", path, NULL);
}

HttpRequest *http_options_request_new(const char *path)
{
    return http_request_new("OPTIONS", path, NULL);
}

static void http_headers_free(HttpHeader **headers)
{
    while (*headers)
    {
        http_header_free(*headers);
        headers++;
    }
}

void http_request_free(HttpRequest *request)
{
    free(request->method);
    free(request->path);

    http_headers_free(request->headers);
    free(request->headers);

    if (request->body)
        free(request->body);

    free(request);
}

static HttpHeader *http_request_find_header(HttpHeader **headers, const char *name)
{
    HttpHeader **headers_ = headers;
    while (*headers_)
    {
        if (strcmp(name, (*headers)->name) == 0)
            return *headers_;

        headers_++;
    }
    return NULL;
}

static unsigned http_request_header_count(const HttpRequest *request)
{
    unsigned count = 0;
    HttpHeader **headers = request->headers;
    while (*headers)
    {
        count++;
        headers++;
    }
    return count;
}

void http_request_set_header(HttpRequest *request, const char *name, const char *value)
{
    HttpHeader *header = http_request_find_header(request->headers, name);
    if (header)
    {
        free(header->value);
        header->value = malloc(strlen(value) + 1);
        strcpy(header->value, value);
    }
    else
    {
        unsigned header_count = http_request_header_count(request);
        request->headers = realloc(request->headers, header_count + 2);
        request->headers[header_count] = http_header_new(name, value);
        request->headers[header_count + 1] = NULL;
    }
}

const char *http_request_get_header(const HttpRequest *request, const char *name)
{
    HttpHeader *header = http_request_find_header(request->headers, name);
    return header ? header->value : NULL;
}

const char *http_response_get_header(const HttpResponse *response, const char *name)
{
    HttpHeader *header = http_request_find_header(response->headers, name);
    return header ? header->value : NULL;
}

static const char *url_path(const char *url)
{
    if (strncmp(url, "https", 5) == 0)
        return url + 5 + 3;
    else if (strncmp(url, "http", 4) == 0)
        return url + 4 + 3;
    else
        return NULL;
}

unsigned http_request_string_length(const HttpRequest *request)
{
    unsigned length = 0;
    length += strlen(request->method);
    length += strlen(SPACE);
    length += strlen(request->path);
    length += strlen(SPACE);
    length += strlen("HTTP/1.1");
    length += strlen(NEWLINE);

    HttpHeader **headers = request->headers;
    while (*headers)
    {
        HttpHeader *header = *headers;
        length += strlen(header->name);
        length += strlen(SEPARATOR);
        length += strlen(header->value);
        length += strlen(NEWLINE);
        headers++;
    }

    if (request->body)
        length += strlen(request->body);

    length += strlen(NEWLINE);
    return length;
}

HttpResponse *http_request_send(const char *domain, const HttpRequest *request)
{
    HostAddress *address = get_host_address(domain);

    if (address->ipv4_address)
        return http_ipv4_request_send(address->ipv4_address, request);

    if (address->ipv6_address)
        return http_ipv6_request_send(address->ipv6_address, request);

    return NULL;
}

HttpResponse *http_ipv4_request_send(const IPv4Address *address, const HttpRequest *request)
{
    char string[http_request_string_length(request)];
    strcpy(string, request->method);
    strcat(string, SPACE);
    strcat(string, request->path);
    strcat(string, SPACE);
    strcat(string, "HTTP/1.1");
    strcat(string, NEWLINE);

    HttpHeader **headers = request->headers;
    while (*headers)
    {
        HttpHeader *header = *headers;
        strcat(string, header->name);
        strcat(string, SEPARATOR);
        strcat(string, header->value);
        strcat(string, NEWLINE);
        headers++;
    }

    if (request->body)
        strcat(string, request->body);

    strcat(string, NEWLINE);

    TcpConnection *connection = tcp_ipv4_connection_open(address, 80);
    ByteBuffer *buffer = tcp_connection_send_string(connection, string);
    //TODO parse response
    //tcp_connection_close(connection);
    //tcp_connection_free(connection);
    //byte_buffer_free(buffer);
}

HttpResponse *http_ipv6_request_send(const IPv6Address *address, const HttpRequest *request)
{

}

HttpResponse *http_get(const char *url)
{
    HttpRequest *get = http_get_request_new(url_path(url));



    return NULL;
}

HttpResponse *http_post(const char *url, const char *data)
{

}