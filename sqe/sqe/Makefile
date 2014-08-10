OUT = ./sqe
LIB = -lsquirrel -lsqstdlib -ldl
SRCS = main.cpp app.cpp sqdlfcn.cpp libsc.cpp loadmap.cpp loadpath.cpp

sqe32:
	g++ -g -Wall -s -fno-exceptions -rdynamic -fno-rtti -I./ -o ${OUT} ${SRCS} ${LIB}
#	g++ -Wall -O2 -s -fno-exceptions -rdynamic -fno-rtti -I./ -o ${OUT} ${SRCS} ${LIB}
	@cp ./sqe ${SQE_DEV_ROOT}/bin/

.PHONY : clean
clean:
	@rm -f ${OUT}
	@rm -f *.o
