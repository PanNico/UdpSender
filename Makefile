TARGET=bin/udp_sender
CFLAGS:=$(CFLAGS) -I./include -Wall
LDFLAGS:=$(LDFLAGS)
PREPARE=obj bin

all: $(PREPARE) $(TARGET)

udp_sender: $(PREPARE) bin/udp_sender

clean:
	rm -rf bin obj

$(PREPARE):
	mkdir $@

bin/udp_sender: obj/udp_sender.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

obj/udp_sender.o: src/udp_sender.c include/udp_sender.h
	$(CC) -c $(CFLAGS) $< -o $@

