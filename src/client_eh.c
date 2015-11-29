#include <stdlib.h>
#include <stdio.h>
#include "client_eh.h"

struct eh_ctx {
    int fd;
};


// Get message from socket
static size_t receivebytes(int fd, char **msg){
    char buff[100];
    size_t len=0;

    len=read(fd,buff,10000);
    if(len < 1) return 0;

    (*msg)=malloc((len+1)*sizeof(char));
    strncpy((*msg),buff,len);
    (*msg)[len]='\0';

    return len;
}

// Send message to socket
static int sendbytes(const char* msg, int fd){
    size_t len=strlen(msg);

    if(len>0)
        write(fd,msg,len);

    return 0;
}

// Getter
static int get_fd(event_handler *self){
    return self->ctx->fd;
}

// Event Handler - Client
static int handle_event(event_handler *self, const struct epoll_event *e){
    size_t msg_len = -1; 
    char *buff = 0;

    if (e->events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP))
        return 1;
    else if (e->events & EPOLLIN) {
        msg_len = receivebytes(e->data.fd, &buff);
	printf("Command: %s", buff);
	sendbytes("Done",self->ctx->fd);
	// InProgress: Operation handling
   }
    return 0;
}

// Destructor
static void destroy_client_eh(event_handler *self){
    close(self->ctx->fd);
    free(self->ctx);
    free(self);
}

// Constructor
event_handler* create_client_eh(int cli_fd){
    event_handler *s_eh = malloc(sizeof(event_handler));
    eh_ctx *ctx = malloc(sizeof(eh_ctx));

    ctx->fd = cli_fd;

    s_eh->ctx = ctx;
    s_eh->get_fd = get_fd;
    s_eh->handle_event = handle_event;
    s_eh->destroy = destroy_client_eh;
    return s_eh;
}



