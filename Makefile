TOPTARGETS := all clean

SUBDIRS := $(wildcard */.)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

mc0 : v0/mc0.c
	gcc -g -Wall v0/mc0.c -o v0/mc0

mc1 : v1/mc1.c
	gcc -g -Wall v1/mc1.c -o v1/mc1

mc2 : v2/mc2.c
	gcc -g -Wall v2/mc2.c -o v2/mc2

.PHONY: $(TOPTARGETS) $(SUBDIRS)
