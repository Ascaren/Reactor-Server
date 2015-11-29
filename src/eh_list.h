#ifndef EH_LIST_H
#define EH_LIST_H

#include <stdlib.h>
#include "event_handler.h"

typedef struct eh_list_ctx eh_list_ctx;

typedef struct eh_list{
    eh_list_ctx *ctx;
    int (*add_eh)(struct eh_list *self, event_handler *eh);
    int (*rm_eh)(struct eh_list *self, event_handler *eh);
    event_handler* (*get_by_fd)(struct eh_list *self, int fd);
} eh_list;

eh_list* create_ehl(size_t size);
void destroy_ehl(eh_list* ehl);

#endif
