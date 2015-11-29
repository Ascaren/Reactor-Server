#include <stdlib.h>
#include "server_eh.h"

int main(int argc, const char *argv[])
{
    int port = 5050;
    int size = 10;
    reactor *r = create_reactor(size);
    event_handler *s_eh = create_server_eh(r,port,size);
    if (s_eh != NULL){
        r->add_eh(r,s_eh);
        r->event_loop(r);
    }

    return 0;
}
