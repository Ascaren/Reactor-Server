#include "reactor.h"

struct reactor_ctx{
    int epoll_fd;
    size_t size;
    eh_list *ehl;
};


// Add socket to epoll 
static void add_eh(reactor *self, event_handler *eh){
    struct epoll_event e;
    memset(&e, 0, sizeof(struct epoll_event));
    e.events = EPOLLIN;
    e.data.fd = eh->get_fd(eh);
    if(!self->ctx->ehl->add_eh(self->ctx->ehl, eh)){
        if(epoll_ctl(self->ctx->epoll_fd, EPOLL_CTL_ADD, eh->get_fd(eh), &e) < 0) {
            fprintf(stderr, "Cannot add socket to epoll\n");
            self->ctx->ehl->rm_eh(self->ctx->ehl, eh);
            close(self->ctx->epoll_fd);
        }
    }
}

// Remove socket from epoll
static void rm_eh(reactor *self, event_handler *eh){
    struct epoll_event e;
    memset(&e, 0, sizeof(struct epoll_event));
    e.events = EPOLLIN;
    e.data.fd = eh->get_fd(eh);
    if (epoll_ctl(self->ctx->epoll_fd, EPOLL_CTL_DEL, eh->get_fd(eh), &e) < 0) {
        fprintf(stderr, "Cannot remove socket from epoll\n");
        close(self->ctx->epoll_fd);
        return;
    }
    self->ctx->ehl->rm_eh(self->ctx->ehl, eh);
}

// Waiting for events 
static void event_loop(reactor *self){
    int i=0;
    struct epoll_event *es=malloc(self->ctx->size*sizeof(struct epoll_event));
    memset(es, 0, self->ctx->size*sizeof(struct epoll_event));

    while(1){
        i = epoll_wait(self->ctx->epoll_fd, es, self->ctx->size, -1);
        if (i < 0) {
            fprintf(stderr, "Cannot wait for events\n");
            close(self->ctx->epoll_fd);
            return;
        }

        for(--i;i>-1;--i){
            event_handler *eh = self->ctx->ehl->get_by_fd(self->ctx->ehl, es[i].data.fd);
            if(eh->handle_event(eh, &es[i]))
                self->rm_eh(self, eh);
        }
    }
}

// Constructor
reactor* create_reactor(int size){
    int epoll_fd = epoll_create(size);
    if (epoll_fd < 0) {
        fprintf(stderr, "Cannot create epoll\n");
        return NULL;
    }
    reactor *r = malloc(sizeof(reactor));
    reactor_ctx *ctx = malloc(sizeof(reactor_ctx));
    eh_list *ehl = create_ehl(size);

    r->ctx = ctx;
    r->ctx->epoll_fd = epoll_fd;
    r->ctx->ehl = ehl;
    r->ctx->size = size;

    r->add_eh = add_eh;
    r->rm_eh = rm_eh;
    r->event_loop = event_loop;

    return r;
}

// Destructor
void destroy_reactor(reactor *r){
    destroy_ehl(r->ctx->ehl);
    close(r->ctx->epoll_fd);
    free(r->ctx);
    free(r);
}
