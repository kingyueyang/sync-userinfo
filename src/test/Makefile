all:test_user_basic_info

CC=gcc -std=c99 -g
PROTO=../../proto

test_user_basic_info:test_user_basic_info.o ${PROTO}/community.pb-c.c
	${CC} -o $@ $^ -lprotobuf-c -lcurl
  
test_user_basic_info.o:test_user_basic_info.c
	${CC} -c $<

.PYHON:
clean:
	rm -f *.o
	rm -f test_user_basic_info
