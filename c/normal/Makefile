# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#make-Deduces
# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#index-CPPFLAGS

CPPFLAGS += -g

.PHONY: all
all: c_main

c_main: c_main.o c_headers.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

c_main.o: c_headers.o c_headers.h

c_headers.o: c_headers.h

.PHONY: clean
clean:
	rm -f c_main *.o
