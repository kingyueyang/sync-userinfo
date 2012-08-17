all:sync-userinfo

XML=/usr/include/libxml2
INC=`/usr/local/apr/bin/apr-1-config --includes` -I./ `mysql_config --include` -I${XML}
LIB=-levent -lcurl -lprotobuf-c -lpthread -laprutil-1 -lapr-1 -llog4c -lconfig `/usr/local/apr/bin/apr-1-config --link-ld` `/usr/local/apr/bin/apu-1-config --link-ld` `mysql_config --libs`  `xml2-config --cflags` -lxml2
#CC=gcc -std=c99 -g
#CC=gcc -O2 -Wall
CC=gcc -g -Wall
EXEC=sync-userinfo

OBJ=sync-userinfo.o receiver.o \
    inf_basic_info.o inf_header_info.o \
    inf_education_info.o inf_employment_info.o \
    mysql_connector.o utils.o \
    logging.o config.o  \
    gen_constellation.o g_mobile.o\
    ../proto/community.pb-c.c

${EXEC}:${OBJ}
	${CC} -o $@ $^ ${LIB}
  
sync-userinfo.o:sync-userinfo.c
	${CC} -c $< ${INC}

receiver.o:receiver.c
	${CC} -c $< ${INC}

g_mobile.o:g_mobile.c
	${CC} -c $< -I${INC}

inf_basic_info.o:inf_basic_info.c
	${CC} -c $< ${INC}

inf_header_info.o:inf_header_info.c
	${CC} -c $< ${INC}

inf_education_info.o:inf_education_info.c
	${CC} -c $< ${INC}

inf_employment_info.o:inf_employment_info.c
	${CC} -c $< ${INC}

utils.o:utils.c
	${CC} -c -o $@ $<

mysql_connector.o:mysql_connector.c
	${CC} -c -o $@ $< ${INC}

logging.o:logging.c
	${CC} -c -o $@ $<

config.o:config.c
	${CC} -c -o $@ $< ${INC}

notify.o:notify.c
	${CC} -c -o $@ $< ${INC}

gen_constellation.o:gen_constellation.c
	${CC} -c -o $@ $< ${INC}

.PYHON:
clean:
	rm -f *.o
	rm -f sync-userinfo

