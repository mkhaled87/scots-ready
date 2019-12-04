SUBDIRS = dcdc unicycle vehicle1 vehicle2

.PHONY: all clean

all:
	for dir in $(SUBDIRS); do $(MAKE) all -C $$dir $@; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir $@; done
