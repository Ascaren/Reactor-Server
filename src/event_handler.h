#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

typedef struct eh_ctx eh_ctx;

typedef struct event_handler{
    eh_ctx *ctx;
    int (*get_fd) (struct event_handler *self);
    int (*handle_event) (struct event_handler *self, const struct epoll_event *e);
    void (*destroy) (struct event_handler *self);
}event_handler;

#endif
