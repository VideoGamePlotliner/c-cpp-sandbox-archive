# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#make-Deduces
# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#index-CPPFLAGS
# https://www.gnu.org/software/make/manual/make.html#index-C_002c-rule-to-compile

# https://www.man7.org/linux/man-pages/man7/unix.7.html
# "EXAMPLES" section of https://www.man7.org/linux/man-pages/man3/inet_pton.3.html
# "EXAMPLES" section of https://www.man7.org/linux/man-pages/man2/sendmmsg.2.html
# "EXAMPLES" section of https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html

CPPFLAGS += -g -Wall -Werror -Wpedantic

ALL_MAN7_OUTPUTS = client server inet_pton_example sendmmsg_example getaddrinfo_example_client getaddrinfo_example_server select_tut_example

.PHONY: all
all: $(ALL_MAN7_OUTPUTS)

client: client.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

client.o: connection.h

server: server.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

server.o: connection.h

inet_pton_example: inet_pton_example.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

inet_pton_example.o:

sendmmsg_example: sendmmsg_example.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

sendmmsg_example.o:

getaddrinfo_example_client: getaddrinfo_example_client.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

getaddrinfo_example_client.o:

getaddrinfo_example_server: getaddrinfo_example_server.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

getaddrinfo_example_server.o:

select_tut_example: select_tut_example.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

select_tut_example.o:

.PHONY: clean
clean:
	rm -f $(ALL_MAN7_OUTPUTS) *.o
