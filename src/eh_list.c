#include "eh_list.h"

struct eh_list_ctx{
    size_t size;
    event_handler **handlers;
};


//
static event_handler* get_by_fd(eh_list *ehl, int fd){
    size_t i = 0;
    for(;i<ehl->ctx->size;i++){
        if(ehl->ctx->handlers[i] && ehl->ctx->handlers[i]->get_fd(ehl->ctx->handlers[i])==fd)
            return ehl->ctx->handlers[i];
    }
    return 0; //return NULL if fd isnt found
}

// 
static int add_eh(eh_list *ehl, event_handler *eh){
    size_t i=0;
    if(get_by_fd(ehl,eh->get_fd(eh)) != 0)
        return 1; //event handler already exists
    for (;i<ehl->ctx->size;++i){
        if(ehl->ctx->handlers[i]==0){
            ehl->ctx->handlers[i]=eh;
            return 0;
        }
    }
    return 1; //there is no place in event handler list
}

// 
static int rm_eh(eh_list *ehl, event_handler *eh){
    size_t i=0;
    int fd = eh->get_fd(eh);

    for(;i<ehl->ctx->size;++i){
        if(ehl->ctx->handlers[i] && ehl->ctx->handlers[i]->get_fd(ehl->ctx->handlers[i])==fd){
            ehl->ctx->handlers[i]->destroy(ehl->ctx->handlers[i]);
            ehl->ctx->handlers[i]=0;
            return 0;
        }
    }
    return 1; //user not in list
}

// Constructor
eh_list* create_ehl(size_t size){
    eh_list* res = malloc(sizeof(eh_list));
    res->ctx = malloc(sizeof(eh_list_ctx));
    res->ctx->size = size;
    res->ctx->handlers = malloc(size*sizeof(event_handler*));
    memset(res->ctx->handlers, 0, size*sizeof(event_handler*));

    res->add_eh=add_eh;
    res->rm_eh=rm_eh;
    res->get_by_fd=get_by_fd;

    return res;
}

// Destructor
void destroy_ehl(eh_list* ehl){
    size_t i = 0;
    for(;i<ehl->ctx->size;++i){
        if(ehl->ctx->handlers[i]){
            ehl->ctx->handlers[i]->destroy(ehl->ctx->handlers[i]);
            ehl->ctx->handlers[i]=0;
        }
    }
    free(ehl->ctx->handlers);
    free(ehl->ctx);
    free(ehl);
}
