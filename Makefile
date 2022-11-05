.PHONY: fmt
fmt:
	clang-format -i *.c */*.c */*.h

.PHONY: build
build: fmt
	cc mkr.c mackerelclient/env.c mackerelclient/host.c -l curl -l jansson -o mkr

.PHONY: mkrc
mkrc: build
	./mkr ${ARGS}
