MAJOR   = 1
MINOR   = 0
OUT     = libhello.so.${MAJOR}.${MINOR}

all:
	gcc -v --shared -fPIC -o ${OUT} hello.c 
	rm -f libhello.so
	rm -f libhello.so.${MAJOR}
	ln -s ${OUT} libhello.so.${MAJOR}
	ln -s libhello.so.${MAJOR} libhello.so

.PHONY: install
install:
	mkdir -p ~/.sqe/sel
	cp ./libhello.* ~/.sqe/sel/

.PHONY: clean
clean:
	rm -f ./libhello.*
