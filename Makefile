.PHONY: fmt
fmt:
	clang-format -i *.c */*.c */*.h

.PHONY: build
build: fmt
	cc mkr.c mackerelclient/env.c -o mkr

.PHONY: mkrc
mkrc: build
	./mkr ${ARGS}
