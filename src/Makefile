all:sync-userinfo

CC=gcc -std=c99 -g
EXEC=sync-userinfo
OBJ=sync-userinfo.o receiver.o inf_basic_info.o inf_header_info.o

${EXEC}:${OBJ}
	${CC} -o $@ $^ -levent
  
sync-userinfo.o:sync-userinfo.c
	${CC} -c $<

receiver.o:receiver.c
	${CC} -c $<

inf_basic_info.o:inf_basic_info.c
	${CC} -c $<

inf_header_info.o:inf_header_info.c
	${CC} -c $<

.PYHON:
clean:
	rm -f *.o
	rm -f sync-userinfo
