# Help from:
# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#make-Deduces
# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#index-CPPFLAGS
# https://www.gnu.org/software/make/manual/make.html#index-C_002c-rule-to-compile

# Copied/adapted from:
# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#Variables-Simplify
# https://www.gnu.org/software/make/manual/make.html#make-Deduces
# https://www.gnu.org/software/make/manual/make.html#Combine-By-Prerequisite
# https://www.gnu.org/software/make/manual/make.html#Cleanup

CPPFLAGS += -g -Wall -Werror -Wpedantic

objects = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

.PHONY: all
all: edit

edit: $(objects)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $+

# It is implied that the following files exist:
# main.c
# kbd.c
# command.c
# display.c
# insert.c
# search.c
# files.c
# utils.c

main.o: defs.h
kbd.o: defs.h command.h
command.o: defs.h command.h
display.o: defs.h buffer.h
insert.o: defs.h buffer.h
search.o: defs.h buffer.h
files.o: defs.h buffer.h command.h
utils.o: defs.h

# OR you could use this syntax:
# $(objects) : defs.h
# kbd.o command.o files.o : command.h
# display.o insert.o search.o files.o : buffer.h

.PHONY: clean
clean:
	rm -f edit *.o
