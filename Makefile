.PHONY: all lib run clean

all: lib run

lib:
	$(MAKE) -C lib all

run:
	$(MAKE) -C run

clean:
	$(MAKE) -C lib clean || true
	$(MAKE) -C run clean || true
