#Kubson zgin
CFLAGS = -Wall -g
LDFLAGS = 
SOURCES = ../src/main.c ../src/reactor.c ../src/eh_list.c ../src/server_eh.c ../src/client_eh.c    
NAME = server

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(NAME)
