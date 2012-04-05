all:sync-userinfo

CC=gcc -std=c99 -g
EXEC=sync-userinfo
OBJ=sync-userinfo.o receiver.o inf_basic_info.o inf_header_info.o utils.o inf_education_info.o inf_employment_info.o queue.c ../proto/community.pb-c.c

${EXEC}:${OBJ}
	${CC} -o $@ $^ -levent -lcurl -lprotobuf-c
  
sync-userinfo.o:sync-userinfo.c
	${CC} -c $<

receiver.o:receiver.c
	${CC} -c $<

inf_basic_info.o:inf_basic_info.c
	${CC} -c $<

inf_header_info.o:inf_header_info.c
	${CC} -c $<

inf_education_info.o:inf_education_info.c
	${CC} -c $<

inf_employment_info.o:inf_employment_info.c
	${CC} -c $<

queue.o:queue.c
	${CC} -c $<

utils.o:utils.c
	${CC} -c -o $@ $<

.PYHON:
clean:
	rm -f *.o
	rm -f sync-userinfo

