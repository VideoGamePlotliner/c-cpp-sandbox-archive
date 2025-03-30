# https://www.gnu.org/software/make/manual/make.html#Simple-Makefile
# https://www.gnu.org/software/make/manual/make.html#index-C_002b_002b_002c-rule-to-compile
# https://www.gnu.org/software/make/manual/make.html#Variables-Simplify
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#make-Deduces
# https://www.gnu.org/software/make/manual/make.html#Rule-Introduction
# https://www.gnu.org/software/make/manual/make.html#Pattern_002dspecific

CPPFLAGS += -g -Wall -Werror -Wpedantic

MAIN_FILE_0001 = cpp_apportionment_test_main
MAIN_FILE_0002 = cpp_args_test_main
MAIN_FILE_0003 = cpp_assert_test_main
MAIN_FILE_0004 = cpp_gen_trie_test_main
MAIN_FILE_0005 = cpp_protoent_test_main
MAIN_FILE_0006 = cpp_sockets_test_main
MAIN_FILE_0007 = cpp_trie_test_main
MAIN_FILE_0008 = man7_getaddrinfo_example_test_main
MAIN_FILE_0009 = man7_inet_pton_example_test_main
MAIN_FILE_0010 = man7_select_tut_example_test_main
MAIN_FILE_0011 = man7_sendmmsg_example_test_main
MAIN_FILE_0012 = man7_test_main

# https://www.gnu.org/software/make/manual/make.html#Wildcard-Pitfall
# https://www.gnu.org/software/make/manual/make.html#Wildcard-Function
ALL_CPP_FILES = $(wildcard *.cpp)

# https://www.gnu.org/software/make/manual/make.html#Text-Functions
ALL_MAIN_FILES = $(ALL_CPP_FILES:.cpp=)

# https://www.gnu.org/software/make/manual/make.html#Automatic-Prerequisites
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#Preprocessor-Options
%.d: %.cpp
	set -e; rm -f $@; \
	$(CXX) -M $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;

# https://www.gnu.org/software/make/manual/make.html#Include
include $(ALL_CPP_FILES:.cpp=.d)

.PHONY: all
all: $(ALL_MAIN_FILES)

# https://www.gnu.org/software/make/manual/make.html#Secondary-Expansion
.SECONDEXPANSION:
$(ALL_MAIN_FILES): $$@.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# https://www.gnu.org/software/make/manual/make.html#index-test-_0028standard-target_0029
# Remember to run 'make' before running 'make check'
.PHONY: check
check:
	./$(MAIN_FILE_0001)
	./$(MAIN_FILE_0002) AB CD EF GH IJ KL MN
	./$(MAIN_FILE_0003)
	./$(MAIN_FILE_0004)
	./$(MAIN_FILE_0005)
	./$(MAIN_FILE_0006)
	./$(MAIN_FILE_0007)
	./$(MAIN_FILE_0008)
	./$(MAIN_FILE_0009)
	./$(MAIN_FILE_0010)
	./$(MAIN_FILE_0011)
	./$(MAIN_FILE_0012)

.PHONY: clean
clean:
	rm -f $(ALL_MAIN_FILES) *.o *.d
