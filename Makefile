BUILD_SUBDIRS = cudd-3.0.0
CLEAR_SUBDIRS = cudd-3.0.0 examples
VERSION = $(shell cat VERSION | grep -P '\d+.\d+.\d+' -o)

.PHONY: all clean

all:
	for dir in $(BUILD_SUBDIRS); do $(MAKE) all -C $$dir $@; done

clean:
	rm -f -r Debug
	rm -f -r Release
	rm -f -r x64
	rm -f ./bin/pFaces.*
	rm -f ./bin/*.log
	rm -f ./bin/*.tune
	rm -f ./bin/*.render	
	rm -f ./bin/*.bin
	rm -f ./bin/*.dll	
	rm -f ./bin/*.bits	
	rm -f ./bin/*.dot
	rm -f ./bin/*.dd
	rm -f ./bin/*.tmp
	rm -f ./include/*.gch
	for dir in $(CLEAR_SUBDIRS); do $(MAKE) clean -C $$dir $@; done
	find . -name '~*' -delete
	cd examples
	find . -name \*.raw -delete
	find . -name \*.bits -delete
	find . -name \*.bitmap -delete
	find . -name \*.bdd -delete	
	find . -name \*.scs -delete
	find . -name \*.o -delete
	find . -name \*.iobg -delete
	find . -name \*.ipdb -delete
	find . -name \*.log -delete
	find . -name \*.driver -delete
	find . -name \*.rende -delete
	find . -name \*.lib -delete
	find . -name \*.dll -delete
	find . -name \*.so -delete
	find . -name \*.exp -delete
	find . -name \*.ilk -delete
	find . -name \*.pdb -delete
	find . -name ".DS_Store" -type f -delete
