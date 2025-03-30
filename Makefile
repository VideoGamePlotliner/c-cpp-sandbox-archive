# https://www.gnu.org/software/make/manual/make.html#Recursion
# https://www.gnu.org/software/make/manual/make.html#Combine-By-Prerequisite
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
# https://www.gnu.org/software/make/manual/make.html#index-test-_0028standard-target_0029

.PHONY: all
all:
	$(MAKE) $@ -C c
	$(MAKE) $@ -C cpp

.PHONY: clean
clean:
	$(MAKE) $@ -C c
	$(MAKE) $@ -C cpp
	rm -f *.d

.PHONY: check
check:
	$(MAKE) $@ -C cpp
