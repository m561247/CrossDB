help:
	@echo "make build                Build crossdb library and tool"
	@echo "make debug                Build crossdb library and tool with debug"
	@echo "make run                  Run crossdb tool"
	@echo "make clean                Clean build result"
	@echo "make install              Install crossdb(lib&tool&header) to Linux/FreeBSD"
	@echo "make uninstall            Uninstall crossdb from Linux/FreeBSD"
	@echo "make installmac           Install crossdb(lib&tool&header) to MacOS"
	@echo "make uninstallmac         Uninstall crossdb from MacOS"
	@echo "make example              Build and run example (need to install crossdb first)"
	@echo "make bench                Build and run bench test (need to install crossdb first)"
	@echo "make bench-sqlite         Build and run sqlite bench test (need to install sqlite3 first)"

.PHONY: build
build:
	mkdir -p build
	$(CC) -o build/libcrossdb.so -fPIC -shared -lpthread -O2 src/crossdb.c
	$(CC) -o build/crossdb src/main.c -lpthread -O2
	cp include/crossdb.h build/

debug:
	$(CC) -o build/libcrossdb.so -fPIC -lpthread -shared -g src/crossdb.c
	$(CC) -o build/crossdb src/main.c -lpthread -g
	cp include/crossdb.h build/

run:
	build/crossdb

clean:
	rm -rf build/*
	make -C examples/c/ clean
	make -C bench/basic/ clean

wall:
	$(CC) -o build/crossdb src/main.c -lpthread -O2 -Wall

gdb:
	$(CC) -o build/crossdb src/main.c -lpthread -g
	gdb build/crossdb

install:
	install -c build/libcrossdb.so /usr/lib/
	install -c build/crossdb.h /usr/include/
	install -c build/crossdb /usr/bin/

uninstall:
	rm -rf /usr/lib/libcrossdb.so
	rm -rf /usr/include/crossdb.h
	rm -rf /usr/bin/crossdb

installmac:
	$(CC) -o build/libcrossdb.so -dynamiclib -lpthread -O2 src/crossdb.c
	install -c build/libcrossdb.so /usr/local/lib/
	install -c build/crossdb.h /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
	install -c build/crossdb /usr/local/bin/

uninstallmac:
	rm -rf /usr/local/lib/libcrossdb.so
	rm -rf /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/crossdb.h
	rm -rf /usr/local/bin/crossdb

example:
	make -C examples/c/

.PHONY: bench
bench:
	make -C bench/basic/

bench-sqlite:
	make -C bench/basic/ sqlite
