#ifndef REACTOR_H
#define REACTOR_H

#include "eh_list.h"
#include "event_handler.h"

typedef struct reactor_ctx reactor_ctx;

typedef struct reactor{
    reactor_ctx *ctx;
    void (* add_eh)(struct reactor *self, event_handler *eh);
    void (* rm_eh)(struct reactor *self, event_handler *eh);
    void (* event_loop)(struct reactor *self);
}reactor;

reactor* create_reactor(int size);
void destroy_reactor(reactor *r);

#endif
