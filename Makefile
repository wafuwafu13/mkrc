.PHONY: build
build:
	cc mkr.c -o mkr

.PHONY: mkrc
mkrc: build
	./mkr ${ARGS}
