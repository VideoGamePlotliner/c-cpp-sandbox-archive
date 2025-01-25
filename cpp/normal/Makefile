# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#index-C_002b_002b_002c-rule-to-compile
# https://www.gnu.org/software/make/manual/make.html#Variables-Simplify
# https://www.gnu.org/software/make/manual/make.html#Wildcard-Pitfall
# https://www.gnu.org/software/make/manual/make.html#Wildcard-Function

CPPFLAGS += -g

.PHONY: all
all: cpp_main

cpp_main: cpp_main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $+

cpp_main.o: $(wildcard *.hpp) ../gen/output/cpp_gen_trie.hpp

.PHONY: clean
clean:
	rm -f cpp_main *.o
