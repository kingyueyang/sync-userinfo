all:sync-userinfo

CC=gcc -std=c99 -g

sync-userinfo:sync-userinfo.o receiver.o
	${CC} -o $@ $^ -levent
  
sync-userinfo.o:sync-userinfo.c
	${CC} -c $<

receiver.o:receiver.c
	${CC} -c $<

.PYHON:
clean:
	rm -f *.o
	rm -f sync-userinfo

