SUBDIRS = bdd hscc16 khepera

.PHONY: all clean

all:
	for dir in $(SUBDIRS); do $(MAKE) all -C $$dir $@; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir $@; done
