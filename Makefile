.PHONY: build
build:
	cc mkr.c mackerelclient/env.c -o mkr

.PHONY: mkrc
mkrc: build
	./mkr ${ARGS}
