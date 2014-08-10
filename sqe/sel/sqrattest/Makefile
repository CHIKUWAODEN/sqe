MAJOR   = 1
MINOR   = 0
OUT     = libsqrattest.so.${MAJOR}.${MINOR}

all:
	gcc --shared -fPIC -o ${OUT} sqrattest.cpp
	rm -f libsqrattest.so
	rm -f libsqrattest.so.${MAJOR}
	ln -s ${OUT} libsqrattest.so.${MAJOR}
	ln -s libsqrattest.so.${MAJOR} libsqrattest.so

.PHONY: install
install:
	mkdir -p ~/.sqe/sel
	cp ./libsqrattest.* ~/.sqe/sel/

.PHONY: clean
clean:
	rm -f ./libsqrattest.*
