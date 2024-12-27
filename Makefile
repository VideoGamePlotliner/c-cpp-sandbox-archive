# https://www.gnu.org/software/make/manual/make.html#Recursion
# https://www.gnu.org/software/make/manual/make.html#Combine-By-Prerequisite
# https://www.gnu.org/software/make/manual/make.html#Pattern-Examples
# https://www.gnu.org/software/make/manual/make.html#Automatic-Variables

.PHONY: all clean
all clean:
	$(MAKE) $@ -C c
	$(MAKE) $@ -C cpp
