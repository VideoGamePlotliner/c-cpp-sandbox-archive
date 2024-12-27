# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#index-C_002b_002b_002c-rule-to-compile

CPPFLAGS += -g

NON_MAIN_O_FILES = cpp_args.o cpp_trie.o
NON_MAIN_HPP_FILES = cpp_assert.hpp cpp_alias.hpp cpp_sockets.hpp man7_connection.hpp man7_client.hpp man7_server.hpp

.PHONY: all
all: cpp_main

cpp_main: cpp_main.o $(NON_MAIN_O_FILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $+

cpp_main.o: $(NON_MAIN_O_FILES) $(NON_MAIN_HPP_FILES)

cpp_args.o: cpp_args.hpp

cpp_trie.o: ../gen/output/cpp_gen_trie.hpp cpp_trie.hpp

cpp_assert.hpp:

cpp_alias.hpp:

cpp_sockets.hpp: cpp_assert.hpp cpp_alias.hpp

man7_connection.hpp:

man7_client.hpp: man7_connection.hpp

man7_server.hpp: man7_connection.hpp

.PHONY: clean
clean:
	rm -f cpp_main *.o
