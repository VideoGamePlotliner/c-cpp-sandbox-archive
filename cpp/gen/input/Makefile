# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#index-C_002b_002b_002c-rule-to-compile

CPPFLAGS += -g -Wall -Werror -Wpedantic

.PHONY: all
all: cpp_gen_main

cpp_gen_main: cpp_gen_main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $+
# https://www.gnu.org/software/make/manual/make.html#Utilities-in-Makefiles
# Avoid using '-p' with 'mkdir'.
# https://www.gnu.org/software/make/manual/make.html#Errors
	-mkdir ../output
	./$@ > ../output/cpp_gen_trie.hpp

cpp_gen_main.o:

.PHONY: clean
clean:
	rm -rf ../output
	rm -f cpp_gen_main *.o
