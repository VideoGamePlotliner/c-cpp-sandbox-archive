# https://www.gnu.org/software/make/manual/make.html#Recursion
# https://www.gnu.org/software/make/manual/make.html#Combine-By-Prerequisite
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#index-test-_0028standard-target_0029

.PHONY: all
all:
	$(MAKE) $@ -C gen
	$(MAKE) $@ -C normal

.PHONY: clean
clean:
# If ../gen/output/cpp_gen_trie.hpp does not exist, create it temporarily and rm it when you're done.
	$(MAKE) -C gen
# Now clean.
	$(MAKE) $@ -C normal
	$(MAKE) $@ -C gen

.PHONY: check
check:
	$(MAKE) $@ -C normal
