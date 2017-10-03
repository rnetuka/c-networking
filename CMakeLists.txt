cmake_minimum_required(VERSION 3.9)
project(Networking C)

set(CMAKE_C_STANDARD 11)

add_executable(Networking main.c socket.c socket.h http/request.c http/request.h tcp/connection.c tcp/connection.h url.c url.h ethernet.h ethernet.c mac.h mac.c os.h ipaddress.h udp.h ip.h udp.c ip.c ipaddress.c os.c arp/packet.h arp/packet.c bbuffer.h bbuffer.c pprint.h pprint.c dns/question.h dns/answer.h dns/question.c dns/answer.c dns/util.h dns/util.c dns/message.h dns/message.c dns/encoding.h dns/encoding.c dns/cache.h dns/cache.c dns/resolver.c dns/resolver.h arp/resolver.h arp/resolver.c arp/cache.h arp/cache.c dns/result.h tcp/packet.h tcp/packet.c http/response.h http/response.c http/header.h http/header.c)