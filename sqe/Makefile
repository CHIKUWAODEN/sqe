MAKE=make
SEL_INSTALL_DIR=~/.sqe/sel
SQE_DEV_ROOT=$(shell pwd)
export SQE_DEV_ROOT

sqe32:
	cd sel; $(MAKE);
	cd sqe; $(MAKE);

.PHONY: install_extend
install_extend:
	cd sel; $(MAKE) install;

.PHONY: test
test:
	cd test; $(MAKE) test;

.PHONY: archive
archive:
	$(MAKE) clean;
	./archive.sh

.PHONY: clean
clean:
	@rm -f ./bin/*
	@rm -f ./sqe.tar.gz
	@rm -rf ./sqe.*
	@cd sqe; $(MAKE) clean;
	@cd sel; $(MAKE) clean;
