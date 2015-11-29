#ifndef CLIENT_EH_H
#define CLIENT_EH_H

#include "event_handler.h"

event_handler* create_client_eh(int cli_fd);
void create_userlist(size_t size);

static int sendbytes(const char* msg, int fd);
static size_t receivebytes(int fd, char **msg);

#endif
