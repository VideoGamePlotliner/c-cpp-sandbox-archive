# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#make-Deduces
# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#index-CPPFLAGS
# https://www.gnu.org/software/make/manual/make.html#index-C_002c-rule-to-compile

# https://www.man7.org/linux/man-pages/man7/unix.7.html

CPPFLAGS += -g

.PHONY: all
all: client server

client: client.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

client.o: connection.h

server: server.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

server.o: connection.h

.PHONY: clean
clean:
	rm -f client server *.o
