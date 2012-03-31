all:test_user_basic_info test_user_header_info

CC=gcc -std=c99 -g
PROTO=../../proto

utils.o:../utils.c
	${CC} -c -o $@ $<

test_user_basic_info:test_user_basic_info.o utils.o ${PROTO}/community.pb-c.c
	${CC} -o $@ $^ -lprotobuf-c -lcurl
  
test_user_basic_info.o:test_user_basic_info.c
	${CC} -c $<

test_user_header_info:test_user_header_info.o utils.o ${PROTO}/community.pb-c.c
	${CC} -o $@ $^ -lprotobuf-c -lcurl
  
test_user_header_info.o:test_user_header_info.c
	${CC} -c $<

.PYHON:
clean:
	rm -f *.o
	rm -f test_user_basic_info
	rm -f test_user_header_info
